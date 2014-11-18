COMPILER = gcc
CCFLAGS = -g -Wall -pthread

debug:
	make DEBUG=TRUE

book: book.o
	$(COMPILER) $(CCFLAGS) -o book src/main.c src/tokenizer.c src/book.c 

book.o: main.c
	$(COMPILER) $(CCFLAGS) -c src/main.c src/book.c src/tokenizer.c

ifeq ($(DEBUG), TRUE)
CCFLAGS += -g
endif

clean:
	rm -f book
	rm -f *.o
