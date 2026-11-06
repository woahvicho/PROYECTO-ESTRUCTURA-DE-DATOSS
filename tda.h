#ifndef TDA_H
#define TDA_H

#include "estructuras.h"

// Funciones de Lista
Lista* crearLista();
void insertarListaFinal(Lista* l, void* dato);

// Funciones de Mapa
Mapa* crearMapa(int capacidad);
int hashString(Mapa* m, const char* clave);
void redimensionarMapa(Mapa* m); // [NUEVO]
void insertarMapa(Mapa* m, const char* clave, void* valor);
void* buscarMapa(Mapa* m, const char* clave);

// Funciones de Montículo Mínimo [NUEVO]
Monticulo* crearMonticulo(int capacidad);
int compararFechas(const char* f1, const char* f2);
void insertarMonticulo(Monticulo* m, Tarea* t);
Tarea* extraerMinimoMonticulo(Monticulo* m);

#endif