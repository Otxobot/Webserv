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

	int optval = 1;
	setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

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
		//char *buffer = new char[1024];
		//// Recibir la petición HTTP del cliente
		//read(clientSocket, buffer, 1024);
		//std::cout << "La petición fue: \n" << buffer << std::endl;
        //std::cout << "Cliente conectado." << std::endl;

        // Leer el contenido del archivo HTML
        std::string htmlContent = readHtmlFile("index.html");

        // Enviar la respuesta HTTP con el contenido HTML al cliente
        std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(htmlContent.size()) + "\r\n\r\n" + htmlContent;
        send(clientSocket, response.c_str(), response.size(), 0);

        // Cerrar la conexión con el cliente
        close(clientSocket);
    }

    // Cerrar el socket del servidor
    close(serverSocket);

    return 0;
}
