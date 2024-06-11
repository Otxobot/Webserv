/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response_cgi_POST.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:36:21 by abasante          #+#    #+#             */
/*   Updated: 2024/06/11 16:36:45 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "../cgi/cgi.hpp"

int    Response::handle_POST_CGI()
{
    std::string content = runCGI(this->_request, this->_server._root , this->_server._root + "/" + this->_server._locations["/cgi-bin"]._cgi);
    if (!content.empty())
    {
        this->_response.append("HTTP/1.1");
        this->_response.append(" ");
        this->_response.append("200 OK");
        this->_response.append("\r\n");
        this->_response.append("Content-Type: text/html\r\n");
        this->_response.append("Content-Length: ");
        std::stringstream ss;
        ss << content.length();
        this->_response.append(ss.str());
        this->_response.append("\r\n");
        this->_response.append("\r\n");
        this->_response.append(content);
        return (0);
    }
    else
        return (1);
}
