Webserv - Servidor HTTP Personalizado

Versión: 21.2
Descripción General

Webserv es una implementación personalizada de un servidor HTTP escrita en C++98. Sigue los principios fundamentales del Protocolo de Transferencia de Hipertexto (HTTP) y está diseñado para manejar solicitudes de clientes de manera eficiente. Este proyecto permite servir contenido estático, procesar solicitudes HTTP y soportar funcionalidades básicas de CGI.

El proyecto es un ejercicio educativo diseñado para profundizar en el conocimiento de HTTP y los mecanismos de los servidores web.
Funcionalidades

    Implementación completa de los elementos esenciales de HTTP/1.1:
        Soporte para los métodos GET, POST y DELETE.
        Compatibilidad con navegadores web modernos.
        Códigos de estado HTTP precisos en las respuestas.
    Arquitectura no bloqueante:
        Utiliza poll() o equivalentes (select(), epoll() o kqueue) para todas las operaciones de E/S.
    Configurable mediante un archivo de configuración personalizado.
    Páginas de error predeterminadas para rutas no coincidentes.
    Subida de archivos y capacidad para servir sitios web estáticos.
    Capacidad para escuchar en múltiples puertos.

Requisitos

    Compilador: g++ con las banderas -Wall -Wextra -Werror.
    Estándar: C++98.
    Prohibido:
        Librerías externas.
        Librerías Boost.
        Uso de fork() para propósitos distintos a CGI.

Instalación

    Clona el repositorio:

git clone <repository-url>
cd webserv

Compila el proyecto utilizando el Makefile proporcionado:

    make

Uso

Ejecuta el servidor con el siguiente comando:

./webserv [archivo_de_configuración]

