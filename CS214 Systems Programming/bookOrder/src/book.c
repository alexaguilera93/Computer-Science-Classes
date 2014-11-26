#include "book.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>
#include "tokenizer.h"
#include <pthread.h>
#include <semaphore.h>
#include "uthash.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>


#define BUFFER_SIZE BUFSIZ

#define FALSE 0
#define TRUE 1





//get categrories to start threads
char **process_categories(char *fileName){
	FILE *fp;
	fp = fopen(fileName, "r");
	char ch;
	struct stat st;
	if(stat(fileName, &st) != 0){
		fclose(fp);
		printf("Invalid categorie file\n");
		return NULL;
	}
	int sizeCheck = st.st_size;
	if(sizeCheck == 0){
		fclose(fp);
		printf("Empty Category file\n");
		return NULL;
	}
	else{
		char *fileString;
		fileString = (char*)malloc(sizeof(char) * sizeCheck + 1);
		memset(fileString, 0, sizeCheck + 1);
		int cat_count = 0;
		int count = 0;
		while((ch=fgetc(fp)) != EOF){
			if(ch == '\n'){
				cat_count++;
			}
			fileString[count] = ch;
			count++;
		}
		//int consumer_buff_size = BUFFER_SIZE / cat_count;
		TokenizerT *tokenizer = TKCreate(" \n", fileString);
		char *token;
		char **return_string = (char**)malloc((sizeof(char*) * cat_count) + sizeof(char*));
		int j = 0;
		int a;
		while((token = TKGetNextToken(tokenizer)) != NULL){
			char *new_string;
			a = strlen(token);
			new_string = (char*)malloc(a + 1);
			strcpy(new_string, token);
			new_string[a] = '\0';
			return_string[j] = new_string;
			j++;
		}
		return_string[j] = NULL;
		return return_string;
	}
}
//put database into memory
int process_database(char *fileName){
	FILE *fp;
	fp = fopen(fileName, "r");
	char ch;
	struct stat st;
	if(stat(fileName, &st) != 0){
		fclose(fp);
		printf("Invalid database file\n");
		return 1;
	}
	int sizeCheck = st.st_size;
	if(sizeCheck == 0){
		fclose(fp);
		printf("Empty Database file\n");
		return 1;
	}
	else{
		char *fileString;
		fileString = (char*)malloc(sizeof(char) * sizeCheck + 1);
		memset(fileString, 0, sizeCheck + 1);
		int count = 0;
		int linecount = 0;
		while((ch=fgetc(fp)) != EOF){
			if(ch == '\n'){
				linecount++;
			}
			fileString[count] = ch;
			count++;
		}
		fileString[sizeCheck] = '\0';
		TokenizerT *tokenizer = TKCreate("|\"", fileString);
		char *token;
		while((token = TKGetNextToken(tokenizer)) != NULL){
			struct database *newdb = (struct database*)malloc(sizeof(struct database));
			int len1 = strlen(token);
			char *custname = (char*)malloc(len1 + 1);
			strcpy(custname, token);
			custname[len1] = '\0';
			newdb->name = custname;
			free(token);
			token = TKGetNextToken(tokenizer);
			int id = atoi(token);
			free(token);
			newdb->customer_id = id;
			token = TKGetNextToken(tokenizer);
			float balance = atof(token);
			newdb->balance = balance;
			free(token);
			token = TKGetNextToken(tokenizer);
			len1 = strlen(token);
			char *address = (char*)malloc(len1 + 1);
			strcpy(address, token);
			free(token);
			address[len1] = '\0';
			newdb->address = address;
			token = TKGetNextToken(tokenizer);
			len1 = strlen(token);
			char *state = (char*)malloc(len1 + 1);
			strcpy(state, token);
			free(token);
			state[len1] = '\0';
			newdb->state = state;
			token = TKGetNextToken(tokenizer);
			int zipcode;
			zipcode = atoi(token);
			newdb->zipcode = zipcode;
			free(token);
			add_db(newdb);
		}
		TKDestroy(tokenizer);
	}
	return 0;
}
struct database *entries = NULL; //hash table for database
//add a database entrie;
void add_db(struct database *s){
	HASH_ADD_INT(entries,customer_id,s);
}

void *consumer_func(void *arg){

}

void *producer_func(void *arg){
	char *orderFile = (char *)arg;
	FILE *fp = fopen(orderFile, "r");
	while(!feof(fp)){
		char order[200];
		if(fgets(order, 200, fp) != NULL){
			printf("%s\n", order);
		}
	}
	pthread_exit("Thread exiting\n");

}
