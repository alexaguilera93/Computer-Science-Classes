COMPILER = gcc
CCFLAGS  = -m32 -Wall  -ansi -pedantic
all: prime

prime: prime.s
	$(COMPILER) $(CCFLAGS) -o prime prime.s

clean:
	rm -f prime
