/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 15:28:36 by abasante          #+#    #+#             */
/*   Updated: 2024/05/13 16:28:55 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../Webserv.hpp"
#include "../server/Server.hpp"

#include <string>

class Request
{
    private:
        std::string request;
        std::string _status;
    public:
        Request();
        ~Request();
        int		Request_start(std::string request);
        int     request_line();
        int     request_headers();
        int     request_body();
        void    printRequestInformation();
        
};

#endif