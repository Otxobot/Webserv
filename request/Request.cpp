/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikferna <mikferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 15:28:33 by abasante          #+#    #+#             */
/*   Updated: 2024/05/29 17:02:48 by mikferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include <fstream>
#include <string>

Request::Request() : target(""), queryUrl(""), url(""), _statusCode(200)
{
	this->headers.clear();
}

Request::~Request()
{
    //Default destructor
	this->target.clear();
	this->url.clear();
}

void	Request::printRequestInformation()
{
	std::cout << "Method: " << this->Method << std::endl;
	std::cout << "Target: " << this->target << std::endl;
	std::cout << "Protocol: " << this->protocol << std::endl; 
	for (std::map<std::string, std::string>::iterator it = this->headers.begin(); it != this->headers.end(); it++)
	{
		if (!it->second.empty())
			std::cout << "" << it->first << ": " << it->second  << std::endl;
	}
	if (!this->queryUrl.empty())
		std::cout << "Query: " << this->queryUrl << std::endl;
	if (!this->url.empty())
		std::cout << "Url: " << this->url << std::endl;
	std::cout << this->body << std::endl;
}

int Request::Request_start(std::string request)
{
	std::ofstream file("/home/mikferna/Desktop/Webserv/html/request.txt");
	if (file.is_open()) {
		file << request;
		file.close();
	} else {
		std::cerr << "Failed to open file for writing." << std::endl;
	}
    int status_code = 0;
	this->request = request;
	std::cout << this->request << std::endl;
	std::string contentLengthKey = "Content-Length:";
    size_t pos = this->request.find(contentLengthKey);
    if (pos != std::string::npos) {
        size_t start = pos + contentLengthKey.length();
        size_t end = this->request.find("\r\n", start);
        if (end != std::string::npos) {
            std::string contentLengthValue = this->request.substr(start, end - start);
			this->bodyLength = std::atoi(contentLengthValue.c_str());
			// if (this->bodyLength > BODY_MAX_LENGTH) {
			// 	return REQUEST_ENTITY_TOO_LARGE;
        }
    }
	if ((status_code = this->request_line()) || (status_code = this->request_headers()) || (status_code = this->request_body()))
		{
			this->_statusCode = status_code;
			return (status_code);
		}
	std::cout <<"+++++++++++++++++++++++++++++ Request ++++++++++++++++++++++++++++++++" << std::endl;
	//printRequestInformation();
	return 0;
}

int		Request::request_line()
{
	std::string Method;
	if (request.find("\r\n") != std::string::npos)
	{
		std::string tmp = request.substr(0, request.find(' '));

		if (tmp == "GET" || tmp == "POST" || tmp == "DELETE")
		{
			this->Method = tmp;
			request.erase(0, this->Method.length() + 1);
		}
		else
			return NOT_IMPLEMENTED;
		if (request.find(' ') == 0)
			return BAD_REQUEST;
		tmp = request.substr(0, request.find(' '));
		if (tmp[0] != '/')
			return BAD_REQUEST;

		if (tmp.length() < REQUEST_URI_MAX_LENGTH)
		{
			this->target = tmp;
			if (this->target.find("?") != std::string::npos)
			{
				this->url = this->target.substr(0, this->target.find("?"));
				this->queryUrl = this->target.substr(this->target.find('?') + 1);
			}
			request.erase(0, this->target.length() + 1);
			if (!this->url.empty())
				this->target = this->url;
		}
		else
			return REQUEST_URI_TOO_LONG;

		if (request.find(' ') == 0)
			return BAD_REQUEST;
		
		size_t end = request.find("\r\n");
		tmp = request.substr(0, end);
		
		if (tmp == "HTTP/1.1")
		{
			this->protocol = tmp;
			request.erase(0, end + 2);
		}
		else
			return HTTP_VERSION_NOT_SUPPORTED;
	}
	return EXIT_SUCCESS;
}

std::string ltrim(const std::string &s)
{
	size_t start = s.find_first_not_of(" ");
	return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string &s)
{
	size_t end = s.find_last_not_of(" ");
	return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string &s)
{
	return rtrim(ltrim(s));
}

int		Request::request_headers()
{
	 size_t header_end, header_dil;
	 std::string header;
	 std::string value;

	while ((header_end = this->request.find("\r\n")) != std::string::npos)
	{
		std::string tmp = this->request.substr(0, header_end);
		if (header_end == 0)
		{
			this->request.erase(0, 2);
			break;
		}
		if ((header_dil = tmp.find(':', 0)) != std::string::npos)
		{
			if (header_dil == 0 || tmp[header_dil - 1] == ' ')
			 	return BAD_REQUEST;
			header = tmp.substr(0, header_dil);
			value = tmp.substr(header_dil + 1, header_end - header_dil - 1);
			if (tmp == "Host" && this->headers.count(header))
				return BAD_REQUEST;
			if (header.length() > HEADER_MAX_LENGTH || value.length() > VALUE_MAX_LENGTH)
				return BAD_REQUEST;
			this->headers[header] = trim(value);
		}
		else
		 	return BAD_REQUEST;
		request.erase(0, header_end + 2);
		tmp.clear();
	}
	return EXIT_SUCCESS;
}

//aqui el value lo coge muy mal

int Request::request_body() {
    if (this->headers["Content-Type"].find("boundary") != std::string::npos) {
        std::string boundary = "--" + this->headers["Content-Type"].substr(this->headers["Content-Type"].find("boundary=") + 9);
        size_t pos = 0;
        
        while ((pos = this->request.find(boundary)) != std::string::npos) {
            std::string part = this->request.substr(0, pos);
            this->request.erase(0, pos + boundary.length() + 2); // +2 for CRLF
            
            if (part.find("Content-Disposition") != std::string::npos) {
                size_t dispPos = part.find("Content-Disposition:") + 21;
                size_t dispEnd = part.find(";", dispPos);
                this->ContentDiposition = part.substr(dispPos, dispEnd - dispPos);
                this->headers["Content-Disposition"] = this->ContentDiposition;
                
                size_t namePos = part.find("name=\"") + 6;
                size_t nameEnd = part.find("\"", namePos);
                this->name = part.substr(namePos, nameEnd - namePos);
                this->headers["name"] = this->name;

                size_t filenamePos = part.find("filename=\"");
                if (filenamePos != std::string::npos) {
                    filenamePos += 10;
                    size_t filenameEnd = part.find("\"", filenamePos);
                    std::string filename = part.substr(filenamePos, filenameEnd - filenamePos);
                    this->headers["filename"] = filename;
                }

                size_t contentTypePos = part.find("Content-Type: ");
                if (contentTypePos != std::string::npos) {
                    contentTypePos += 14;
                    size_t contentTypeEnd = part.find("\r\n", contentTypePos);
                    this->contentType = part.substr(contentTypePos, contentTypeEnd - contentTypePos);
                    this->headers["Content-Type"] = this->contentType;
                }
                
                size_t dataStart = part.find("\r\n\r\n") + 4;
                size_t dataEnd = part.find("\r\n--", dataStart);
                this->value = part.substr(dataStart, dataEnd - dataStart);
                this->headers["value"] = this->value;
            }
        }
		std::cout << "Content-Disposition: " << this->headers["Content-Disposition"] << std::endl;
		std::cout << "name: " << this->headers["name"] << std::endl;
		std::cout << "filename: " << this->headers["filename"] << std::endl;
		std::cout << "Content-Type: " << this->headers["Content-Type"] << std::endl;
		std::cout << "value: " << this->headers["value"] << std::endl;
    } else {
        // Handle non-multipart request body
        if (this->request.find("\n") == std::string::npos) {
            this->body = this->request;
        } else {
            while (!this->request.empty()) {
                std::string tmp = this->request.substr(0, this->request.find("\n"));
                if (!tmp.empty() && tmp[tmp.length() - 1] == '\r') {
                    tmp.erase(tmp.length() - 1);
                }
                this->body.append(tmp);
                if (!this->request.empty()) {
                    this->request.erase(0, this->request.find("\n") + 1);
                }
            }
        }
    }
    return EXIT_SUCCESS;
}


std::string Request::getTarget()
{
	if (this->target[0] != '/')
		this->target = "/" + this->target;
	return (this->target);
}

int		Request::getPort()
{
	std::string tmp;
	for (std::map<std::string, std::string>::iterator it = this->headers.begin(); it != this->headers.end(); it++)
		if (it->first == "Host")
			tmp = it->second;
	if (tmp.find(":") != std::string::npos)
	{
		tmp.erase(0, tmp.find(":") + 1);
		this->port = atoi(tmp.c_str());
	}
	else
		this->port = 80;
	
	return this->port;
}

std::string &Request::getProtocol()
{
	return (this->protocol);
}

std::string Request::getMethod()
{
	return (this->Method);
}

int Request::getStatusCode()
{
	return (this->_statusCode);
}

std::string Request::getBody()
{
	return (this->body);
}

int Request::getBodyLength()
{
	return (this->bodyLength);
}

std::string Request::getHeader(const std::string &name) const
{
    std::map<std::string, std::string>::const_iterator it = headers.find(name);
    if (it != headers.end()) {
        return it->second;
    }
    return "";
}

void Request::reset()
{
	this->Method.clear();
    this->target.clear();
    this->protocol.clear();
    this->headers.clear();
    this->queryUrl.clear();
    this->url.clear();
}
