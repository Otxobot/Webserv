/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:51:05 by abasante          #+#    #+#             */
/*   Updated: 2024/05/02 14:43:42 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Location.hpp"

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
	parseServers(file, contador, location_counter);
}

void Config::parseServers(std::ifstream &file, int contador, int location_times)
{
	location_times = 0;
	//Aqui hay que parsear todos los servidores y sus locations
	std::vector<Config>	_serversConfig;
	_serversConfig.resize(contador);

	file.clear();
	file.seekg(0, std::ios::beg);

	std::string line;
	std::string line_sin_comillas;
	int i = 0;
	_serversConfig[i]._index = i;
	std::getline(file, line);
	while (std::getline(file, line))
	{
		while (line.find("location:") != std::string::npos)
		{
			line_sin_comillas = this->trim_comillas(line.substr(12));
			std::cout << "location" << line_sin_comillas << std::endl;
			this->_locations[line_sin_comillas] = this->parseLocation(file, line);
		}
		if (line.find("server:") != std::string::npos)
		{
			i++;
			_serversConfig[i]._index = i;
			std::cout << std::endl << std::endl <<std::endl << std::endl;
			continue;
		}
		if (line.find("servername:") != std::string::npos)
		{
			_serversConfig[i]._servername = trim_comillas(line.substr(14));
			std::cout << "servername: " << _serversConfig[i]._servername << std::endl;
		}
		if (line.find("root:") != std::string::npos)
		{
			_serversConfig[i]._root = trim_comillas(line.substr(8));
			std::cout << "root: " << _serversConfig[i]._root << std::endl;
		}
		if (line.find("listen:") != std::string::npos)
		{
			_serversConfig[i]._listen = trim_comillas(line.substr(10));
			std::cout << "listen: " << _serversConfig[i]._listen << std::endl;
		}
		if (line.find("buffer_size:") != std::string::npos)
		{
			_serversConfig[i]._buffer_size = std::stoi(line.substr(15));
			std::cout << "buffer_size: " << _serversConfig[i]._buffer_size << std::endl;
		}
	}
}

Location Config::parseLocation(std::ifstream &file, std::string &line)
{
	Location loc;
	while (std::getline(file, line))
	{
		if (line.find("location:") != std::string::npos || line.find("server:") != std::string::npos)
		{
			break;
		}
		if (line.find("allow: ") != std::string::npos)
		{
			if (line.substr(12).find("GET") != std::string::npos)
			{
				loc.setAllowGET(true);
			}
			if (line.substr(12).find("POST") != std::string::npos)
			{
				loc.setAllowPOST(true);
			}
			if (line.substr(12).find("DELETE") != std::string::npos)
			{
				loc.setAllowDELETE(true);
			}
		}
		if (line.find("file: ") != std::string::npos)
		{
			loc.setFile(trim_comillas(line.substr(11)));
		}
		if (line.find("redirect: ") != std::string::npos)
		{
			loc.setRedirect(trim_comillas(line.substr(15)));
		}
		if (line.find("root: ") != std::string::npos)
		{
			loc.setRoot(trim_comillas(line.substr(11)));
		}
		if (line.find("autoindex: ") != std::string::npos)
		{
			if (line.substr(15) == "on")
				loc.setAutoindex(true);
		}
		if (line.find("handle_delete: ") != std::string::npos)
		{
			loc.setHandleDelete(line.substr(20));
		}
		if (line.find("handle_post: ") != std::string::npos)
		{
			loc.setHandlePost(line.substr(18));
		}
		if (line.find("error_page: ") != std::string::npos)
		{
			loc.setErrorPage(line.substr(17));
		}
		if (line.find("cgi: ") != std::string::npos)
		{
			loc.setCgi(line.substr(10));
		}
		if (line.find("upload: ") != std::string::npos)
		{
			loc.setUpload(line.substr(13));
		}
		if (line.find("buffer_size: ") != std::string::npos)
		{
			loc.setBufferSize(std::stoi(line.substr(18)));
		}
	}
	return (loc);
}

std::string Config::trim_comillas(const std::string& line)
{
    std::string line_sin_espacios = line;

    size_t first_non_space = line_sin_espacios.find_first_not_of(" \t");
    if (first_non_space != std::string::npos && line_sin_espacios[first_non_space] == '"')
        line_sin_espacios.erase(0, first_non_space);
    while (!line_sin_espacios.empty() && line_sin_espacios[0] == '"')
        line_sin_espacios = line_sin_espacios.substr(1);
    while (!line_sin_espacios.empty() && line_sin_espacios.back() == '"')
        line_sin_espacios.pop_back();

    return line_sin_espacios;
}

