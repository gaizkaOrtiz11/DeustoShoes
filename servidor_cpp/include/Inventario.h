#ifndef INVENTARIO_H
#define INVENTARIO_H

#include "Zapatilla.hpp"

class Inventario {
private:
    Zapatilla* zapatillas;
    int capacidad;
    int cantidad;

    void redimensionar();

public:
    Inventario();
    ~Inventario();

    void agregar(const Zapatilla& z);
    void mostrar() const;
    int getCantidad() const;
};

#endif
