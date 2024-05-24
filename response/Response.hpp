/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikferna <mikferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:00:43 by abasante          #+#    #+#             */
/*   Updated: 2024/05/23 12:35:27 by mikferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <dirent.h>
#include <ctime>
#include <cstdlib>
#include "../Webserv.hpp"

#define OK 200
#define CREATED 201
#define NO_CONTENT 204
#define BAD_REQUEST 400
#define FORBIDDEN 403
#define NOT_FOUND 404
#define METHOD_NOT_ALLOWED 405
#define PAYLOAD_TOO_LARGE 413
#define INTERNAL_SERVER_ERROR 500
#define BAD_GATEWAY 502
#define GATEWAY_TIMEOUT 504

class Config;

class Response {
private:
    std::vector<Config> _servers;
    Request *_request;
    Config *_server;
    std::string _body;
    std::string _statusLine;
    std::string _headers;
    std::string _resp;
    int _statusCode;
    bool isCGI;

public:
    Response();
    void createResponse(std::vector<Config> &, Request &);
    void createBody();
    std::string getRespContentType();
    void setErrorPage(int errorCode);
    std::string getDefaultErrorPage(int status);
    std::string &getStatusLine();
    std::string getRespHeader();
    std::string getBodyContent();
    ~Response();
    void clear();
    bool isDirectory(const std::string &s);
    void deleteMethod(std::string _Path);
    void parseCgiResp(std::string &s);
    void postMethod();
    std::string getUploadPath();
    Location getRedirection(std::string locName);
    int checkForPerfectMatch(std::string _path, std::vector<Location> _locations);
    int checkForMatchOne(std::string _path, std::vector<Location> _locations);
    std::string getDefaultPage();
};

#endif
