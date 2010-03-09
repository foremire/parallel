#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

//global varialbes and definitions
#define TOTAL_NUMBER 1000
#define MAX_NUM 100
char * usage = "usage: sum thread_num\n";
char * msg_template = "hello world from thread %d of %d\n";

int main( int argc, char * argv[]){
  int thread_num = 1;
  int thread_id = 0;
  int cycle = 0;
  int sum = 0;
  int partial_sum = 0;
  
  int remainder = 0;
  int num_per_thread = 0;
 
  struct timeval __start;
  struct timeval  __end;
  double t1 = 0.0f;
  double t2 = 0.0f;

  struct timeval tv;
  int num_array [TOTAL_NUMBER];

  if(2 > argc){
    puts(usage);
    exit(0);
  }

  thread_num = atoi(argv[1]);
  printf("num of threads: %d\n", thread_num);
  
  // Get how many numbers should be processed per process
  num_per_thread = TOTAL_NUMBER / thread_num;
  remainder = TOTAL_NUMBER % thread_num;
    
  // initilize random generator
  gettimeofday(&tv, NULL);
  srand(tv.tv_sec * tv.tv_usec);

  gettimeofday(&__start, NULL);
  // calculate the sum in parallel
  omp_set_num_threads(thread_num);
#pragma omp parallel private (thread_id, partial_sum, cycle) shared (sum)
  {
    thread_id = omp_get_thread_num();
    //printf(msg_template, thread_id, thread_num);
    partial_sum = 0;

    // initilize the array by random value and calculate the partial sum
    if(0 == thread_id){
      for(cycle = 0; cycle < num_per_thread + remainder; ++ cycle){
        num_array[cycle] = rand() % MAX_NUM;
        partial_sum += num_array[cycle];
      }
    }else{
      for(cycle = thread_id * num_per_thread + remainder; 
          cycle < (thread_id + 1) * num_per_thread + remainder; ++ cycle){
        num_array[cycle] = rand() % MAX_NUM;
        partial_sum += num_array[cycle];
      }
    }
#pragma omp atomic
    sum += partial_sum;
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
  printf("running time: %.6lfs\n", t2 - t1);
  //printf("%d %.6lf\n", thread_num, t2 - t1);

  return 0;
}
