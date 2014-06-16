#comments
COMPILER = gcc
CCFLAGS = -ansi -pedantic -Wall -std=c99
all: wordstat

debug:
	make DEBUG=TRUE


wordstat: wordstat.o
	$(COMPILER) $(CCFLAGS) -o wordstat wordstat.c

wordstat.o: wordstat.c
	$(COMPILER) $(CCFLAGS) -c wordstat.c

ifeq ($(DEBUG), TRUE)
CCFLAGS += -g
endif

clean:
	rm -f wordstat
	rm -f *.o
