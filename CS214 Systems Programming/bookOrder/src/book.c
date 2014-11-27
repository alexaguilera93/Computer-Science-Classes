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
		free(fileString);
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
/*free all consumer queues*/
void free_consumer_queues(void){
	struct consumer_queue *a, *b;
	HASH_ITER(hh, consumer_queue, a, b){
		HASH_DEL(consumer_queue, a);
		destroy_queue(a->queue);
		free(a->category);
		free(a->queue);
		free(a);
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
		//printf("loop 2\n");
		if(p->length == 0){
			continue;
		}
		pthread_mutex_lock(&lock_database);
		struct order *process = (struct order*)dequeue(p);
		struct database *customer = find_entry(process->customer_id);
		float check = customer->balance - process->price;
		pthread_mutex_lock(&lock_file);
		if(check < 0){

			struct rejected_orders *rej = (struct rejected_orders*)malloc(sizeof(struct rejected_orders));
			memset(rej, 0, sizeof(struct rejected_orders));
			int len3 = strlen(process->title);
			rej->title = (char*)malloc(len3 + 1);
			strcpy(rej->title, process->title);
			rej->title[len3] = '\0';
			rej->price = process->price;
			enqueue(customer->rejected_orders, rej);
		}
		else{
			customer->balance = customer->balance - process->price;
			struct successful_order *suc = (struct successful_order*)malloc(sizeof(struct successful_order));
			memset(suc, 0, sizeof(struct successful_order));
			int l4 = strlen(process->title);
			suc->title = (char*)malloc(l4 + 1);
			memset(suc->title, 0, l4 + 1);
			strcpy(suc->title, process->title);
			suc->title[l4] = '\0';
			suc->price = process->price;
			suc->remaining_balance = customer->balance;
			enqueue(customer->successful_orders, suc);
		}
		free(process->title);
		free(process->category);
		free(process);
		pthread_mutex_unlock(&lock_file);
		pthread_mutex_unlock(&lock_database);
	}
	pthread_exit("Thread Finished\n");
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
		break;
		}	
		//printf("stuck in loop \n");
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
		//printf("getting here\n");
		f->queue->isopen = FALSE;
	}
	fclose(fp);
	pthread_exit("Thread exiting\n");

}

int main(int argc, char **argv){
	if(argc != 4){
		printf("Invalid number of arguments\n");
	}
	char *databaseFile = argv[1];
	char *orderFile = argv[2];
	char *categorieFile = argv[3];
	char **categories = process_categories(categorieFile);
	if(categories == NULL){
		exit(EXIT_FAILURE);
	}
	int a = process_database(databaseFile);
	if(a == 1){
		exit(EXIT_FAILURE);
	}
	struct category_thread *all_cat_threads = NULL;
	int m = 0;
	while(categories[m] != NULL){
		add_consumer_queue(categories[m]);

		char *category = (char*)malloc(strlen(categories[m] + 1));
		strcpy(category, categories[m]);
		category[strlen(categories[m])] = '\0';

		pthread_t cat_thread;
		pthread_create(&cat_thread, NULL, consumer_func, category);		   struct category_thread *c_thread = (struct category_thread*)malloc(sizeof(struct category_thread));
		c_thread->thread = cat_thread;
		if(all_cat_threads == NULL){
			all_cat_threads = c_thread;
			c_thread->next = NULL;
		}
		else{
			c_thread->next = all_cat_threads;
			all_cat_threads = c_thread;
		}
		m++;

	}
	/*producer thread*/
	pthread_t a_thread;
	pthread_create(&a_thread, NULL, producer_func, orderFile);
	pthread_join(a_thread, NULL);

	struct category_thread *tmp = NULL;

	for(tmp = all_cat_threads; tmp != NULL; tmp=tmp->next){
		pthread_join(tmp->thread, NULL);
	}

	struct database *trace, *temp1;
	FILE *rt;
	rt = fopen("finalresults.txt", "w");
	HASH_ITER(hh, entries, trace, temp1){
		fprintf(rt, "=== BEGIN CUSTOMER INFO ===\n###BALANCE###\nCustomer name:%s\nCustomer ID number: %d\nRemaining credit balance after all purchases (a dollar amount) %.2f\n###SUCCESSFUL ORDERS###\n", trace->name, trace->customer_id, trace->balance);
		struct successful_order *tr1;
		while((tr1 = dequeue(trace->successful_orders))){
			fprintf(rt, "%s|%.2f|%.2f\n", tr1->title, tr1->price, tr1->remaining_balance);
			free(tr1->title);
			free(tr1);
		}
		fprintf(rt, "### REJECTED ORDERS ###\n");
		struct rejected_orders *tr2;
		while((tr2 = dequeue(trace->rejected_orders))){
			fprintf(rt, "%s|%.2f\n", tr2->title, tr2->price);
			free(tr2->title);
			free(tr2);
		}
		fprintf(rt, "=== END CUSTOMER INFO ===\n\n");
	}
	struct category_thread *g = all_cat_threads;
	while(g != NULL){
		struct category_thread *u = g;
		g = g->next;
		free(u);
	}
	free_db();
	free_consumer_queues();
	pthread_mutex_destroy(&lock_file);
	pthread_mutex_destroy(&lock_database);
	return 0;
}
