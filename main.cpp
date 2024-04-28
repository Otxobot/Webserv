//#include "Webserv.hpp"
#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read
#include <cerrno>

int main() {
  // Create a socket (IPv4, TCP)
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    std::cout << "Failed to create socket. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  // Listen to port 8080 on any address
  sockaddr_in sockaddr;
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = INADDR_ANY;
  sockaddr.sin_port = htons(8080); // htons is necessary to convert a number to
                                   // network byte order

  if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) 
  {
    std::cout << "Failed to bind to port 8080. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  // Start listening. Hold at most 10 connections in the queue
  if (listen(sockfd, 10) < 0) 
  {
    std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  // Grab a connection from the queue
  int addrlen = sizeof(sockaddr);
  int connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
  if (connection < 0) 
  {
    std::cout << "Failed to grab connection. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  // Send a message to the connection
  std::string response =  "<!DOCTYPE html>\n<html lang=\"en\">\n\
	    				  <head>\n\
	    				  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\
	    				  <title>AUTO INDEX</title>\n\
	    				  </head>\n\
	    				  <body>\n\
	    				  <div style=\"margin-left: 5%; margin-top:10%;\">\n\
	    				  <hr>\n";;

  send(connection, response.c_str(), response.size(), 0);

  // Read from the connection
  char buffer[1000];
  read(connection, buffer, 1000);
  std::cout << "The message was: \n" << buffer;


  // Close the connections
  close(connection);
  close(sockfd);
}
