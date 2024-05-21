/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   include.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcarrasc <hcarrasc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 16:00:25 by hcarrasc          #+#    #+#             */
/*   Updated: 2024/05/01 14:42:30 by hcarrasc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDE_HPP
#define INCLUDE_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>

bool        Empty(std::string &line);
bool        Allow(std::string &line);
bool        AutoIndex(std::string &line);
bool        ErrorPage(std::string &line);
bool        Buffersize(std::string &line);

bool        isInteger(std::string n);
bool		FtRoot(std::string line);
bool	    firstLocation(std::string line);
bool		secondLocation(std::string line);
bool	    indoorLocation(std::string line);
bool 		checkPorts(std::vector<int> &Allport);
bool        doesNotStartWithDot(std::string &line);
std::string trimChars(const std::string& str, const std::string& chars);

void		parseError(std::string line, std::string error, int index);

#endif
