#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

std::string readHtmlFile(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if (!file) {
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void handleRequest(int clientSocket) {
    // Leer la solicitud del cliente
    char request[1024];
    recv(clientSocket, request, sizeof(request), 0);

    // Comprobar si la solicitud es para la página principal
    if (strstr(request, "GET /index.html") != nullptr) {
        // Leer el contenido de la página index.html
        std::string htmlContent = readHtmlFile("index.html");

        // Enviar la respuesta HTTP con el contenido HTML al cliente
        std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(htmlContent.size()) + "\r\n\r\n" + htmlContent;
        send(clientSocket, response.c_str(), response.size(), 0);
    }
    // Comprobar si la solicitud es para redirigir a Google
    else if (strstr(request, "GET /google") != nullptr) {
        // Redirigir al cliente a google.com
        std::string redirectResponse = "HTTP/1.1 302 Found\r\nLocation: http://www.google.com\r\n\r\n";
        send(clientSocket, redirectResponse.c_str(), redirectResponse.size(), 0);
    }
    // Comprobar si la solicitud es para generar un error 403
    else if (strstr(request, "GET /error") != nullptr) {
        // Devolver un error 403 Forbidden
        std::string forbiddenResponse = "HTTP/1.1 403 Forbidden\r\n\r\n";
        send(clientSocket, forbiddenResponse.c_str(), forbiddenResponse.size(), 0);
    }
    // Si la solicitud no es para ninguna página conocida, devolver un error 404
    else {
        std::string notFoundResponse = "HTTP/1.1 404 Not Found\r\n\r\n";
        send(clientSocket, notFoundResponse.c_str(), notFoundResponse.size(), 0);
    }

    // Cerrar la conexión con el cliente
    close(clientSocket);
}


int main() {
    // Crear un socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error al crear el socket." << std::endl;
        return 1;
    }

    // Configurar la dirección del servidor
    struct sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(8080);

    // Enlazar el socket a la dirección
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error al enlazar el socket." << std::endl;
        close(serverSocket);
        return 1;
    }

    // Escuchar por conexiones entrantes
    if (listen(serverSocket, 10) == -1) {
        std::cerr << "Error al poner el socket en modo de escucha." << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Servidor escuchando en el puerto 8080..." << std::endl;

    while (true) {
        // Aceptar una conexión entrante
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            std::cerr << "Error al aceptar la conexión." << std::endl;
            close(serverSocket);
            return 1;
        }

        std::cout << "Cliente conectado." << std::endl;

        // Manejar la solicitud del cliente
        handleRequest(clientSocket);
    }

    // Cerrar el socket del servidor
    close(serverSocket);

    return 0;
}
