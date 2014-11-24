#ifndef BOOK_H
#define BOOK_H

struct database_ {
	char *name;
	int customerId;
	float balance;
	char *address;
	char *state;
	char *zipcode;
	UT_hash_handle hh;
};
typedef struct  database_ database;

//char** process_catagories(char *catagoriesName);

void initialize(struct SharedData *sptr, int refcount, char **catStrings);

void consumer_func(void *arg);

void producer_func(void *arg);

#endif
