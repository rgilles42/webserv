# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ppaglier <ppaglier@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/10/18 15:48:36 by ppaglier          #+#    #+#              #
#    Updated: 2022/01/30 02:48:50 by ppaglier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	webserv

SRC_DIR		=	.
HEADER_DIR	=	includes
OBJ_DIR		=	objs
SRC_DIR		=	srcs

CXX			=	g++
CXXFLAGS	+=	-Wall -Wextra -Werror -std=c++98 -g3

HEADER_FILE	=	$(HEADER_DIR)/webserv.hpp \
				$(HEADER_DIR)/core/Core.hpp \
				$(HEADER_DIR)/Logger.hpp \
				$(HEADER_DIR)/utils/common.hpp \
				$(HEADER_DIR)/utils/Socket.hpp \
				$(HEADER_DIR)/utils/Resource.hpp \
				$(HEADER_DIR)/utils/ci_less.hpp \
				$(HEADER_DIR)/utils/OutStream.hpp \
				$(HEADER_DIR)/utils/Byte.hpp \
				$(HEADER_DIR)/utils/Token.hpp \
				$(HEADER_DIR)/utils/Lexer.hpp \
				$(HEADER_DIR)/utils/Block.hpp \
				$(HEADER_DIR)/utils/Parser.hpp \
				$(HEADER_DIR)/utils/MimeTypes.hpp \
				$(HEADER_DIR)/utils/Address.hpp \
				$(HEADER_DIR)/utils/Directive.hpp \
				$(HEADER_DIR)/utils/Env.hpp \
				$(HEADER_DIR)/utils/Args.hpp \
				$(HEADER_DIR)/utils/Singleton.hpp \
				$(HEADER_DIR)/Config.hpp \
				$(HEADER_DIR)/http/Server.hpp \
				$(HEADER_DIR)/http/Route.hpp \
				$(HEADER_DIR)/http/HttpVersion.hpp \
				$(HEADER_DIR)/http/HttpStatusCode.hpp \
				$(HEADER_DIR)/http/HttpHeaders.hpp \
				$(HEADER_DIR)/http/HttpRequest.hpp \
				$(HEADER_DIR)/http/HttpResponse.hpp \
				$(HEADER_DIR)/events/IEvents.hpp \
				$(HEADER_DIR)/events/ClientEvent.hpp \
				$(HEADER_DIR)/events/ServerEvent.hpp \
				$(HEADER_DIR)/events/Poll.hpp \
				$(HEADER_DIR)/events/CGIEvent.hpp \
				$(HEADER_DIR)/events/EventsManager.hpp \
				$(HEADER_DIR)/methods/Methods.hpp

SRC_FILE	=	${SRC_DIR}/main.cpp \
				${SRC_DIR}/core/Core.cpp \
				$(SRC_DIR)/Logger.cpp \
				$(SRC_DIR)/utils/common.cpp \
				$(SRC_DIR)/utils/Socket.cpp \
				${SRC_DIR}/utils/Resource.cpp \
				$(SRC_DIR)/utils/OutStream.cpp \
				$(SRC_DIR)/utils/Byte.cpp \
				$(SRC_DIR)/utils/Token.cpp \
				$(SRC_DIR)/utils/Lexer.cpp \
				$(SRC_DIR)/utils/Block.cpp \
				$(SRC_DIR)/utils/Parser.cpp \
				$(SRC_DIR)/utils/MimeTypes.cpp \
				$(SRC_DIR)/utils/Address.cpp \
				$(SRC_DIR)/utils/Directive.cpp \
				$(SRC_DIR)/utils/Env.cpp \
				$(SRC_DIR)/utils/Args.cpp \
				$(SRC_DIR)/Config.cpp \
				$(SRC_DIR)/http/Server.cpp \
				$(SRC_DIR)/http/Route.cpp \
				$(SRC_DIR)/http/HttpVersion.cpp \
				$(SRC_DIR)/http/HttpStatusCode.cpp \
				$(SRC_DIR)/http/HttpHeaders.cpp \
				$(SRC_DIR)/http/HttpRequest.cpp \
				$(SRC_DIR)/http/HttpResponse.cpp \
				$(SRC_DIR)/events/ClientEvent.cpp \
				$(SRC_DIR)/events/ServerEvent.cpp \
				$(SRC_DIR)/events/Poll.cpp \
				$(SRC_DIR)/events/CGIEvent.cpp \
				$(SRC_DIR)/events/EventsManager.cpp \
				$(SRC_DIR)/methods/Methods.cpp


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
