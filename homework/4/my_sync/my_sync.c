#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#include "my_sync.h"

void my_mutex_init(my_mutex_t * p){
  p->lock = FREE;
}

void my_mutex_lock(my_mutex_t * p){
  while(LOCKED == p->lock){
    usleep(LOCK_WAIT_INTERVAL);
  }
  p->lock = LOCKED;
}

void my_mutex_unlock(my_mutex_t * p){
  p->lock = FREE;
}

void my_mutex_destroy(my_mutex_t * p){
  p->lock = FREE;
}

void my_barrier_init(my_barrier_t * p, int n){
  p->sense = BARRIER_NA;
  p->num_p = n;
  p->cur_p = 0;
}

void my_barrier_wait(my_barrier_t * p){
  static int local_sense = BARRIER_NA;
  // Atomically add the the counter of current peers
  __sync_fetch_and_add(&(p->cur_p), 1);
  p->sense = BARRIER_NA;
  local_sense = BARRIER_NA;

  // check wheter I'am the last one to reach the barrier
  if(p->num_p == p->cur_p){
    // yes, notify other peers
    p->sense = BARRIER_AVAIL;
  }else{
    // else, wait for other peers
    while(p->sense == BARRIER_NA){
      //usleep(BARRIER_WAIT_INTERVAL);
    }
  }
}

void my_barrier_destroy(my_barrier_t * p){
  p->sense = BARRIER_NA;
  p->num_p = 1;
  p->cur_p = 0;
}
