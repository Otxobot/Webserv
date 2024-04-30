/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:51:05 by abasante          #+#    #+#             */
/*   Updated: 2024/04/30 14:17:38 by abasante         ###   ########.fr       */
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
	_index = 0;
	_port = 0;
	_ip_host = 0;
	_client_max_body_size = 0;
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
	_servers.resize(contador);
	//parseServers(file, _servers, contador);
}

// void Config::parseServers(std::ifstream &file, std::vector<Config> &_servers, int contador)
// {
// 	//Aqui hay que parsear todos los servidores y sus locations
// }