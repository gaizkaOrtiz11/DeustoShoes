DeustoShoes - Sistema de Gestión de Inventario
DeustoShoes es una aplicación Cliente-Servidor diseñada para gestionar un inventario de zapatillas. El sistema permite realizar operaciones CRUD (Crear, Leer, Actualizar, Borrar) sobre una base de datos SQLite a través de una conexión TCP por Sockets.

🛠️ Requisitos Previos
Dado que el código utiliza la librería <winsock2.h>, este proyecto está diseñado para ejecutarse en entornos Windows.

Necesitas tener instalado:

Compilador GCC/G++ (MinGW es recomendado para Windows).

Make (para automatizar la compilación).
⚙️ Compilación
El proyecto está dividido en dos partes que deben compilarse por separado.

1. Compilar el Servidor
Navega a la carpeta del servidor y ejecuta make:
cd servidor_cpp
make

2. Compilar el Cliente
cd cliente_c
make

🚀 Ejecución
Para utilizar el sistema, primero debes iniciar el servidor y luego conectar el cliente.
Paso 1: Iniciar el Servidor
En una terminal, dentro de la carpeta servidor_cpp:
./servidor.exe

Paso 2: Iniciar el Cliente
Abre una nueva terminal (manteniendo la del servidor abierta), ve a la carpeta cliente_c y ejecuta:
./cliente.exe


📖 Comandos Disponibles
Una vez que el cliente esté conectado, puedes utilizar los siguientes comandos para interactuar con el sistema:
Comando,Descripción,Ejemplo de Uso
CONSULTAR,Muestra todas las zapatillas en la base de datos.,CONSULTAR
INSERTAR,Añade una nueva zapatilla.,INSERTAR Nike AirMax 42 120.50
BORRAR,Elimina una zapatilla por su ID.,BORRAR 1
BUSCAR,Busca zapatillas por marca.,BUSCAR Adidas
BUSCAR_TALLA,Busca zapatillas por talla específica.,BUSCAR_TALLA 40
MODIFICAR,Actualiza los datos de una zapatilla existente.,MODIFICAR 1 Nike AirForce 43 130
AYUDA,Muestra la lista de comandos disponibles.,AYUDA
SALIR,Cierra la conexión y termina el cliente.,SALIR



📋 Características
Arquitectura Cliente-Servidor:

Servidor (C++): Gestiona la lógica de negocio, la conexión con la base de datos y atiende múltiples peticiones secuenciales.

Cliente (C): Interfaz de consola ligera que envía comandos al servidor y muestra las respuestas.

Persistencia de Datos: Utiliza SQLite para almacenar la información de las zapatillas (Marca, Modelo, Talla, Precio).

Comunicación: Uso de Sockets de Windows (Winsock2) mediante el protocolo TCP/IP en el puerto 5000.


