#ifndef MULTI_QUEUE_H
#define MULTI_QUEUE_H

typedef struct _queue_item{
  int thread_write_num;
  int thread_id;
  int cum_sum;
  struct _queue_item * next;
}queue_item;


typedef struct _queue{
  queue_item * head;
  queue_item * tail;
  int item_num;
  int queue_id;
}queue;

typedef void * (* thread_func)(void *);

typedef struct _thread_param{
  queue * queues;
  int thread_id;
  int thread_num;
  int write_times;
  int queue_num;
}thread_param;

void create_threads(pthread_t **ppThreads, int thread_num, thread_func func, 
    thread_param ** pparam, thread_param common_param);

void join_threads(pthread_t * threads, int thread_num, thread_param * param);

void * queue_thread(void * p);

// initialize the queue
void queue_init(queue * q, int id);

// push an item to the end of the queue
void queue_push(queue * q, int thread_id, int thread_written_num);

// output the queue
void queue_output(queue * q, thread_param param);

// free the queue
void queue_free(queue *q);

double GetTime( void );

#endif
