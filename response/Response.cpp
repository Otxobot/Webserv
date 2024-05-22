/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:00:31 by abasante          #+#    #+#             */
/*   Updated: 2024/05/22 15:59:53 by abasante         ###   ########.fr       */
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

std::string Response::getStatusCodeTranslate()
{
    std::string status = "";
    switch (this->_statusCode)
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

void Response::makeBody()
{
    std::string path_to_requested = this->_request.getTarget();
    this->_isLocation = false;
    this->_isCGI = false;

    if (path_to_requested.empty())
        path_to_requested.append("/");
    for (size_t i = 0; i < this->_servers.size(); i++)
    {
        if (this->_servers[i].getPort() == this->_request.getPort())
        {
            this->_server = this->_servers[i];
            break;
        }
    }
    std::map<std::string, Location> locations = this->_server._locations;
    
    std::vector<Location> all_locations;
    
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
    //this->makeBody();
    if (request.getMethod() == "GET")
    {
        std::cout << "entro en el trozo del response de GET" << std::endl;
        //en caso de que el get estuviera accediendo a un archivo que si puede coger
        this->_response.append(protocol);
        this->_response.append(" ");
        int number = this->_statusCode;
        std::ostringstream oss;
        oss << number;
        std::string status_code = oss.str();
        this->_response.append(status_code);
        this->_response.append(" OK\r\n");
        this->_response.append("Date: ");
        this->_response.append(tm);
        this->_response.append(" GMT\r\n");
        this->_response.append("Content-Type: ");
        this->_response.append("");
        std::cout << "response:\n" << this->_response << std::endl;
        
    }
    if (request.getMethod() == "POST")
    {
        
    }
    if (request.getMethod() == "DELETE")
    {
        
    }
}