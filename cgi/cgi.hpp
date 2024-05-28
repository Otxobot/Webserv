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
#include "../Webserv.hpp"

std::string runCGI(Request& _request, const std::string& root, const std::string& cgi_path);
#endif