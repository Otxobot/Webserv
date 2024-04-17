
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

const int PORT = 8080;
const int BACKLOG = 500;
const int BUFFER_SIZE = 1024;

int main() {
    // Create socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Socket creation failed: " << strerror(errno) << std::endl;
        return 1;
    }

    // Set socket options
    // int opt = 1;
    // if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
    //     std::cerr << "Setsockopt failed: " << strerror(errno) << std::endl;
    //     return 1;
    // }

    // Bind socket to address and port
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1) 
    {
        std::cerr << "Bind failed: " << strerror(errno) << std::endl;
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_fd, BACKLOG) == -1)
    {
        std::cerr << "Listen failed: " << strerror(errno) << std::endl;
        return 1;
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    // Accept incoming connection
    int client_socket;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_socket == -1) 
    {
        std::cerr << "Accept failed: " << strerror(errno) << std::endl;
        return 1;
    }
    // Receiving data from a client
    char buffer[1024] = {0};
    recv(client_socket, buffer, sizeof(buffer), 0);
    std::cout << "Message from client: " << buffer << std::endl;
    // Respond to client
    // const char *message = "Hello, client! This is the server.";
    // if (send(client_socket, message, strlen(message), 0) == -1) 
    // {
    //     std::cerr << "Send failed: " << strerror(errno) << std::endl;
    // }

    // Close sockets
    // close(client_socket);
    // close(server_fd);

    return 0;
}
