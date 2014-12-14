#include <stdio.h>
#include "mymalloc.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#define BSIZE 5000
#define BIGGESTCHUNK 200

static char large_block[BSIZE];
static char small_block[BSIZE];
//return pointer to memory
void *my_malloc(unsigned int size){
	static int init = 0;
	static MemEnt *root1 = 0;
	static MemEnt *root2 = 0;
	MemEnt *point;
	MemEnt *succ;
	if(!init){
		root1 = (MemEnt*)large_block;
		root1->prev = 0;
		root1->succ = 0;
		root1->size = BSIZE -sizeof(MemEnt);
		root1->free = 1;
		root2 = (MemEnt*)small_block;
		root2->prev = 0;
		root2->succ = 0;
		root2->size = BSIZE - sizeof(MemEnt);
		root2->free = 1;
		init = 1;
	}
	if(size < BIGGESTCHUNK){
		point = root2;
	}
	else{
		point = root1;
	}
	while(point != 0){
		if(point->size < size){
			point = point->succ;
		
			if(point == 0){
				fprintf(stderr, "Error, not enough memory left for your data to be alloced in file %s at line %d.\n",__FILE__,__LINE__);
				return NULL;
			}
		}
			else if(!point->free){
				point = point->succ;
			}	
			else if(point->size < (size + sizeof(MemEnt))){
				point->free = 0;
				return (char*)point + sizeof(MemEnt);
			}
			else{
				succ = (MemEnt*)((char*)point + sizeof(MemEnt) + size);
				succ->prev = point;
				succ->succ = point->succ;
					if(point->succ != 0){
						point->succ->prev = succ;
					}
				point->succ = succ;
				succ->free = 1;
				succ->size = point->size - sizeof(MemEnt) - size;
				point->size = size;
				point->free = 0;
				return (char*)point + sizeof(MemEnt);
			}
		
	}
	return 0;
}


void my_free(void *pointer){
	MemEnt *ptr;
	MemEnt *succ;
	MemEnt *prev;

	if((pointer < (void*)large_block || pointer > ((void*)large_block + BSIZE)) && (pointer < (void*)small_block || pointer > ((void*)small_block + BSIZE)))
	{
		fprintf(stderr, "Error trying to free pointer that was never allocated in file %s at line %d.\n",__FILE__,__LINE__);
		return;
	} 


	int validMem = 0;
	ptr = (MemEnt*)large_block;
	while(ptr != 0){
		if(pointer == (void*)ptr + sizeof(MemEnt)){
			validMem = 1;
			break;
		}
		ptr = ptr->succ;
	}
	ptr = (MemEnt*)small_block;
	while(ptr != 0 && validMem == 0){
		if(pointer == (void*)ptr + sizeof(MemEnt)){
			 validMem = 1;
			break;
		}
 	 ptr = ptr->succ;
	}

	if(validMem == 0){
		fprintf(stderr, "Error: pointer was never alloced in file %s at line %d.\n",__FILE__,__LINE__);
		return;
	}
	ptr = (MemEnt*)((char*)pointer - sizeof(MemEnt));
	if(ptr->free){
		fprintf(stderr, "Error:Multiple frees of same pointer without malloc in file %s at line %d.\n",__FILE__,__LINE__);
		return;
	}
	if(prev->free && (prev = ptr->prev) != 0){
		prev->size = prev->size + sizeof(MemEnt) + ptr->size;
		ptr->free = 1;
		prev->succ = ptr->succ;
		if(ptr->succ != 0){
				ptr->succ->prev = prev;
	
			}
	}
	else{
		ptr->free = 1;
		prev = ptr;
	}
	if((succ = ptr->succ) != 0 && succ->free){
		prev->size = prev->size + sizeof(MemEnt*) + succ->size;
		prev->free = 1;
		prev->succ = succ->succ;
		if(succ->succ != 0){
			succ->succ->prev = prev;
		}
	}

}
