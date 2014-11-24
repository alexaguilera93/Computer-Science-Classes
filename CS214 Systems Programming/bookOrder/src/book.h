#ifndef BOOK_H
#define BOOK_H

struct SharedData {
	int	isopen;
	unsigned int	front;
	unsigned int	count;
	unsigned int	bufsize;
	char		buff[SBUFSIZE];
	char *		catFileName;
	char *		dbFile;
	char *		ordersFile;
	sem_t		empty_count;
	sem_t		full_count;
	sem_t		use_queue;

};

//char** process_catagories(char *catagoriesName);

void initialize(struct SharedData *sptr, int refcount, char **catStrings);

void consumer_func(void *arg);

void producer_func(void *arg);

#endif
