# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mikferna <mikferna@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/29 11:39:15 by abasante          #+#    #+#              #
#    Updated: 2024/05/22 16:00:13 by mikferna         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

FILES = main ./server/Server ./config/Config ./config/Location ./request/Request ./response/Response \
		./parseo_conf/parseo ./parseo_conf/utils_parseo ./parseo_conf/location_utils ./cgi/cgi

SRC = $(addsuffix .cpp, $(FILES))

OBJ = $(addsuffix .o, $(FILES))

CFLAGS = c++ -Wall -Wextra -Werror --std=c++98 #-fsanitize=address -g3

all : $(NAME)

$(NAME): $(OBJ)
		$(CFLAGS) $(OBJ) -o $(NAME)

%.o:%.cpp
	$(CFLAGS) -c $< -o $@

clean :
		rm -f $(OBJ)
fclean : clean
		rm -f $(NAME)
re : fclean all

.PHONY: all clean fclean re