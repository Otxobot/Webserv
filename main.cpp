/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 11:50:38 by abasante          #+#    #+#             */
/*   Updated: 2024/06/12 18:39:43 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "./request/Request.hpp"
#include "./parseo_conf/include.hpp"

int main(int ac, char **av)
{
	if (ac != 2)
  	{
    	std::cout << "Usage: " << av[0] << " <config file>" << std::endl;
    	return 1;
  	}
	if (!verifyConf(av[1]))
	{
		return (1);
	}
	std::string confFilename;
	Config config;
	confFilename = av[1];

	Server server(config, confFilename);
	return 0;
}
