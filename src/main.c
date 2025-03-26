#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"


int main(void){
    //BASE DE DATOS
    sqlite3 *db;
    sqlite3_stmt *stmt;
    sqlite3_open("Proyecto.db",&db);

    

    //MENU
    char opt;
    printf("---------Bienvenido Al Almacen----------");
    printf("--------------DeustoShoes---------------");

    //scanf("Introduce la opcion a realizae %c",opt);
}