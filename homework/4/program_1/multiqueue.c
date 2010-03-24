#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

#include "multiqueue.h"

char * usage = "Usage:\nexecutable <thread_num> <quere_num> <write_times>\n";
char * malloc_error = "malloc error!\n";

int main( int argc, char *argv[] )
{
  int thread_num = 0;
  int queue_num = 0;
  int write_times = 0;

  // Check for correct number of arguments
  if (argc < 4)
  {
    puts(usage);
    return 0;
  }

  // Get parameters
  thread_num = atoi(argv[1]);
  queue_num = atoi(argv[2]);
  write_times = atoi(argv[3]);

  // Set the minimum to 1
  thread_num = (thread_num < 1) ? 1 : thread_num;
  queue_num = (queue_num < 1) ? 1 : queue_num;
  write_times = (write_times < 1) ? 1 : write_times;

  // Set a random seed
  srand((int)GetTime());

  // Print the program parameters.
  printf("Running with P=%d M=%d N=%d\n", thread_num, queue_num, write_times);

  return 0;
}

double GetTime( void )
{
  // ------------------- Local Variables ------------------
  struct timeval tp;
  double localtime;

  // ------------------- Get Time -------------------------
  gettimeofday( &tp, NULL );
  localtime = ( double ) tp.tv_usec;
  localtime /= 1e6;
  localtime += ( double ) tp.tv_sec;

  // ------------------- Return Time ----------------------
  return( localtime );
}

// Initialize the queue
void queue_init(queue * q){
  q->head = NULL;
  q->tail = NULL;
  q->item_num = 0;
}

// push an item to the end of the queue
void queue_push(queue * q, int thread_num){
  queue_item * item = NULL;
  if(NULL == (item = (queue_item *)malloc(sizeof(queue_item)))){
    puts(malloc_error);
    return;
  }

  // initialize the item
  item->thread_num = thread_num;
  item->next = NULL;

  // push the item to the end of the queue
  if(NULL == q->head){
    q->head = item;
    q->tail = item;
    q->item_num = 1;
  }else if(NULL != q->tail){
    q->tail->next = item;
    q->tail = item;
    ++(q->item_num);
  }else{
    q->tail = item;
    ++(q->item_num);
  }
}

// free the queue
void queue_free(queue *q){
  if(NULL == q->head){
    return;
  }
  queue_item * item = q->head;
  queue_item * next = item;
  while(item){
    next = item->next;
    free(item);
    item = next;
  }
  q->head = NULL;
  q->tail = NULL;
  q->item_num = 0;
}

