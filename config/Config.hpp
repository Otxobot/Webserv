/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikferna <mikferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:51:14 by abasante          #+#    #+#             */
/*   Updated: 2024/05/02 12:59:43 by mikferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

// #define D_ROOT					"root:"
// #define D_FILE					"file:"
// #define D_AUTO_INDEX			"autoindex:"
// #define D_ALLOW					"allow:"
// #define D_ERROR_PAGE			"error_page:"
// #define D_BUFFER_SIZE			"buffer_size:"
// #define D_UPLOAD				"upload:"
// #define D_CGI					"cgi:"
// #define D_REDIR					"redirect:"
// #define D_HANDLE_POST			"handle_post:"
// #define D_HANDLE_DELETE			"handle_delete:"

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
		int									_ip_host; //in_addr_t
		std::string							_servername;
		std::string							_root;
		std::string							_listen;
		std::string							_file;
		bool								_autoindex;
		std::map<int, std::string>			_errorpage;
		std::map<std::string, Location>		_locations;
		unsigned long						_client_max_body_size;
	public:
		Config(void);
		~Config();

		void		parseConfig(std::string configFile);
		void		parseServers(std::ifstream &file, int contador, int location_times);
		Location	parseLocation(std::ifstream &file, std::string &line);
		std::string trim_comillas(const std::string& line);
};

#endif