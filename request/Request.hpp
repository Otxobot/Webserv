/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikferna <mikferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 15:28:36 by abasante          #+#    #+#             */
/*   Updated: 2024/05/15 16:42:50 by mikferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#define REQUEST_URI_MAX_LENGTH 2048
#define HEADER_MAX_LENGTH 1000
#define VALUE_MAX_LENGTH 4000

#define BAD_REQUEST 400
#define NOT_IMPLEMENTED 501
#define HTTP_VERSION_NOT_SUPPORTED 505
#define REQUEST_URI_TOO_LONG 414

#define EXIT_SUCCESS 0

#include <string>
#include <iostream>
#include <map>
#include <string.h>
# include <stdlib.h>

class Request
{
    private:
        std::string request;
        //std::string _status;
        std::string Method;
        std::string target;
        std::string url;
        std::string protocol;
        std::string queryUrl;
        std::string body;
        std::map<std::string, std::string> headers;
        int			port;
        int         _status;
        std::string ContentDiposition;
        std::string name;
        std::string value;

        std::string contentType;
    public:
        Request();
        ~Request();
        int		Request_start(std::string request);
        int     request_line();
        int     request_headers();
        int     request_body();
        void    printRequestInformation();
        std::string &getMethod();
        std::string &getTarget();
        std::string &getProtocol();
        int			&getPort1();
        std::string		getQueryString();
        std::map<std::string, std::string> &getHeaders();
        std::string	autoindex_run(std::string rooted_path);
        std::string		getReqValue(std::string _key);
        std::string		getUrl();
        void	setTarget(std::string _target);
	    void	clear();
};

#endif