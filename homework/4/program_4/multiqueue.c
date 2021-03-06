#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

#include "my_sync.h"
#include "multiqueue.h"

char * usage = "Usage:\nexecutable <thread_num> <quere_num> <write_times>\n";
char * malloc_error = "malloc error!\n";

long g_validate_cum_sum_first;
long g_validate_cum_sum_second;
double gElapsedTime;

int main(int argc, char *argv[]){
  int thread_num = 0;
  int queue_num = 0;
  int write_times = 0;

  int cycleI = 0;

  g_validate_cum_sum_first = 0;
  g_validate_cum_sum_second = 0;

  queue * queues = NULL;

  // Check for correct number of arguments
  if(argc < 4)
  {
    puts(usage);
    return 0;
  }

  // Get parameters
  thread_num = atoi(argv[1]);
  queue_num = atoi(argv[2]);
  write_times = atoi(argv[3]);

  // Make sure the parameters is not less than 1
  thread_num = (thread_num < 1) ? 1 : thread_num;
  queue_num = (queue_num < 1) ? 1 : queue_num;
  write_times = (write_times < 1) ? 1 : write_times;

  // Set a random seed
  srand((int)GetTime());

  // Print the program parameters.
  printf("Running with P=%d M=%d N=%d\n", thread_num, queue_num, write_times);

  if(NULL == (queues = (queue *)malloc(queue_num * sizeof(queue)))){
    puts(malloc_error);
    exit(-1);
  }

  // initialize the queues
  for(cycleI = 0; cycleI < queue_num; ++ cycleI){
    queue_init(&queues[cycleI], cycleI);
  }

  pthread_t * threads = NULL;
  thread_param * param = NULL;
  
  thread_param common_param;
  common_param.queues = queues;
  common_param.thread_id = 0;
  common_param.thread_num = thread_num;
  common_param.write_times = write_times;
  common_param.queue_num = queue_num;

  create_threads(&threads, thread_num, queue_thread, &param, common_param);
  join_threads(threads, thread_num, param);
  threads = NULL;
  param = NULL;
  gElapsedTime = GetTime() - gElapsedTime;
 
  // output the result
  for(cycleI = 0; cycleI < queue_num; ++ cycleI){
    printf("\n");
    queue_output(&queues[cycleI], common_param);
  }

  queue_free(queues);

  long expect_cum_sum_first = thread_num * write_times * (write_times - 1) / 2;
  long expect_cum_sum_second = write_times * thread_num * (thread_num - 1) / 2;
  printf("\nCumulative Sum: %ld,%ld\n", g_validate_cum_sum_first, g_validate_cum_sum_second);
  printf("Expected Sum: %ld,%ld\n", expect_cum_sum_first, expect_cum_sum_second);
  printf("Time Elapsed: %fs\n", gElapsedTime);

  return 0;
}

void create_threads(pthread_t **ppThreads, int thread_num, thread_func func, 
    thread_param ** pparam, thread_param common_param){
  
  pthread_t *threads;
  threads = *ppThreads = (pthread_t *)malloc(sizeof( pthread_t ) * thread_num);
  if(NULL == threads){
    puts(malloc_error);
    exit(-1);
  }

  thread_param * parameters;
  parameters = *pparam = (thread_param *)malloc(sizeof(thread_param) * thread_num);
  if(NULL == pparam){
    puts(malloc_error);
    exit(-1);
  }

  // Initialize the parameters
  int cycleI = 0;
  for (cycleI = 0; cycleI < thread_num; ++ cycleI){
    parameters[cycleI] = common_param;
    parameters[cycleI].thread_id = cycleI;
  }

  // ------------------- Create Threads ---------------------
  // Create threads
  for (cycleI = 0; cycleI < thread_num - 1; ++ cycleI){
    pthread_create(&threads[cycleI],
    NULL,	// Default attibutes
    func,	///void *(*start_routine)(void *),
    &parameters[cycleI]);
  }

  // Timing: To help other threads complete creation, the
  // master thread will sleep.
  usleep(150000);
  gElapsedTime = GetTime();
  func(&parameters[thread_num - 1]);

  return;
}

void join_threads(pthread_t * threads, int thread_num, thread_param * param){
  int cycleI;
  void *retvalue;

  for (cycleI = 0; cycleI < thread_num - 1; ++cycleI){
    pthread_join(threads[cycleI], &retvalue);
    if (NULL == retvalue){
    }
  }

  free(threads);
  free(param);
  return;
}

