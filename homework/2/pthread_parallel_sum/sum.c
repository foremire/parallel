#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

//global varialbes and definitions
#define TOTAL_NUMBER 1000
#define MAX_NUM 100
char * usage = "usage: sum thread_num\n";
char * msg_template = "hello world from thread %d of %d\n";


typedef struct _thread_info{
  int thread_id;
  int thread_num;
  int num_per_thread;
  int remainder;
  int partial_sum;
  int * num_array;
  pthread_t child_descriptor;
}thread_info;

void * child(void * param){
  thread_info *info;
  int cycle = 0;
  info = (thread_info *)param;
  //printf(msg_template, info->thread_id, info->thread_num);
    
  // initilize the array by random value and calculate the partial sum
  if(0 == info->thread_id){
    for(cycle = 0; cycle < info->num_per_thread + info->remainder; ++ cycle){
      info->num_array[cycle] = rand() % MAX_NUM;
      info->partial_sum += info->num_array[cycle];
    }
  }else{
    for(cycle = info->thread_id * info->num_per_thread + info->remainder; 
        cycle < (info->thread_id + 1) * info->num_per_thread + info->remainder; ++ cycle){
      info->num_array[cycle] = rand() % MAX_NUM;
      info->partial_sum += info->num_array[cycle];
    }
  }

  return (void *)info;;
}


int main( int argc, char * argv[]){
  int thread_num = 1;
  int cycle = 0;
  int sum = 0;
  void * ret = NULL;
  
  int remainder = 0;
  int num_per_thread = 0;
 
  struct timeval __start;
  struct timeval  __end;
  double t1 = 0.0f;
  double t2 = 0.0f;

  struct timeval tv;
  int num_array [TOTAL_NUMBER];
  thread_info * info = NULL;

  if(2 > argc){
    puts(usage);
    exit(0);
  }

  thread_num = atoi(argv[1]);
  if(1 > thread_num){
    exit(0);
  }
  printf("num of threads: %d\n", thread_num);

  if(NULL == (info = (thread_info *)malloc(thread_num * sizeof(thread_info)))){
    printf("malloc error!\n");
    exit(-1);
  }
  
  // Get how many numbers should be processed per process
  num_per_thread = TOTAL_NUMBER / thread_num;
  remainder = TOTAL_NUMBER % thread_num;
    
  // initilize random generator
  gettimeofday(&tv, NULL);
  srand(tv.tv_sec * tv.tv_usec);

  gettimeofday(&__start, NULL);

  for(cycle = 0; cycle < thread_num; ++ cycle){
    info[cycle].thread_num = thread_num;
    info[cycle].thread_id = cycle;
    info[cycle].num_per_thread = num_per_thread;
    info[cycle].remainder = remainder;
    info[cycle].partial_sum = 0;
    info[cycle].num_array = num_array;
    pthread_create(&(info[cycle].child_descriptor), NULL, 
        child, (void *)&(info[cycle]));
  }
  for(cycle = 0; cycle < thread_num; ++ cycle){
    pthread_join(info[cycle].child_descriptor, &ret);
    sum += info[cycle].partial_sum;
  }

  gettimeofday(&__end, NULL);
  printf("parallel sum: %d\n", sum);
  
  // calculate the sum in serial
  sum = 0;
  for(cycle = 0; cycle < TOTAL_NUMBER; ++ cycle){
    sum += num_array[cycle];
  }
  printf("serial sum: %d\n", sum);
 
  t1 = __start.tv_sec + (__start.tv_usec/1000000.0);
  t2 = __end.tv_sec + (__end.tv_usec/1000000.0);
  printf("exec time: %.6lf\n", t2 - t1);
  //printf("%d %.6lf\n", thread_num, t2 - t1);

  free(info);

  return 0;
}
