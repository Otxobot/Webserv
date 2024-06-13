/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 11:39:11 by abasante          #+#    #+#             */
/*   Updated: 2024/06/13 11:45:07 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(Config config, std::string configFile) : _masterSockFD(0),
				   _port(0),
				   _host(""),
				   _addrLen(0),
				   _maxSockFD(0)
{
				   this->servers_parsed = config.parseConfig(configFile);
				   this->makeSockets();
				   this->waitingForConnections();
}

Server::Server(std::vector<Config> &servers) : servers_parsed(servers),
												   _masterSockFD(0),
												   _port(0),
												   _host(""),
												   _addrLen(0),
												   _maxSockFD(0)
{
	this->makeSockets();
	this->waitingForConnections();
}

Server::Server(Server const &ths)
{
	*this = ths;
	return;
}

Server::~Server()
{
	servers_parsed.clear();
	_masterSockFDs.clear();
	_ports.clear();
	_clients.clear();
	_accptMaster.clear();
}

// Assignment operator=
Server &Server::operator=(Server const &ths)
{
	if (this != &ths)
	{
		this->servers_parsed = ths.servers_parsed;
		this->_masterSockFDs = ths._masterSockFDs;
		this->_masterSockFD = ths._masterSockFD;
		this->_ports = ths._ports;
		this->_port = ths._port;
		this->_host = ths._host;
		this->_serverAddr = ths._serverAddr;
		this->_clientAddr = ths._clientAddr;
		this->_addrLen = ths._addrLen;
		this->_masterFDs = ths._masterFDs;
		this->_readFDs = ths._readFDs;
		this->_writeFDs = ths._writeFDs;
		this->_maxSockFD = ths._maxSockFD;
		this->_clients = ths._clients;
		this->_accptMaster = ths._accptMaster;
	}
	return *this;
}

void Server::makeSockets()
{
	FD_ZERO(&_masterFDs);
	FD_ZERO(&_writeFDs);
	FD_SET(STDIN_FILENO, &_masterFDs);
	for (std::vector<Config>::iterator itServer = servers_parsed.begin(); itServer != servers_parsed.end(); itServer++)
	{
		std::cout << "============================================" << std::endl;
		std::cout << "Esta entrando en el servidor-> "<< itServer->getIndex() << std::endl;
		_port = itServer->getPort();
		std::cout << "EL PUERTO ES: " << _port << std::endl;
		_host = itServer->getHost();
		std::cout << "EL HOST ES: " << _host << std::endl;
			try
			{
				this->createSocket();	
				this->bindSocket();
				this->listenSocket();
			}
			catch (const std::exception &e)
			{
				close(_masterSockFD);
				std::cerr << e.what() << '\n';
			}
	}
}

void Server::createSocket()
{
	if ((_masterSockFD = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw std::runtime_error("Unable to create a socket.");
	if (fcntl(_masterSockFD, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Unable to set the socket  /*(+ std::wstring(_masterSockFD) +*/  to non-blocking.");
	int opt = 1;
	if (setsockopt(_masterSockFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1)
		throw std::runtime_error("Unable to set socket option to the socket" /* + std::wstring(_masterSockFD)*/);
}

void Server::bindSocket()
{
	std::memset(&_serverAddr, 0, sizeof(_serverAddr));
	_addrLen = sizeof(_serverAddr);
	_serverAddr.sin_family = AF_INET;
	//Htons cambia el puerto a network byte order, en vez de host byte order
	_serverAddr.sin_port = htons(_port);
	if (_host == "ANY")
		_serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		_serverAddr.sin_addr.s_addr = inet_addr(_host.c_str());
	bind(_masterSockFD, (struct sockaddr *)&_serverAddr, sizeof(_serverAddr));
}

void Server::listenSocket()
{
	if (listen(_masterSockFD, BACKLOG) == -1)
		throw std::runtime_error("Unable to listen for connections in the socket ");
	FD_SET(_masterSockFD, &_masterFDs);
	if (_masterSockFD > _maxSockFD)
	{
		_maxSockFD = _masterSockFD;
	}
	_masterSockFDs.push_back(_masterSockFD);
}

void Server::waitingForConnections()
{
	int running = 1;
	std::cout << BLUE <<"\t<Server running... waiting for connections./>" << RESET << std::endl;
	while (running)
	{
		FD_ZERO(&_readFDs);
		_readFDs = _masterFDs;
		usleep(2000);
		struct timeval _tv = {3, 500};
		int activity = select(_maxSockFD + 1, &_readFDs, &_writeFDs, NULL, &_tv);
		if (activity == -1)
		{
			std::cout << RED << "Select failed to multiplexing Input/Output." << RESET << std::endl;
			break;
		}
		if (activity > 0)
		{
			if (FD_ISSET(0, &_readFDs))
			{
				std::cout << BLUE << "Shutting down server gracefuly" << RESET << std::endl;
				running = 0;
				for (std::vector<int>::iterator it = _masterSockFDs.begin(); it != _masterSockFDs.end(); it++)
				{
					close(*it);
				}
				FD_ZERO(&_masterFDs);
				FD_ZERO(&_readFDs);
				FD_ZERO(&_writeFDs);
				break;
			}
			for (int sockFD = 1; sockFD < _maxSockFD + 1; sockFD++)
			{
				if (FD_ISSET(sockFD, &_readFDs))
				{
					int newConnect = 0;
					for (std::vector<int>::iterator it = _masterSockFDs.begin(); it != _masterSockFDs.end(); it++)
					{
						if (sockFD == *it)
						{
							newConnect = 1;
							break;
						}
					}
					if (newConnect)
						this->newConnectHandling(sockFD);
					else
						this->acceptedConnectHandling(sockFD);
				}
			}
		}
	}
}

