#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include <time.h>
// Declaraciones de funciones
void iniciarSesion();
void gestionarInventario();
void registrarMovimientos();
void generarReportes();
void administrarProveedores();
void configurarSistema();
void cerrarSesion();

sqlite3 *db;
char rolUsuario[20];
int idUsuarioGlobal = -1; // -1 significa "sin sesión"

int main(void){
    // BASE DE DATOS
    sqlite3_stmt *stmt;
    sqlite3_open("Proyecto.db", &db);

    char opcion;
    int sesionIniciada = 0;

    while (1) {
        if (!sesionIniciada) {
            iniciarSesion();
            sesionIniciada = 1;
        }

        printf("\n----- MENU PRINCIPAL (%s) -----\n", rolUsuario);
        printf("1. Gestionar Inventario\n");
        printf("2. Registrar Movimientos\n");
        printf("3. Generar Reportes\n");
        printf("4. Administrar Proveedores\n");
        printf("5. Configuracion del Sistema\n");
        printf("6. Cerrar Sesion\n");
        printf("Selecciona una opcion: ");
        scanf(" %c", &opcion);

        switch(opcion){
            case '1': gestionarInventario(); break;
            case '2': registrarMovimientos(); break;
            case '3': generarReportes(); break;
            case '4': administrarProveedores(); break;
            case '5': configurarSistema(); break;
            case '6': cerrarSesion(idUsuarioGlobal); sesionIniciada = 0; break;
            default: printf("\nOpcion no valida.\n");
        }
    }

    sqlite3_close(db);
    return 0;
}

