# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/10/18 15:48:36 by ppaglier          #+#    #+#              #
#    Updated: 2021/11/30 17:37:16 by ppaglier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	webserv

SRC_DIR		=	.
HEADER_DIR	=	includes
OBJ_DIR		=	objs
SRC_DIR		=	srcs

CXX			=	g++
CXXFLAGS	+=	-Wall -Wextra -Werror -std=c++98 -g

HEADER_FILE	=	$(HEADER_DIR)/webserv.hpp \
				$(HEADER_DIR)/Ressource.hpp \
				$(HEADER_DIR)/HttpVersion.hpp \
				$(HEADER_DIR)/HttpStatusCode.hpp \
				$(HEADER_DIR)/HttpHeaders.hpp \
				$(HEADER_DIR)/HttpRequest.hpp \
				$(HEADER_DIR)/HttpResponse.hpp \
				$(HEADER_DIR)/utils.hpp \
				$(HEADER_DIR)/Json.hpp \
				$(HEADER_DIR)/utils/ci_less.hpp \
				$(HEADER_DIR)/utils/Byte.hpp \
				$(HEADER_DIR)/utils/Token.hpp \
				$(HEADER_DIR)/utils/Lexer.hpp \
				$(HEADER_DIR)/utils/Block.hpp \
				$(HEADER_DIR)/utils/Parser.hpp \
				$(HEADER_DIR)/utils/MimeTypes.hpp \
				$(HEADER_DIR)/Config.hpp \
				$(HEADER_DIR)/http/Server.hpp \
				$(HEADER_DIR)/http/Route.hpp

SRC_FILE	=	${SRC_DIR}/main.cpp \
				${SRC_DIR}/Ressource.cpp \
				$(SRC_DIR)/HttpStatusCode.cpp \
				$(SRC_DIR)/HttpHeaders.cpp \
				$(SRC_DIR)/HttpRequest.cpp \
				$(SRC_DIR)/HttpResponse.cpp \
				${SRC_DIR}/utils.cpp \
				${SRC_DIR}/Json.cpp \
				$(SRC_DIR)/utils/Byte.cpp \
				$(SRC_DIR)/utils/Token.cpp \
				$(SRC_DIR)/utils/Lexer.cpp \
				$(SRC_DIR)/utils/Block.cpp \
				$(SRC_DIR)/utils/Parser.cpp \
				$(SRC_DIR)/utils/MimeTypes.cpp \
				$(SRC_DIR)/Config.cpp \
				$(SRC_DIR)/http/Server.cpp \
				$(SRC_DIR)/http/Route.cpp

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

initService:
	@echo "${_BLUE}[Shutdown local services..]${_END}"
	@sudo service nginx stop
	@echo "${_BLUE}[Shutdown local services done!]${_END}"

.PHONY: all clean fclean re test