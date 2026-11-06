#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tda.h"

// ==========================================
// TDA LISTA
// ==========================================
Lista* crearLista() {
    Lista* l = (Lista*)malloc(sizeof(Lista));
    l->cabeza = NULL;
    l->ultimo = NULL;
    l->size = 0;
    return l;
}

void insertarListaFinal(Lista* l, void* dato) {
    NodoLista* nuevo = (NodoLista*)malloc(sizeof(NodoLista));
    nuevo->dato = dato;
    nuevo->siguiente = NULL;
    if (l->size == 0) {
        l->cabeza = nuevo;
        l->ultimo = nuevo;
    } else {
        l->ultimo->siguiente = nuevo;
        l->ultimo = nuevo;
    }
    l->size++;
}

// ==========================================
// TDA MAPA
// ==========================================
Mapa* crearMapa(int capacidad) {
    Mapa* m = (Mapa*)malloc(sizeof(Mapa));
    m->capacidad = capacidad;
    m->tamaño = 0;
    m->arreglo = (NodoHash**)calloc(capacidad, sizeof(NodoHash*));
    return m;
}

int hashString(Mapa* m, const char* clave) {
    unsigned long hash = 5381;
    int c;
    while ((c = *clave++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % m->capacidad;
}

void redimensionarMapa(Mapa* m) {
    int nueva_capacidad = m->capacidad * 2;
    NodoHash** nuevo_arreglo = (NodoHash**)calloc(nueva_capacidad, sizeof(NodoHash*));
    
    for (int i = 0; i < m->capacidad; i++) {
        NodoHash* actual = m->arreglo[i];
        while (actual != NULL) {
            NodoHash* siguiente = actual->siguiente;
            unsigned long hash = 5381;
            char* clave = actual->clave;
            int c;
            while ((c = *clave++)) { hash = ((hash << 5) + hash) + c; }
            int nuevo_indice = hash % nueva_capacidad;
            
            actual->siguiente = nuevo_arreglo[nuevo_indice];
            nuevo_arreglo[nuevo_indice] = actual;
            actual = siguiente;
        }
    }
    free(m->arreglo);
    m->arreglo = nuevo_arreglo;
    m->capacidad = nueva_capacidad;
}

void insertarMapa(Mapa* m, const char* clave, void* valor) {
    float factor_carga = (float)(m->tamaño + 1) / m->capacidad;
    if (factor_carga > 0.75) {
        redimensionarMapa(m);
    }

    int indice = hashString(m, clave);
    NodoHash* nuevo = (NodoHash*)malloc(sizeof(NodoHash));
    nuevo->clave = strdup(clave);
    nuevo->valor = valor;
    nuevo->siguiente = m->arreglo[indice];
    m->arreglo[indice] = nuevo;
    m->tamaño++;
}

void* buscarMapa(Mapa* m, const char* clave) {
    int indice = hashString(m, clave);
    NodoHash* actual = m->arreglo[indice];
    while (actual != NULL) {
        if (strcmp(actual->clave, clave) == 0) return actual->valor;
        actual = actual->siguiente;
    }
    return NULL;
}

// ==========================================
// TDA MONTICULO MINIMO
// ==========================================
Monticulo* crearMonticulo(int capacidad) {
    Monticulo* m = (Monticulo*)malloc(sizeof(Monticulo));
    m->capacidad = capacidad;
    m->tamaño = 0;
    m->arreglo = (Tarea**)malloc(capacidad * sizeof(Tarea*));
    return m;
}

int compararFechas(const char* f1, const char* f2) {
    int d1, m1, a1, d2, m2, a2;
    sscanf(f1, "%d-%d-%d", &d1, &m1, &a1);
    sscanf(f2, "%d-%d-%d", &d2, &m2, &a2);
    if (a1 != a2) return a1 - a2;
    if (m1 != m2) return m1 - m2;
    return d1 - d2;
}

void insertarMonticulo(Monticulo* m, Tarea* t) {
    if (m->tamaño == m->capacidad) return; 
    
    int i = m->tamaño;
    m->arreglo[i] = t;
    m->tamaño++;
    
    while (i != 0 && compararFechas(m->arreglo[i]->fecha_limite, m->arreglo[(i - 1) / 2]->fecha_limite) < 0) {
        Tarea* temp = m->arreglo[i];
        m->arreglo[i] = m->arreglo[(i - 1) / 2];
        m->arreglo[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
}

Tarea* extraerMinimoMonticulo(Monticulo* m) {
    if (m->tamaño <= 0) return NULL;
    if (m->tamaño == 1) {
        m->tamaño--;
        return m->arreglo[0];
    }
    
    Tarea* raiz = m->arreglo[0];
    m->arreglo[0] = m->arreglo[m->tamaño - 1];
    m->tamaño--;
    
    int i = 0;
    while (1) {
        int izq = 2 * i + 1;
        int der = 2 * i + 2;
        int menor = i;
        
        if (izq < m->tamaño && compararFechas(m->arreglo[izq]->fecha_limite, m->arreglo[menor]->fecha_limite) < 0)
            menor = izq;
        if (der < m->tamaño && compararFechas(m->arreglo[der]->fecha_limite, m->arreglo[menor]->fecha_limite) < 0)
            menor = der;
            
        if (menor != i) {
            Tarea* temp = m->arreglo[i];
            m->arreglo[i] = m->arreglo[menor];
            m->arreglo[menor] = temp;
            i = menor;
        } else {
            break;
        }
    }
    return raiz;
}
