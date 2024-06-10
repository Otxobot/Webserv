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
#include <fstream>
#include <cstdlib>
#include <sys/stat.h>
#include <sstream>

Response::Response(): _headers(""), _statusCode(200)
{
    //Default constructor
}

Response::~Response()
{
    //Default destructor
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

std::string Response::getHeaders()
{
    return this->_headers;
}

std::string Response::getUploadPath() {
    std::string uploadPath = this->_server._root + "/uploaded_files";

    // Verificar si la carpeta existe
    struct stat info;
    if (stat(uploadPath.c_str(), &info) != 0) {
        // Si no existe, intenta crear la carpeta
        int result = system(("mkdir -p " + uploadPath).c_str());
        if (result != 0) {
            // Si la creación falla, lanzar una excepción o manejar el error de alguna otra manera
            throw std::runtime_error("Error: Unable to create directory");
        }
        std::cout << "Directory created: " << uploadPath << std::endl;
    }

    return uploadPath;
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
        case 413:
            status = "Payload Too Large\r\n";
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

Config Response::calibrate_host_location(std::vector<Config> &_servers, Request &_request)
{
    int i = 0;
    int size = _servers.size();
    std::cout << "PORT--->" << _request.getPort() << std::endl;
    while (i < size)
    {
        if (_servers[i]._port == _request.getPort())
        {
            std::cout << "SE HA CALIBRADO:" <<i<< std::endl;
            std::cout << _servers[i]._port << std::endl;
            return (_servers[i]);
        }
        i++;
    }
    std::cout << "NO SE ESTA CALIBRANDO:" <<i<< std::endl;
    return (_servers[i]);
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

std::string readFileToString(const std::string& filename) {
    std::ifstream file(filename.c_str()); // c_str() needed for C++98
    if (file) {
        std::ostringstream ss;
        ss << file.rdbuf();
        std::string content = ss.str();
        if (content.empty()) {
            std::cerr << "Warning: File content is empty" << std::endl;
        }
        return content;
    } else {
        std::cerr << "Error: Could not open the file " << filename << std::endl;
        return "<!DOCTYPE html>\n"
               "<html>\n"
               "<head>\n"
               "<title>Error Page</title>\n"
               "<style>\n"
               "body { font-family: Arial, sans-serif; text-align: center; padding: 50px; }\n"
               "h1 { font-size: 50px; }\n"
               "p { font-size: 24px; }\n"
               "</style>\n"
               "</head>\n"
               "<body>\n"
               "<h1>Error page</h1>\n"
               "<p>Status code: 500 </p>\n"
               "<p> Internal Server Error\r\n </p>\n"
               "</body>\n"
               "</html>\n";
    }
}

void Response::handle_SC_error(int sc)
{
    this->_response.append("HTTP/1.1");
    this->_response.append(" ");
    this->_statusCode = sc;
    std::ostringstream oss;
    oss << sc;
    std::string statusCodeStr = oss.str();
    std::string html;
    if (this->_server._errorpage.count(sc)) {
    std::string filename = this->_server._root + "/" + this->_server._errorpage[sc];
    html = readFileToString(filename);
    }
    else {
    html = 
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
    }
    std::ostringstream ossSize;
    ossSize << html.size();
    std::string html_size = ossSize.str();
    this->_body = html;
    this->_response.append(oss.str());
    this->_response.append(" " + this->getStatusCodeTranslate(sc) + "Content-Type: ");
    this->_response.append("text/html\r\n");
            this->_response.append("Content-Length: ");
            this->_response.append(html_size);
            this->_response.append("\r\nConnection: Closed");
            this->_response.append("\r\n\r\n");
            this->_response.append(_body);
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
    if (this->_server._port == 0)
    {
        std::cout << "entro aqui\n";
        this->_statusCode = 404;
        this->handle_SC_error(this->_statusCode);
        return;
    }
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
    std::cout << "status_code-> " << status_code << std::endl;
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
        //std::cout << this->_response << std::endl;
        return ;
    }
    this->_response.append("Date: ");
    this->_response.append(tm);
    this->_response.append(" GMT\r\n");
    this->_response.append("Content-Type: ");
    this->createBody();
    //std::cout << this->_response << std::endl;
}

std::streampos getFileSize(const std::string& filePath) {
    std::ifstream file(filePath.c_str(), std::ios::binary | std::ios::ate);
    if (file.is_open()) {
        std::streampos fileSize = file.tellg();
        file.close();
        return fileSize;
    } else {
        std::cerr << "Unable to open file: " << filePath << std::endl;
        return -1;
    }
}

// void    Response::PostMethod()
// {
//     std::string file_path;
//     std::string buffer;

//     // if (!this->_server._root + "/uploaded_files")
//     //     this->handle_SC_error(401);
    
//     if (1)
//     {
//         file_path = getUploadPath();
//         std::cout << "file_path---> " << file_path << std::endl;
//         if (!isDirectory(file_path))
//             this->handle_SC_error(404);
//         else
//         {
//             std::string fileName = file_path + '/' + this->_request.headers["name"];
//             std::ifstream fileCheck(fileName.c_str());
//             if (fileCheck.is_open())
//             {
//                 fileCheck.close();
//                 this->handle_SC_error(403);
//                 return;
//             }
//             else
//             {
//                 std::ofstream file(fileName.c_str());
//                 std::stringstream ss(this->_request.headers["value"]);
//                 while(std::getline(ss, buffer))
//                     file << buffer.append("\n");
//                 file.close();
//             }
//         }
//     }
// }

void Response::handlePostRequest(const std::string& protocol, const std::string& tm) {
    this->_response.append(protocol);
    this->_response.append(" ");
    int number = this->_statusCode;
    std::ostringstream oss;
    oss << number;
    std::string status_code = oss.str();
    std::cout << status_code << std::endl;
    
    if (status_code == "200" || status_code == "201") {
        this->_response.append(status_code);
        this->_response.append(" OK\r\n");
        this->_response.append("Date: ");
        this->_response.append(tm);
        this->_response.append(" GMT\r\n");
    } else {
        this->_response.append(status_code);
        std::string message = this->getStatusCodeTranslate(number);
        this->_response.append(" ");
        this->_response.append(message);
        this->_response.append("\r\nDate: ");
        this->_response.append(tm);
        this->_response.append(" GMT\r\n");
        this->handle_SC_error(number);
        //std::cout << this->_response << std::endl;
        return;
    }

    // Handle saving the request data
    std::string uri = this->_request.getTarget();
    Location our_location;

    if (uri.empty())
        uri.append("/");
    our_location = this->_server._locations[uri];

    // Detect the content type and file extension
    std::string contentType = this->_request.headers["Content-Type"];
    std::cout << "Content-Type----> " << contentType << std::endl;
    // std::cout << "this->_request.headers[name]-> " << this->_request.headers["name"] << std::endl;
    // std::cout << "this->_request.headers[content-type]-> " << this->_request.headers["ContentType"] << std::endl;
    // std::cout << "this->_request.headers[value]-> " << this->_request.headers["value"] << std::endl;
    std::string fileExtension;

    if (contentType == "text/html") {
        fileExtension = ".html";
    } else if (contentType == "text/plain") {
        fileExtension = ".txt";
    } else if (contentType == "image/jpeg") {
        fileExtension = ".jpg";
    } else if (contentType == "image/png") {
        fileExtension = ".png";
    } else if (contentType == "application/pdf") {
        fileExtension = ".pdf";
    } else {
        std::cout << "Unsupported Media Type" << std::endl;
        this->_statusCode = 415; // Unsupported Media Type
        this->handle_SC_error(this->_statusCode);
        return;
    }

    // Construct the file path
    std::string filePath = this->_server._root + "/uploaded_file" + fileExtension;
    std::cout << "filePath---> " << filePath << std::endl;
    std::ofstream outFile(filePath.c_str(), std::ios::binary);

    if (outFile.is_open()) {
        std::cout << "get body length-> " << this->_request.getBodyLength() << std::endl;

        // Write body to file
        outFile.write(this->_request.headers["value"].c_str(), this->_request.headers["value"].size());
        outFile.close();

        std::ostringstream len_stream;
        len_stream << getFileSize(filePath);
        std::string content_length = len_stream.str();
        this->_response.append("Content-Length: ");
        this->_response.append(content_length);
        this->_response.append("\r\n");
        this->_response.append("Connection: Closed\r\n");
        this->_response.append("\r\n");
    } else {
        this->_statusCode = 500;
        this->handle_SC_error(this->_statusCode);
    }
    // std::ofstream textFile("/home/mikferna/Desktop/Webserv/html/primera_pagina/testeo.txt");
    // if (textFile.is_open()) {
    //     textFile << this->_request.headers["value"];
    //     textFile.close();
    // } else {
    //     std::cout << "Error opening the file" << std::endl;
    //     // Handle error opening the file
    // }
    //std::cout << this->_request.headers["value"].size() << std::endl;
    std::cout << "\n\n" << this->_request.headers["value"] << "\n\nLAGARTO\n" << std::endl;
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