// Thread function to manipulate the queue
void * queue_thread(void * p){
  thread_param * param = (thread_param *)p;
  if(NULL == param){
    return NULL;
  }

  queue * queues = param->queues;
  if(NULL == queues){
    return NULL;
  }
  
  int thread_id = param->thread_id;
  int write_times = param->write_times;
  int queue_num = param->queue_num;

  int cycleI = 0;
  for(cycleI = 0; cycleI < write_times; ++ cycleI){
    // randomly select a queue to write
    srand((int)GetTime());
    int queue_id = random() % queue_num;
    queue_push(&queues[queue_id], thread_id, cycleI);
  }

  return((void *)1);
}

// initialize the queue
void queue_init(queue * q, int id){
  q->head = NULL;
  q->tail = NULL;
  q->item_num = 0;
  q->queue_id = id;
  my_mutex_init(&(q->mutex));
}

// push an item to the end of the queue
void queue_push(queue * q, int thread_id, int thread_write_num){
  queue_item * item = NULL;
  if(NULL == (item = (queue_item *)malloc(sizeof(queue_item)))){
    puts(malloc_error);
    return;
  }

  // lock the current queue
  my_mutex_lock(&(q->mutex));

  // initialize the item
  item->thread_id = thread_id;
  item->thread_write_num = thread_write_num;
  item->next = NULL;
  item->prev = NULL;
  item->previous_3_sum = 0;

  // get the cumulative sum of all the first elements in the queue
  int cum_sum_first = 0;
  queue_item * ptr = q->head;
  while(ptr){
    cum_sum_first += ptr->thread_write_num;
    ptr = ptr->next;
  }
  item->cum_sum = cum_sum_first;
  item->cum_sum += thread_write_num;

  // if there is more than or equal 3 items in the queue,
  // calculate the sum of the previous_3_sum
  if(q->item_num >=3){
    ptr = q->tail;
    item->previous_3_sum += queue_item_sum(ptr->prev);
    ptr = ptr->prev;
    item->previous_3_sum += queue_item_sum(ptr->prev);
    ptr = ptr->prev;
    item->previous_3_sum += queue_item_sum(ptr->prev);
  }

  // push the item to the end of the queue
  if(NULL == q->head){
    q->head = item;
    q->tail = item;
    q->item_num = 1;
  }else if(NULL != q->tail){
    item->prev = q->tail;
    q->tail->next = item;
    q->tail = item;
    ++(q->item_num);
  }else{
    // this should not happen
    q->tail = item;
    ++(q->item_num);
  }
  
  // unlock the current queue
  my_mutex_unlock(&(q->mutex));
}

// get the sum of all elements in the queue item
int queue_item_sum(queue_item * item){
  if(NULL == item){
    return 0;
  }
  int sum = item->thread_write_num + item->thread_id + item->cum_sum;
  return sum;
}

// output the queue
void queue_output(queue * q, thread_param param){
  if(NULL == q->head){
    printf("Empty Queue");
    return;
  }

  int cum_sum_first = 0;
  int cum_sum_second = 0;
  int cum_sum_fourth = 0;

  queue_item * ptr = q->head;
  while(ptr){
    cum_sum_first += ptr->thread_write_num;
    cum_sum_second += ptr->thread_id;
    cum_sum_fourth += ptr->previous_3_sum;
    ptr = ptr->next;
  }

  printf("Queue %d of %d:\n", q->queue_id + 1, param.queue_num);
  printf("N=%d\n", param.write_times);
  printf("%d items in queue\n", q->item_num);
  printf("Cumulative sum of first elements = %d\n", cum_sum_first);
  printf("Cumulative sum of second elements = %d\n", cum_sum_second);
  printf("Cumulative sum of fourth elements = %d\n", cum_sum_fourth);
  printf("Last element of last item = %d\n", q->tail->cum_sum);
  printf("Summary: [Queue %d/%d, N=%d, %d times, Sums = (%d,%d,%d)]\n",
      q->queue_id + 1, param.queue_num, param.write_times, q->item_num,
      cum_sum_first, cum_sum_second, q->tail->cum_sum);
  g_validate_cum_sum_first += cum_sum_first;
  g_validate_cum_sum_second += cum_sum_second;
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
  my_mutex_destroy(&(q->mutex));
}

double GetTime(void){
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

