/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:51:05 by abasante          #+#    #+#             */
/*   Updated: 2024/04/29 15:57:02 by abasante         ###   ########.fr       */
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
	buffer_size = 0;
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
	
}