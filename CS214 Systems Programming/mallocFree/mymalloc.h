#ifndef MALLOC_H
#define MALLOC_H

struct MemEnt_{
	unsigned int size;
	unsigned int free;
	struct MemEnt_ *prev;
	struct MemEnt_ *succ;
};
typedef struct MemEnt_ MemEnt;

void *my_malloc(unsigned int size, char *file, int line);

void my_free(void *pointer, char *file, int line);
#endif
