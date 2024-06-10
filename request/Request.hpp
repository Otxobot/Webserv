/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 15:28:36 by abasante          #+#    #+#             */
/*   Updated: 2024/05/28 12:05:56 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#define REQUEST_URI_MAX_LENGTH 2048
#define HEADER_MAX_LENGTH 1000
#define VALUE_MAX_LENGTH 4000

//  Information 1xx
#define CONTINUE 100
#define SWITCHING_PROTOCOL 101

// Successful 2xx
#define OK 200
#define CREATED 201
#define ACCEPTED 202
#define NON_AUTHORITATIVE 203
#define NO_CONTENT 204
#define RESET_CONTENT 205
#define PARTIAL_CONTENT 206

// Redirection 3xx
#define MULTIPLE_CHOICES 300
#define MOVED_PERMANENTLY 301
#define FOUND 302
#define SEE_OTHER 303
#define NOT_MODIFIED 304
#define USE_PROXY 305

// Client Error 4xx
#define BAD_REQUEST 400
#define UNAUTHORIZED 401
#define PAYMENT_REQUIRED 402
#define FORBIDEN 403
#define NOT_FOUND 404
#define METHOD_NOT_ALLOWED 405
#define NOT_ACCEPTABLE 406
#define PROXY_AUTHENTCATION_REQUIRED 407
#define REQUEST_TIMEOUT 408
#define CONFLICT 409
#define GONE 410
#define LENGTH_REQUIRED 411
#define PRECONDITION_FAILED 412
#define REQUEST_ENTITY_TOO_LARGE 413
#define REQUEST_URI_TOO_LONG 414
#define UNSUPPORTED_MEDIA_TYPE 415
#define REQUEST_RANGE_NOT_SATISFIABLE 416
#define EXPECTATION_FAILED 417

// Server Error 5xx
#define INTERNAL_SERVER_ERROR 500
#define NOT_IMPLEMENTED 501
#define BAD_GETEWAY 502
#define SERVICE_UNABAILABLE 503
#define GATEWAY_TIMEOUT 504
#define HTTP_VERSION_NOT_SUPPORTED 505

#include <string>
#include <iostream>
#include <map>
#include <string.h>
#include <stdlib.h>

class Request
{
    private:
        std::string request;
        std::string Method;
        std::string target;
        std::string queryUrl;
        std::string url;
        std::string protocol;
        std::string body;
        int			port;
        int         _statusCode;
        std::string ContentDiposition;
        std::string name;
        std::string value;

        std::string contentType;
    public:
        std::map<std::string, std::string> headers;
        Request();
        ~Request();
        int		Request_start(std::string request);
        int     request_line();
        int     request_headers();
        int     request_body();
        void    printRequestInformation();
        void    reset();
        std::string getTarget();
        int     getPort();
        std::string &getProtocol();
        std::string getMethod();
        int     getStatusCode();
        std::string getQueryString() const; // nuevo método
        std::string getReqValue(const std::string& key) const; // nuevo método
};

#endif