void iniciarSesion(){
    char usuario[50];
    char contrasena[50];
    char sql[256];
    sqlite3_stmt *stmt;

    printf("\n--- Inicio de Sesion ---\n");
    printf("Usuario: ");
    scanf("%s", usuario);
    printf("Contrasena: ");
    scanf("%s", contrasena);

    sprintf(sql, "SELECT id, rol FROM Usuarios WHERE nombre = '%s' AND contrasena = '%s';", usuario, contrasena);

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            idUsuarioGlobal = sqlite3_column_int(stmt, 0); // 💡 Guardamos el ID global
            const unsigned char *rol = sqlite3_column_text(stmt, 0);
            strcpy(rolUsuario, (const char *)rol);
            printf("Bienvenido, %s! Rol: %s | ID: %d\n", usuario, rolUsuario, idUsuarioGlobal);
        } else {
            printf("Usuario o contrasena incorrectos.\n");
            iniciarSesion(); // reintento
        }
        sqlite3_finalize(stmt);
    } else {
        printf("Error al verificar usuario.\n");
    }}
    void gestionarInventario(){
        printf("\n--- Gestion de Inventario ---\n");
        printf("1. Ver productos\n");
        printf("2. Anadir producto\n");
        printf("3. Modificar producto\n");
        printf("4. Eliminar producto\n");
        printf("Seleccione una opcion:\n ");
    
        char opcion;
        scanf(" %c", &opcion);
    
        char sql[512];
        sqlite3_stmt *stmt;
    
        switch(opcion){
            case '1':
                printf("Mostrando productos...\n");
                sprintf(sql, "SELECT id, nombre, marca, modelo, cantidad, precio FROM Zapatilla;");
                if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
                    while (sqlite3_step(stmt) == SQLITE_ROW) {
                        printf("ID: %d | Nombre: %s | Marca: %s | Modelo: %s | Cantidad: %d | Precio: %.2f\n",
                               sqlite3_column_int(stmt, 0),
                               sqlite3_column_text(stmt, 1),
                               sqlite3_column_text(stmt, 2),
                               sqlite3_column_text(stmt, 3),
                               sqlite3_column_int(stmt, 4),
                               sqlite3_column_double(stmt, 5));
                    }
                    sqlite3_finalize(stmt);
                } else {
                    printf("Error al preparar consulta.\n");
                }
                break;
            case '2': {
                char nombre[50], marca[50], modelo[50];
                int cantidad;
                float precio;
                printf("Nombre: "); scanf("%s", nombre);
                printf("Marca: "); scanf("%s", marca);
                printf("Modelo: "); scanf("%s", modelo);
                printf("Cantidad: "); scanf("%d", &cantidad);
                printf("Precio: "); scanf("%f", &precio);
                sprintf(sql, "INSERT INTO Zapatilla (nombre, marca, modelo, cantidad, precio) VALUES ('%s', '%s', '%s', %d, %.2f);",
                        nombre, marca, modelo, cantidad, precio);
                if (sqlite3_exec(db, sql, 0, 0, 0) == SQLITE_OK) {
                    printf("Producto anadido con exito.\n");
                } else {
                    printf("Error al anadir producto.\n");
                }
                break;
            }
            case '3': {
                int id, nuevaCantidad;
                printf("ID del producto a modificar: ");
                scanf("%d", &id);
                printf("Nueva cantidad: ");
                scanf("%d", &nuevaCantidad);
                sprintf(sql, "UPDATE Zapatilla SET cantidad = %d WHERE id = %d;", nuevaCantidad, id);
                if (sqlite3_exec(db, sql, 0, 0, 0) == SQLITE_OK) {
                    printf("Producto modificado con exito.\n");
                } else {
                    printf("Error al modificar producto.\n");
                }
                break;
            }
            case '4': {
                int id;
                printf("ID del producto a eliminar: ");
                scanf("%d", &id);
                sprintf(sql, "DELETE FROM Zapatilla WHERE id = %d;", id);
                if (sqlite3_exec(db, sql, 0, 0, 0) == SQLITE_OK) {
                    printf("Producto eliminado correctamente.\n");
                } else {
                    printf("Error al eliminar producto.\n");
                }
                break;
            }
            default:
                printf("Opcion invalida.\n");
        }
    }
    
    void registrarMovimientos(){
        printf("\n--- Registro de Movimientos ---\n");
        int tipo, producto_id, cantidad;
        char motivo[100], sql[512];
    
        printf("Tipo de movimiento (1=Entrada, 2=Salida): ");
        scanf("%d", &tipo);
        printf("ID del producto: ");
        scanf("%d", &producto_id);
        printf("Cantidad: ");
        scanf("%d", &cantidad);
        
    
        sprintf(sql, "INSERT INTO Movimientos (tipo, producto_id, cantidad, fecha, usuario_id) VALUES (%d, %d, %d, datetime('now'), 1, '%s');",
                tipo, producto_id, cantidad);
    
        if (sqlite3_exec(db, sql, 0, 0, 0) == SQLITE_OK) {
            printf("Movimiento registrado con exito.\n");
        } else {
            printf("Error al registrar movimiento.\n");
        }
    }
    
    void generarReportes(){
        printf("\n--- Generacion de Reportes ---\n");

        char sql[256];
        sqlite3_stmt *stmt;
        FILE *archivo = fopen("reporte_inventario.log", "w");

        if (archivo == NULL) {
            printf("Error al crear el archivo de reporte.\n");
            return;
        }

        fprintf(archivo, "Reporte de Inventario - Generado automaticamente\n\n");
        sprintf(sql, "SELECT id, nombre, marca, modelo, cantidad, precio FROM Zapatilla;");

        int totalProductos = 0;
        double valorTotal = 0.0;

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
            int cantidad = sqlite3_column_int(stmt, 4);
            double precio = sqlite3_column_double(stmt, 5);
            totalProductos += cantidad;
            valorTotal += cantidad * precio;

            fprintf(archivo, "ID: %d | Nombre: %s | Marca: %s | Modelo: %s | Cantidad: %d | Precio: %.2f\n",
                    sqlite3_column_int(stmt, 0),
                    sqlite3_column_text(stmt, 1),
                    sqlite3_column_text(stmt, 2),
                    sqlite3_column_text(stmt, 3),
                    cantidad,
                    precio);
        }
        sqlite3_finalize(stmt);

        fprintf(archivo, "\nResumen:\n");
        fprintf(archivo, "Total de unidades en inventario: %d\n", totalProductos);
        fprintf(archivo, "Valor total del inventario: %.2f euros\n", valorTotal);

        printf("Reporte generado exitosamente en 'reporte_inventario.log'.\n");
        } else {
        printf("Error al generar el reporte.\n");
        }

        fclose(archivo);
    }
    
    void administrarProveedores() {
        printf("\n--- Administracion de Proveedores ---\n");
        printf("1. Ver proveedores\n");
        printf("2. Anadir proveedor\n");
        printf("3. Modificar proveedor\n");
        printf("4. Eliminar proveedor\n");
        printf("Seleccione una opcion: ");
    
        char opcion;
        scanf(" %c", &opcion);
    
        char sql[512];
        sqlite3_stmt *stmt;
    
        switch(opcion) {
            case '1': {
                // Ver proveedores
                sprintf(sql, "SELECT id, nombre, contacto, direccion FROM Proveedores;");
                if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
                    printf("\n--- Lista de Proveedores ---\n");
                    while (sqlite3_step(stmt) == SQLITE_ROW) {
                        printf("ID: %d | Nombre: %s | Contacto: %s | Direccion: %s\n",
                            sqlite3_column_int(stmt, 0),
                            sqlite3_column_text(stmt, 1),
                            sqlite3_column_text(stmt, 2),
                            sqlite3_column_text(stmt, 3));
                    }
                    sqlite3_finalize(stmt);
                } else {
                    printf("Error al obtener proveedores.\n");
                }
                break;
            }
            case '2': {
                // Anadir proveedor
                char nombre[50], contacto[50], direccion[100];
                printf("Nombre: "); scanf(" %[^\n]", nombre);
                printf("Contacto: "); scanf(" %[^\n]", contacto);
                printf("Direccion: "); scanf(" %[^\n]", direccion);
                sprintf(sql, "INSERT INTO Proveedores (nombre, contacto, direccion) VALUES ('%s', '%s', '%s');",
                        nombre, contacto, direccion);
                if (sqlite3_exec(db, sql, 0, 0, 0) == SQLITE_OK) {
                    printf("Proveedor anadido con exito.\n");
                } else {
                    printf("Error al anadir proveedor.\n");
                }
                break;
            }
            case '3': {
                // Modificar proveedor
                int id;
                char nuevoContacto[50], nuevaDireccion[100];
                printf("ID del proveedor a modificar: "); scanf("%d", &id);
                printf("Nuevo contacto: "); scanf(" %[^\n]", nuevoContacto);
                printf("Nueva direccion: "); scanf(" %[^\n]", nuevaDireccion);
                sprintf(sql, "UPDATE Proveedores SET contacto = '%s', direccion = '%s' WHERE id = %d;",
                        nuevoContacto, nuevaDireccion, id);
                if (sqlite3_exec(db, sql, 0, 0, 0) == SQLITE_OK) {
                    printf("Proveedor modificado con exito.\n");
                } else {
                    printf("Error al modificar proveedor.\n");
                }
                break;
            }
            case '4': {
                // Eliminar proveedor
                int id;
                printf("ID del proveedor a eliminar: ");
                scanf("%d", &id);
                sprintf(sql, "DELETE FROM Proveedores WHERE id = %d;", id);
                if (sqlite3_exec(db, sql, 0, 0, 0) == SQLITE_OK) {
                    printf("Proveedor eliminado correctamente.\n");
                } else {
                    printf("Error al eliminar proveedor.\n");
                }
                break;
            }
            default:
                printf("Opcion invalida.\n");
        }
    }
    
    
    void configurarSistema() {
        printf("\n--- Configuracion del Sistema ---\n");
        printf("1. Ver usuarios\n");
        printf("2. Cambiar contrasena de usuario\n");
        printf("3. Cambiar rol de usuario\n");
        printf("Seleccione una opcion: ");
    
        char opcion;
        scanf(" %c", &opcion);
    
        char sql[256];
        sqlite3_stmt *stmt;
    
        switch (opcion) {
            case '1': {
                // Mostrar usuarios
                sprintf(sql, "SELECT id, nombre, rol FROM Usuarios;");
                if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
                    printf("\n--- Lista de Usuarios ---\n");
                    while (sqlite3_step(stmt) == SQLITE_ROW) {
                        printf("ID: %d | Nombre: %s | Rol: %s\n",
                            sqlite3_column_int(stmt, 0),
                            sqlite3_column_text(stmt, 1),
                            sqlite3_column_text(stmt, 2));
                    }
                    sqlite3_finalize(stmt);
                } else {
                    printf("Error al obtener usuarios.\n");
                }
                break;
            }
    
            case '2': {
                // Cambiar contrasena
                int id;
                char nuevaContrasena[100];
                printf("ID del usuario: ");
                scanf("%d", &id);
                printf("Nueva contrasena: ");
                scanf(" %[^\n]", nuevaContrasena);
    
                sprintf(sql, "UPDATE Usuarios SET contrasena = '%s' WHERE id = %d;", nuevaContrasena, id);
                if (sqlite3_exec(db, sql, 0, 0, 0) == SQLITE_OK) {
                    printf("Contrasena actualizada.\n");
                } else {
                    printf("Error al cambiar la contrasena.\n");
                }
                break;
            }
    
            case '3': {
                // Cambiar rol
                int id;
                char nuevoRol[50];
                printf("ID del usuario: ");
                scanf("%d", &id);
                printf("Nuevo rol: ");
                scanf(" %[^\n]", nuevoRol);
    
                sprintf(sql, "UPDATE Usuarios SET rol = '%s' WHERE id = %d;", nuevoRol, id);
                if (sqlite3_exec(db, sql, 0, 0, 0) == SQLITE_OK) {
                    printf("Rol actualizado correctamente.\n");
                } else {
                    printf("Error al actualizar el rol.\n");
                }
                break;
            }
    
            default:
                printf("Opcion invalida.\n");
        }
    }
    
    
    void cerrarSesion(int idUsuario) {
        char sql[256];
        char fecha[50];
    
        // Obtener la fecha y hora actual
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        sprintf(fecha, "%04d-%02d-%02d %02d:%02d:%02d",
                tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                tm.tm_hour, tm.tm_min, tm.tm_sec);
    
        // Guardar la hora de ultima conexion
        sprintf(sql, "UPDATE Usuarios SET ultima_conexion = '%s' WHERE id = %d;", fecha, idUsuario);
        if (sqlite3_exec(db, sql, 0, 0, 0) != SQLITE_OK) {
            printf("⚠️  No se pudo actualizar la ultima conexion.\n");
        }
    
        printf("\n✅ Sesion cerrada correctamente. Hasta pronto.\n");
    }
    
    