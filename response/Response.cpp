/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:00:31 by abasante          #+#    #+#             */
/*   Updated: 2024/05/22 12:52:08 by abasante         ###   ########.fr       */
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
    for (std::map<std::string, Location>::iterator it = locations.begin(); it != locations.end() ; it++)
    {
        all_locations.push_back(it->second);
    }
    
}

void Response::responseCreation(std::vector<Config> &servers, Request &request)
{
    time_t _time;
	std::string tm;
	time(&_time);
	tm = ctime(&_time);

    tm.erase(tm.length() - 1);
    this->_request = request;
    this->_servers = servers;
    this->makeBody();
    
}