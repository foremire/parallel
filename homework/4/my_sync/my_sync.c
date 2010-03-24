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
  // wait for the lock to be freed, lock it then
  while(!__sync_bool_compare_and_swap(&(p->lock), FREE, LOCKED)){
    usleep(LOCK_WAIT_INTERVAL);
  }
  // lock the lock
  //__sync_lock_test_and_set(&(p->lock), LOCKED);
}

void my_mutex_unlock(my_mutex_t * p){
  __sync_lock_test_and_set(&(p->lock), FREE);
}

void my_mutex_destroy(my_mutex_t * p){
  p->lock = FREE;
}

void my_barrier_init(my_barrier_t * p, int n){
  p->sense = BARRIER_NA;
  p->num_p = n;
  p->cur_p = 0;
  p->occupied = BARRIER_FREE;
  my_mutex_init((my_mutex_t*)&(p->lock));
}

void my_barrier_wait(my_barrier_t * p){
  while(BARRIER_OCCUPIED == p->occupied){
    usleep(BARRIER_WAIT_INTERVAL);
  }

  // lock it first
  my_mutex_lock((my_mutex_t*)&(p->lock));
  // Atomically add the the counter of current peers
  ++(p->cur_p);
  
  if(p->cur_p == p->num_p){
    // yes, notify other peers
    p->sense = BARRIER_AVAIL;
    p->occupied = BARRIER_OCCUPIED;
  }else{
    p->sense = BARRIER_NA;
  }
  // unlock it then
  my_mutex_unlock((my_mutex_t*)&(p->lock));

  // else, wait for other peers
  while(BARRIER_NA == p->sense){
    usleep(BARRIER_WAIT_INTERVAL);
  }
 
  my_mutex_lock((my_mutex_t*)&(p->lock));
  --(p->cur_p);
  if(0 == p->cur_p){
    p->occupied = BARRIER_FREE;
  }
  my_mutex_unlock((my_mutex_t*)&(p->lock));
}

void my_barrier_destroy(my_barrier_t * p){
  p->sense = BARRIER_NA;
  p->num_p = 1;
  p->cur_p = 0;
  p->occupied = BARRIER_FREE;
  my_mutex_destroy((my_mutex_t*)&(p->lock));
}
