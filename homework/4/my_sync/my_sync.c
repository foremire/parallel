#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#include "my_sync.h"

void my_mutex_init(my_mutex_t * p){
  p = NULL;
}

void my_mutex_lock(my_mutex_t * p){
  p = NULL;
}

void my_mutex_unlock(my_mutex_t * p){
  p = NULL;
}

void my_mutex_destroy(my_mutex_t * p){
  p = NULL;
}

void my_barrier_init(my_barrier_t * p, int n){
  p = NULL;
  n = 0;
}

void my_barrier_wait(my_barrier_t * p){
  p = NULL;
}

void my_barrier_destroy(my_barrier_t * p){
  p = NULL;
}
