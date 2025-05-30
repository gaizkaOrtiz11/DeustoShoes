#include "zapatilla.hpp"
#include "Inventario.h"
#include <iostream>


Inventario::Inventario() {
    capacidad = 10;
    cantidad = 0;
    zapatillas = new Zapatilla[capacidad];
}

Inventario::~Inventario() {
    delete[] zapatillas;
}

void Inventario::redimensionar() {
    capacidad *= 2;
    Zapatilla* nuevo = new Zapatilla[capacidad];
    for (int i = 0; i < cantidad; i++) {
        nuevo[i] = zapatillas[i];
    }
    delete[] zapatillas;
    zapatillas = nuevo;
}

void Inventario::agregar(const Zapatilla& z) {
    if (cantidad == capacidad) {
        redimensionar();
    }
    zapatillas[cantidad] = z;
    cantidad++;
}

void Inventario::mostrar() const {
    for (int i = 0; i < cantidad; i++) {
        std::cout << "ID: " << zapatillas[i].getId()
                  << " | Marca: " << zapatillas[i].getMarca()
                  << " | Modelo: " << zapatillas[i].getModelo()
                  << " | Talla: " << zapatillas[i].getTalla()
                  << " | Precio: " << zapatillas[i].getPrecio()
                  << " €" << std::endl;
    }
}


int Inventario::getCantidad() const {
    return cantidad;
}
