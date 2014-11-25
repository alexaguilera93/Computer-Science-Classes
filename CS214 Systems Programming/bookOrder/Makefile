COMPILER = gcc
CCFLAGS = -g -Wall -pthread

debug:
	make DEBUG=TRUE

book: main.o tokenizer.o book.o
	@cd src && $(COMPILER) $(CCFLAGS) -o book main.c tokenizer.c book.c 

main.o: main.c
	@cd src && $(COMPILER) $(CCFLAGS) -c main.c book.c toknizer.c

book.o: book.c
	@cd src && $(COMPILER) $(CCFLAGS) -c book.c tokenizer.c

tokenizer.o: tokenizer.c
	@cd src && $(COMPILER) $(CCFLAGS) -c tokenizer.c

ifeq ($(DEBUG), TRUE)
CCFLAGS += -g
endif

clean:
	rm -f book
	rm -f *.o
