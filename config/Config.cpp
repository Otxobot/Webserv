/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:51:05 by abasante          #+#    #+#             */
/*   Updated: 2024/04/30 17:16:10 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

class Config;

Config::Config()
{
	//Constructor Default
	_servername = "";
	_listen = "";
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
	file.clear();
	file.seekg(0, std::ios::beg); //Reiniciar el puntero del archivo para leerlo desde el principio
	int location_counter = 0;
	std::string line1;
	while (std::getline(file, line1))
	{
		if (line1.find("location:") != std::string::npos)
		{
			location_counter++;
		}
	}
	parseServers(file, contador, location_counter);
}

void Config::parseServers(std::ifstream &file, int contador, int location_times)
{
	//Aqui hay que parsear todos los servidores y sus locations
	std::vector<Config>	_servers;
	_servers.resize(contador);

	location_times = 0;
	file.clear();
	file.seekg(0, std::ios::beg);

	std::string line;
	int i = 0;
	int location_counter = 0;
	std::getline(file, line);
	_servers[i]._index = i;
	while (std::getline(file, line))
	{
		if (line.find("server:") != std::string::npos)
		{
			i++;
			_servers[i]._index = i;
			continue;
		}
		if (line.find("location:") != std::string::npos)
		{
			std::string line_sin_comillas;
			line_sin_comillas = this->trim_comillas(line);
			this->_locations[line_sin_comillas] = this->parseLocation(file, line_sin_comillas, _servers[i]);
		}
		if (line.find("servername:") != std::string::npos)
		{
			_servers[i]._servername = line.substr(10);
		}
		if (line.find("root:") != std::string::npos)
		{
			_servers[i]._root = line.substr(5);
		}
		if (line.find("listen:") != std::string::npos)
		{
			_servers[i]._listen = line.substr(7);
		}
	}
}

Location Config::parseLocation(std::ifstream &file, std::string line_sin_comillas, Config &server)
{
	std::string line;
	while (std::getline(file, line))
	{
		if (line.find("root") != std::string::npos)
		{
			this->_locations[line_sin_comillas] = 
		}	
	}
}

std::string Config::trim_comillas(std::string line)
{
    size_t first = line.find_first_of("\"");
    size_t last = line.find_last_of("\"");
    if (first != std::string::npos && last != std::string::npos && first < last)
    {
        return line.substr(first + 1, last - first - 1);
    }
    else if (line == "/")
    {
        // Devuelve una cadena vacía si no se encuentran comillas o si están en un orden incorrecto
        return line.substr(10);
    }
}