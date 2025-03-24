#ifndef ZAPATILLA_H
#define ZAPATILLA_H
#include "marca.h"

typedef struct zapatilla
{
    int id;
    int precio;
    Marca marca;
    char* nombre;
};
