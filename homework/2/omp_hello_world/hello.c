#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

char * usage = "usage: hello thread_num\n";
char * msg_template = "hello world from thread %d of %d\n";

int main( int argc, char * argv[]){
  int thread_num = 1;
  int thread_id = 0;

  if(2 > argc){
    puts(usage);
    exit(0);
  }

  thread_num = atoi(argv[1]);
  if(1 > thread_num){
    exit(0);
  }

  omp_set_num_threads(thread_num);

#pragma omp parallel private (thread_id)
  {
    thread_id = omp_get_thread_num();
    printf(msg_template, thread_id, thread_num);
  }
  return 0;
}
