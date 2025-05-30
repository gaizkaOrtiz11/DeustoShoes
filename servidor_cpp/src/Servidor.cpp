// Servidor.cpp
#include "Servidor.hpp"
#include <iostream>
#include <sstream>


Servidor::Servidor() {
    WSADATA wsaData;
    int resultado = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (resultado != 0) {
        std::cerr << "Error al iniciar Winsock: " << resultado << std::endl;
        exit(1);
    }

    servidorSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (servidorSocket == INVALID_SOCKET) {
        std::cerr << "Error al crear el socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        exit(1);
    }
}

Servidor::~Servidor() {
    cerrar();
    WSACleanup();
}

bool Servidor::iniciar(int puerto) {
    direccionServidor.sin_family = AF_INET;
    direccionServidor.sin_addr.s_addr = INADDR_ANY;
    direccionServidor.sin_port = htons(puerto);

    if (bind(servidorSocket, (sockaddr*)&direccionServidor, sizeof(direccionServidor)) == SOCKET_ERROR) {
        std::cerr << "Error al hacer bind: " << WSAGetLastError() << std::endl;
        return false;
    }

    if (listen(servidorSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Error al escuchar: " << WSAGetLastError() << std::endl;
        return false;
    }

    std::cout << "Servidor escuchando en el puerto " << puerto << std::endl;
    return true;
}

void Servidor::escuchar() {
    sockaddr_in clienteDireccion;
    int tamCliente = sizeof(clienteDireccion);

    clienteSocket = accept(servidorSocket, (sockaddr*)&clienteDireccion, &tamCliente);
    if (clienteSocket == INVALID_SOCKET) {
        std::cerr << "Error al aceptar conexión: " << WSAGetLastError() << std::endl;
        return;
    }

    std::cout << "Cliente conectado." << std::endl;

    BDManager gestorBD;
    if (!gestorBD.abrirBD("zapatillas.db")) {
        std::cerr << "Error al abrir la base de datos." << std::endl;
        return;
    }

    char buffer[1024];
    int bytesRecibidos;

    while ((bytesRecibidos = recv(clienteSocket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[bytesRecibidos] = '\0';
        std::string comando(buffer);
        std::cout << "Comando recibido: " << comando << std::endl;

        std::string respuesta;
        std::istringstream iss(comando);
        std::string palabra;
        iss >> palabra;

        if (palabra == "CONSULTAR") {
            respuesta = gestorBD.obtenerListadoZapatillas();
        } 
        else if (palabra == "INSERTAR") {
            std::string marca, modelo;
            int talla;
            double precio;

            if (iss >> marca >> modelo >> talla >> precio) {
                std::string sql = "INSERT INTO zapatillas (marca, modelo, talla, precio) VALUES ('" 
                                + marca + "', '" + modelo + "', " + std::to_string(talla) + ", " + std::to_string(precio) + ");";
                if (gestorBD.ejecutarConsulta(sql)) {
                    respuesta = "Zapatilla insertada correctamente.";
                } else {
                    respuesta = "Error al insertar zapatilla.";
                }
            } else {
                respuesta = "Formato incorrecto. Uso: INSERTAR <marca> <modelo> <talla> <precio>";
            }
        }else if (palabra == "BORRAR") {
    int id;
    if (iss >> id) {
        std::string sql = "DELETE FROM zapatillas WHERE id = " + std::to_string(id) + ";";
        if (gestorBD.ejecutarConsulta(sql)) {
            respuesta = "Zapatilla eliminada correctamente.";
        } else {
            respuesta = "Error al eliminar zapatilla.";
        }
    } else {
        respuesta = "Formato incorrecto. Uso: BORRAR <id>";
    }
} else if (palabra == "BUSCAR") {
    std::string marca;
    if (iss >> marca) {
        std::string sql = "SELECT id, marca, modelo, talla, precio FROM zapatillas WHERE marca = '" + marca + "';";
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(gestorBD.getDB(), sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            std::ostringstream salida;
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                int id = sqlite3_column_int(stmt, 0);
                const unsigned char* m = sqlite3_column_text(stmt, 1);
                const unsigned char* modelo = sqlite3_column_text(stmt, 2);
                int talla = sqlite3_column_int(stmt, 3);
                double precio = sqlite3_column_double(stmt, 4);

                salida << "ID: " << id
                       << ", Marca: " << m
                       << ", Modelo: " << modelo
                       << ", Talla: " << talla
                       << ", Precio: " << precio << " €\n";
            }
            sqlite3_finalize(stmt);
            respuesta = salida.str().empty() ? "No se encontraron zapatillas de esa marca." : salida.str();
        } else {
            respuesta = "Error al preparar la consulta.";
        }
    } else {
        respuesta = "Formato incorrecto. Uso: BUSCAR <marca>";
    }
}else if (palabra == "BUSCAR_TALLA") {
    int talla;
    if (iss >> talla) {
        std::string sql = "SELECT id, marca, modelo, talla, precio FROM zapatillas WHERE talla = " + std::to_string(talla) + ";";
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(gestorBD.getDB(), sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            std::ostringstream salida;
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                int id = sqlite3_column_int(stmt, 0);
                const unsigned char* marca = sqlite3_column_text(stmt, 1);
                const unsigned char* modelo = sqlite3_column_text(stmt, 2);
                int t = sqlite3_column_int(stmt, 3);
                double precio = sqlite3_column_double(stmt, 4);

                salida << "ID: " << id
                       << ", Marca: " << marca
                       << ", Modelo: " << modelo
                       << ", Talla: " << t
                       << ", Precio: " << precio << " €\n";
            }
            sqlite3_finalize(stmt);
            respuesta = salida.str().empty() ? "No hay zapatillas con esa talla." : salida.str();
        } else {
            respuesta = "Error al preparar la consulta.";
        }
    } else {
        respuesta = "Formato incorrecto. Uso: BUSCAR_TALLA <talla>";
    }
}else if (palabra == "MODIFICAR") {
    int id, talla;
    double precio;
    std::string marca, modelo;

    if (iss >> id >> marca >> modelo >> talla >> precio) {
        std::string sql = "UPDATE zapatillas SET marca = '" + marca +
                          "', modelo = '" + modelo +
                          "', talla = " + std::to_string(talla) +
                          ", precio = " + std::to_string(precio) +
                          " WHERE id = " + std::to_string(id) + ";";

        if (gestorBD.ejecutarConsulta(sql)) {
            respuesta = "Zapatilla modificada correctamente.";
        } else {
            respuesta = "Error al modificar zapatilla.";
        }
    } else {
        respuesta = "Formato incorrecto. Uso: MODIFICAR <id> <marca> <modelo> <talla> <precio>";
    }
}else if (palabra == "AYUDA") {
    respuesta =
        "Comandos disponibles:\n"
        "- CONSULTAR\n"
        "- INSERTAR <marca> <modelo> <talla> <precio>\n"
        "- BORRAR <id>\n"
        "- BUSCAR <marca>\n"
        "- BUSCAR_TALLA <talla>\n"
        "- MODIFICAR <id> <marca> <modelo> <talla> <precio>\n"
        "- AYUDA\n"
        "- SALIR";
}

else if (palabra == "SALIR") {
            respuesta = "Desconectando.";
            send(clienteSocket, respuesta.c_str(), static_cast<int>(respuesta.size()), 0);
            break;
        } 
        else {
            respuesta = "Comando no permitido.";
        }

        send(clienteSocket, respuesta.c_str(), static_cast<int>(respuesta.size()), 0);
    }

    closesocket(clienteSocket);
    std::cout << "Cliente desconectado." << std::endl;
}



void Servidor::cerrar() {
    closesocket(servidorSocket);
}
