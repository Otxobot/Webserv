#ifndef SERVER_HPP
#define SERVER_HPP

#include "Webserv.hpp"

class Server
{
    private:
        
    public:
        Server();
        Server(Server const &obj);
        Server & operator=(Server const &rhs);
        ~Server();

};

#endif