/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseo.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:17:05 by abasante          #+#    #+#             */
/*   Updated: 2024/06/13 11:42:43 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.hpp"

int		spacesCount(std::string line) {
	int space;

	space = 0;
	while (line[space] == '\t' || line[space] == ' ') {
		space++;
	}
	return (space);
}

bool		finishCheck(int listCount, int rootCount, int nameCount, int locaCount) {
	if (listCount == 0 || rootCount == 0 || nameCount == 0 || locaCount == 0)
		return false;
	if (listCount > 1 || locaCount > 1 || rootCount > 1 || nameCount > 1)
		return false;
	return true;
}

bool		listenFunction(std::string line, std::vector<int> &ports) {
	int					i = 0;
	size_t				auxi;
	std::string			auxs;
	std::string 		arg1;
	std::string 		arg2;
	std::string			arg3;
	std::istringstream 	iss(line);

	iss >> arg1 >> arg2 >> arg3;
	if(arg1 != "listen:" || arg2.empty() || !arg3.empty())
		return false;

	auxi = arg2.find(":");
	if (auxi != std::string::npos) {
		auxs = arg2.substr(auxi + 1);
		
		while (auxs[i]) {
			if (auxs[i] < '0' || auxs[i] > '9')
				return false;
			i++;
		}
		int port = atoi(auxs.c_str());
        if (port > 0 && port <= 65535) {
            ports.push_back(port);
			if (!checkPorts(ports))
				return false;
        }
	}
	return true;
}

bool	locationFunction(std::ifstream &file, std::string &line, int &index) {
	std::string 		arg1;
	std::string			arg2;

	if (!firstLocation(line))
		return false;
	while (std::getline(file, line)) {
		std::istringstream 	iss(line);
		iss >> arg1 >> arg2;
		if (spacesCount(line) == 2 && arg1 != "location:") {
			std::cerr << "no recognize parameter error" << std::endl;
			return false;
		}
		else if (spacesCount(line) == 2 && doesNotStartWithDot(line)) {
			std::cerr << "secondLocation error" << std::endl;
			return false;
		}
		else if (spacesCount(line) == 4 && !indoorLocation(line)) {
			std::cerr << "indoorLocation error" << std::endl;
			return false;
		}
		else if (!line.empty() && (spacesCount(line) != 2 && spacesCount(line) != 4)) {

			std::cerr << "to many spaces error:  "<< line << std::endl;
			return false;
		}
		else if (arg1 == "server:") {
			std::cerr << "server without separation error: " << std::endl;
			return false;
		}
		else if (line.empty()) {
			index++;
			return true;
		}
		index++;
	}
	return true;
}

bool 	serverFunction(std::string line, int &servCount, int &listCount, int &locaCount, int &rootCount, int &nameCount) {
	std::string arg1;
	std::string arg2;
	std::istringstream iss(line);

	iss >> arg1 >> arg2;
	if(arg1 != "server:" || !arg2.empty())
		return false;
	if (servCount == 0) {
		servCount++;
	} else {
		if (listCount == 0 || locaCount == 0 || rootCount == 0 || nameCount == 0) {
			std::cerr << "param cantity wrong error" << std::endl;
			return false;
		}
		if (listCount > 1 || locaCount > 1 || rootCount > 1 || nameCount > 1) {
			std::cerr << "param cantity wrong error" << std::endl;
			return false;
		}
		listCount = 0;
		locaCount = 0;
		nameCount = 0;
		rootCount = 0;
		servCount++;
	}
	return true;
}

bool	paramsFunction(std::ifstream &file, std::vector<int> &ports, std::string &line, int &listCount, int &locaCount, int &rootCount, int &nameCount, int &index) {
	std::string arg1;
	std::string arg2;
	std::string arg3;
	std::istringstream iss(line);

	iss >> arg1 >> arg2 >> arg3;
	if(line.find("servername:") != std::string::npos) {
		if(arg1 != "servername:" || arg2.empty() || !arg3.empty()) {
			std::cerr << "servername error" << std::endl;
			return false;
		}
		nameCount++;
	}
	else if (line.find("listen:") != std::string::npos) {
		if(!listenFunction(line, ports)) {
			std::cerr << "listenFunction error" << std::endl;
			return false;
		}
		listCount++;
	}
	else if(line.find("root:") != std::string::npos) {
		if(arg1 != "root:" || !arg3.empty() || !FtRoot(line)) {
			std::cerr << "FtRoot error" << std::endl;
			return false;
		}
		rootCount++;;
	}
	else if (line.find("location:") != std::string::npos) {
		if (!locationFunction(file, line, index)) {
			std::cerr << "locationFunction error" << std::endl;
			return false;
		}
		locaCount++;
	}
	else if (line.find("buffer_size:") != std::string::npos) {
		if (!Buffersize(line)) {
			std::cerr << "buffer_size error" << std::endl;
			return false;
		}
	}
	else if (line.find("error_page:") != std::string::npos) {
		if (!ErrorPage(line)) {
			std::cerr << "error_page error" << std::endl;
			return false;
		}
	}
	else {
		std::cerr << "unrecognized parameter error" << std::endl;
		return false;
	}
	return true;
}

bool	verifyConf(const std::string &fileName) {
	std::ifstream file(fileName.c_str());
	if (!file.is_open()) {
		std::cerr << "Error: No se pudo abrir el archivo" << fileName << std::endl;
		return false;
	}

	std::string 		line;
	std::vector	<int>	ports;

	int					index = 1;
	int					servCount = 0;
	int					listCount = 0;
	int					rootCount = 0;
	int					nameCount = 0;
	int					locaCount = 0;
	
	while (std::getline(file, line)) {
		if (line.empty() || line[0] == '#') {
			continue ;
		}
		if (spacesCount(line) == 0) {
			if (!serverFunction(line, servCount, listCount, locaCount, rootCount, nameCount)) {
				std::cerr << "serverFunction error" << std::endl;
				file.close();
				return false;
			}
		} else if (spacesCount(line) == 2) {
				if (!paramsFunction(file, ports, line, listCount, locaCount, rootCount, nameCount, index)) {
					parseError(line, "Params Error", index);
					file.close();
					return false;
				}
		} else {
			parseError(line, "Space Error", index);
			file.close();
			return false;
		}
		index++;
	}
	if (!finishCheck(listCount, rootCount, nameCount, locaCount)) {
		parseError(line, "Params Error", index);
		file.close();
		return false;
	}
	file.close();
	std::cout << "Conf file correct" << std::endl;
	return true;
}
