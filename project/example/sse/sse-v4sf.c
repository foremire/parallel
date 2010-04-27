#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


typedef float ftype;
// vector of four single floats
typedef float v4sf __attribute__ ((vector_size(16))); 

#define VECTOR_LENGTH 120
#define SSE_LENGTH 4
#define RUN_TIME 5000000

double get_duration(struct timeval __start);


int main()
{
  ftype vec_a[VECTOR_LENGTH];
  ftype vec_b[VECTOR_LENGTH];

  ftype imd_ret[SSE_LENGTH];

  ftype sse_ret = 0.0f;
  ftype nor_ret = 0.0f;

  double sse_time = 0.0f;
  double nor_time = 0.0f;

  struct timeval tv;

  //initilize random generator
  gettimeofday(&tv, NULL);
  srand(tv.tv_sec * tv.tv_usec);
  
  int i = 0;
  for(i = 0; i < VECTOR_LENGTH; ++i){
    vec_a[i] = (ftype)rand() / ((ftype)(RAND_MAX)+ 1.00) * 2.0 - 1.0;
    vec_b[i] = (ftype)rand() / ((ftype)(RAND_MAX)+ 1.00) * 2.0 - 1.0;
  }
 
  v4sf acc;
  v4sf oprand_a;
  v4sf oprand_b;

  int j = 0;
  gettimeofday(&tv, NULL);
  for(j = 0; j < RUN_TIME; ++j){
    acc = __builtin_ia32_xorps(acc, acc);
    for(i = 0; i < (VECTOR_LENGTH - SSE_LENGTH + 1); i += SSE_LENGTH){
      oprand_a = __builtin_ia32_loadups(&vec_a[i]);
      oprand_b = __builtin_ia32_loadups(&vec_b[i]);
      acc = __builtin_ia32_addps(acc, __builtin_ia32_mulps(oprand_a, oprand_b));
    }

    __builtin_ia32_storeups(imd_ret, acc);

    sse_ret = 0.0f;
    for(i = 0; i < SSE_LENGTH; ++i){
      sse_ret += imd_ret[i];
    }
  }
  sse_time = get_duration(tv);

  gettimeofday(&tv, NULL);
  for(j = 0; j < RUN_TIME; ++j){
    nor_ret = 0;
    for(i = 0; i < VECTOR_LENGTH; ++i){
      nor_ret += vec_a[i] * vec_b[i];
    }
  }
  nor_time = get_duration(tv);

  printf("SSE: %3.4f, NORMAL: %3.4f\n", sse_ret, nor_ret);
  printf("SSE Time: %3.4f, NORMAL Time: %3.4f\n", sse_time, nor_time);
  printf("Speed Up: %3.4f\n", nor_time / sse_time);
  return 0;
}

double get_duration(struct timeval __start){
  struct timeval  __end;

  double t_start = 0.0;
  double t_end = 0.0;
  double duration = 0.0;
  
  gettimeofday(&__end, NULL);
  t_start = (__start.tv_sec + (__start.tv_usec/1000000.0));
  t_end = (__end.tv_sec + (__end.tv_usec/1000000.0));
  duration= t_end - t_start;

  return duration;
}
