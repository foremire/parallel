#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// vector of four single floats
typedef double v2df __attribute__ ((vector_size(16))); 

#define VECTOR_LENGTH 120
#define SSE_LENGTH 2

int main()
{
  double vec_a[VECTOR_LENGTH];
  double vec_b[VECTOR_LENGTH];

  double imd_ret[SSE_LENGTH];

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
 
  v2df acc;
  v2df oprand_a;
  v2df oprand_b;
  acc = __builtin_ia32_xorpd(acc, acc);

  for(i = 0; i < (VECTOR_LENGTH - SSE_LENGTH + 1); i += SSE_LENGTH){
    oprand_a = __builtin_ia32_loadupd(&vec_a[i]);
    oprand_b = __builtin_ia32_loadupd(&vec_b[i]);
    acc = __builtin_ia32_addpd(acc, __builtin_ia32_mulpd(oprand_a, oprand_b));
  }

  __builtin_ia32_storeupd(imd_ret, acc);

  for(i = 0; i < SSE_LENGTH; ++i){
    sse_ret += imd_ret[i];
  }

  for(i = 0; i < VECTOR_LENGTH; ++i){
    nor_ret += vec_a[i] * vec_b[i];
  }

  printf("SSE: %3.4f, NORMAL: %3.4f\n", sse_ret, nor_ret);
  return 0;
}
