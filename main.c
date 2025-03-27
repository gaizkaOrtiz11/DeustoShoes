#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
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
            case '6': cerrarSesion(); sesionIniciada = 0; break;
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

    sprintf(sql, "SELECT rol FROM Usuarios WHERE nombre = '%s' AND contrasena = '%s';", usuario, contrasena);

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char *rol = sqlite3_column_text(stmt, 0);
            strcpy(rolUsuario, (const char *)rol);
            printf("Bienvenido, %s! Rol: %s\n", usuario, rolUsuario);
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
    
    void administrarProveedores(){
        printf("\n--- Administracion de Proveedores ---\n");
        // Logica de gestion de proveedores
    }
    
    void configurarSistema(){
        printf("\n--- Configuracion del Sistema ---\n");
        // Ajustes como permisos o notificaciones
    }
    
    void cerrarSesion(){
        printf("\nSesion cerrada correctamente.\n");
    }
    