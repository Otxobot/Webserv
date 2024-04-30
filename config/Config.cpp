/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:51:05 by abasante          #+#    #+#             */
/*   Updated: 2024/04/30 13:05:18 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

class Config;

Config::Config()
{
	//Constructor Default
	_servername = "";
	_listen = "";
	_location = "";
	_root = "";
	_file = "";
	_autoindex = false;
	_allowGET = false;
	_allowPOST = false;
	_allowDELETE = false;
	_errorpage = "";
	_buffer_size = 0;
	_upload = "";
	_cgi = false;
	_redirect = "";
	_handlePOST = "";
	_handleDELETE = "";
}

Config::~Config()
{
	//Destructor Default
	
}

void Config::parseConfig(std::string configFile)
{
	std::vector<Config>	_servers;
	std::ifstream file(configFile);

	if (!file.is_open())
	{
		std::cout << "no se ha abierto bien el archivo" << std::endl;
	}

	std::string line;
	int contador = 0;
	while (std::getline(file, line))
	{
		if (line.find("server:") != std::string::npos)
		{
			contador++;
		}
	}
	
	std::cout << "Cantidad de servers: " << contador << std::endl;
	_servers.resize(contador);

	int i = 0;
	while (i < contador)
	{
		_servers[i] = Config();
		i++;
	}
	file.clear(); // Limpia los flags de error/fin de archivo
	file.seekg(0, std::ios::beg); 
	i = 0;
	std::string line1;
	std::getline(file, line1);
	while(std::getline(file, line1))
	{
		//std::cout << i << std::endl;
 		if (line1.find("server:") != std::string::npos)
		{
			i++;
			std::cout << "============================" << std::endl;
			std::cout << "============================" << std::endl;
			continue;
		}
		if (line1.find("servername: ")  != std::string::npos)
		{
			_servers[i]._servername = line1.substr(14);
			std::cout << _servers[i]._servername << std::endl;
		}
		if (line1.find("listen: ") != std::string::npos)
		{
			_servers[i]._listen = line1.substr(10);
			std::cout << _servers[i]._listen << std::endl;
		}
		if (line1.find("location: ") != std::string::npos)
		{
			// aqui tiene que hacer algo mas
			continue;
		}
		if (line1.find("root: ") != std::string::npos)
		{
			// aqui hay que hacer un manejo mas tambien
			// _servers[i]._root = line1.substr(8);
			// std::cout << _servers[i]._root << std::endl;
			continue;
		}
		if (line1.find("buffer_size: ") != std::string::npos)
		{
			_servers[i]._buffer_size = std::stoi(line1.substr(14));
			std::cout << _servers[i]._buffer_size << std::endl;
		}	
	}
}