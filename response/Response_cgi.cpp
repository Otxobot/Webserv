#include "Response.hpp"
#include "../cgi/cgi.hpp"

int    Response::handle_GET_CGI()
{
    std::string content = runCGI(this->_request, this->_server._root , this->_server._root + "/" + this->_server._locations["/cgi-bin"]._cgi);
    if (!content.empty())
    {
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
        std::cout << "content2: " << content << std::endl << std::endl;
        this->_response.append(content);
        return (0);
    }
    else
        return (1);
}
