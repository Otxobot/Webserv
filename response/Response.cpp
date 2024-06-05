/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:00:31 by abasante          #+#    #+#             */
/*   Updated: 2024/06/03 16:47:36 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

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

Config Response::calibrate_host_location(std::vector<Config> _servers, Request _request)
{
    int i = 0;
    int size = _servers.size();
    while (i < size)
    {
        if (_servers[i]._port == _request.getPort())
        {
            return (_servers[i]);
        }
        i++;
    }
    return _servers[i];
}

void Response::handle_SC_error(int sc)
{
    this->_response.append("HTTP/1.1");
    this->_response.append(" ");
    this->_statusCode = sc;
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

    std::ostringstream ossSize;
    ossSize << html.size();
    std::string html_size = ossSize.str();
    // Print the HTML to the console (or to an HTTP response, etc.)
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

void Response::responseCreation(std::vector<Config> &servers, Request &request)
{
    this->_request = request;
    this->_servers = servers;
    this->_server = this->calibrate_host_location(this->_servers, this->_request);
    
    std::string uri = this->_request.getTarget();
    std::string method = this->_request.getMethod();
    this->_statusCode = this->_request.getStatusCode();
   
    if ((method != "GET" && method != "POST" && method != "DELETE") ||
    ((!this->_server._locations[uri]._allowGET && method == "GET") || (!this->_server._locations[uri]._allowDELETE && method == "DELETE") ||
    ((!this->_server._locations[uri]._allowPOST && method == "POST"))))
    {
        std::cout << "ENTRA AQUI"  << std::endl;
        this->handle_SC_error(this->_statusCode);
        return ;
    }

    // else if (this->_request.getTarget() == cgi && method == "POST")
    // {
    //     this->handle_POST_CGI();
    //     return ;
    // }
    std::cout << "querrryyyy->" << this->_request.getQueryString() << std::endl;
    if (this->_request.getQueryString().find("=") != std::string::npos && method == "GET")
    {
        if (this->handle_GET_CGI())
        {
            perror("py scritp couldn't be executed");
            //return ;
        }
    }

    if (request.getMethod() == "GET")
    {
        if (!this->_server._locations[uri]._redirect.empty())
        {
            std::string redirect = this->_server._locations[uri]._redirect;
            redirect.erase(0, redirect.find_first_not_of(' '));       // leading spaces
            redirect.erase(redirect.find_last_not_of(' ') + 1);
            std::cout << "TIENE REDIRECCION" << std::endl;
            this->_response.append("HTTP/1.1");
            this->_response.append(" ");
            this->_statusCode = 301;
            this->_response.append("301");
            this->_response.append(" ");
            this->_response.append(getStatusCodeTranslate(301));
            this->_response.append("Location: ");
            this->_response.append(redirect);
            this->_response.append("\r\nContent-Type: text/html; charset=UTF-8\r\n");
            this->_response.append("Content-Length: 0\r\n");
            this->_response.append("Connection: close");
            this->_response.append("\r\n\r\n");
            return ;
        }
        else
        {
            this->handle_GET();
        }
    }
    // if (request.getMethod() == "POST")
    //     this->handle_POST();
    if (request.getMethod() == "DELETE")
        this->handle_DELETE();
}

void Response::handle_GET()
{
    time_t _time;
	std::string tm;
	time(&_time);
	tm = ctime(&_time);
    tm.erase(tm.length() - 1);

    std::string contentType;
    if (this->_server._locations[this->_request.getTarget()]._file.find(".html") != std::string::npos)
        contentType = "text/html\r\n";
    else if (this->_server._locations[this->_request.getTarget()]._file.find(".txt") != std::string::npos)
        contentType = "text/plain\r\n";
    this->createBody();
    int number = this->_statusCode;
    if (number != 200)
        return ;
    this->_response.append("HTTP/1.1");
    this->_response.append(" ");
    std::ostringstream oss;
    oss << number;
    std::string status_code = oss.str();
    this->_response.append(status_code);
    this->_response.append(" OK\r\n");
    this->_response.append("Date: ");
    this->_response.append(tm);
    this->_response.append(" GMT\r\n");
    this->_response.append("Content-Type: ");
    this->_response.append(contentType);
    this->_response.append("Server: ");
    this->_response.append(this->_server._servername + "\r\n");
    this->_response.append(this->_body);
    std::cout << "=====================RESPONSE====================" << std::endl;
    std::cout << this->_response << std::endl;
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

void Response::handle_DELETE()
{
    time_t _time;
	std::string tm;
	time(&_time);
	tm = ctime(&_time);
    tm.erase(tm.length() - 1);

    this->_response.append("HTTP/1.1");
    this->_response.append(" ");
    std::string _path = this->_request.getTarget();
    if (isDirectory(_path))
            this->handle_SC_error(409);
        else
        {
            if (access(_path.c_str(), F_OK))
               this->handle_SC_error(500);
            else
            {
                if (!access(_path.c_str(), W_OK))
                {
                    if (std::remove(_path.c_str()))
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

void Response::createBody()
{
    std::string uri = this->_request.getTarget();
    std::cout << uri << std::endl;
    uri.erase(0, uri.find_first_not_of(' '));
    uri.erase(uri.find_last_not_of(' ') + 1);
    Location our_location;

    if (uri.empty())
        uri.append("/");

    our_location = this->_server._locations[uri];
    std::string path;
    if (!our_location._file.empty())
        path = this->_server._root + "/" + our_location._file;
    else
        path = this->_server._root + uri;
    std::cout << "path-> " << path << std::endl;
    std::ifstream file(path.c_str());
    if (!file.is_open())
    {
        this->handle_SC_error(404);
        return ;
    }
    else
    {
        std::ostringstream ss;
        ss << file.rdbuf();
        std::string Content = ss.str();
        this->_body.append("Content-Length: ");
        std::ostringstream oss;
        oss.str("");
        oss << Content.size();
        this->_body.append(oss.str());
        this->_body.append("\r\nConnection: close\r\n");
        this->_body.append("\r\n\r\n");
        this->_body.append(ss.str());
    }
}
