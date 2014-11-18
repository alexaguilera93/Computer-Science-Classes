#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>
#include "tokenizer.h"

#define FALSE	0;
#define	TRUE	1;
#define	SBUFSIZE 16;

struct SharedData {
	int	isopen;
	unsigned int	front;
	unsigned int	count;
	unsigned int	bufsize;
	char		buff[SBUFSIZE];
	pthread_mutex_t	mutex;
	pthread_cond_t	dataAvailable;
	pthread_cond_t	spaceAvailable;

};
