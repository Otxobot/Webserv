/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 11:39:11 by abasante          #+#    #+#             */
/*   Updated: 2024/05/22 15:05:13 by abasante         ###   ########.fr       */
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
		struct timeval _tv = {1, 0};
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

void Server::acceptedConnectHandling(int &accptSockFD)
{
	char buffer[BUFFER_SIZE + 1] = {0};
	bzero(buffer, sizeof(buffer));
	int valRead = recv(accptSockFD, buffer, BUFFER_SIZE, 0);
	if (valRead > 0)
	{
		buffer[valRead] = '\0';
		std::map<int, std::string>::iterator it = _clients.find(accptSockFD);
		if (it != _clients.end())
			it->second += buffer;
		std::string req(buffer);
		this->_request.reset();
		this->_request.Request_start(req);
		if (FD_ISSET(accptSockFD, &_writeFDs))
		{
			this->responseHandling(accptSockFD);
		}
	}
	if (valRead == 0)
	{
		close(accptSockFD);
		FD_CLR(accptSockFD, &_masterFDs);
		FD_CLR(accptSockFD, &_writeFDs);
		_clients.erase(accptSockFD);
	}
	else
		return; // Socket is connected but doesn't send request.
}

// std::string Server::get_body(std::string file_name)
// {
// 	std::string _body;
// 	std::ifstream file(file_name.c_str());
// 	if (file)
// 	{
// 		std::ostringstream ss;
// 		ss << file.rdbuf();
// 		_body = ss.str();
// 		file.close(); // close the file(filename)
// 	}
// 	return _body;
// }

// int Server::num_len(int n)
// {
// 	int i;

// 	i = 1;
// 	while (n /= 10)
// 		i++;
// 	return (i);
// }

// char *Server::ft_itoa(int n)
// {
// 	char *str;
// 	int numlen;
// 	unsigned int nb;

// 	numlen = num_len(n);
// 	nb = n;
// 	if (n < 0)
// 	{
// 		nb = -n;
// 		numlen++;
// 	}
// 	if (!(str = (char *)malloc(sizeof(char) * numlen + 1)))
// 		return (0);
// 	str[numlen] = '\0';
// 	str[--numlen] = nb % 10 + '0';
// 	while (nb /= 10)
// 		str[--numlen] = nb % 10 + '0';
// 	if (n < 0)
// 		*(str) = '-';
// 	return (str);
// }

#include "../response/Response.hpp"

void Server::responseHandling(int &accptSockFD)
{
	std::string body;
	std::string path = _request.getTarget().erase(0, 1);
	//char *header = strdup("HTTP/1.1 200 OK\r\nContent-Length: ");

	Response _resp;
	_resp.responseCreation(this->servers_parsed, this->_request);
	// this->_request.clear();

	//std::string all = std::string(header) + std::string(ft_itoa(_resp.GetBody().size())) + "\r\n\r\n" + _resp.GetBody();

	if (FD_ISSET(accptSockFD, &_writeFDs))
	{
		std::cout << "something with the socket" << std::endl;
		//if (send)
	// 	if () // if connection is set to close in request close
	// 	{
	// 		close(accptSockFD);
	// 		FD_CLR(accptSockFD, &_masterFDs);
	// 		FD_CLR(accptSockFD, &_writeFDs);
	// 	}
	}
	// _resp.clear();
	// this->_request.clear();
}