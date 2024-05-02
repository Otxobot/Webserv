/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 13:26:19 by abasante          #+#    #+#             */
/*   Updated: 2024/05/02 14:42:54 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location()
{
	//Constructor Default
	_allowGET = false;
	_allowPOST = false;
	_allowDELETE = false;
	_file = "";
	_redirect = "";
	_root = "";
	_autoindex = false;
	_handle_delete = "";
	_handle_post = "";
	_error_page = "";
	_cgi = "";
	_upload = "";
	_buffer_size = 0;
}

Location::Location(const Location &other)
{
	//Constructor Copy
	_allowGET = other._allowGET;
	_allowPOST = other._allowPOST;
	_allowDELETE = other._allowDELETE;
	_file = other._file;
	_redirect = other._redirect;
	_root = other._root;
	_autoindex = other._autoindex;
	_handle_delete = other._handle_delete;
	_handle_post = other._handle_post;
	_error_page = other._error_page;
	_cgi = other._cgi;
	_upload = other._upload;
	_buffer_size = other._buffer_size;
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
		_file = rhs._file;
		_redirect = rhs._redirect;
		_root = rhs._root;
		_autoindex = rhs._autoindex;
		_handle_delete = rhs._handle_delete;
		_handle_post = rhs._handle_post;
		_error_page = rhs._error_page;
		_cgi = rhs._cgi;
		_upload = rhs._upload;
		_buffer_size = rhs._buffer_size;
	}
	return (*this);
}

void Location::setAllowGET(bool allowGET)
{
	_allowGET = allowGET;
	//std::cout << "Allow GET: " << _allowGET << std::endl;
}

void Location::setAllowPOST(bool allowPOST)
{
	_allowPOST = allowPOST;
	//std::cout << "Allow POST: " << _allowPOST << std::endl;
}

void Location::setAllowDELETE(bool allowDELETE)
{
	_allowDELETE = allowDELETE;
	std::cout << "Allow DELETE: " << _allowDELETE << std::endl;
}

void Location::setFile(std::string file)
{
	_file = file;
	std::cout << "File: " << _file << std::endl;
}

void Location::setRedirect(std::string redirect)
{
	_redirect = redirect;
	std::cout << "Redirect: " << _redirect << std::endl;
}

void Location::setRoot(std::string root)
{
	_root = root;
	std::cout << "Root: " << _root << std::endl;
}

void Location::setAutoindex(bool autoindex)
{
	_autoindex = autoindex;
	std::cout << "Autoindex: " << _autoindex << std::endl;
}

void Location::setHandleDelete(std::string handle_delete)
{
	_handle_delete = handle_delete;
	std::cout << "Handle Delete: " << _handle_delete << std::endl;
}

void Location::setHandlePost(std::string handle_post)
{
	_handle_post = handle_post;
	std::cout << "Handle Post: " << _handle_post << std::endl;
}

void Location::setErrorPage(std::string error_page)
{
	_error_page = error_page;
	std::cout << "Error Page: " << _error_page << std::endl;
}

void Location::setCgi(std::string cgi)
{
	_cgi = cgi;
	std::cout << "Cgi: " << _cgi << std::endl;
}

void Location::setUpload(std::string upload)
{
	_upload = upload;
	std::cout << "Upload: " << _upload << std::endl;
}

void Location::setBufferSize(int buffer_size)
{
	_buffer_size = buffer_size;
	std::cout << "Buffer Size: " << _buffer_size << std::endl;
}