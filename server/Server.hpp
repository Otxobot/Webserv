/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 11:39:08 by abasante          #+#    #+#             */
/*   Updated: 2024/05/02 16:29:27 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "../Webserv.hpp"
#include "../config/Config.hpp"
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h> 

class Config;

class Server
{
    private:
        std::vector<Config> servers_parsed;
        //Request _request;
	    // Making sockets
	    int _masterSockFD;
	    std::vector<int> _masterSockFDs;
	    std::vector<int> _ports;
	    int _port;
	    std::string _host;

	    // Socket infos
	    struct sockaddr_in _serverAddr;
	    struct sockaddr_in _clientAddr;
	    socklen_t _addrLen;

	    // fd_set structures select()
	    fd_set _masterFDs;
	    fd_set _readFDs;
	    fd_set _writeFDs;

	    // Max of fds
	    int _maxSockFD;

	    // Clients sockets data request will hold by second element
	    std::map<int, std::string> _clients;

	    // first is accept socket ; second is master socket
	    std::map<int, int> _accptMaster;

	    // HttpServer _server;
	    int _isvalid;

    public:
	    Server(Config config, std::string configFile);
	    Server(std::vector<Config> &);
	    Server(Server const &);
	    ~Server();
	    Server &operator=(const Server &);
	    std::string get_body(std::string file_name);
	    char *ft_itoa(int n);
	    static int num_len(int n);
	    void makeSockets();
	    void createSocket();
	    void bindSocket();
	    void listenSocket();
	    void waitingForConnections();
	    void newConnectHandling(int &);
	    void accptedConnectHandling(int &);
	    void responseHandling(int &);
};

#endif