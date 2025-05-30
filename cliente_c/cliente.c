// cliente.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in servidor;
    char mensaje[1024], respuesta[4096];
    int puerto = 5000;

    // Inicializar WinSock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Error al iniciar Winsock. Codigo: %d\n", WSAGetLastError());
        return 1;
    }

    // Crear socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("No se pudo crear el socket: %d\n", WSAGetLastError());
        return 1;
    }

    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(puerto);
    servidor.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Conectar al servidor
    if (connect(sock, (struct sockaddr*)&servidor, sizeof(servidor)) < 0) {
        printf("Error al conectar con el servidor\n");
        return 1;
    }

    printf("Bienvenido a DeustoShoes. Comandos disponibles: CONSULTAR, INSERTAR, BORRAR, BUSCAR, BUSCAR_TALLA, MODIFICAR, AYUDA, SALIR\n");

    while (1) {
        printf("Comando> ");
        fgets(mensaje, sizeof(mensaje), stdin);
        mensaje[strcspn(mensaje, "\n")] = 0;  // Quitar salto de línea

        // Enviar mensaje
        send(sock, mensaje, strlen(mensaje), 0);

        if (strcmp(mensaje, "SALIR") == 0) break;

        // Recibir respuesta
        int recibido = recv(sock, respuesta, sizeof(respuesta) - 1, 0);
        if (recibido > 0) {
            respuesta[recibido] = '\0';
            printf("Respuesta del servidor:\n%s\n", respuesta);
        } else {
            printf("El servidor cerró la conexión.\n");
            break;
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