void Server::newConnectHandling(int &sockFD)
{
	int accptSockFD = accept(sockFD, (struct sockaddr *)&_clientAddr, &_addrLen);
	if (accptSockFD == -1)
		throw std::runtime_error("Unable to accept the connection from client by the socket ");
	if (fcntl(accptSockFD, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Unable to set the socket to non-blocking.");
	FD_SET(accptSockFD, &_masterFDs);
	FD_SET(accptSockFD, &_writeFDs);
	if (accptSockFD > _maxSockFD)
		_maxSockFD = accptSockFD;
	_clients.insert(std::pair<int, std::string>(accptSockFD, ""));
	std::map<int, int>::iterator it = _accptMaster.find(accptSockFD);
	if (it != _accptMaster.end())
		it->second = sockFD;
	else
		_accptMaster.insert(std::pair<int, int>(accptSockFD, sockFD));
}

int get_buffer_server(std::vector<Config> &_servers, int port)
{
    int i = 0;
    int size = _servers.size();
    while (i < size)
    {
        if (_servers[i]._port == port)
        {
            if (_servers[i]._buffer_size == 0)
                return INT_MAX;
            return (_servers[i]._buffer_size);
        }
        i++;
    }
    return -1;
}

//supuestamente llega bien el txt, no sabemos si las imagenes llegan bien, a veces parecen llegar bien y a veces no
void Server::acceptedConnectHandling(int &accptSockFD)
{
    char buffer[BUFFER_SIZE + 1];
    std::string requestData;
    int valRead;
	int ultValread = 0;
    int port;
    while ((valRead = recv(accptSockFD, buffer, BUFFER_SIZE, 0)) > 0)
    {
		ultValread += valRead;
        buffer[valRead] = '\0';  // Null-terminate the buffer
        requestData.append(buffer, valRead);
        // Check if the request headers have been fully received
        size_t headerEnd = requestData.find("\r\n\r\n");
        if (headerEnd != std::string::npos)
        {
            // Extract headers to determine Content-Length if it exists
            std::string headers = requestData.substr(0, headerEnd + 4);
            size_t contentLengthPos = headers.find("Content-Length: ");
            if (contentLengthPos != std::string::npos)
            {
                size_t contentLengthEnd = headers.find("\r\n", contentLengthPos);
                int contentLength = atoi(headers.substr(contentLengthPos + 16, contentLengthEnd - contentLengthPos - 16).c_str());
                // Check if the full request has been received
                size_t totalRequestSize = headerEnd + 4 + contentLength;
                if (requestData.size() >= totalRequestSize)
                {
                    // Full request received, process it
                    this->_request.reset();
                    this->_request.Request_start(requestData);
                    port = this->_request.getPort(); // Corrección aquí
                    if (get_buffer_server(this->servers_parsed, port) <= ultValread ||
                        get_buffer_server(this->servers_parsed, port) == -1)
						{
							std::string response = "HTTP/1.1 413 Payload Too Large\r\n"
                                   "Content-Type: text/html\r\n"
                                   "Content-Length: 76\r\n\r\n"
                                   "<html><body><h1>413 Payload Too Large</h1></body></html>\r\n";
							send(accptSockFD, response.c_str(), response.length(), 0);
            				close(accptSockFD);
							FD_CLR(accptSockFD, &_masterFDs);
							FD_CLR(accptSockFD, &_writeFDs);
							_clients.erase(accptSockFD);
                        	break ;
						}
                    if (FD_ISSET(accptSockFD, &_writeFDs))
                    {
                        this->responseHandling(accptSockFD);
                    }
                    break;
                }
            }
            else
            {
                // No Content-Length header, assume no body
                this->_request.reset();
                this->_request.Request_start(requestData);
                if (FD_ISSET(accptSockFD, &_writeFDs))
                {
                    this->responseHandling(accptSockFD);
                }
                break;
            }
        }
    }
    if (valRead == 0)
    {
        // Socket closed
        close(accptSockFD);
        FD_CLR(accptSockFD, &_masterFDs);
        FD_CLR(accptSockFD, &_writeFDs);
        _clients.erase(accptSockFD);
    }
    else if (valRead < 0)
    {
		// Other error occurred
		perror("recv failed");
		close(accptSockFD);
		FD_CLR(accptSockFD, &_masterFDs);
		FD_CLR(accptSockFD, &_writeFDs);
		_clients.erase(accptSockFD);
    }
}

#include "../response/Response.hpp"

void Server::responseHandling(int &accptSockFD)
{
	std::string body;
	std::string path = _request.getTarget().erase(0, 1);

	Response _resp;
	_resp.responseCreation(this->servers_parsed, this->_request);
	this->_request.reset();

	if (FD_ISSET(accptSockFD, &_writeFDs))
	{
		std::string response = _resp._response;
		send(accptSockFD, response.c_str(), response.size(), 0);
		shutdown(accptSockFD, SHUT_WR);
		if (!this->_request.headers["Connection"].compare("close"))
		{
			close(accptSockFD);
			FD_CLR(accptSockFD, &_masterFDs);
			FD_CLR(accptSockFD, &_writeFDs);
		}
	}
	this->_request.reset();
}
