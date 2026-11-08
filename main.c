#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tda.h"

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
        printf("1. Crear proyecto\n2. Modificar proyecto\n3. Listar proyectos\n4. Ver progreso\n0. Volver\nOpcion: ");
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
        printf("1. Agregar tarea/subtarea\n2. Modificar tarea\n3. Eliminar tarea\n4. Listar tareas\n5. Ver arbol\n0. Volver\nOpcion: ");
        if (scanf("%d", &opcion) != 1) {opcion = -1; }
        limpiarBuffer();

        switch (opcion) {
            case 1: printf("\n[INFO] Agregar tarea...\n"); break;
            case 2: printf("\n[INFO] Modificar tarea...\n"); break;
            case 3: printf("\n[INFO] Eliminar tarea...\n"); break;
            case 4: printf("\n[INFO] Listar tareas...\n"); break;
            case 5: printf("\n[INFO] Ver arbol...\n"); break;
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
            case 2: printf("\n[INFO] Agregar a proyecto...\n"); break;
            case 3: printf("\n[INFO] Ver tareas...\n"); break;
            case 0: break;
            default: printf("\n[ERROR] Invalido.\n");
        }
    } while (opcion != 0);
}

void menuConsultas() {
    int n;
    printf("\n--- 4. CONSULTAS ---\nN tareas proximas a vencer: ");
    if (scanf("%d", &n) == 1) {
        printf("\n[INFO] Consultando monticulo...\n");
    } else {
        printf("\n[ERROR] Dato invalido.\n");
    }
    limpiarBuffer();
}

void menuArchivos() {
    printf("\n--- 5. ARCHIVOS ---\n[INFO] Guardar/Cargar CSV...\n");
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