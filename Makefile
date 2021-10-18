# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/10/18 15:48:36 by ppaglier          #+#    #+#              #
#    Updated: 2021/10/18 15:50:08 by ppaglier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	webserv

SRC_DIR		=	.
HEADER_DIR	=	includes
OBJ_DIR		=	objs
SRC_DIR		=	srcs

CXX			=	clang++
CXXFLAGS	+=	-Wall -Wextra -Werror -std=c++98

HEADER_FILE	=	$(HEADER_DIR)/webserv.hpp

SRC_FILE	=	${SRC_DIR}/main.cpp

OBJ_SRC		=	$(SRC_FILE:%.cpp=$(OBJ_DIR)/%.o)
OBJ_HEAD	=	$(HEADER_FILE:%.hpp=$(OBJ_DIR)/%.o)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: %.hpp
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(NAME)

$(NAME): $(OBJ_SRC) $(OBJ_HEAD)
	@echo "\e[34m[Objs Compiled..]"
	@$(CXX) $(CXXFLAGS) $(OBJ_SRC) $(LIBS) -o $(NAME)
	@echo "\e[34m[$(NAME) Compiled..]\033[0m"

clean:
	@echo "\e[34m[Cleaning Objs..]"
	rm -fr $(OBJ_DIR)

fclean:	 clean
	@echo "\e[34m[Full Cleaning $(NAME)..]"
	rm -f $(NAME)

re:	fclean all

test: all
	@echo "\e[34m[Starting test for $(NAME)..]\033[0m"
	./$(NAME)
	@echo "\e[34m[Test for $(NAME) done !]\033[0m"

.PHONY: all clean fclean re test