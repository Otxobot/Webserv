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
				   this->makeSockets();
				   this->waitingForConnections();
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
	//this->waitingForConnections();
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
	std::cout << RED << "Server destructor is closing fds..." << std::endl;
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
		std::cout << "============================================" << std::endl;
		std::cout << "Esta entrando en el servidor-> "<<itServer->getIndex() << std::endl;
		_port = itServer->getPort();
		_host = itServer->getHost();
		std::cout << "El host es: " << _host << std::endl;
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

/*Server::createSocket(): Esta función crea un nuevo socket y configura algunas de sus opciones. 
Primero, crea un nuevo socket utilizando la función socket(). 
Luego, utiliza la función fcntl() para configurar el socket como no bloqueante, 
lo que significa que las llamadas a funciones como recv() y send() no bloquearán el programa si no hay datos disponibles. 
Finalmente, utiliza setsockopt() para configurar la opción SO_REUSEADDR del socket, 
lo que permite que el socket reutilice un puerto local sin esperar que el kernel libere el puerto después de cerrar el socket.*/

void Server::createSocket()
{
	if ((_masterSockFD = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw std::runtime_error("Unable to create a socket.");

	// set socket to non-blocking
	if (fcntl(_masterSockFD, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Unable to set the socket  /*(+ std::wstring(_masterSockFD) +*/  to non-blocking.");
	// set socket option of reusing address
	int opt = 1;
	if (setsockopt(_masterSockFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1)
		throw std::runtime_error("Unable to set socket option to the socket" /* + std::wstring(_masterSockFD)*/);
}

/*Server::bindSocket(): Esta función vincula el socket creado en createSocket() a una dirección y puerto específicos. 
Primero, inicializa la estructura _serverAddr a cero y luego establece la familia de direcciones a AF_INET (IPv4), 
el puerto a _port y la dirección IP a _host. 
Luego, utiliza la función bind() para vincular el socket a la dirección y puerto especificados.*/

// Socket binding
void Server::bindSocket()
{
	std::memset(&_serverAddr, 0, sizeof(_serverAddr));
	_addrLen = sizeof(_serverAddr);
	_serverAddr.sin_family = AF_INET;
	std::cout << "EL PUERTO ES: " << _port << std::endl;
	//Htons cambia el puerto a network byte order, en vez de host byte order
	_serverAddr.sin_port = htons(_port);
	if (_host == "ANY")
		_serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		_serverAddr.sin_addr.s_addr = inet_addr(_host.c_str());
	//_serverAddr.sin_addr.s_addr = (_host == "ANY") ? htonl(INADDR_ANY) : inet_addr(_host.c_str());
	bind(_masterSockFD, (struct sockaddr *)&_serverAddr, sizeof(_serverAddr));
}

/*Server::listenSocket(): Esta función pone el socket en modo de escucha, 
lo que significa que el socket está listo para aceptar conexiones entrantes. 
Utiliza la función listen() para hacer esto. 
Luego, agrega el descriptor del socket al conjunto de descriptores de archivo _masterFDs utilizando la macro FD_SET(). 
Esto permite que el programa monitoree el socket para ver si hay datos entrantes.
Finalmente, agrega el descriptor del socket al vector _masterSockFDs.*/

// Listen for incoming connections from clients
void Server::listenSocket()
{
	if (listen(_masterSockFD, BACKLOG) == -1)
		throw std::runtime_error("Unable to listen for connections in the socket ");
	// set socket to fd_set struct
	FD_SET(_masterSockFD, &_masterFDs);
	if (_masterSockFD > _maxSockFD)
	{
		//aqui se esta renovando el maximo socket constantemente. 
		_maxSockFD = _masterSockFD;
	}
	// Add the socket to the sockets vector _masterSockFDs
	_masterSockFDs.push_back(_masterSockFD);
}

void Server::waitingForConnections()
{
	int running = 1;
	std::cout << BLUE <<"\t<Server running... waiting for connections./>" << std::endl;
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
			{
				std::cout << BLUE << "Shutting down server gracefuly" << std::endl;
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
						std::cout << "_masterSockFDs: " << *it << " || sockFD: " << sockFD << std::endl;
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
			std::cout << _buffRes << std::endl;
			//this->responseHandling(accptSockFD);
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

// void Server::responseHandling(int &accptSockFD)
// {
// 	std::string body;
// 	std::string path = _request.getTarget().erase(0, 1);
// 	char *header = strdup("HTTP/1.1 200 OK\r\nContent-Length: ");

// 	Response _resp;
// 	_resp.creatResponse(this->_servers, this->_request);
// 	this->_request.clear();

// 	std::string all = std::string(header) + std::string(ft_itoa(_resp.GetBody().size())) + "\r\n\r\n" + _resp.GetBody();

// 	if (FD_ISSET(accptSockFD, &_writeFDs))
// 	{
// 		if (send(accptSockFD, _resp.getRespHeader().c_str(), _resp.getRespHeader().length(), 0) != (ssize_t)_resp.getRespHeader().length())
// 			throw std::runtime_error("Unable to send the response to client in socket " + std::to_string(accptSockFD));
// 		if (!this->_request.getReqValue("Connection").compare("close")) // if connection is set to close in request close
// 		{
// 			close(accptSockFD);
// 			FD_CLR(accptSockFD, &_masterFDs);
// 			FD_CLR(accptSockFD, &_writeFDs);
// 		}
// 	}
// 	_resp.clear();
// 	this->_request.clear();
// }