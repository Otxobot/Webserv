/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:00:43 by abasante          #+#    #+#             */
/*   Updated: 2024/05/21 16:07:34 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../config/Config.hpp"
#include "../config/Location.hpp"
#include "../request/Request.hpp"

// #include <unistd.h>
// #include <fstream>
// #include <sstream>
// #include <sys/stat.h>
// #include <dirent.h>
#include <string>

#define DEFAULT "/index.html"

class Config;
class Location;
class Request;

class Response
{
    private:
        std::vector<Config> _servers;
        Config  _server;
        Request _request;
        std::string _body;
        std::string _statusLine;
        std::string _headers;
        std::string _response;
        int _statusCode;
        bool _isCGI;
        bool _isLocation;
    public:
        Response();
        ~Response();
        void        responseCreation(std::vector<Config> &servers, Request &request);
        std::string getHeaders();
        std::string getStatusCodeTranslate();
        void        makeBody();

};

#endif