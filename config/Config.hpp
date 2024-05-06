/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:51:14 by abasante          #+#    #+#             */
/*   Updated: 2024/05/02 16:55:54 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "../Webserv.hpp"
#include "../server/Server.hpp"
#include "./Location.hpp"
#include <string>
#include <fstream>
#include <vector>
#include <map>

class Location;

class Config
{
	private:
		int									_index;
		int									_port; //uint16_t
		std::string									_host; //in_addr_t
		std::string							_servername;
		std::string							_root;
		std::string							_listen;
		std::string							_file;
		bool								_autoindex;
		std::map<int, std::string>			_errorpage;
		std::map<std::string, Location>		_locations;
		unsigned long						_client_max_body_size;
		int									_buffer_size;
	public:
		Config();
		~Config();

		std::vector<Config>		parseConfig(std::string configFile);
		std::vector<Config>		parseServers(std::ifstream &file, int contador);
		Location				parseLocation(std::ifstream &file, std::string &line);
		std::string 			trim_comillas(const std::string& line);
		int     				getPorts();
		std::string 			getHost();
};

#endif