# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ggoncalv <ggoncalv@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/07/06 11:14:47 by ggoncalv          #+#    #+#              #
#    Updated: 2026/07/15 15:50:13 by ggoncalv         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

CXX := c++
CXXFLAGS += -Wall -Werror -Wextra -g
CXXFLAGS += -std=c++98
CXXFLAGS += -Wshadow
#CXXFLAGS += -Wno-shadow

RM := rm -f

INCLUDES := -I./include
VALGRIND := valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes
GDB := gdb -q --tui

SRCS := src/main.cpp src/Server.cpp src/Poller.cpp src/Client.cpp src/Parser.cpp\
 		src/CommandHandler.cpp src/PassCommand.cpp src/UserCommand.cpp\
		src/NickCommand.cpp src/Channel.cpp src/JoinCommand.cpp\
		src/PrivmsgCommand.cpp src/PartCommand.cpp src/QuitCommand.cpp\
		src/TopicCommand.cpp src/InviteCommand.cpp src/KickCommand.cpp\
		src/ModeCommand.cpp

OBJS := $(SRCS:.cpp=.o)

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
	@echo "\n »» Compiling ««"
	@$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)
	@echo "\e[1;32mEXERCISE COMPILED.\e[0m\n"

val:	$(NAME)
	@$(VALGRIND) ./$(NAME) $(ARGS)

gdb:	$(NAME)
	@$(GDB) -ex "break main" -ex "run" --args ./$(NAME) $(ARGS)

clean:
	@$(RM) $(OBJS)
	@echo "\n\e[1;33mObject files removed.\e[0m"

fclean: clean
	@echo "\e[1;36mExecutable file removed.\e[0m"
	@$(RM) $(NAME)
	@echo "\e[1;35mEVERYTHING CLEANED.\e[0m\n"

re: fclean all

.PHONY: all clean fclean re val gdb