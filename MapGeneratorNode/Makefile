##
## Makefile
## 
## Made by maxime ginters
## Login   <ginter_m@epitech.eu>
## 
## Started on  Sun Mar 24 16:06:38 2013 maxime ginters
## Last update Thu Apr 04 23:23:08 2013 maxime ginters
##

CC	= g++

RM	= rm -f

CXXFLAGS	+= -Wextra -Wall -Werror
CXXFLAGS	+= -I. -I../ConfigParser -I../

LDFLAGS	= -lboost_system

NAME	= mapgenerator

SRCS	= main.cpp ../ConfigParser/Config.cpp ../Object.cpp ../MapGenerator.cpp ../Map.cpp

OBJS	= $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(LDFLAGS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

