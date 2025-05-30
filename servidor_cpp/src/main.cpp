// main.cpp
#include "Servidor.hpp"
#include <iostream>

int main() {
    Servidor servidor;

    const int PUERTO = 5000;

    if (!servidor.iniciar(PUERTO)) {
        std::cerr << "No se pudo iniciar el servidor en el puerto " << PUERTO << std::endl;
        return 1;
    }

    servidor.escuchar();

    return 0;
}
