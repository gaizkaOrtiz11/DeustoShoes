// BDManager.cpp
#include "BDManager.hpp"
#include <iostream>

BDManager::BDManager() {
    db = nullptr;
}

BDManager::~BDManager() {
    cerrarBD();
}

bool BDManager::abrirBD(const std::string& nombreBD) {
    int resultado = sqlite3_open(nombreBD.c_str(), &db);
    if (resultado != SQLITE_OK) {
        std::cerr << "No se pudo abrir la base de datos: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    std::cout << "Base de datos abierta correctamente." << std::endl;
    return true;
}
#include <sstream>

std::string BDManager::obtenerListadoZapatillas() {
    std::string resultadoFinal;
    std::string sql = "SELECT id, marca, modelo, talla, precio FROM zapatillas;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        return "Error al preparar la consulta.";
    }

    std::ostringstream salida;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char* marca = sqlite3_column_text(stmt, 1);
        const unsigned char* modelo = sqlite3_column_text(stmt, 2);
        int talla = sqlite3_column_int(stmt, 3);
        double precio = sqlite3_column_double(stmt, 4);

        salida << "ID: " << id
            << ", Marca: " << marca
            << ", Modelo: " << modelo
            << ", Talla: " << talla
            << ", Precio: " << precio << " EUROS\n";

    }

    sqlite3_finalize(stmt);
    resultadoFinal = salida.str();

    if (resultadoFinal.empty()) {
        return "No hay zapatillas registradas.";
    }

    return resultadoFinal;
}

sqlite3* BDManager::getDB() {
    return db;
}



void BDManager::cerrarBD() {
    if (db != nullptr) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool BDManager::ejecutarConsulta(const std::string& consulta) {
    char* mensajeError = nullptr;
    int resultado = sqlite3_exec(db, consulta.c_str(), nullptr, nullptr, &mensajeError);

    if (resultado != SQLITE_OK) {
        std::cerr << "Error al ejecutar consulta: " << mensajeError << std::endl;
        sqlite3_free(mensajeError);
        return false;
    }

    std::cout << "Consulta ejecutada correctamente." << std::endl;
    return true;
}
