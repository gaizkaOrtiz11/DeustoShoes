#ifndef ZAPATILLA_HPP
#define ZAPATILLA_HPP

#include <string>

class Zapatilla {
private:
    int id;
    std::string marca;
    std::string modelo;
    int talla;
    double precio;

public:
    Zapatilla();
    Zapatilla(int id, const std::string& marca, const std::string& modelo, int talla, double precio);

    int getId() const;
    std::string getMarca() const;
    std::string getModelo() const;
    int getTalla() const;
    double getPrecio() const;

    void setId(int id);
    void setMarca(const std::string& marca);
    void setModelo(const std::string& modelo);
    void setTalla(int talla);
    void setPrecio(double precio);

    void imprimir() const;
};

#endif