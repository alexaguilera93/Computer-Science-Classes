COMPILER = gcc
CCFLAGS = -g -Wall
all: index search

debug:
	make DEBUG=TRUE

search: search.o
	$(COMPILER) $(CCFLAGS) -o search search.c stokenizer.c

search.o:
	$(COMPILER) $(CCFLAGS) -c search.c stokenizer.c

index: index.o
	$(COMPILER) $(CCFLAGS) -o index main.c list.c tokenizer.c mergesort.c

index.o: main.c
	$(COMPILER) $(CCFLAGS) -c main.c list.c tokenizer.c mergesort.c

ifeq ($(DEBUG), TRUE)
CCFLAGS += -g
endif

clean:
	rm -f search
	rm -f index
	rm -f *.o



