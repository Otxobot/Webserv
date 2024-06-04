#include "Response.hpp"
#include "../cgi/cgi.hpp"

void    Response::handle_GET_CGI()
{
    std::string content = runCGI(this->_request, "html/primera_pagina", "html/primera_pagina/cgiscript.py");
    std::cout << "content from runCGI: " << content << std::endl;
    this->_response.append("HTTP/1.1");
    this->_response.append(" ");
    this->_response.append("200 OK");
    this->_response.append("\r\n");
    this->_response.append("Content-Type: text/html\r\n");
    this->_response.append("Content-Length: ");
    std::stringstream ss;
    ss << content.length();
    this->_response.append(ss.str());
    this->_response.append("\r\n");
    this->_response.append("\r\n");
    this->_response.append(content);
}
