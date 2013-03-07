CXX		= g++
CXXFLAGS	= -Wall
LDFLAGS		= 
LIBS		=
SRCS		:= $(wildcard src/*.cc) optparser_example.cc
OBJS		= $(SRCS:%.cc=%.o)
PROGRAM		= optpex
DEST		=


all:		$(PROGRAM)

$(PROGRAM):	$(OBJS)
		$(CXX) $(OBJS) $(LDFLAGS) $(LIBS) -o $(PROGRAM)

.cc .o:
		$(CXX) -c $<

clean:
		rm -f *.o $(PROGRAM)
		rm -f src/*.o

install:	$(PROGRAM)
