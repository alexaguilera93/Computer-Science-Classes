#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <malloc.h>
#include <semaphore.h>
#include "book.h"
#include "tokenizer.h"
/* Usage: ./bookOrder <databaseinputfile> <bookOrderinputfile> <catagorieFile>*/




int main(int argc, char **argv){
	/*if invalid # of arguments */
	if(argc != 4){
		printf("Invalid # of arguments\n");
		return 1;
	}
	char *databaseFile = argv[1];
	char *orderFile = argv[2];
	char *categorieFile = argv[3];
	//read database into memory
	int a =	process_database(databaseFile);
	if(a == 1){
		exit(EXIT_FAILURE);
	}
	char **b = process_categories(categorieFile);
	if(b == NULL){
		exit(EXIT_FAILURE);
	}
	/* start making consumer threads and storing them in memory */
	struct category_thread *consumer_threads = NULL; 
	int m = 0;
	while(b[m] != NULL){
		int len1 = strlen(b[m]);
		char *cat = (char*)malloc(len1 + 1);
		memset(cat, 0, len1 + 1);
		strcpy(cat, b[m]);
		cat[len1] = '\0';
		add_consumer_queue(b[m]);
		pthread_t cat_thread;
		pthread_create(&cat_thread, NULL, consumer_func, cat);
		struct category_thread *c_thread = (struct category_thread*)malloc(sizeof(struct category_thread));
		c_thread->thread = cat_thread;
		if(consumer_threads == NULL){
			c_thread->next = NULL;
			consumer_threads = c_thread;
		}
		else{
			c_thread->next = consumer_threads;
			consumer_threads = c_thread;
		}
		m++;
	}

	/* producer thread  */
	int res;
	pthread_t a_thread;
	void *thread_result;
	res = pthread_create(&a_thread, NULL, producer_func, (void *)orderFile);
	if(res != 0){
		perror("Producer Thread creation failed\n");
		exit(EXIT_FAILURE);
	}
	printf("Producer Thread started, waiting to finish\n");
	res = pthread_join(a_thread, &thread_result);
	if(res != 0){
		printf("producer thread failed to join\n");
		exit(EXIT_FAILURE);
	}
	else{
		printf("thread joined!\n");
	}
	int i = 0;
	while(b[i] != NULL){
		printf("%s freed\n", b[i]);
		free(b[i]);
		i++;
	}
	free(b);
	return 0;
}
