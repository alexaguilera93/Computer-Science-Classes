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
