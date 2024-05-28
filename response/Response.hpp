/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:00:43 by abasante          #+#    #+#             */
/*   Updated: 2024/05/22 17:18:08 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <map>
#include "../request/Request.hpp"
#include "../cgi/cgi.hpp"// Agrega la inclusión de Request.hpp aquí

class Response {
private:
    int statusCode;
    std::string body;
    std::map<std::string, std::string> headers;
    std::map<int, std::string> statusMessages;

    void initStatusMessages();

public:
    Response();

    std::string generateStatusLine();
    std::string generateHeaders();
    std::string readFile(const std::string& path);
    std::string directoryListing(const std::string& path);
    std::string getContentType(const std::string& path);
    std::string generateResponse(Request& request, const std::string& root); // Declaración de generateResponse
};

#endif // RESPONSE_HPP
