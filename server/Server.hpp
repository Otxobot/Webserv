/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 11:39:08 by abasante          #+#    #+#             */
/*   Updated: 2024/04/29 12:16:02 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Webserv.hpp"
#include "../config/Config.hpp"

class Config;

class Server
{
    private:
        
    public:
        Server();
        Server(Server const &obj);
        Server & operator=(Server const &rhs);
        ~Server();

};

#endif