#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "archivo.h"

// funcion para recorrer las tareas y guardar sus datos
void guardarTareas(FILE* archivo, char* nombre_proyecto, Lista* lista) {
    NodoLista* actual = lista->cabeza;
    while (actual != NULL) {
        Tarea* t = (Tarea*)actual->dato;
        
        int id_padre = 0;
        if (t->padre != NULL) {
            id_padre = t->padre->id;
        }

        fprintf(archivo, "TAREA,%s,%d,%d,%s,%s,%s,%s\n",
                nombre_proyecto, t->id, id_padre, t->descripcion,
                t->responsable->nombre, t->fecha_limite, t->estado);

        //para guardar las subtareas de esta tarea
        guardarTareas(archivo, nombre_proyecto, t->subtareas);
        actual = actual->siguiente;
    }
}

void guardarCSV(Mapa* mapa_proyectos, Mapa* mapa_usuarios) {
    FILE* archivo = fopen("datos.csv", "w");
    if (archivo == NULL) {
        printf("[ERROR] No se pudo crear el archivo.\n");
        return;
    }

    //guardar usuarios
    for (int i = 0; i < mapa_usuarios->capacidad; i++) {
        NodoHash* actual = mapa_usuarios->arreglo[i];
        while (actual != NULL) {
            Usuario* u = (Usuario*)actual->valor;
            fprintf(archivo, "USUARIO,%s\n", u->nombre);
            actual = actual->siguiente;
        }
    }

    //guardar proyectos
    for (int i = 0; i < mapa_proyectos->capacidad; i++) {
        NodoHash* actual = mapa_proyectos->arreglo[i];
        while (actual != NULL) {
            Proyecto* p = (Proyecto*)actual->valor;
            fprintf(archivo, "PROYECTO,%s,%s,%s,%s\n",
                    p->nombre, p->descripcion, p->fecha_inicio, p->fecha_fin);

            //guardar miembros del proyecto
            for (int j = 0; j < p->mapa_miembros->capacidad; j++) {
                NodoHash* miembro = p->mapa_miembros->arreglo[j];
                while (miembro != NULL) {
                    Usuario* u = (Usuario*)miembro->valor;
                    fprintf(archivo, "MIEMBRO,%s,%s\n", p->nombre, u->nombre);
                    miembro = miembro->siguiente;
                }
            }

            //guardar tareas del proyecto
            guardarTareas(archivo, p->nombre, p->tareas_nivel_1);
            actual = actual->siguiente;
        }
    }

    fclose(archivo);
    printf("[EXITO] Datos guardados en 'datos.csv'.\n");
}

void cargarCSV(Mapa* mapa_proyectos, Mapa* mapa_usuarios, int* generador_id) {
    FILE* archivo = fopen("datos.csv", "r");
    if (archivo == NULL) {
        printf("[ERROR] No existe el archivo 'datos.csv'.\n");
        return;
    }

    char linea[500];
    while (fgets(linea, 500, archivo) != NULL) {
        char* tipo = strtok(linea, ",\n\r");
        if (tipo == NULL) continue;

        if (strcmp(tipo, "USUARIO") == 0) {
            char* nombre = strtok(NULL, ",\n\r");
            if (buscarMapa(mapa_usuarios, nombre) == NULL) {
                Usuario* u = (Usuario*)malloc(sizeof(Usuario));
                strcpy(u->nombre, nombre);
                u->tareas_asignadas = crearLista();
                insertarMapa(mapa_usuarios, nombre, u);
            }
        }
        else if (strcmp(tipo, "PROYECTO") == 0) {
            char* nombre = strtok(NULL, ",\n\r");
            char* desc = strtok(NULL, ",\n\r");
            char* inicio = strtok(NULL, ",\n\r");
            char* fin = strtok(NULL, ",\n\r");

            if (buscarMapa(mapa_proyectos, nombre) == NULL) {
                Proyecto* p = (Proyecto*)malloc(sizeof(Proyecto));
                strcpy(p->nombre, nombre);
                strcpy(p->descripcion, desc);
                strcpy(p->fecha_inicio, inicio);
                strcpy(p->fecha_fin, fin);
                p->tareas_nivel_1 = crearLista();
                p->mapa_tareas_id = crearMapa(50);
                p->mapa_miembros = crearMapa(20);
                p->total_tareas = 0;
                p->tareas_completadas = 0;
                insertarMapa(mapa_proyectos, nombre, p);
            }
        }
        else if (strcmp(tipo, "MIEMBRO") == 0) {
            char* nom_proy = strtok(NULL, ",\n\r");
            char* nom_user = strtok(NULL, ",\n\r");

            Proyecto* p = (Proyecto*)buscarMapa(mapa_proyectos, nom_proy);
            Usuario* u = (Usuario*)buscarMapa(mapa_usuarios, nom_user);
            if (p != NULL && u != NULL && buscarMapa(p->mapa_miembros, nom_user) == NULL) {
                insertarMapa(p->mapa_miembros, nom_user, u);
            }
        }
        else if (strcmp(tipo, "TAREA") == 0) {
            char* nom_proy = strtok(NULL, ",\n\r");
            char* id_txt = strtok(NULL, ",\n\r");
            char* id_padre_txt = strtok(NULL, ",\n\r");
            char* desc = strtok(NULL, ",\n\r");
            char* nom_resp = strtok(NULL, ",\n\r");
            char* fecha = strtok(NULL, ",\n\r");
            char* estado = strtok(NULL, ",\n\r");

            Proyecto* p = (Proyecto*)buscarMapa(mapa_proyectos, nom_proy);
            Usuario* u = (Usuario*)buscarMapa(mapa_usuarios, nom_resp);

            if (p != NULL && u != NULL && buscarMapa(p->mapa_tareas_id, id_txt) == NULL) {
                int id = atoi(id_txt);
                int id_padre = atoi(id_padre_txt);

                if (id >= *generador_id) {
                    *generador_id = id + 1;
                }

                Tarea* padre = NULL;
                if (id_padre != 0) {
                    padre = (Tarea*)buscarMapa(p->mapa_tareas_id, id_padre_txt);
                }

                Tarea* t = (Tarea*)malloc(sizeof(Tarea));
                t->id = id;
                strcpy(t->descripcion, desc);
                t->responsable = u;
                strcpy(t->fecha_limite, fecha);
                strcpy(t->estado, estado);
                t->padre = padre;
                t->subtareas = crearLista();

                insertarMapa(p->mapa_tareas_id, id_txt, t);
                insertarListaFinal(u->tareas_asignadas, t);
                p->total_tareas++;
                if (strcmp(estado, "Completada") == 0) {
                    p->tareas_completadas++;
                }

                if (padre == NULL) {
                    insertarListaFinal(p->tareas_nivel_1, t);
                } else {
                    insertarListaFinal(padre->subtareas, t);
                }
            }
        }
    }

    fclose(archivo);
    printf("[EXITO] Datos cargados desde 'datos.csv'.\n");
}