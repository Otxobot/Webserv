/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 15:28:36 by abasante          #+#    #+#             */
/*   Updated: 2024/05/14 12:49:45 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define RESET "\033[0m"

#include <string>
#include <iostream>

class Request
{
    private:
        std::string request;
        std::string _status;
    public:
        Request();
        ~Request();
        int		Request_start(std::string request);
        int     request_line();
        int     request_headers();
        int     request_body();
        void    printRequestInformation();
        
};

#endif