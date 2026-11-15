#ifndef ARCHIVO_H
#define ARCHIVO_H

#include "tda.h"

void guardarCSV(Mapa* mapa_proyectos, Mapa* mapa_usuarios);
void cargarCSV(Mapa* mapa_proyectos, Mapa* mapa_usuarios, int* generador_id);

#endif