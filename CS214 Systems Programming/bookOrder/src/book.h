#ifndef BOOK_H
#define BOOK_H

#define BUFSIZE BUFSIZ
#include "uthash.h"
struct database {
	char *name;
	int customer_id;
	float balance;
	char *address;
	char *state;
	int zipcode;/*
	sem_t empty_count;
	sem_t full_count;
	sem_t use_queue;
	struct successful_order s_orders;
	struct rejected_orders r_orders;
	*/
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
	char **titles;
	float *prices;
	float remaining_balance;

};

struct rejected_orders{
	int num;
	char **titles;
	float *price;
	float remaining_balance;
};

struct consumer_queue{
	char *category;
	int isopen;
};

char** process_categories(char *fileName);

int process_database(char *fileName);

void *consumer_func(void *arg);

void *producer_func(void *arg);

void add_db(struct database *s);

#endif
