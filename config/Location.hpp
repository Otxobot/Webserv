/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 13:26:21 by abasante          #+#    #+#             */
/*   Updated: 2024/05/23 14:01:02 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "../Webserv.hpp"
#include "../request/Request.hpp"
#include "Config.hpp"
#include <string>
#include <vector>

class Location
{
	private:
		std::string		location_path;
		//int					_index;
		bool			_allowGET;
		bool			_allowPOST;
		bool			_allowDELETE;
		std::string					_redirect;
		std::string					_root;
		bool						_autoindex;
		std::string					_handle_delete;
		std::string					_handle_post;
		std::string					_error_page;
		std::string					_cgi;
		std::string					_upload;
		int							_buffer_size;
	public:
		std::string					_file;
		Location();
		Location(const Location &other);
		Location &operator=(const Location &rhs);
		~Location();
		void setAllowGET(bool allowGET);
		void setAllowPOST(bool allowPOST);
		void setAllowDELETE(bool allowDELETE);
		void setFile(std::string file);
		void setRedirect(std::string redirect);
		void setRoot(std::string root);
		void setAutoindex(bool autoindex);
		void setHandleDelete(std::string handle_delete);
		void setHandlePost(std::string handle_post);
		void setErrorPage(std::string error_page);
		void setCgi(std::string cgi);
		void setUpload(std::string upload);
		void setBufferSize(int buffer_size);
};

#endif