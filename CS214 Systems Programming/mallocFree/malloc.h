#ifndef MALLOC_H
#define MALLOC_H

struct MemEnt_{
	unsigned int size;
	unsigned int free;
	struct MemEnt_ *prev;
	struct MemEnt_ *succ;


} typedef struct MemEnt_ MemEnt;

#endif
