#ifndef BOOK_H
#define BOOK_H
#include "uthash.h"
struct database {
	char *name;
	int customer_id;
	float balance;
	char *address;
	char *state;
	int zipcode;
	UT_hash_handle hh;
};

struct BookOrder{
	char *title;
	int customer_id
	char *category;
	float price;
};

char** process_catagories(char *fileName);

int process_databae(char *fileName);

void consumer_func(void *arg);

void producer_func(void *arg);

void add_db(struct database *s);

#endif
