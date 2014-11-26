#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>

typedef struct QueueItem* QueueItemPtr;

typedef struct QueueItem{
	void *data;
	QueueItemPtr next;
} QueueItem;

typedef struct Queue{
	QueueItem *head;
	QueueItem *tail;
	int isopen;
	int length;
	pthread_mutex_t mutex;
	pthread_cond_t dataAvailable;
	pthread_cond_t spaceAvailable;
}Queue;

Queue *create_queue();
int queue_length(Queue *queue);
void enqueue(Queue *queue, void *data);
void *dequeue(Queue *queue);
void destroy_queue(Queue *queue);

#endif
