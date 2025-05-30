// BDManager.hpp
#ifndef BDMANAGER_HPP
#define BDMANAGER_HPP

#include <string>
#include <sqlite3.h>

class BDManager {
private:
    sqlite3* db;

public:
    BDManager();
    ~BDManager();

    bool abrirBD(const std::string& nombreBD);
    std::string obtenerListadoZapatillas();
    void cerrarBD();
    bool ejecutarConsulta(const std::string& consulta);
    sqlite3* getDB();
};


#endif
