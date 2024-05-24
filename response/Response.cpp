/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:00:31 by abasante          #+#    #+#             */
/*   Updated: 2024/05/23 16:18:21 by abasante         ###   ########.fr       */
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

void Response::enter_location(Config server, std::string uri)
{
    Location our_location;
    if (uri.empty())
        uri.append("/");
    std::cout << server._port << std::endl;
    our_location = server._locations[uri];
    if (!our_location._file.empty())
    {
        std::cout <<"our_location._file->"<< our_location._file << std::endl;
    }
}


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
    this->_response.append("Content-Type: ");
    this->_response.append("text/html\r\n");
            this->_response.append("Content-Length: ");
            this->_response.append("\r\nConnection: Closed\r\n");
            this->_response.append("\r\n\r\n");
            this->_response.append(html);
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
    std::cout << uri << std::endl;
    this->enter_location(this->_server, uri);
    this->_statusCode = this->_request.getStatusCode();
    if (this->_request.getMethod() != "GET" && this->_request.getMethod() != "POST" && this->_request.getMethod() != "DELETE")
    {
            this->_statusCode = 501;
            this->handle_SC_error(this->_statusCode);
    }
    // else
    // {
    //     this->_statusCode = 405;
    //     this->handle_SC_error(this->_statusCode);
    // }
    if (request.getMethod() == "GET")
    {
        this->_response.append(protocol);
        this->_response.append(" ");
        
        //this->check_for_statusCode();
        int number = this->_statusCode;
        //int number = 505;
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
        this->_response.append("text/html\r\n");
        std::cout << this->_response << std::endl;
        this->createBody();
        // std::ifstream file1("./html/index.html");
        // if (file1)
        // {
        //     std::ostringstream ss;
        //     ss << file1.rdbuf();
        //     std::string htmlContent = ss.str();
        //     this->_response.append("Content-Length: ");
        //     oss.str("");
        //     oss << htmlContent.size();
        //     this->_response.append(oss.str());
        //     this->_response.append("\r\nConnection: Closed\r\n");
        //     this->_response.append("\r\n\r\n");
        //     this->_response.append(ss.str());
        // }else{
        //     std::cerr << "Error opening file" << std::endl;
        // }

    }
    if (request.getMethod() == "POST")
    {
        
    }
    if (request.getMethod() == "DELETE")
    {
        
    }
}