/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 15:53:02 by mikferna          #+#    #+#             */
/*   Updated: 2024/05/28 12:07:11 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <map>
#include <sstream>
#include "../request/Request.hpp"
#include <cstdio>
// #include "../Webserv.hpp"

std::string runCGI(Request& _request, const std::string& root, const std::string& cgi_path);

#endif