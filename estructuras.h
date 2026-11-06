#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

typedef struct Tarea Tarea;
typedef struct Usuario Usuario;
typedef struct Proyecto Proyecto;
typedef struct NodoLista NodoLista;
typedef struct Lista Lista;
typedef struct NodoHash NodoHash;
typedef struct Mapa Mapa;
typedef struct Monticulo Monticulo; // [NUEVO] TDA Montículo Mínimo

// TDA Lista (O(1) al insertar al final)[cite: 4]
struct NodoLista {
    void* dato;
    NodoLista* siguiente;
};

struct Lista {
    NodoLista* cabeza;
    NodoLista* ultimo;
    int size;
};

// TDA Mapa (Hash con encadenamiento)[cite: 4]
struct NodoHash {
    char* clave;
    void* valor;
    NodoHash* siguiente;
};

struct Mapa {
    NodoHash** arreglo;
    int capacidad;
    int tamaño;
};

// Estructura Usuario[cite: 4]
struct Usuario {
    char nombre[50];
    Lista* tareas_asignadas;
};

// Estructura Tarea (Árbol general iterativo)[cite: 4]
struct Tarea {
    int id;
    char descripcion[150];
    Usuario* responsable;
    char fecha_limite[11];
    char estado[15];
    Tarea* padre;
    Lista* subtareas;
};

// Estructura Proyecto[cite: 4]
struct Proyecto {
    char nombre[50];
    char descripcion[200];
    char fecha_inicio[11];
    char fecha_fin[11];
    Lista* tareas_nivel_1;
    Mapa* mapa_tareas_id;
    Mapa* mapa_miembros;
    int total_tareas;
    int tareas_completadas;
};

// TDA Montículo Mínimo (O(n) construir, O(log n) extraer)[cite: 4]
struct Monticulo {
    Tarea** arreglo;
    int capacidad;
    int tamaño;
};

#endif