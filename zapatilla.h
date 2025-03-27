#ifndef ZAPATILLA_H
#define ZAPATILLA_H

#include "marca.h"

typedef struct Zapatilla {
    int id;
    float precio;
    Marca marca;
    char nombre[50];
} Zapatilla;

void imprimirZapatilla(Zapatilla z);

#endif