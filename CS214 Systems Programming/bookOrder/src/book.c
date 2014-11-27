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
#include <sys/queue.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include "queue.h"

#define BUFFER_SIZE BUFSIZ


#define FALSE 0
#define TRUE 1

pthread_mutex_t lock_file = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_database = PTHREAD_MUTEX_INITIALIZER;



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
			newdb->successful_orders = create_queue();
			newdb->rejected_orders = create_queue();
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
	printf("got category queue %s\n", category);
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
	char *category = (char*)arg;
	Queue *p = get_consumer_queue(category);
	free(category);
	if(!p){
		return NULL;
	}
	while(p->isopen || p->length > 0){
		if(p->length == 0){
			continue;
		}
		pthread_mutex_lock(&lock_database);
		struct order *process = (struct order*)dequeue(p);
		struct database *customer = find_entry(process->customer_id);
		customer->balance = customer->balance - process->price;
		pthread_mutex_lock(&lock_file);
		if(customer->balance < 0){
			struct rejected_orders *reject = (struct rejected_orders*)malloc(sizeof(struct rejected_orders));
			memset(reject, 0, sizeof(struct rejected_orders));
			reject->title = (char*)malloc(strlen(process->title) + 1);
			memset(reject->title, 0, strlen(process->title) + 1);
			strcpy(reject->title, process->title);
			reject->title[strlen(process->title)] = '\0';
			reject->price = process->price;
			enqueue(customer->rejected_orders, reject);
		}
		else{
			struct successful_order *success = (struct successful_order*)malloc(sizeof(struct successful_order));
			success->title = (char*)malloc(strlen(process->title) + 1);
			memset(success->title, 0, strlen(process->title + 1));
			strcpy(success->title, process->title);
			success->title[strlen(process->title)] = '\0';
			success->price = process->price;
			success->remaining_balance = customer->balance;
			//printf("enqueue failed for %s\n");
			enqueue(customer->successful_orders, success);
		}
		free(process->title);
		free(process->category);
		free(process);
		pthread_mutex_unlock(&lock_database);
		pthread_mutex_unlock(&lock_file);
	}
	return NULL;
	//pthread_exit("Thread Finished\n");
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
	char order[LINE_MAX];
	while(fgets(order, LINE_MAX, fp) != NULL){
		TokenizerT *tokenizer = TKCreate("|\n", order);
		char *token;
		if((token = TKGetNextToken(tokenizer)) == NULL){
		TKDestroy(tokenizer);
		break;
		}	
		struct order *send_order = (struct order*)malloc(sizeof(struct order));
		memset(send_order, 0, sizeof(struct order));
		send_order->title = token;
		char *price = TKGetNextToken(tokenizer);
		send_order->price = atof(price);
		free(price);
		char *cust_id = TKGetNextToken(tokenizer);
		send_order->customer_id = atoi(cust_id);
		free(cust_id);
		char *category1 = TKGetNextToken(tokenizer);
		send_order->category = category1;
		Queue *p = get_consumer_queue(send_order->category);
		
		if(!p){
			free(send_order->title);
			free(send_order->category);
			free(send_order);
			TKDestroy(tokenizer);
			continue;
		}
		else{
			enqueue(p, (void *)send_order);
			TKDestroy(tokenizer);
		}

	}
	struct consumer_queue *f;
	for(f = consumer_queue; f != NULL; f = f->hh.next){
		f->queue->isopen = FALSE;
	}
	fclose(fp);
	pthread_exit("Thread exiting\n");

}

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
	int a = process_database(databaseFile);
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
	/* producer thread */
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
	struct category_thread *cleanup = NULL;
	for(cleanup = consumer_threads; cleanup != NULL; cleanup = cleanup->next){
		pthread_join(cleanup->thread,NULL);
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

