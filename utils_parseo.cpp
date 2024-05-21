/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parseo.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 16:02:01 by hcarrasc          #+#    #+#             */
/*   Updated: 2024/05/21 13:17:30 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.hpp"

bool isInteger(std::string n)
{
	for(std::string::size_type i = 0; i < n.length(); i++)
	{
		if(!std::isdigit(n[i]))
			return false;
	}
	return true;
}

bool	FtRoot(std::string line) {
	std::string arg1;
	std::string arg2;
	std::string arg3;
	int			i = 0;
	int			c = 0;

	std::istringstream iss(line);
	iss >> arg1 >> arg2 >> arg3;
	
	if (arg1 != "root:" || !arg3.empty() || (arg2[0] != '"' && arg2[0] != '.'))
		return false;
	if (arg2[i] == '"') {
		while (arg2[i]) {
			if (arg2[i] == '"')
				c++;
			i++;
		}
	}
	if (c != 0 && c != 2)
		return false;
	if (c == 0 && arg2[0] == '.' && arg2[1] == '/')
		return true;
	if (c == 2 && arg2[0] == '"' && arg2[1] == '.' && arg2[2] == '/')
		return true;
	return false;
}

bool	indoorLocation(std::string line) {
	std::istringstream iss(line);
	std:: string arg1;

	iss >> arg1;

	if (arg1 == "allow:") {
		if (!Allow(line)) {
			return false;
		}
	}
	else if (arg1 == "buffer_size:") {
		if(!Buffersize(line)) {
			std::cerr << "location buffer_size error" << std::endl;
			return false;
		}
	}
	else if (arg1 == "autoindex:") {
		if(!AutoIndex(line)) {
			std::cerr << "location autoindex error" << std::endl;
			return false;
		}
	}
	else if (arg1  == "redirect:") {
		if(!Empty(line)) {
			std::cerr << "location redirect error" << std::endl;
			return false;
		}
	}
	else if (arg1 == "error_page:") {
		if(!ErrorPage(line)) {
			std::cerr << "location error_page error" << std::endl;
			return false;
		}
	}
	else if(arg1 == "cgi:") {
		if(!Empty(line)) {
			std::cerr << "location cgi error" << std::endl;
			return false;
		}
	}
	else if(arg1 == "file:") {
		if(!Empty(line)) {
			std::cerr << "location file error" << std::endl;
			return false;
		}
	}
	else if(arg1 == "root:") {
		if(!Empty(line)) {
			std::cerr << "location root error" << std::endl;
			return false;	
		}
	}
	else if(arg1 == "handle_post:") {
		if(!Empty(line)) {
			std::cerr << "location handle_post error" << std::endl;
			return false;
		}
	}
	else if(arg1 == "handle_delete:") {
		if(!Empty(line)) {
			std::cerr << "location handle_delete error" << std::endl;
			return false;
		}
	}
	else if(arg1 == "upload:") {  
		if(!Empty(line)) {
			std::cerr << "location upload error" << std::endl;
			return false;
		}
	}
	else {
		std::cerr << "location param error" << std::endl;
		return false;
	}
	return true;
}

bool	firstLocation(std::string line) {
	std::string arg1;
	std::string arg2;
	std::string arg3;

	std::istringstream iss(line);
	iss >> arg1 >> arg2 >> arg3;

	if (arg1 != "location:" || !arg3.empty())
		return false;
	if (arg2 != "/" && arg2 != "\"/\"")
		return false;
	return true;
}

void	parseError(std::string line, std::string error, int index) {
	std:: cout << line << " <--- " << "[line: " << index << "] ";
	std::cerr << error << std::endl;
}

bool checkPorts(std::vector<int> &Allport)
{
	for(std::vector<int>::size_type i = 0; i < Allport.size(); i++)
	{
		for(std::vector<int>::size_type j = i + 1; j < Allport.size(); j ++)
		{
			if(Allport[i] == Allport[j])
			{
				std::cout << Allport[i] << " <---- ";
				return false;
			}
		}
	}
	return true;
}