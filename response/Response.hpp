/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:00:43 by abasante          #+#    #+#             */
/*   Updated: 2024/06/12 16:22:22 by abasante         ###   ########.fr       */
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
#include <sstream>
#include <dirent.h>

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
        std::string _statusLine;
        std::string _headers;
        int _statusCode;
        bool _isCGI;
        bool _isLocation;
    public:
        std::string _body;
        std::string _response;
        Response();
        ~Response();
        void        responseCreation(std::vector<Config> &servers, Request &request);
        std::string getHeaders();
        std::string getStatusCodeTranslate(int status_code);
        void        createBody();
        //void        get_body();
        Config      calibrate_host_location(std::vector<Config> _servers, Request _request);
        void        enter_location(Config server, std::string uri);
        int         check_for_statusCode();
        void        handle_SC_error(int sc);

        void        handle_GET();
        void        handle_POST(const std::string& protocol);
        void        writeUrlEncodedToFile(const std::string& content);
        void        handle_DELETE();
        
        int         handle_GET_CGI();
        int         handle_POST_CGI();
        void        createDirectoryListing(std::string directoryPath, std::string uri);
        void        handle_GET_autoindex(std::string open);
};

#endif