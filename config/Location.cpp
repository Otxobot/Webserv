/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 13:26:19 by abasante          #+#    #+#             */
/*   Updated: 2024/04/30 17:31:01 by abasante         ###   ########.fr       */
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


Location & Location::operator=(Location const &rhs)
{
	//Operator de asignacion
	if (this != &rhs)
	{
		_allowGET = rhs._allowGET;
		_allowPOST = rhs._allowPOST;
		_allowDELETE = rhs._allowDELETE;
		_path = rhs._path;
		_root = rhs._root;
		_autoindex = rhs._autoindex;
		_index = rhs._index;
		_return = rhs._return;
		_alias = rhs._alias;
		_cgi_path = rhs._cgi_path;
		//_cgi_ext = rhs._cgi_ext;
		_client_max_body_size = rhs._client_max_body_size;
	}
	return (*this);
}

void Location::setAllowGET(bool allowGET)
{
	_allowGET = allowGET;
}

void Location::setAllowPOST(bool allowPOST)
{
	_allowPOST = allowPOST;
}

void Location::setAllowDELETE(bool allowDELETE)
{
	_allowDELETE = allowDELETE;
}

void Location::setFile(std::string file)
{
	_file = file;
}

void Location::setRedirect(std::string redirect)
{
	_redirect = redirect;
}

void Location::setRoot(std::string root)
{
	_root = root;
}

void Location::setAutoindex(bool autoindex)
{
	_autoindex = autoindex;
}

void Location::setHandleDelete(std::string handle_delete)
{
	_handle_delete = handle_delete;
}

void Location::setHandlePost(std::string handle_post)
{
	_handle_post = handle_post;
}

void Location::setErrorPage(std::string error_page)
{
	_error_page = error_page;
}

void Location::setCgi(std::string cgi)
{
	_cgi = cgi;
}

void Location::setUpload(std::string upload)
{
	_upload = upload;
}

void Location::setBufferSize(int buffer_size)
{
	_buffer_size = buffer_size;
}