#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

char * usage = "usage: hello thread_num\n";
char * msg_template = "hello world from thread %d of %d\n";

typedef struct _thread_info{
  int thread_id;
  int thread_num;
}thread_info;

void * child(void * param){
  thread_info *p;
  p = (thread_info *)param;
  printf(msg_template, p->thread_id, p->thread_num);
  return NULL;
}

int main( int argc, char * argv[]){
  int thread_num = 1;
  int cycle = 0;
  thread_info info;
  pthread_t child_descriptor;
  void * ret = NULL;

  if(2 > argc){
    puts(usage);
    exit(0);
  }

  thread_num = atoi(argv[1]);
  if(1 > thread_num){
    exit(0);
  }
  info.thread_num = thread_num;

  for(cycle = 0; cycle < thread_num; ++ cycle){
    info.thread_id = cycle;
    pthread_create(&child_descriptor, NULL, child, (void *)&info);
    pthread_join(child_descriptor, &ret);
  }

  return 0;
}
