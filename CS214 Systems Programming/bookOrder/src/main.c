#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <malloc.h>
#include <semaphore.h>
#include "book.h"
#include "tokenizer.h"
/* Usage: ./bookOrder <databaseinputfile> <bookOrderinputfile> <catagorieFile>*/

void process_database(char *fileName);


int main(int argc, char **argv){
	/*if invalid # of arguments */
	if(argc != 4){
		printf("Invalid # of arguments\n");
		return 1;
	}
	char *databaseFile = argv[1];
	//char *orderFile = argv[2];
	//char *catagorieFile = argv[3];
	process_database(databaseFile);
	
	return 0;
}
