#ifndef MULTI_QUEUE_H
#define MULTI_QUEUE_H

typedef struct _queue_item{
  int thread_write_num;
  int thread_num;
  int cum_sum;
  struct _queue_item * next;
}queue_item;


typedef struct _queue{
  queue_item * head;
  queue_item * tail;
  int item_num;
}queue;

typedef void * (* thread_func)(void *);

typedef struct _thread_param{
  queue * queues;
  int thread_id;
}thread_param;


// Initialize the queue
void queue_init(queue * q);

// push an item to the end of the queue
void queue_push(queue * q, int thread_num);

// free the queue
void queue_free(queue *q);

double GetTime( void );

#endif
