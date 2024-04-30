/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 13:26:21 by abasante          #+#    #+#             */
/*   Updated: 2024/04/30 17:31:14 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "../Webserv.hpp"
#include "Config.hpp"
#include <string>
#include <vector>

class Location
{
	private:
		int					_index;
		bool			_allowGET;
		bool			_allowPOST;
		bool			_allowDELETE;
		std::string					_path;
		std::string					_root;
		bool						_autoindex;
		//bool						_cgi;
		std::string					_return;
		std::string					_alias;
		std::vector<std::string>	_cgi_path;
		//std::vector<std::string>	_cgi_ext;
		unsigned long				_client_max_body_size;
	public:
		Location();
		Location(const Location &other);
		Location &operator=(const Location &rhs);
		~Location();
		
};

#endif