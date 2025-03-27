#include "zapatilla.h"
#include <stdio.h>

void imprimirZapatilla(Zapatilla z) {
    printf("Zapatilla: %s - Marca: %s - Precio: %.2f\n", z.nombre, z.marca.nombre, z.precio);
}
