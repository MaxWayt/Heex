##
## Makefile
##

CXX	= g++

RM	= rm -f

CXXFLAGS	+= -Wextra -Wall -Werror
CXXFLAGS	+= -I. -std=c++11 -ggdb3
CXXFLAGS	+= -IMapGeneratorNode -IConfigParser
CXXFLAGS	+= -fno-omit-frame-pointer -O0
CXXFLAGS	+= -fopenmp

LDFLAGS	= -lGL -lGLU -lglut -lgomp -lboost_system

HEEX	= heex
HEEXMAIN	= main

MAPGENERATOR	= mapgenerator
MAPGENERATORMAIN	= MapGeneratorNode/main

SRCS	= Object.cpp Map.cpp MapGenerator.cpp ModelMgr.cpp ConfigParser/Config.cpp

OBJS	= $(SRCS:.cpp=.o)

all: $(HEEX) $(MAPGENERATOR)

$(HEEX): $(OBJS)
	$(CXX) $(CXXFLAGS) $(HEEXMAIN).cpp -c -o $(HEEXMAIN).o
	$(CXX) $(OBJS) $(HEEXMAIN).o -o $(HEEX) $(LDFLAGS)

$(MAPGENERATOR): $(OBJS)
	$(CXX) $(CXXFLAGS) $(MAPGENERATORMAIN).cpp -c -o $(MAPGENERATORMAIN).o
	$(CXX) $(OBJS) $(MAPGENERATORMAIN).o -o $(MAPGENERATOR) $(LDFLAGS)

clean:
	$(RM) $(OBJS)
	$(RM) $(HEEXMAIN).o
	$(RM) $(MAPGENERATORMAIN).o

fclean: clean
	$(RM) $(HEEX)
	$(RM) $(MAPGENERATOR)


re: fclean all

.PHONY: all clean fclean re $(MAPGENERATOR) $(HEEX)

