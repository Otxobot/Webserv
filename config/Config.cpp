/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:51:05 by abasante          #+#    #+#             */
/*   Updated: 2024/05/22 13:25:36 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Config.hpp"
#include "Location.hpp"
#include "../server/Server.hpp"

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
	_host = "";
	_client_max_body_size = 0;
}

Config::~Config()
{
	//Destructor Default
	
}

std::vector<Config> Config::parseConfig(std::string configFile)
{
	std::ifstream file(configFile.c_str());
	std::vector<Config> serversConfig;

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
	serversConfig = parseServers(file, contador);
	return (serversConfig);
}

std::vector<Config> Config::parseServers(std::ifstream &file, int contador)
{
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
			_serversConfig[i]._locations[line_sin_comillas] = this->parseLocation(file, line);

		}
		if (line.find("server:") != std::string::npos)
		{
			i++;
			_serversConfig[i]._index = i;
			continue;
		}
		if (line.find("servername:") != std::string::npos)
		{
			_serversConfig[i]._servername = trim_comillas(line.substr(14));
		}
		if (line.find("root:") != std::string::npos)
		{
			_serversConfig[i]._root = trim_comillas(line.substr(8));
		}
		if (line.find("listen:") != std::string::npos)
		{
			_serversConfig[i]._listen = trim_comillas(line.substr(10));
			std::string listen = line.substr(10);
			size_t colonPos = listen.find(":");
			std::string host = listen.substr(0, colonPos);
			if (host.empty())
				_serversConfig[i]._host = "ANY";
			else
				_serversConfig[i]._host = host;
			std::string port_str = listen.substr(colonPos + 1);
    		int port = atoi(port_str.c_str());
			_serversConfig[i]._port = port;
			_serversConfig[i]._ports.push_back(port);
		}
		if (line.find("buffer_size: ") != std::string::npos)
        {
			std::string line_to_num = line.substr(15);
			int buffer_size = std::atoi(line_to_num.c_str());
			_serversConfig[i]._buffer_size = buffer_size;
        }
	}
	return (_serversConfig);
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
		if (line.find("allow: ") == std::string::npos)
		{
			loc.setAllowGET(true);
			loc.setAllowPOST(true);
			loc.setAllowDELETE(true);
		}
        if (line.find("file: ") != std::string::npos)
        {
            loc.setFile(trim_comillas(line.substr(10)));
        }
        if (line.find("redirect: ") != std::string::npos)
        {
            loc.setRedirect(trim_comillas(line.substr(14)));
        }
        if (line.find("root: ") != std::string::npos)
        {
            loc.setRoot(trim_comillas(line.substr(10)));
        }
        if (line.find("autoindex: ") != std::string::npos)
        {
            line.substr(14) == "on" ? loc.setAutoindex(true) : loc.setAutoindex(false);
        }
        if (line.find("handle_delete: ") != std::string::npos)
        {
            loc.setHandleDelete(trim_comillas(line.substr(19)));
        }
        if (line.find("handle_post: ") != std::string::npos)
        {
            loc.setHandlePost(trim_comillas(line.substr(17)));
        }
        if (line.find("error_page: ") != std::string::npos)
        {
            loc.setErrorPage(trim_comillas(line.substr(16)));
        }
        if (line.find("cgi: ") != std::string::npos)
        {
            loc.setCgi(trim_comillas(line.substr(9)));
        }
        if (line.find("upload: ") != std::string::npos)
        {
            loc.setUpload(trim_comillas(line.substr(12)));
        }
    }
    return (loc);
}

std::string Config::trim_comillas(const std::string& line) {
    std::string line_sin_espacios = line;

    size_t first_non_space = line_sin_espacios.find_first_not_of(" \t");
    if (first_non_space != std::string::npos && line_sin_espacios[first_non_space] == '"')
        line_sin_espacios.erase(0, first_non_space);

    while (!line_sin_espacios.empty() && line_sin_espacios[0] == '"')
        line_sin_espacios.erase(0, 1);

    size_t last_non_space = line_sin_espacios.find_last_not_of(" \t");
    if (last_non_space != std::string::npos && line_sin_espacios[last_non_space] == '"')
        line_sin_espacios.erase(last_non_space + 1);

    while (!line_sin_espacios.empty() && line_sin_espacios[line_sin_espacios.length() - 1] == '"')
        line_sin_espacios.erase(line_sin_espacios.length() - 1);

    return line_sin_espacios;
}

std::vector<int>  Config::getPorts()
{
    return this->_ports;
}

int Config::getPort()
{
	return this->_port;
}

std::string    Config::getHost()
{
    return (this->_host);
}

int Config::getIndex()
{
	return this->_index;
}

// std::map<std::string, Location> Config::getLocations()
// {
// 	std::cout << "entro a get locations" << std::endl;
// 	return (this->_locations);
// }
