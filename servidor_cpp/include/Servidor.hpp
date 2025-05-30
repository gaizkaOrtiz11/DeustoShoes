// Servidor.hpp
#ifndef SERVIDOR_HPP
#define SERVIDOR_HPP

#include <winsock2.h>
#include <windows.h>
#include <string>
#include <vector>
#include "BDManager.hpp"

#pragma comment(lib, "ws2_32.lib")  // Necesario para enlazar WinSock

class Servidor {
private:
    SOCKET servidorSocket;
    SOCKET clienteSocket;
    sockaddr_in direccionServidor;

public:
    Servidor();
    ~Servidor();
    bool iniciar(int puerto);
    void escuchar();
    void cerrar();
};

#endif
