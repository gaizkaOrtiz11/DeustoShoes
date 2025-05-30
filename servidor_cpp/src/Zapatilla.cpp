#include "Zapatilla.hpp"
#include <iostream>

Zapatilla::Zapatilla() : id(0), marca(""), modelo(""), talla(0), precio(0.0) {}

Zapatilla::Zapatilla(int id, const std::string& marca, const std::string& modelo, int talla, double precio)
    : id(id), marca(marca), modelo(modelo), talla(talla), precio(precio) {}

int Zapatilla::getId() const {
    return id;
}

std::string Zapatilla::getMarca() const {
    return marca;
}

std::string Zapatilla::getModelo() const {
    return modelo;
}

int Zapatilla::getTalla() const {
    return talla;
}

double Zapatilla::getPrecio() const {
    return precio;
}

void Zapatilla::setId(int id) {
    this->id = id;
}

void Zapatilla::setMarca(const std::string& marca) {
    this->marca = marca;
}

void Zapatilla::setModelo(const std::string& modelo) {
    this->modelo = modelo;
}

void Zapatilla::setTalla(int talla) {
    this->talla = talla;
}

void Zapatilla::setPrecio(double precio) {
    this->precio = precio;
}

void Zapatilla::imprimir() const {
    std::cout << "ID: " << id
              << " - Marca: " << marca
              << " - Modelo: " << modelo
              << " - Talla: " << talla
              << " - Precio: " << precio << " €" << std::endl;
}
