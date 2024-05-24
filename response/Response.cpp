/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikferna <mikferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:00:31 by abasante          #+#    #+#             */
/*   Updated: 2024/05/23 12:39:40 by mikferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response() : _request(NULL), _server(NULL), isCGI(false) {}

std::string &Response::getStatusLine() {
    return _statusLine;
}

void Response::createResponse(std::vector<Config> &servers, Request &request) {
    _servers = servers;
    _request = &request;
    createBody();
}

void Response::createBody() {
    std::string requestPath = _request->getTarget();
    if (requestPath.empty()) {
        requestPath.append("/");
    }
    
    for (size_t i = 0; i < _servers.size(); i++) {
        if (_servers[i].getPort() == _request->getPort()) {
            _server = &_servers[i];
            break;
        }
    }

    if (!_server) {
        setErrorPage(NOT_FOUND);
        return;
    }

    // Handle different HTTP methods
    if (_request->getMethod() == "GET") {
        getMethod(requestPath);
    } else if (_request->getMethod() == "POST") {
        postMethod();
    } else if (_request->getMethod() == "DELETE") {
        deleteMethod(requestPath);
    } else {
        setErrorPage(METHOD_NOT_ALLOWED);
    }

    _statusLine = "HTTP/1.1 " + std::to_string(_statusCode) + " " + getStatusCodeTranslate() + "\r\n";
    _headers = "Content-Type: " + getRespContentType() + "\r\n";
    _headers += "Content-Length: " + std::to_string(_body.size()) + "\r\n\r\n";
    _resp = _statusLine + _headers + _body;
}

void Response::getMethod(std::string _uri) {
    // Logic for handling GET method
    std::string filePath = _server->getRootDir() + _uri;
    if (isDirectory(filePath)) {
        autoindexRun(filePath);
    } else {
        getBody(filePath);
    }
}

void Response::getBody(std::string file_name) {
    std::ifstream file(file_name.c_str(), std::ios::in | std::ios::binary);
    if (!file) {
        setErrorPage(NOT_FOUND);
        return;
    }
    std::ostringstream oss;
    oss << file.rdbuf();
    _body = oss.str();
    file.close();
    _statusCode = OK;
}

bool Response::isDirectory(const std::string &s) {
    struct stat buf;
    if (stat(s.c_str(), &buf) != -1)
        return S_ISDIR(buf.st_mode);
    return false;
}

void Response::autoindexRun(std::string rooted_path) {
    DIR *dir;
    struct dirent *entry;
    dir = opendir(rooted_path.c_str());
    if (!dir) {
        setErrorPage(FORBIDDEN);
        return;
    }

    std::ostringstream oss;
    oss << "<html><body><h1>Index of " << rooted_path << "</h1><ul>";
    while ((entry = readdir(dir)) != NULL) {
        oss << "<li><a href=\"" << entry->d_name << "\">" << entry->d_name << "</a></li>";
    }
    oss << "</ul></body></html>";
    closedir(dir);

    _body = oss.str();
    _statusCode = OK;
}

void Response::setErrorPage(int errorCode) {
    _statusCode = errorCode;
    _body = getDefaultErrorPage(errorCode);
}

std::string Response::getDefaultErrorPage(int status) {
    switch (status) {
        case BAD_REQUEST: return "400 Bad Request";
        case FORBIDDEN: return "403 Forbidden";
        case NOT_FOUND: return "404 Not Found";
        case METHOD_NOT_ALLOWED: return "405 Method Not Allowed";
        case PAYLOAD_TOO_LARGE: return "413 Payload Too Large";
        case INTERNAL_SERVER_ERROR: return "500 Internal Server Error";
        case BAD_GATEWAY: return "502 Bad Gateway";
        case GATEWAY_TIMEOUT: return "504 Gateway Timeout";
        default: return "200 OK";
    }
}

std::string Response::getStatusCodeTranslate() {
    switch (_statusCode) {
        case OK: return "OK";
        case CREATED: return "Created";
        case NO_CONTENT: return "No Content";
        case BAD_REQUEST: return "Bad Request";
        case FORBIDDEN: return "Forbidden";
        case NOT_FOUND: return "Not Found";
        case METHOD_NOT_ALLOWED: return "Method Not Allowed";
        case PAYLOAD_TOO_LARGE: return "Payload Too Large";
        case INTERNAL_SERVER_ERROR: return "Internal Server Error";
        case BAD_GATEWAY: return "Bad Gateway";
        case GATEWAY_TIMEOUT: return "Gateway Timeout";
        default: return "Unknown Status";
    }
}

std::string Response::getRespContentType() {
    if (_request->getReqValue("Content-Type").size()) {
        return _request->getReqValue("Content-Type");
    }

    if (_request->getTarget().find(".html") != std::string::npos)
        return "text/html; charset=UTF-8";
    else if (_request->getTarget().find(".css") != std::string::npos)
        return "text/css";
    else if (_request->getTarget().find(".json") != std::string::npos)
        return "application/json";
    else if (_request->getTarget().find(".xml") != std::string::npos)
        return "application/xml";
    else if (_request->getTarget().find(".js") != std::string::npos)
        return "application/javascript";
    else
        return "text/html; charset=UTF-8";
}

std::string Response::getRespHeader() {
    return _headers;
}

std::string Response::getBodyContent() {
    return _body;
}

void Response::clear() {
    _body.clear();
    _statusLine.clear();
    _headers.clear();
    _resp.clear();
    _statusCode = 0;
    _request = NULL;
    _server = NULL;
    isCGI = false;
}

void Response::deleteMethod(std::string _Path) {
    std::string filePath = _server->getRootDir() + _Path;
    if (!access(filePath.c_str(), F_OK) && !isDirectory(filePath.c_str())) {
        if (!access(filePath.c_str(), W_OK)) {
            if (remove(filePath.c_str()) == 0)
                setErrorPage(OK);
            else
                setErrorPage(INTERNAL_SERVER_ERROR);
        } else
            setErrorPage(FORBIDDEN);
    } else
        setErrorPage(NOT_FOUND);
}

void Response::parseCgiResp(std::string &s) {
    std::string temp;
    std::istringstream iss(s);
    while (std::getline(iss, temp)) {
        if (temp.find("Status:") != std::string::npos)
            _statusLine = temp + "\r\n";
        if (temp.find("Content-Type:") != std::string::npos)
            _headers += temp + "\r\n";
        if (temp.find("<html>") != std::string::npos)
            _body = temp + "\n";
    }
}

void Response::postMethod() {
    std::string _path = _server->getUploadPath();
    std::ofstream _outPutFile;
    std::string bodyContent = _request->getBody();
    _outPutFile.open((_path + "upload").c_str(), std::ios::out | std::ios::binary);
    if (!_outPutFile) {
        setErrorPage(FORBIDDEN);
        return;
    }
    _outPutFile << bodyContent;
    _outPutFile.close();
    _statusCode = CREATED;
}

std::string Response::getUploadPath() {
    return _server->getUploadPath();
}

location Response::getRedirection(std::string locName) {
    return _server->getRedirection(locName);
}

int Response::checkForPerfectMatch(std::string _path, std::vector<location> _locations) {
    for (size_t i = 0; i < _locations.size(); i++) {
        if (_locations[i].getLocation() == _path)
            return i;
    }
    return -1;
}

int Response::checkForMatchOne(std::string _path, std::vector<location> _locations) {
    for (size_t i = 0; i < _locations.size(); i++) {
        if (_locations[i].getLocation() == _path.substr(0, _locations[i].getLocation().length()))
            return i;
    }
    return -1;
}

std::string Response::getDefaultPage() {
    return _server->getDefaultPage();
}

Response::~Response() {}

