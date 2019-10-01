CC = gcc
CXX = g++
ECHO = echo
RM = rm
TAR = tar cvfz

CFLAGS = -Wall -ggdb3

BIN = bushel
OBJS = environment.o

all: $(BIN)

# dijkstrasCurse: dijkstrasCurse.c mapElements dij display heap
# 	gcc -Wall -ggdb3 dijkstrasCurse.c mapElements.o display.o pathFinder.o heap.o -o dijkstrasCurse -lm
$(BIN): $(OBJS) bushel.o
	@$(ECHO) Linking $@
	@$(CXX) $^ -o $@ #-lncurses

-include $(OBJS:.o=.d)

%.o: %.cpp %.h
	@$(ECHO) Compiling $<
	@$(CXX) $(CFLAGS) -MMD -MF $*.d -c $<

#Clean up!
clean:
	@$(ECHO) Deleting the junk
	@$(RM) *.o $(BIN) *.d