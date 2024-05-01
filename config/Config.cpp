/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:51:05 by abasante          #+#    #+#             */
/*   Updated: 2024/04/30 17:48:54 by abasante         ###   ########.fr       */
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
	std::ifstream file(configFile.c_str());

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
	parseServers(file, contador);
}

void Config::parseServers(std::ifstream &file, int contador/*, int location_times*/)
{
	//Aqui hay que parsear todos los servidores y sus locations
	std::vector<Config>	_servers;
	_servers.resize(contador);

	file.clear();
	file.seekg(0, std::ios::beg);

	std::string line;
	std::string line_sin_comillas;
	int i = 0;
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
			line_sin_comillas = this->trim_comillas(line);
			std::cout << "after trimming->"<<line_sin_comillas << std::endl;
			//this->_locations[line_sin_comillas] = this->parseLocation(file, line_sin_comillas, _servers[i]);
			//line_sin_comillas = "";
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

// Location Config::parseLocation(std::ifstream &file, std::string line_sin_comillas, Config &server)
// {
// 	std::string line;
// 	while (std::getline(file, line))
// 	{
// 		if (line.find("root") != std::string::npos)
// 		{
// 			//std::cout << line << std::endl;
// 		}
// 	}
// 	return (server._locations[line_sin_comillas]);
// }

// std::string Config::trim_comillas(std::string line)
// {
// 	std::cout << "line before trimming->" << line << std::endl;
// 	std::string line_sin_comillas;
// 	if (line.find("\"") != std::string::npos)
// 	{
// 		std::cout << "no ha encontrado comillas, trata el caso /" << std::endl;
// 		return ("/");
// 	}
// 	// line_sin_comillas = line.find('\"');
// 	// int i = 0;
// 	// while (line_sin_comillas[i] != '\"')
// 	// {
// 	// 	line_sin_comillas += line[i];
// 	// }
// 	return (line_sin_comillas);
// }
std::string Config::trim_comillas(std::string line)
{
    std::cout << "line before trimming->" << line << std::endl;
    std::string::size_type i = 0;
    while (i < line.size())
    {
        if (line[i] == '\"')
        {
            line.erase(i, 1);
        }
        else
        {
            ++i;
        }
    }
    return line;
}