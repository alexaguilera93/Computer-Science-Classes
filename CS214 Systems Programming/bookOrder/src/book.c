#include "book.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>
#include "tokenizer.h"
#include <pthread.h>
#include <semaphore.h>

#define FALSE 0
#define TRUE 1
#define SBUFSIZE 16 

/*put catagories file into shared memory*/

char** process_catagories(char *catagoriesName){
	FILE *fp;
	
	return 0;
}
//initialize SharedData
void initialize(struct SharedData *sptr, int refcount, char **catStrings){
	sptr->isopen = TRUE;
	stpr->refcount = refcount;
	sptr->front = 0;
	sptr->count = 0;
	sptr->bufsize = SBUFSIZE;
	sptr->catagories = catStrings;
	sem_init(&sptr->empty_count, 0, SBUFSIZE);
	sem_init(&sptr->full_count, 0, 0);
	sem_init(&sptr->use_queue,0,1);
}

void consumer_func(void *arg){



}

void producer_func(void *arg){


}
