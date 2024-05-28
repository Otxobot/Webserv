/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:00:31 by abasante          #+#    #+#             */
/*   Updated: 2024/05/22 18:12:19 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <dirent.h>

Response::Response() : statusCode(200) {
    initStatusMessages();
}

void Response::initStatusMessages() {
    statusMessages[200] = "OK";
    statusMessages[201] = "Created";
    statusMessages[202] = "Accepted";
    statusMessages[204] = "No Content";
    statusMessages[301] = "Moved Permanently";
    statusMessages[302] = "Found";
    statusMessages[400] = "Bad Request";
    statusMessages[401] = "Unauthorized";
    statusMessages[403] = "Forbidden";
    statusMessages[404] = "Not Found";
    statusMessages[405] = "Method Not Allowed";
    statusMessages[500] = "Internal Server Error";
    statusMessages[501] = "Not Implemented";
    statusMessages[502] = "Bad Gateway";
    statusMessages[503] = "Service Unavailable";
}

std::string Response::generateStatusLine() {
    std::ostringstream oss;
    oss << "HTTP/1.1 " << statusCode << " " << statusMessages[statusCode] << "\r\n";
    return oss.str();
}

std::string Response::generateHeaders() {
    std::ostringstream oss;
    for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it) {
        oss << it->first << ": " << it->second << "\r\n";
    }
    oss << "\r\n";
    return oss.str();
}

std::string Response::readFile(const std::string& path) {
    std::ifstream file(path.c_str());
    if (!file.is_open()) {
        statusCode = 404;
        return "404 Not Found";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string Response::directoryListing(const std::string& path) {
    std::ostringstream oss;
    oss << "<html><body><h1>Directory listing for " << path << "</h1><ul>";
    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir(path.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            oss << "<li><a href=\"" << ent->d_name << "\">" << ent->d_name << "</a></li>";
        }
        closedir(dir);
    }
    oss << "</ul></body></html>";
    return oss.str();
}

std::string Response::getContentType(const std::string& path) {
    size_t dot = path.find_last_of('.');
    if (dot == std::string::npos) return "text/plain";
    std::string ext = path.substr(dot + 1);
    if (ext == "html" || ext == "htm") return "text/html";
    if (ext == "css") return "text/css";
    if (ext == "js") return "application/javascript";
    if (ext == "png") return "image/png";
    if (ext == "jpg" || ext == "jpeg") return "image/jpeg";
    if (ext == "gif") return "image/gif";
    return "text/plain";
}

std::string Response::generateResponse(Request& request, const std::string& root) {
    std::string target = root + request.getTarget();
    struct stat info;
    std::string method = request.getMethod();

    if (stat(target.c_str(), &info) != 0) {
        statusCode = 404;
        body = "404 Not Found";
    } else if (S_ISDIR(info.st_mode)) {
        if (method == "GET") {
            body = directoryListing(target);
            headers["Content-Type"] = "text/html";
        } else {
            statusCode = 405;
            body = "405 Method Not Allowed";
        }
    } else {
        if (method == "GET") {
            body = readFile(target);
            headers["Content-Type"] = getContentType(target);
        } else if (method == "POST") {
            // Assuming CGI scripts are used for POST methods
            std::string cgi_path = "./cgi/cgiscript.py"; // Define the correct path to your CGI script here
            body = runCGI(request, root, cgi_path);
            headers["Content-Type"] = "text/html"; // Assuming CGI returns HTML
        } else if (method == "DELETE") {
            if (remove(target.c_str()) == 0) {
                statusCode = 204;
                body = "204 No Content";
            } else {
                statusCode = 500;
                body = "500 Internal Server Error";
            }
        } else {
            statusCode = 405;
            body = "405 Method Not Allowed";
        }
    }

    std::ostringstream contentLengthStream;
    contentLengthStream << body.size();
    headers["Content-Length"] = contentLengthStream.str();

    return generateStatusLine() + generateHeaders() + body;
}
