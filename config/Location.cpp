/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 13:26:19 by abasante          #+#    #+#             */
/*   Updated: 2024/04/30 14:15:24 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location()
{
	//Constructor Default
	_allowGET = false;
	_allowPOST = false;
	_allowDELETE = false;
	_path = "";
	_root = "";
	_autoindex = false;
	_index = 0;
	_return = "";
	_alias = "";
	_client_max_body_size = 0;
}

Location::Location(const Location &other)
{
	//Constructor Copy
	_allowGET = other._allowGET;
	_allowPOST = other._allowPOST;
	_allowDELETE = other._allowDELETE;
	_path = other._path;
	_root = other._root;
	_autoindex = other._autoindex;
	_index = other._index;
	_return = other._return;
	_alias = other._alias;
	_cgi_path = other._cgi_path;
	//_cgi_ext = other._cgi_ext;
	_client_max_body_size = other._client_max_body_size;
}

Location::~Location()
{
	//Destructor Default
}
