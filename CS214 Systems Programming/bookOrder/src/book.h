#ifndef BOOK_H
#define BOOK_H

struct SharedData {
	int	isopen;
	unsigned int	front;
	unsigned int	count;
	unsigned int	bufsize;
	char		buff[SBUFSIZE];
	pthread_mutex_t	mutex;
	pthread_cond_t	dataAvailable;
	pthread_cond_t	spaceAvailable;
	char **		catagories;

};

char** process_catagories(char *catagoriesName);

void initialize(struct SharedData *sptr, int refcount, char **catStrings);

void consumer_func(void *arg);

void producer_func(void *arg);

#endif
