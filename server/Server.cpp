/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 11:39:11 by abasante          #+#    #+#             */
/*   Updated: 2024/04/29 15:39:12 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{
	//Default server constructor
}

Server::Server(Server const &obj)
{
	*this = obj;
}

Server & Server::operator=(Server const &rhs)
{
	if (this == &rhs)
		return (*this);
	return (*this);	
}

Server::~Server()
{
	
}
