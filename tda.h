#ifndef TDA_H
#define TDA_H

#include "estructuras.h"

//funciones de lista
Lista* crearLista();
void insertarListaFinal(Lista* l, void* dato);

//funciones de mapa
Mapa* crearMapa(int capacidad);
int hashString(Mapa* m, const char* clave);
void redimensionarMapa(Mapa* m); // [NUEVO]
void insertarMapa(Mapa* m, const char* clave, void* valor);
void* buscarMapa(Mapa* m, const char* clave);

//funcion de montículo mínimo
Monticulo* crearMonticulo(int capacidad);
int compararFechas(const char* f1, const char* f2);
void insertarMonticulo(Monticulo* m, Tarea* t);
Tarea* extraerMinimoMonticulo(Monticulo* m);

#endif
