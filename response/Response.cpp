/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:00:31 by abasante          #+#    #+#             */
/*   Updated: 2024/05/29 16:34:27 by abasante         ###   ########.fr       */
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

void Response::createBody()
{
    std::string uri = this->_request.getTarget();
    Location our_location;

    if (uri.empty())
        uri.append("/");

    std::map<std::string, Location>::iterator it = this->_server._locations.find(uri);
    if (it == this->_server._locations.end())
    {
        std::cout << "por que coño no entra aqui " << std::endl;
        std::cout << "URI not found in locations: " << uri << std::endl;
        this->handle_SC_error(404);
        return ;
    }
    our_location = this->_server._locations[uri];
    std::string path = this->_server._root + "/" + our_location._file;
    std::ifstream file(path.c_str());
    if (!file.is_open())
    {
        std::cout << "Failed to open file: " << our_location._file << std::endl;
        this->handle_SC_error(500);
        return ;
    }
    else
    {
        std::ostringstream ss;
        ss << file.rdbuf();
        std::string htmlContent = ss.str();
        this->_body.append("Content-Length: ");
        std::ostringstream oss;
        oss.str("");
        oss << htmlContent.size();
        this->_body.append(oss.str());
        this->_body.append("\r\nConnection: close\r\n");
        this->_body.append("\r\n\r\n");
        this->_body.append(ss.str());
    }
}

// void Response::check_if_cgi(std::string uri)
// { // Assuming getURI() is a function that returns the URI of the request
//     std::size_t found = uri.find_last_of(".");
//     std::string extension = uri.substr(found+1);

//     // List of CGI extensions
//     std::vector<std::string> cgi_extensions = {"cgi", "pl", "py", "php"};

//     // Check if the extension is in the list of CGI extensions
//     _isCGI = std::find(cgi_extensions.begin(), cgi_extensions.end(), extension) != cgi_extensions.end();
// }

void Response::responseCreation(std::vector<Config> &servers, Request &request)
{
    time_t _time;
	std::string tm;
	time(&_time);
	tm = ctime(&_time);
    tm.erase(tm.length() - 1);

    this->_request = request;
    this->_servers = servers;
    this->_server = this->calibrate_host_location(this->_servers, this->_request);

    std::string protocol = request.getProtocol();
    std::string uri = this->_request.getTarget();
    std::string cgi = "/cgi";
    std::string method = this->_request.getMethod();
    this->_statusCode = this->_request.getStatusCode();

    //this->check_if_cgi(uri);

    // if (this->_request.getMethod() != "GET" && this->_request.getMethod() != "POST" && this->_request.getMethod() != "DELETE")
    // {
    //         this->_statusCode = 501;
    //         this->handle_SC_error(this->_statusCode);
    //         return ;
    // }
    if ((method != "GET" && method != "POST" && method != "DELETE") ||
    ((!this->_server._locations[uri]._allowGET && method == "GET") || (!this->_server._locations[uri]._allowDELETE && method == "DELETE") ||
    ((!this->_server._locations[uri]._allowPOST && method == "POST"))))
    {
        std::cout << "esta entrando aqui " << std::endl;
        this->_response.append(protocol);
        this->_response.append(" ");
        this->_statusCode = 405;
        this->handle_SC_error(this->_statusCode);
    }
    if (request.getMethod() == "GET")
    {
        this->_response.append(protocol);
        this->_response.append(" ");
        this->createBody();
        int number = this->_statusCode;
        if (number != 200)
            return ;
        std::ostringstream oss;
        oss << number;
        std::string status_code = oss.str();
        this->_response.append(status_code);
        this->_response.append(" OK\r\n");
        this->_response.append("Date: ");
        this->_response.append(tm);
        this->_response.append(" GMT\r\n");
        //aqui faltaria algo, un tipo de parseo del request o algo para saber que headers hay que meter en el response para cada caso diferente
        this->_response.append("Content-Type: ");
        this->_response.append("text/html\r\n");
        this->_response.append("Server: ");
        this->_response.append(this->_server._servername + "\r\n");
        this->_response.append(this->_body);
        std::cout << "=====================RESPONSE====================" << std::endl;
        std::cout << this->_response << std::endl;
    }
    if (request.getMethod() == "POST")
    {
        
    }
    if (request.getMethod() == "DELETE")
    {
        
    }
}
