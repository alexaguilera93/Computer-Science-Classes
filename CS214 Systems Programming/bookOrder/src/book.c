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
#include "queue.h"

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

/* functions for database hashtable */

struct database *entries = NULL; //hash table for database

/*add a database entrie */

void add_db(struct database *s){
	HASH_ADD_INT(entries,customer_id,s);
}

/* find database entrie from customer_id */

struct database *find_entry(int customer_id){
	struct database *find;
	HASH_FIND_INT(entries, &customer_id, find);
	return find;
}

/* free database of all memory */

void free_db(void){
	struct database *db;
	struct database *temp;
	HASH_ITER(hh, entries, db, temp){
		HASH_DEL(entries, db);
		destroy_queue(db->successful_orders);
		destroy_queue(db->rejected_orders);
		free(db->name);
		free(db->address);
		free(db->state);
		free(db->successful_orders);
		free(db->rejected_orders);
		free(db);
	}
}

/* functions for consumer queue hashtable */

struct consumer_queue *consumer_queue = NULL;//hash table

/* function that returns queue that maches category */
Queue *get_consumer_queue(char *category){
	struct consumer_queue *find;
	HASH_FIND_STR(consumer_queue, category, find);
	if(find == NULL){
		return NULL;
	}
	return find->queue;
}

/*Add a new queue to hashtable */

void add_consumer_queue(char *category){
	int len = strlen(category);
	char *temp = (char*)malloc(len + 1);
	strcpy(temp, category);
	struct consumer_queue *new_item =(struct consumer_queue*)malloc(sizeof(struct consumer_queue));
	new_item->category = temp;
	new_item->queue = create_queue();
	if(get_consumer_queue(category) == NULL){
		HASH_ADD_KEYPTR(hh, consumer_queue, new_item->category, strlen(new_item->category), new_item);
	}	
}

/* Consumer Function:
 * 
 * This function will take in a string that has an order and will process 
 * that order through the database and make the appropriate changes to 
 * the database, all while making sure that as it modifies the 
 * database item that no other thread can modify it while it is
 * doing its instructions
 *
 */

void *consumer_func(void *arg){

}


/* Producer Function:
 *
 * This function will read in the orders from the orders file and start
 * feeding them to the queues for the consumer threads to read and 
 * modify the database structure accodringly
 *
 */

void *producer_func(void *arg){
	char *orderFile = (char *)arg;
	FILE *fp = fopen(orderFile, "r");
	while(!feof(fp)){
		char order[LINE_MAX];
		if(fgets(order, LINE_MAX, fp) != NULL){
			//printf("%s\n", order);
		}
	}
	fclose(fp);
	pthread_exit("Thread exiting\n");

}
