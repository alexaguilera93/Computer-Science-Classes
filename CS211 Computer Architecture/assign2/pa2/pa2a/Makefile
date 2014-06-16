COMPILER = gcc
CCFLAGS = -Wall -g -ansi -pedantic -m32
all: game

game: game.c
	$(COMPILER) $(CCFLAGS) -o game game.c conway.s

clean:
	rm -f game
	rm -f *.o
	
