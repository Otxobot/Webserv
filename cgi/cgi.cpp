/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 15:53:04 by mikferna          #+#    #+#             */
/*   Updated: 2024/05/28 11:57:10 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"
std::string int_to_string(int value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}
std::string runCGI(Request& _request, const std::string& root, const std::string& cgi_path) {
    char const *argv[3];
    int Ifd[2];
    pid_t pid;
    std::string content;
    extern char **environ;
    // Establecer variables de entorno
    setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
    setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
    setenv("SERVER_PORT", int_to_string(_request.getPort()).c_str(), 1);
    setenv("REQUEST_METHOD", _request.getMethod().c_str(), 1);
    setenv("SERVER_NAME", "webserv", 1);
    setenv("REDIRECT_STATUS", "1", 1);
    setenv("PATH_INFO", (root + _request.getTarget()).c_str(), 1);
    setenv("SCRIPT_FILENAME", (root + _request.getTarget()).c_str(), 1);
    if (!_request.getReqValue("Cookie").empty()) {
        setenv("HTTP_COOKIE", _request.getReqValue("Cookie").c_str(), 1);
    }
    if (_request.getMethod() == "GET") {
        setenv("QUERY_STRING", _request.getQueryString().c_str(), 1);
        setenv("CONTENT_LENGTH", "0", 1);
    }
    // Crear pipe
    if (pipe(Ifd)) {
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
        close(Ifd[0]);
        dup2(Ifd[1], 1); // Redirigir stdout al pipe
        close(Ifd[1]);
        argv[0] = cgi_path.c_str();
        argv[1] = (root + _request.getTarget()).c_str();
        argv[2] = NULL;
        if (execve(argv[0], (char* const*)argv, environ) == -1) {
            perror("Error: Execve Can't run");
            exit(EXIT_FAILURE);
        }
    } else { // Proceso padre
        close(Ifd[1]);
        int ret;
        char buffer[1024];
        while ((ret = read(Ifd[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[ret] = '\0';
            content += buffer;
        }
        close(Ifd[0]);
        waitpid(pid, NULL, 0);
    }
    return content;
}