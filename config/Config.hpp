/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:51:14 by abasante          #+#    #+#             */
/*   Updated: 2024/04/30 13:30:37 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#define D_SERVER				"server:"
#define D_SERVER_NAME			"servername:"
#define D_LISTEN				"listen:"
#define D_LOCATION				"location:"
#define D_ROOT					"root:"
#define D_FILE					"file:"
#define D_AUTO_INDEX			"autoindex:"
#define D_ALLOW					"allow:"
#define D_ERROR_PAGE			"error_page:"
#define D_BUFFER_SIZE			"buffer_size:"
#define D_UPLOAD				"upload:"
#define D_CGI					"cgi:"
#define D_REDIR					"redirect:"
#define D_HANDLE_POST			"handle_post:"
#define D_HANDLE_DELETE			"handle_delete:"

#include "../Webserv.hpp"
#include "../server/Server.hpp"
#include "./Location.hpp"
#include <string>
#include <fstream>

class Location;

class Config
{
	private:
		int									_port; //uint16_t
		int									_ip_host; //in_addr_t
		std::string							_servername;
		std::string							_root;
		std::string							_listen;
		std::string							_location;
		std::string							_file;
		bool								_autoindex;
		std::vector<int, std::string>		_errorpage;
		std::vector<Location>				_locations;
		unsigned long						_client_max_body_size;
		
	public:
		Config(void);
		~Config();
		
		static void	parseConfig(std::string configFile);
};

#endif