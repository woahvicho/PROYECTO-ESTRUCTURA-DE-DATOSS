#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tda.h"
#include "archivo.h"

Mapa* mapa_global_proyectos = NULL;
Mapa* mapa_global_usuarios = NULL;
int generador_id_tareas = 1;

void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


//Antonio:Funcion para poder leer cadenas,evite problemas con la lectura de cadenas con espacios.
void leerCadena(char* buffer, int max) {
    fgets(buffer, max, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}


//Antonio:Funcion para los proyectos.
void menuProyectos() {
    int opcion;
    do {
        printf("\n--- 1. GESTION DE PROYECTOS ---\n");
        printf("1. Crear proyecto\n2. Modificar proyecto\n3. Mostrar lista de proyectos\n4. Ver progreso\n0. Volver\nOpcion: ");
        if (scanf("%d", &opcion) != 1) {opcion = -1; }
        limpiarBuffer();

        switch (opcion) {
            case 1: {
                printf("\n--- CREAR PROYECTO ---\n");
                char nombre[50];
                printf("Ingrese nombre del proyecto: ");
                leerCadena(nombre, 50);

                //Antonio: aqui valido que el proyecto no exista antes de crearlo.
                if (buscarMapa(mapa_global_proyectos, nombre) != NULL) {
                    printf("[ERROR] Ya existe un proyecto con el nombre '%s'.\n", nombre);
                } else {
                    char desc[200], inicio[15], fin[15];
                    printf("Ingrese descripcion: ");
                    leerCadena(desc, 200);
                    printf("Fecha de inicio (DD-MM-YYYY): ");
                    leerCadena(inicio, 15);
                    printf("Fecha de fin (DD-MM-YYYY): ");
                    leerCadena(fin, 15);

                    //Antonio: Aqui valido que la fecha de inicio no sea despues a la de fin.
                    if (compararFechas(inicio, fin) > 0) {
                        printf("[ERROR] La fecha de termino no puede ser anterior a la de inicio.\n");
                    } else {
                        Proyecto* p = (Proyecto*)malloc(sizeof(Proyecto));
                        strcpy(p->nombre, nombre);
                        strcpy(p->descripcion, desc);
                        strcpy(p->fecha_inicio, inicio);
                        strcpy(p->fecha_fin, fin);
                        p->tareas_nivel_1 = crearLista();
                        p->mapa_tareas_id = crearMapa(50); // mapa del proyecto
                        p->mapa_miembros = crearMapa(20);  // miembros del proyecto
                        p->total_tareas = 0;
                        p->tareas_completadas = 0;
                        
                        insertarMapa(mapa_global_proyectos, nombre, p);
                        printf("[EXITO] Proyecto '%s' creado correctamente.\n", nombre);
                    }
                }
                break;
            }
            //Antonio:Funcion para modificar proyectos que ya esten creados o registrados en el sistema
            case 2: {
                printf("\n--- MODIFICAR PROYECTO ---\n");
                char nombre[50];
                printf("Ingrese el nombre del proyecto a modificar: ");
                leerCadena(nombre, 50);

                Proyecto* p = (Proyecto*)buscarMapa(mapa_global_proyectos, nombre);
                if (p == NULL) {
                    printf("[ERROR] No se encontro ningun proyecto con el nombre '%s'.\n", nombre);
                } else {
                    printf("Proyecto encontrado. Ingrese los nuevos datos:\n");
                    
                    char desc[200], inicio[15], fin[15];
                    printf("Nueva descripcion: ");
                    leerCadena(desc, 200);
                    
                    printf("Nueva fecha de inicio (DD-MM-YYYY): ");
                    leerCadena(inicio, 15);
                    
                    printf("Nueva fecha de fin (DD-MM-YYYY): ");
                    leerCadena(fin, 15);

                    if (compararFechas(inicio, fin) > 0) {
                        printf("[ERROR] La fecha de termino no puede ser anterior a la de inicio. Modificacion cancelada.\n");
                    } else {
                        strcpy(p->descripcion, desc);
                        strcpy(p->fecha_inicio, inicio);
                        strcpy(p->fecha_fin, fin);
                        printf("[EXITO] Proyecto '%s' actualizado correctamente.\n", nombre);
                    }
                }
                break;
            }
            //Antonio: tiene como funcion mostrar un resumen de los proyectos ya creados o registrados.
            case 3: {
                printf("\n--- LISTA DE PROYECTOS ---\n");
                if (mapa_global_proyectos->tamano == 0) {
                    printf("No hay proyectos registrados en el sistema.\n");
                } else {
                    for (int i = 0; i < mapa_global_proyectos->capacidad; i++) {
                        NodoHash* actual = mapa_global_proyectos->arreglo[i];
                        while (actual != NULL) {
                            Proyecto* p = (Proyecto*)actual->valor;
                            int progreso = (p->total_tareas > 0) ? (p->tareas_completadas * 100 / p->total_tareas) : 0;
                            printf("- %s | Inicio: %s | Fin: %s | Progreso: %d%%\n", 
                                   p->nombre, p->fecha_inicio, p->fecha_fin, progreso);
                            actual = actual->siguiente;
                        }
                    }
                }
                break;
            }
            //Antonio:funcion para mostrar el progreso de los proyectos ya creados
            case 4: {
                printf("\n--- PROGRESO DE PROYECTOS ---\n");
                if (mapa_global_proyectos->tamano == 0) {
                    printf("No hay proyectos que esten registrados en el sistema.\n");
                } else {
                    for (int i = 0; i < mapa_global_proyectos->capacidad; i++) {
                        NodoHash* actual = mapa_global_proyectos->arreglo[i];
                        while (actual != NULL) {
                            Proyecto* p = (Proyecto*)actual->valor;
                            int progreso = (p->total_tareas > 0) ? (p->tareas_completadas * 100 / p->total_tareas) : 0;
                            
                            printf("----------------------------------------\n");
                            printf("Proyecto: %s\n", p->nombre);
                            printf("Descripcion: %s\n", p->descripcion);
                            printf("Fechas: %s al %s\n", p->fecha_inicio, p->fecha_fin);
                            printf("Tareas completadas: %d / %d\n", p->tareas_completadas, p->total_tareas);
                            printf("Progreso actual: %d%%\n", progreso);
                            
                            actual = actual->siguiente;
                        }
                    }
                    printf("----------------------------------------\n");
                }
                break;
            }
            case 0: break;
            default: printf("\n[ERROR] Invalido.\n");
        }
    } while (opcion != 0);
}

void menuTareas() {
    int opcion;
    do {
        printf("\n--- 2. GESTION DE TAREAS ---\n");
        printf("1. Agregar tarea/subtarea\n2. Modificar tarea\n3. Eliminar tarea\n4. Mostrar tareas\n5. Ver arbol\n0. Volver\nOpcion: ");
        if (scanf("%d", &opcion) != 1) {opcion = -1; }
        limpiarBuffer();

        switch (opcion) {
            case 1: {
                printf("\n--- AGREGAR TAREA/SUBTAREA ---\n");
                char nombre_proyecto[50], nombre_usuario[50];
                char desc[150], fecha[15], clave[20];
                int id_padre;

                printf("Ingrese nombre del proyecto: ");
                leerCadena(nombre_proyecto, 50);

                Proyecto* p = (Proyecto*)buscarMapa(mapa_global_proyectos, nombre_proyecto);
                if (p == NULL) {
                    printf("[ERROR] El proyecto '%s' no existe.\n", nombre_proyecto);
                    break;
                }

                printf("ID de la tarea padre (0 si es una tarea de primer nivel): ");
                if (scanf("%d", &id_padre) != 1) { id_padre = -1; }
                limpiarBuffer();

                Tarea* padre = NULL;
                if (id_padre != 0) {
                    sprintf(clave, "%d", id_padre);
                    padre = (Tarea*)buscarMapa(p->mapa_tareas_id, clave);
                    if (padre == NULL) {
                        printf("[ERROR] No existe una tarea con ID %d en el proyecto.\n", id_padre);
                        break;
                    }
                }

                printf("Ingrese descripcion: ");
                leerCadena(desc, 150);

                printf("Ingrese el nombre del responsable: ");
                leerCadena(nombre_usuario, 50);

                Usuario* u = (Usuario*)buscarMapa(mapa_global_usuarios, nombre_usuario);
                if (u == NULL) {
                    printf("[ERROR] El usuario '%s' no esta registrado en el sistema.\n", nombre_usuario);
                    break;
                }
                if (buscarMapa(p->mapa_miembros, nombre_usuario) == NULL) {
                    printf("[ERROR] El usuario '%s' no es miembro del proyecto.\n", nombre_usuario);
                    break;
                }

                printf("Fecha limite (DD-MM-YYYY): ");
                leerCadena(fecha, 15);
                if (strlen(fecha) != 10) {
                    printf("[ERROR] Formato de fecha invalido. Use DD-MM-YYYY.\n");
                    break;
                }

                Tarea* t = (Tarea*)malloc(sizeof(Tarea));
                t->id = generador_id_tareas++;
                strcpy(t->descripcion, desc);
                strcpy(t->fecha_limite, fecha);
                strcpy(t->estado, "Por hacer");
                t->responsable = u;
                t->padre = padre;
                t->subtareas = crearLista();

                if (padre == NULL) {
                    insertarListaFinal(p->tareas_nivel_1, t);
                } else {
                    insertarListaFinal(padre->subtareas, t);

                    Tarea* anc = padre;
                    while (anc != NULL && strcmp(anc->estado, "Completado") == 0) {
                        strcpy(anc->estado, "En proceso");
                        p->tareas_completadas--;
                        anc = anc->padre;
                    }
                }

                sprintf(clave, "%d", t->id);
                insertarMapa(p->mapa_tareas_id, clave, t);
                insertarListaFinal(u->tareas_asignadas, t);
                p->total_tareas++;

                printf("[EXITO] Tarea creada con ID %d.\n", t->id);
                break;
            }
            case 2: {
                printf("\n--- MODIFICAR TAREA ---\n");
                char nombre_proyecto[50], nombre_usuario[50];
                char desc[150], fecha[15], clave[20];
                int id_tarea, campo;

                printf("Ingrese nombre del proyecto: ");
                leerCadena(nombre_proyecto, 50);

                Proyecto* p = (Proyecto*)buscarMapa(mapa_global_proyectos, nombre_proyecto);
                if (p == NULL) {
                    printf("[ERROR] El proyecto '%s' no existe.\n", nombre_proyecto);
                    break;
                }

                printf("ID de la tarea a modificar: ");
                if (scanf("%d", &id_tarea) != 1) { id_tarea = -1; }
                limpiarBuffer();

                sprintf(clave, "%d", id_tarea);
                Tarea* t = (Tarea*)buscarMapa(p->mapa_tareas_id, clave);
                if (t == NULL) {
                    printf("[ERROR] No existe una tarea con ID %d en el proyecto.\n", id_tarea);
                    break;
                }

                printf("Tarea: [ID: %d] %s | Responsable: %s | Vence: %s | Estado: %s\n",
                       t->id, t->descripcion, t->responsable->nombre, t->fecha_limite, t->estado);
                printf("1. Descripcion\n2. Responsable\n3. Fecha limite\n4. Estado\nCampo a modificar: ");
                if (scanf("%d", &campo) != 1) { campo = -1; }
                limpiarBuffer();

                switch (campo) {
                    case 1: {
                        printf("Nueva descripcion: ");
                        leerCadena(desc, 150);
                        strcpy(t->descripcion, desc);
                        printf("[EXITO] Descripcion actualizada.\n");
                        break;
                    }
                    case 2: {
                        printf("Nuevo responsable: ");
                        leerCadena(nombre_usuario, 50);

                        Usuario* u = (Usuario*)buscarMapa(mapa_global_usuarios, nombre_usuario);
                        if (u == NULL) {
                            printf("[ERROR] El usuario '%s' no esta registrado en el sistema.\n", nombre_usuario);
                        } else if (buscarMapa(p->mapa_miembros, nombre_usuario) == NULL) {
                            printf("[ERROR] El usuario '%s' no es miembro del proyecto.\n", nombre_usuario);
                        } else {
                            eliminarLista(t->responsable->tareas_asignadas, t);
                            insertarListaFinal(u->tareas_asignadas, t);
                            t->responsable = u;
                            printf("[EXITO] Responsable actualizado.\n");
                        }
                        break;
                    }
                    case 3: {
                        printf("Nueva fecha limite (DD-MM-YYYY): ");
                        leerCadena(fecha, 15);
                        if (strlen(fecha) != 10) {
                            printf("[ERROR] Formato de fecha invalido. Use DD-MM-YYYY.\n");
                        } else {
                            strcpy(t->fecha_limite, fecha);
                            printf("[EXITO] Fecha limite actualizada.\n");
                        }
                        break;
                    }
                    case 4: {
                        int op_estado;
                        printf("1. Por hacer\n2. En proceso\n3. Completado\nNuevo estado: ");
                        if (scanf("%d", &op_estado) != 1) { op_estado = -1; }
                        limpiarBuffer();

                        if (op_estado < 1 || op_estado > 3) {
                            printf("[ERROR] Estado invalido.\n");
                            break;
                        }

                        int estaba_completada = (strcmp(t->estado, "Completado") == 0);
                        int quedara_completada = (op_estado == 3);

                        if (quedara_completada) {
                            int pendientes = 0;
                            NodoLista* actual = t->subtareas->cabeza;
                            while (actual != NULL) {
                                Tarea* sub = (Tarea*)actual->dato;
                                if (strcmp(sub->estado, "Completado") != 0) pendientes = 1;
                                actual = actual->siguiente;
                            }
                            if (pendientes) {
                                printf("[ERROR] La tarea tiene subtareas pendientes.\n");
                                break;
                            }
                        }

                        if (op_estado == 1) strcpy(t->estado, "Por hacer");
                        else if (op_estado == 2) strcpy(t->estado, "En proceso");
                        else strcpy(t->estado, "Completado");

                        if (!estaba_completada && quedara_completada) {
                            p->tareas_completadas++;
                        } else if (estaba_completada && !quedara_completada) {
                            p->tareas_completadas--;

                            Tarea* anc = t->padre;
                            while (anc != NULL && strcmp(anc->estado, "Completado") == 0) {
                                strcpy(anc->estado, "En proceso");
                                p->tareas_completadas--;
                                anc = anc->padre;
                            }
                        }
                        printf("[EXITO] Estado actualizado a '%s'.\n", t->estado);
                        break;
                    }
                    default: printf("\n[ERROR] Campo invalido.\n");
                }
                break;
            }
            case 3: {
                printf("\n--- ELIMINAR TAREA ---\n");
                char nombre_proyecto[50], clave[20], confirmacion[10];
                int id_tarea;

                printf("Ingrese nombre del proyecto: ");
                leerCadena(nombre_proyecto, 50);

                Proyecto* p = (Proyecto*)buscarMapa(mapa_global_proyectos, nombre_proyecto);
                if (p == NULL) {
                    printf("[ERROR] El proyecto '%s' no existe.\n", nombre_proyecto);
                    break;
                }

                printf("ID de la tarea a eliminar: ");
                if (scanf("%d", &id_tarea) != 1) { id_tarea = -1; }
                limpiarBuffer();

                sprintf(clave, "%d", id_tarea);
                Tarea* t = (Tarea*)buscarMapa(p->mapa_tareas_id, clave);
                if (t == NULL) {
                    printf("[ERROR] No existe una tarea con ID %d en el proyecto.\n", id_tarea);
                    break;
                }

                Tarea** por_borrar = (Tarea**)malloc(p->total_tareas * sizeof(Tarea*));
                int cantidad = 0;
                por_borrar[cantidad++] = t;
                for (int i = 0; i < cantidad; i++) {
                    NodoLista* hijo = por_borrar[i]->subtareas->cabeza;
                    while (hijo != NULL) {
                        por_borrar[cantidad++] = (Tarea*)hijo->dato;
                        hijo = hijo->siguiente;
                    }
                }

                printf("Se eliminara '%s' y %d subtarea(s). Confirmar (s/n): ", t->descripcion, cantidad - 1);
                leerCadena(confirmacion, 10);

                if (confirmacion[0] != 's' && confirmacion[0] != 'S') {
                    printf("Eliminacion cancelada.\n");
                    free(por_borrar);
                    break;
                }

                if (t->padre == NULL) {
                    eliminarLista(p->tareas_nivel_1, t);
                } else {
                    eliminarLista(t->padre->subtareas, t);
                }

                for (int i = 0; i < cantidad; i++) {
                    Tarea* x = por_borrar[i];

                    NodoLista* nodo = x->subtareas->cabeza;
                    while (nodo != NULL) {
                        NodoLista* sig = nodo->siguiente;
                        free(nodo);
                        nodo = sig;
                    }
                    free(x->subtareas);

                    sprintf(clave, "%d", x->id);
                    eliminarMapa(p->mapa_tareas_id, clave);
                    eliminarLista(x->responsable->tareas_asignadas, x);

                    p->total_tareas--;
                    if (strcmp(x->estado, "Completado") == 0) p->tareas_completadas--;

                    free(x);
                }

                printf("[EXITO] Se eliminaron %d tarea(s).\n", cantidad);
                free(por_borrar);
                break;
            }
            case 4: printf("\n[INFO] Mostrar tareas...\n"); break;
            case 5: {
                printf("\n--- ARBOL DE TAREAS ---\n");
                char nombre_proyecto[50];

                printf("Ingrese nombre del proyecto: ");
                leerCadena(nombre_proyecto, 50);

                Proyecto* p = (Proyecto*)buscarMapa(mapa_global_proyectos, nombre_proyecto);
                if (p == NULL) {
                    printf("[ERROR] El proyecto '%s' no existe.\n", nombre_proyecto);
                } else if (p->total_tareas == 0) {
                    printf("El proyecto '%s' no tiene tareas.\n", nombre_proyecto);
                } else {
                    printf("Proyecto: %s\n", p->nombre);

                    //Recorrido en profundidad iterativo: iter[prof] guarda el siguiente nodo por visitar en cada nivel.
                    NodoLista** iter = (NodoLista**)malloc((p->total_tareas + 1) * sizeof(NodoLista*));
                    int prof = 0;
                    iter[0] = p->tareas_nivel_1->cabeza;

                    while (prof >= 0) {
                        NodoLista* actual = iter[prof];
                        if (actual == NULL) {
                            prof--;
                            continue;
                        }
                        iter[prof] = actual->siguiente;

                        Tarea* t = (Tarea*)actual->dato;
                        for (int i = 0; i <= prof; i++) printf("    ");
                        printf("- [ID: %d] %s (%s | %s | %s)\n",
                               t->id, t->descripcion, t->responsable->nombre, t->fecha_limite, t->estado);

                        if (t->subtareas->cabeza != NULL) {
                            prof++;
                            iter[prof] = t->subtareas->cabeza;
                        }
                    }
                    free(iter);
                }
                break;
            }
            case 0: break;
            default: printf("\n[ERROR] Invalido.\n");
        }
    } while (opcion != 0);
}

void menuUsuarios() {
    int opcion;
    do {
        printf("\n--- 3. GESTION DE USUARIOS ---\n");
        printf("1. Registrar usuario\n2. Agregar a proyecto\n3. Ver tareas\n0. Volver\nOpcion: ");
        if (scanf("%d", &opcion) != 1) {opcion = -1; }
        limpiarBuffer();

        switch (opcion) {
            case 1: {
                printf("\n--- REGISTRAR USUARIO ---\n");
                char nombre[50];
                printf("Ingrese el nombre del usuario: ");
                leerCadena(nombre, 50);

                //Antonio:aqui verifico si el usuario ya existe o no antes de registrarlo.
                if (buscarMapa(mapa_global_usuarios, nombre) != NULL) {
                    printf("[ERROR] El usuario '%s' ya existe en el sistema.\n", nombre);
                } else {
                    Usuario* nuevo = (Usuario*)malloc(sizeof(Usuario));
                    strcpy(nuevo->nombre, nombre);
                    nuevo->tareas_asignadas = crearLista();
                    
                    insertarMapa(mapa_global_usuarios, nombre, nuevo);
                    printf("[EXITO] Usuario '%s' registrado correctamente.\n", nombre);
                }
                break;
            }
            case 2: {
                printf("\n--- AGREGAR A PROYECTO ---\n");
                char nombre_proyecto[50], nombre_usuario[50];
                printf("Ingrese nombre del proyecto: ");
                leerCadena(nombre_proyecto, 50);
                printf("Ingrese el nombre del usuario: ");
                leerCadena(nombre_usuario, 50);

                Proyecto* p = (Proyecto*)buscarMapa(mapa_global_proyectos, nombre_proyecto);
                Usuario* u = (Usuario*)buscarMapa(mapa_global_usuarios, nombre_usuario);

                if (p == NULL) {
                    printf("[ERROR] El proyecto '%s' no existe.\n", nombre_proyecto);
                } else if (u == NULL) {
                    printf("[ERROR] El usuario '%s' no esta registrado en el sistema.\n", nombre_usuario);
                } else if (buscarMapa(p->mapa_miembros, nombre_usuario) != NULL) {
                    printf("[ERROR] El usuario '%s' ya es miembro del proyecto.\n", nombre_usuario);
                } else {
                    insertarMapa(p->mapa_miembros, nombre_usuario, u);
                    printf("[EXITO] Usuario '%s' agregado al proyecto.\n", nombre_usuario);
                }
                break;
            }
            case 3: {
                printf("\n--- VER TAREAS DE USUARIO ---\n");
                char nombre_usuario[50];
                printf("Ingrese el nombre del usuario: ");
                leerCadena(nombre_usuario, 50);

                Usuario* u = (Usuario*)buscarMapa(mapa_global_usuarios, nombre_usuario);
                if (u == NULL) {
                    printf("[ERROR] El usuario '%s' no existe.\n", nombre_usuario);
                } else if (u->tareas_asignadas->size == 0) {
                    printf("El usuario '%s' no tiene tareas asignadas.\n", nombre_usuario);
                } else {
                    printf("Tareas asignadas a %s:\n", u->nombre);
                    NodoLista* actual = u->tareas_asignadas->cabeza;
                    while (actual != NULL) {
                        Tarea* t = (Tarea*)actual->dato;
                        printf("- [ID: %d] %s | Vence: %s | Estado: %s\n",
                               t->id, t->descripcion, t->fecha_limite, t->estado);
                        actual = actual->siguiente;
                    }
                }
                break;
            }
            case 0: break;
            default: printf("\n[ERROR] Invalido.\n");
        }
    } while (opcion != 0);
}

static int recorrerTareasPendientes(Monticulo* h) 
{
    int contador = 0;

    for (int i = 0; i < mapa_global_proyectos->capacidad; i++) 
    {
        NodoHash* np = mapa_global_proyectos->arreglo[i];

        while (np != NULL) 
        {
            Proyecto* p = (Proyecto*)np->valor;

            for (int j = 0; j < p->mapa_tareas_id->capacidad; j++) 
            {
                NodoHash* nt = p->mapa_tareas_id->arreglo[j];

                while (nt != NULL) 
                {
                    Tarea* t = (Tarea*)nt->valor;

                    if (strcmp(t->estado, "Completada") != 0) 
                    {
                        if (h != NULL) insertarMonticulo(h, t);
                        contador++;
                    }
                    nt = nt->siguiente;
                }
            }
            np = np->siguiente;
        }
    }
    return contador;
}


void menuConsultas() 
{
    int n;
    printf("\n--- 4. CONSULTAS ---\nN tareas proximas a vencer: ");

    if (scanf("%d", &n) != 1 || n <= 0) 
    {
        printf("\n[ERROR] Dato invalido. Ingrese un numero mayor a 0.\n");
        limpiarBuffer();
        return;
    } 

    limpiarBuffer();

    printf("\n[INFO] Consultando monticulo...\n");

    int total = recorrerTareasPendientes(NULL);
    if (total == 0)
    {
        printf("No hay tareas pendientes.\n");
        return;
    } 

    Monticulo* h = crearMonticulo(total);
    recorrerTareasPendientes(h);

    printf("\nLas %d tareas mas proximas a vencer:\n", n);

    for (int k = 0; k < n && k < total; k++) 
    {
        Tarea* t = extraerMinimoMonticulo(h);
        printf("%d. [ID: %d] %s | Vence: %s | Estado: %s\n", k + 1, t->id, t->descripcion, t->fecha_limite, t->estado);
    }
}

void menuArchivos() {
    int opcion;
    do {
        printf("\n--- 5. ARCHIVOS ---\n");
        printf("1. Guardar CSV\n2. Cargar CSV\n0. Volver\nOpcion: ");
        if (scanf("%d", &opcion) != 1) { opcion = -1; }
        limpiarBuffer();

        switch (opcion) {
            case 1: guardarCSV(mapa_global_proyectos, mapa_global_usuarios); break;
            case 2: cargarCSV(mapa_global_proyectos, mapa_global_usuarios, &generador_id_tareas); break;
            case 0: break;
            default: printf("\n[ERROR] Invalido.\n");
        }
    } while (opcion != 0);
}

int main() {
    mapa_global_proyectos = crearMapa(100);
    mapa_global_usuarios = crearMapa(100);
    int opcion = 0;

    do {
        printf("\n=== SISTEMA DE TAREAS Y PROYECTOS ===\n");
        printf("1. Proyectos\n2. Tareas\n3. Usuarios\n4. Consultas\n5. Archivos\n0. Salir\nOpcion: ");
        
        if (scanf("%d", &opcion) != 1) {opcion = -1; }
        limpiarBuffer();

        switch (opcion) {
            case 1: menuProyectos(); break;
            case 2: menuTareas(); break;
            case 3: menuUsuarios(); break;
            case 4: menuConsultas(); break;
            case 5: menuArchivos(); break;
            case 0: printf("\nSaliendo...\n"); break;
            default: printf("\n[ERROR] Opcion fuera de rango.\n");
        }
    } while (opcion != 0);

    return 0;
}