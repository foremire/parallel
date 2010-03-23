#ifndef MY_SYNC_H
#define MY_SYNC_H

// Type definition
typedef struct _my_mutex_t{
  int lock;
}my_mutex_t;

typedef struct _my_barrier_t{
  int lock;
}my_barrier_t;

// Function definition
// For mutex
void my_mutex_init(my_mutex_t * p);

void my_mutex_lock(my_mutex_t * p);

void my_mutex_unlock(my_mutex_t * p);

void my_mutex_destroy(my_mutex_t * p);

// For barrier
void my_barrier_init(my_barrier_t * p, int n);

void my_barrier_wait(my_barrier_t * p);

void my_barrier_destroy(my_barrier_t * p);

#endif
