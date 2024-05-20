#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../config/Config.hpp"
#include "../config/Location.hpp"
#include "../request/Request.hpp"

#include <unistd.h>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <dirent.h>

#define DEFAULT "/index.html"

class Config;
Class Location;
Class Request;

class Response
{
    private:
        std::vector<Config> _servers;
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
        std::string getHeaders();
        
};

#endif