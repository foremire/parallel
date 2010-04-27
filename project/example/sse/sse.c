#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// vector of four single floats
typedef double v4df __attribute__ ((vector_size(32))); 

#define VECTOR_LENGTH 12

int main()
{
  double vec_a[VECTOR_LENGTH];
  double vec_b[VECTOR_LENGTH];

  double sse_ret = 0.0f;
  double nor_ret = 0.0f;

  struct timeval tv;

  //initilize random generator
  gettimeofday(&tv, NULL);
  srand(tv.tv_sec * tv.tv_usec);
  
  int i = 0;
  for(i = 0; i < VECTOR_LENGTH; ++i){
    vec_a[i] = (double)rand() / ((double)(RAND_MAX)+ 1.00) * 2.0 - 1.0;
    vec_b[i] = (double)rand() / ((double)(RAND_MAX)+ 1.00) * 2.0 - 1.0;
  }

  return 0;
}
