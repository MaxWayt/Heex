##
## Makefile
##

CC	= g++

RM	= rm -f

CXXFLAGS	+= -Wextra -Wall -Werror
CXXFLAGS	+= -I. -std=c++11 -ggdb3
CXXFLAGS	+= -fno-omit-frame-pointer -O0

LDFLAGS	= -lGL -lGLU -lglut

NAME	= heex

SRCS	= main.cpp Object.cpp Map.cpp MapGenerator.cpp

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

