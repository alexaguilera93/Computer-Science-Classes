#ifndef BOOK_H
#define BOOK_H

#include "queue.h"
#define BUFSIZE BUFSIZ
#include "uthash.h"

struct database {
	char *name;
	int customer_id;
	float balance;
	char *address;
	char *state;
	int zipcode;
	struct Queue *successful_orders;
	struct Queue *rejected_orders;
	UT_hash_handle hh;
};

struct order{
	char *title;
	int customer_id;
	char *category;
	float price;
};

struct successful_order{
	int num;
	char *title;
	float price;
	float remaining_balance;

};

struct rejected_orders{
	int num;
	char *title;
	float price;
	float remaining_balance;
};

struct consumer_queue{
	char *category;
	Queue *queue;
	UT_hash_handle hh;
};

struct category_thread{
	pthread_t thread;
	struct category_thread *next;
};

struct database *find_customer(int customer_id);

Queue *get_consumer_queue(char *category);

void add_consumer_queue(char *category);

char** process_categories(char *fileName);

int process_database(char *fileName);

void *consumer_func(void *arg);

void *producer_func(void *arg);

void add_db(struct database *s);

#endif
