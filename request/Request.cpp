/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 15:28:33 by abasante          #+#    #+#             */
/*   Updated: 2024/05/13 16:54:17 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request()
{
    //Default constructor
    std::cout << "LLAMA AL CONSTRUCTOR DE REQUEST" << std::endl;
}

Request::~Request()
{
    //Default destructor
}

int Request::Request_start(std::string request)
{
    std::cout << "entra aqui cada vez que hace un request" << std::endl;
    int ret = 0;
	this->request = request;
	if ((ret = this->request_line()) || (ret = this->request_headers())
		|| (ret = this->request_body()))
		{
			this->_status = ret;
			return ret;
		}
	std::cout << CYAN <<"+++++++++++++++++++++++++++++ Request ++++++++++++++++++++++++++++++++" << CYAN <<std::endl;
	printRequestInformation();
	std::cout << CYAN << "+++++++++++++++++++++++++++++ Request ++++++++++++++++++++++++++++++++"<< CYAN << std::endl;

	return 0;
}

int Request::request_line()
{

}

int Request::request_headers()
{
    
}

int Request::request_body()
{
    
}

void Request::printRequestInformation()
{

}
