COMPILER = gcc
CCFLAGS = -g -Wall
all: index

debug:
	make DEBUG=TRUE


index: index.o
	$(COMPILER) $(CCFLAGS) -o index main.c list.c tokenizer.c

index.o: main.c
	$(COMPILER) $(CCFLAGS) -c main.c list.c tokenizer.c

ifeq ($(DEBUG), TRUE)
CCFLAGS += -g
endif

clean:
	rm -f index
	rm -f *.o



