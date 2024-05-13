/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abasante <abasante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 11:39:21 by abasante          #+#    #+#             */
/*   Updated: 2024/05/13 16:42:09 by abasante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "./server/Server.hpp"
#include "./config/Config.hpp"
#include "./config/Location.hpp"

//Aqui van todos los includes que se usaran en el proyecto, en los archivos incluimos Webserv.hpp y ya.
//La configuración va a ser una clase, el servidor va a ser una clase, el request y la respuesta tambien van a ser una clase.
//La configuración despues de parsear va a ser una clase donde esten puestos todos los flags dentro de los attributos de la clase para que el servidor sepa que tiene que hacer

/*Vamos a empezar con un archivo de configuración simple que solo sea un GET con una pagina web unicamente.*/

#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read
#include <cerrno>
#include <vector> // Include the necessary header file
#include <string>

#endif