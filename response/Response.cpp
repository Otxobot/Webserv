/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikferna <mikferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:00:31 by abasante          #+#    #+#             */
/*   Updated: 2024/05/29 17:07:49 by mikferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include <dirent.h>

Response::Response(): _headers(""), _statusCode(200)
{
    //Default constructor
}

Response::~Response()
{
    //Default destructor
}

std::string Response::getHeaders()
{
    return this->_headers;
}

std::string Response::getStatusCodeTranslate(int status_code)
{
    std::string status = "";
    switch (status_code)
    {
        case 200:
            status = "OK";
            break;
        case 301:
            status = "Moved Permanently\r\n";
            break;
        case 400:
            status = "Bad Request\r\n";
            break;
        case 403:
            status = "Forbidden\r\n";
            break;
        case 404:
            status = "Not Found\r\n";
            break;
        case 405:
            status = "Method Not Allowed\r\n";
            break;
        case 409:
            status = "Conflict\r\n";
            break;
        case 500:
            status = "Internal Server Error\r\n";
            break;
        case 501:
            status = "Not Implemented\n\r";
            break;
        case 502:
            status = "Bad Geteway\r\n";
            break;
        case 413:
            status = "Payload Too Large\r\n";
            break;
        case 505:
            status = "HTTP Version Not Supported\r\n";
            break;
        default:
            status = "Internal Server Error\r\n";
            break;
    }
    return status;
}

// void    Response::get_body(std::string file_name)
// {
//     std::ifstream file(file_name);
//     if (access(file_name.c_str(), F_OK) != 0) // Check if The file existe
//         return; 
//     else
//     {
// 	    if (file)
// 	    {
// 	        std::ostringstream ss;
// 	        ss << file.rdbuf();
// 	        this->_body = ss.str();
// 	    }
//         else
//             return ;
// 	    file.close(); // close the file(filename)
//     }
// }

Config Response::calibrate_host_location(std::vector<Config> _servers, Request _request)
{
    int i = 0;
    int size = _servers.size();
    while (i < size)
    {
        if (_servers[i]._port == _request.getPort())
        {
            std::cout << "SE HA CALIBRADO:" <<i<< std::endl;
            return (_servers[i]);
        }
        i++;
    }
    return _servers[i];
}

// void Response::enter_location(Config server, std::string uri)
// {
//     Location our_location;
//     if (uri.empty())
//         uri.append("/");
//     std::cout << server._port << std::endl;
//     our_location = server._locations[uri];
//     if (!our_location._file.empty())
//     {
//         std::cout <<"our_location._file->"<< our_location._file << std::endl;
//     }
// }


// int Response::check_for_statusCode()
// {

// }

void Response::handle_SC_error(int sc)
{
    std::ostringstream oss;
    oss << sc;
    std::string statusCodeStr = oss.str();

    std::string html = 
        "<!DOCTYPE html>"
        "<html>"
        "<head>"
        "<title>Error Page</title>"
        "<style>"
        "body { font-family: Arial, sans-serif; text-align: center; padding: 50px; }"
        "h1 { font-size: 50px; }"
        "p { font-size: 24px; }"
        "</style>"
        "</head>"
        "<body>"
        "<h1>Error page</h1>"
        "<p>Status code: " + statusCodeStr + "</p>"
        "<p>" + this->getStatusCodeTranslate(sc) + "</p>"
        "</body>"
        "</html>";

    // Print the HTML to the console (or to an HTTP response, etc.)
    this->_body = html;
    std::cout << html << std::endl;
    std::ostringstream len_stream;
    this->_response.append("Content-Type: ");
    this->_response.append("text/html\r\n");
            this->_response.append("Content-Length: ");
            len_stream << html.length();
            std::string content_length = len_stream.str();
            this->_response.append(content_length);
            this->_response.append("\r\nConnection: Closed\r\n");
            this->_response.append("\r\n\r\n");
            this->_response.append(html);
}

std::string getExtension(const std::string& fileName) {
    size_t pos = fileName.find_last_of('.');
    if (pos != std::string::npos && pos < fileName.size() - 1) {
        return fileName.substr(pos + 1);
    }
    return "";
}

std::string getContentType(const std::string& extension) {
    if (extension == "html" || extension == "htm") {
        return "text/html";
    } else if (extension == "jpg" || extension == "jpeg") {
        return "image/jpeg";
    } else if (extension == "png") {
        return "image/png";
    }
    return "application/octet-stream"; // predeterminado
}

void Response::createBody()
{
    std::string uri = this->_request.getTarget();
    Location our_location;

    if (uri.empty())
        uri.append("/");
    our_location = this->_server._locations[uri];
    std::string filePath = this->_server._root + "/" + our_location._file;
    std::cout << "filePath-- " << filePath << std::endl;
    std::cout << "our_location._file-- " << our_location._file << std::endl;
    std::ifstream file(filePath.c_str());
    if (file && !our_location._file.empty())
    {
        std::string extension = getExtension(our_location._file);
        std::string contentType = getContentType(extension);

        this->_response.append("Content-Type: ");
        this->_response.append(contentType);
        this->_response.append("\r\n");

        std::ostringstream ss;
        ss << file.rdbuf();
        this->_body = ss.str();
        file.close();

        std::ostringstream oss;
        oss << this->_body.length();
        this->_response.append("Content-Length: ");
        this->_response.append(oss.str());
        this->_response.append("\r\nConnection: Closed\r\n");
        this->_response.append("\r\n");
        this->_response.append(this->_body);
    }
    else
    {
        this->_statusCode = 404;
        this->handle_SC_error(this->_statusCode);
    }
}



void Response::responseCreation(std::vector<Config> &servers, Request &request)
{
    time_t _time;
    std::string tm;
    time(&_time);
    tm = ctime(&_time);
    std::string protocol = request.getProtocol();

    tm.erase(tm.length() - 1);
    this->_request = request;
    this->_servers = servers;
    this->_server = this->calibrate_host_location(this->_servers, this->_request);
    std::string uri = this->_request.getTarget();
    std::cout << "uritron-> " << uri << std::endl;
    std::cout << "server-> " << this->_request.getPort() << std::endl;
    this->_statusCode = this->_request.getStatusCode();
    
    std::string method = this->_request.getMethod();
    if ((method != "GET" && method != "POST" && method != "DELETE") ||
        ((!this->_server._locations[uri]._allowGET && method == "GET") || (!this->_server._locations[uri]._allowDELETE && method == "DELETE") ||
            ((!this->_server._locations[uri]._allowPOST && method == "POST")))) {
        this->_statusCode = 501;
    }

    if (method == "GET") {
        this->handleGetRequest(protocol, tm);
    }
    else if (method == "POST") {
        this->handlePostRequest(protocol, tm);
    }
    else if (method == "DELETE") {
        this->handleDeleteRequest(protocol, tm, uri);
    }
}

void Response::handleGetRequest(const std::string& protocol, const std::string& tm)
{
    this->_response.append(protocol);
    this->_response.append(" ");
    int number = this->_statusCode;
    std::ostringstream oss;
    oss << number;
    std::string status_code = oss.str();
    std::cout << status_code << std::endl;
    if (status_code == "200")
    {
        this->_response.append(status_code);
        this->_response.append(" OK\r\n");
    }
    else
    {
        this->_response.append(status_code);
        std::string message = this->getStatusCodeTranslate(number);
        this->_response.append(" ");
        this->_response.append(message);
        this->_response.append("Date: ");
        this->_response.append(tm);
        this->_response.append(" GMT\r\n");
        this->handle_SC_error(number);
        std::cout << this->_response << std::endl;
        return ;
    }
    this->_response.append("Date: ");
    this->_response.append(tm);
    this->_response.append(" GMT\r\n");
    this->_response.append("Content-Type: ");
    this->createBody();
    std::cout << this->_response << std::endl;
}

void Response::handlePostRequest(const std::string& protocol, const std::string& tm)
{
    this->_response.append(protocol);
    this->_response.append(" ");
    int number = this->_statusCode;
    std::ostringstream oss;
    oss << number;
    std::string status_code = oss.str();
    std::cout << status_code << std::endl;
    if (status_code == "200" || status_code == "201")
    {
        this->_response.append(status_code);
        this->_response.append(" OK\r\n"); // Agrega "OK" al estado
    }
    else
    {
        this->_response.append(status_code);
        std::string message = this->getStatusCodeTranslate(number);
        this->_response.append(" ");
        this->_response.append(message);
        this->_response.append("Date: ");
        this->_response.append(tm);
        this->_response.append(" GMT\r\n");
        this->handle_SC_error(number);
        std::cout << this->_response << std::endl;
        return;
    }
    this->_response.append("Date: ");
    this->_response.append(tm);
    this->_response.append(" GMT\r\n");

    // Handle saving the request data
    std::string uri = this->_request.getTarget();
    Location our_location;

    if (uri.empty())
        uri.append("/");
    our_location = this->_server._locations[uri];
    std::string filePath = this->_server._root;
    std::ofstream outFile(filePath.c_str());
    if (outFile)
    {
        std::cout << "ENTRO PERRO" << std::endl;
        std::cout << "body do be like-> " << this->_request.getBody() << std::endl;

        // Escribir el cuerpo de la solicitud en el archivo línea por línea
        // if (this->_request.getBody().size() > this->_server._buffer_size)
        // {
        //     this->_statusCode = 413;
        //     this->handle_SC_error(this->_statusCode);
        //     return;
        // }
        std::istringstream iss(this->_request.getBody());
        std::string line;
        while (std::getline(iss, line))
        {
            outFile << line << std::endl;
        }

        outFile.close();

        // Agrega Content-Length
        std::ostringstream len_stream;
        std::cout << "BODY ES -> ----" << this->_request.getBody() << "----" << std::endl;
        len_stream << this->_request.getBody().length();
        std::string content_length = len_stream.str();
        this->_response.append("Content-Length: ");
        this->_response.append(content_length); // Longitud del cuerpo
        this->_response.append("\r\n");

        this->_response.append("Connection: Closed\r\n");
        this->_response.append("\r\n");
    }
    else
    {
        std::cout << "ENTRO PERRO2" << std::endl;
        this->_statusCode = 500;
        this->handle_SC_error(this->_statusCode);
    }
    std::cout << "respuesta post-> ///////" << this->_response << "///////" << std::endl;
}

bool isDirectory(std::string path)
{
    DIR *dir;

    if ((dir = opendir(path.c_str())))
    {
        closedir(dir);
        return true;
    }
    return false;
}

void    Response::handleDeleteRequest(const std::string& protocol, const std::string& tm, std::string _Path)
{
    this->_response.append(protocol);
    this->_response.append(" ");
    if (isDirectory(_Path))
            this->handle_SC_error(409);
        else
        {
            if (access(_Path.c_str(), F_OK))
               this->handle_SC_error(500);
            else
            {
                if (!access(_Path.c_str(), W_OK))
                {
                    if (std::remove(_Path.c_str()))
                        this->handle_SC_error(500);
                    else
                    {
                        this->_response.append("204");
                        this->_response.append(" ");
                        this->_response.append("No Content\r\n");
                        this->_response.append("Date: ");
                        this->_response.append(tm);
                        this->_response.append(" GMT\r\n");
                        this->_response.append("Content-Length: 0\r\n");
                        this->_response.append("Connection: close\r\n\r\n");
                    }
                }
                else
                    this->handle_SC_error(403);
            }
        }
}
