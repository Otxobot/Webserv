/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 11:39:11 by abasante          #+#    #+#             */
/*   Updated: 2024/05/02 16:57:37 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(Config config, std::string configFile) : _masterSockFD(0),
				   _port(0),
				   _host(""),
				   _addrLen(0),
				   _maxSockFD(0),
				   _isvalid(1) 
{
				   this->servers_parsed = config.parseConfig(configFile);
}

Server::Server(std::vector<Config> &servers) : servers_parsed(servers),
												   _masterSockFD(0),
												   _port(0),
												   _host(""),
												   _addrLen(0),
												   _maxSockFD(0),
												   _isvalid(1)
{
	this->makeSockets();
	this->waitingForConnections();
}

// Copy constructor
Server::Server(Server const &ths)
{
	*this = ths;
	return;
}

// Destructor
Server::~Server()
{
	for (std::vector<int>::iterator it = _masterSockFDs.begin(); it != _masterSockFDs.end(); it++)
	{
		close(*it);
	}
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
		this->_isvalid = ths._isvalid;
	}
	return *this;
}

void Server::makeSockets()
{
	// fd_set structures initializing
	FD_ZERO(&_masterFDs);
	FD_ZERO(&_writeFDs);
	FD_SET(STDIN_FILENO, &_masterFDs);
	// making master sockets for()
	for (std::vector<Config>::iterator itServer = servers_parsed.begin(); itServer != servers_parsed.end(); itServer++)
	{
		_port = itServer->getPorts();
		_host = itServer->getHost();
		for (std::vector<int>::iterator itPort = _ports.begin(); itPort != _ports.end(); ++itPort)
		{
			_port = *itPort;
			try
			{

				// Socket creating
				this->createSocket();
				// Bind a name to a socket
				this->bindSocket();
				// Listen for socket connections
				this->listenSocket();
			}
			catch (const std::exception &e)
			{
				close(_masterSockFD);
				std::cerr << e.what() << '\n';
			}
		}
	}
}

// Socket creating
void Server::createSocket()
{
	if ((_masterSockFD = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw std::runtime_error("Unable to create a socket.");

	// set socket to non-blocking
	if (fcntl(_masterSockFD, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Unable to set the socket " + std::to_string(_masterSockFD) + " to non-blocking.");
	// set socket option of reusing address
	int opt = 1;
	if (setsockopt(_masterSockFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1)
		throw std::runtime_error("Unable to set socket option to the socket " + std::to_string(_masterSockFD));
}

// Socket binding
void Server::bindSocket()
{
	std::memset(&_serverAddr, 0, sizeof(_serverAddr));
	_addrLen = sizeof(_serverAddr);
	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_port = htons(_port);
	_serverAddr.sin_addr.s_addr = (_host == "ANY") ? htonl(INADDR_ANY) : inet_addr(_host.c_str());
	bind(_masterSockFD, (struct sockaddr *)&_serverAddr, sizeof(_serverAddr));
}

// Listen for incoming connections from clients
void Server::listenSocket()
{
	if (listen(_masterSockFD, BACKLOG) == -1)
		throw std::runtime_error("Unable to listen for connections in the socket " + std::to_string(_masterSockFD));
	// set socket to fd_set struct
	FD_SET(_masterSockFD, &_masterFDs);
	_maxSockFD = (_masterSockFD > _maxSockFD) ? _masterSockFD : _maxSockFD;
	// Add the socket to the sockets vector
	_masterSockFDs.push_back(_masterSockFD);
}

void Server::waitingForConnections()
{
	int running = 1;
	std::cout << "\t<Server running... waiting for connections./>" << std::endl;
	while (running)
	{

		FD_ZERO(&_readFDs);
		_readFDs = _masterFDs;
		usleep(2000);
		struct timeval _tv = {1, 0};
		int activity = select(_maxSockFD + 1, &_readFDs, &_writeFDs, NULL, &_tv);
		if (activity == -1)
			throw std::runtime_error("Select failed to multiplexing Input/Output.");
		if (activity > 0)
		{
			if (FD_ISSET(0, &_readFDs))
			{ /* Check keyboard */
				std::cout << "Shutting down server gracefuly" << std::endl;
				// getchar();
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
					newConnect ? this->newConnectHandling(sockFD) : this->accptedConnectHandling(sockFD);
				}
			}
		}
	}
}

void Server::newConnectHandling(int &sockFD)
{
	int accptSockFD = accept(sockFD, (struct sockaddr *)&_clientAddr, &_addrLen);
	if (accptSockFD == -1)
		throw std::runtime_error("Unable to accept the connection from client by the socket " + std::to_string(accptSockFD));
	if (fcntl(accptSockFD, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Unable to set the socket " + std::to_string(accptSockFD) + " to non-blocking.");
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

void Server::accptedConnectHandling(int &accptSockFD)
{
	char _buffRes[BUFFER_SIZE + 1] = {0};
	bzero(_buffRes, sizeof(_buffRes));
	int valRead = recv(accptSockFD, _buffRes, BUFFER_SIZE, 0);
	if (valRead > 0)
	{
		_buffRes[valRead] = '\0';
		std::map<int, std::string>::iterator it = _clients.find(accptSockFD);
		if (it != _clients.end())
			it->second += _buffRes;
		std::string req(_buffRes);
		//_request.Request_start(req);
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


std::string Server::get_body(std::string file_name)
{
	std::string _body;
	std::ifstream file(file_name);
	if (file)
	{
		std::ostringstream ss;
		ss << file.rdbuf();
		_body = ss.str();
		file.close(); // close the file(filename)
	}
	return _body;
}

int Server::num_len(int n)
{
	int i;

	i = 1;
	while (n /= 10)
		i++;
	return (i);
}

char *Server::ft_itoa(int n)
{
	char *str;
	int numlen;
	unsigned int nb;

	numlen = num_len(n);
	nb = n;
	if (n < 0)
	{
		nb = -n;
		numlen++;
	}
	if (!(str = (char *)malloc(sizeof(char) * numlen + 1)))
		return (0);
	str[numlen] = '\0';
	str[--numlen] = nb % 10 + '0';
	while (nb /= 10)
		str[--numlen] = nb % 10 + '0';
	if (n < 0)
		*(str) = '-';
	return (str);
}
