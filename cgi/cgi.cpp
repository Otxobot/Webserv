/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 15:53:04 by mikferna          #+#    #+#             */
/*   Updated: 2024/06/11 18:05:50 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"
std::string int_to_string(int value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

std::string runCGI(Request& _request, const std::string& root, const std::string& cgi_path) {
    char const *argv[2];
    int Ifd[2];
    int Ofd[2];
    pid_t pid;
    std::string content;
    extern char **environ;

    // Restablecer todas las variables de entorno relevantes
    setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
    setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
    setenv("SERVER_PORT", int_to_string(_request.getPort()).c_str(), 1);
    setenv("REQUEST_METHOD", _request.getMethod().c_str(), 1);
    setenv("SERVER_NAME", "site1.com", 1);
    setenv("REDIRECT_STATUS", "1", 1);
    setenv("PATH_INFO", (root + _request.getTarget()).c_str(), 1);
    setenv("SCRIPT_FILENAME", (root + _request.getTarget()).c_str(), 1);

    if (_request.getMethod() == "GET") {
        setenv("QUERY_STRING", _request.getQueryString().c_str(), 1);
        setenv("CONTENT_LENGTH", "0", 1);
    } else if (_request.getMethod() == "POST") {
        setenv("CONTENT_LENGTH", int_to_string(_request.getBody().size()).c_str(), 1);
        // Eliminar QUERY_STRING para solicitudes POST
        unsetenv("QUERY_STRING");
    }

    // Crear pipes
    if (pipe(Ifd) == -1 || pipe(Ofd) == -1) {
        perror("[CGI ERROR] PIPE");
        return "";
    }

    // Crear proceso hijo
    pid = fork();
    if (pid < 0) {
        perror("[CGI ERROR] FORK");
        return "";
    }
    if (pid == 0) { // Proceso hijo
        close(Ifd[1]);
        close(Ofd[0]);
        dup2(Ifd[0], 0); // Redirigir stdin al pipe
        dup2(Ofd[1], 1); // Redirigir stdout al pipe
        close(Ifd[0]);
        close(Ofd[1]);

        argv[0] = cgi_path.c_str();
        argv[1] = NULL;

        if (execve(argv[0], (char* const*)argv, environ) == -1) {
            perror("Error: Execve Can't run");
            exit(EXIT_FAILURE);
        }
    } else { // Proceso padre
        close(Ifd[0]);
        close(Ofd[1]);
        if (_request.getMethod() == "POST") {
            write(Ifd[1], _request.getBody().c_str(), _request.getBody().size());
        }
        close(Ifd[1]);

        int ret;
        char buffer[1024];
        while ((ret = read(Ofd[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[ret] = '\0';
            content += buffer;
        }
        close(Ofd[0]);
        waitpid(pid, NULL, 0);
    }

    return content;
}
