#ifndef MY_SYNC_H
#define MY_SYNC_H

// Type definition
typedef struct _my_mutex_t{
  volatile int lock;
}my_mutex_t;

#define LOCKED 1
#define FREE   0
#define LOCK_WAIT_INTERVAL 100

// Function definition
// For mutex
void my_mutex_init(my_mutex_t * p);

void my_mutex_lock(my_mutex_t * p);

void my_mutex_unlock(my_mutex_t * p);

void my_mutex_destroy(my_mutex_t * p);

// For barrier
typedef struct _my_barrier_t{
  volatile int sense;
  volatile int num_p;
  volatile int cur_p;
  volatile int occupied;
  volatile my_mutex_t lock;
}my_barrier_t;

#define BARRIER_NA       0
#define BARRIER_AVAIL    1
#define BARRIER_FREE     0
#define BARRIER_OCCUPIED 1
#define BARRIER_WAIT_INTERVAL 100

void my_barrier_init(my_barrier_t * p, int n);

void my_barrier_wait(my_barrier_t * p);

void my_barrier_destroy(my_barrier_t * p);

#endif
