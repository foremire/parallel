#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <omp.h>
#include <papi.h>
#include "mm.h"

// messages
char * thread_num_error = "Invalid thread number P = %d. It must be greater than 0.\n";
char * matrix_size_error = "Invalid matrix size N = %d. It must be greater than 1.\n";
char * matrix_size_info = "Matrix Size N = %d\n";
char * malloc_error = "malloc() ERROR.\n";


// PAPI
int EventSet;

int main( int argc, char *argv[] )
{
  matrix matrixA;
  matrix matrixB;
  matrix matrixC;
  matrix matrixCValid;

  int matrix_size = MATRIX_SIZE;

  // initialize PAPI
  int ret = PAPI_library_init( PAPI_VER_CURRENT );
  if ( ret != PAPI_VER_CURRENT ){
    printf(" Failed on init. Returned Value = %d != %d\n", ret, PAPI_VER_CURRENT );
    exit(-1);
  }
  
  EventSet = PAPI_NULL;
  long_long values[ 2 ];
  ret = PAPI_create_eventset( &EventSet );
  if ( ret != PAPI_OK ){
    printf( "Error creating the event set\n" );
    exit(-1);
  }
  
  ret = PAPI_add_event( EventSet, PAPI_TOT_CYC );
  if ( ret != PAPI_OK ){
    printf( "Error on add_event\n" );
    exit(-1);
  }
  ret = PAPI_add_event( EventSet, PAPI_FP_OPS );
  if ( ret != PAPI_OK ){
    printf( "Error on add_event\n" );
    exit(-1);
  }
  
  init_matrix(&matrixA, matrix_size, matrix_size, TRUE);
  init_matrix(&matrixB, matrix_size, matrix_size, TRUE);
  init_matrix(&matrixC, matrix_size, matrix_size, FALSE);
  init_matrix(&matrixCValid, matrix_size, matrix_size, FALSE);

  // do it in parallel way
  PAPI_start(EventSet);

  omp_mat_mul_baseline(matrixA, matrixB, matrixC);
  
  PAPI_stop(EventSet, values);

  double cycles = ( double ) values[ 0 ];
  double time = cycles / 2.33e9;
  double flop = ( double ) values[ 1 ];
  double mflops = flop / time / 1.0e6;

  printf( "Total Cycles (Millions) = %3.3f\n", cycles / 1.0e6 );
  printf( "Total Time (Seconds) = %3.3f\n", time );
  printf( "Total Flops (Millions) = %3.3f\n", flop / 1.0e6 );
  printf( "MFLOPS = %3.3f\n", mflops );
 
  // do it in serial way
  PAPI_start(EventSet);
  serial_mat_mul(matrixA, matrixB, matrixCValid);
  PAPI_stop(EventSet, values);
  
  double cycles_s = ( double ) values[ 0 ];
  double time_s = cycles_s / 2.33e9;
  double speed_up = time_s / time;
  printf( "Speed up = %3.3f\n", speed_up );

  // validate the result
  validate_result(matrixC, matrixCValid);

  SAFE_FREE(matrixA.data);
  SAFE_FREE(matrixB.data);
  SAFE_FREE(matrixC.data);
  SAFE_FREE(matrixCValid.data);
		
  PAPI_cleanup_eventset(EventSet);
  PAPI_destroy_eventset(&EventSet);
  return 0;
}

void init_matrix(matrix * mat, int xDim, int yDim, int random){
  struct timeval tv;
  int cycleI = 0;
  int cycleJ = 0;
  int size = 0;

  size = xDim * yDim;

  mat->data = malloc(size * sizeof(double));
  if(NULL == mat->data){
    puts(malloc_error);
    exit(-1);
  }
  memset(mat->data, 0, size * sizeof(double));
  mat->xDim = xDim;
  mat->yDim = yDim;

  //initilize random generator
  gettimeofday(&tv, NULL);
  srand(tv.tv_sec * tv.tv_usec);

  if(TRUE == random){
    // initilize the matrix by random numbers between -1.00f and 1.00f
    for(cycleI = 0; cycleI < mat->yDim; ++ cycleI){
      for(cycleJ = 0; cycleJ < mat->xDim; ++ cycleJ){
        mat->data[cycleI * mat->xDim + cycleJ] = 
          (double)rand() / ((double)(RAND_MAX)+ 1.00) * 2.0 - 1.0;
      }
    }
  }
}

void omp_mat_mul_baseline(matrix matrixA, matrix matrixB, matrix matrixC){
  
  int cycleI = 0;
  int cycleJ = 0;
  int cycleK = 0;

  #pragma omp parallel shared (matrixA, matrixB, matrixC)\
    private (cycleI, cycleJ, cycleK)
  {
    #pragma omp for
    for(cycleI = 0; cycleI < matrixA.yDim; ++ cycleI){
      for(cycleJ = 0; cycleJ < matrixB.xDim; ++ cycleJ){
        matrixC.data[cycleI * matrixB.xDim + cycleJ] = 0.0;

        for(cycleK = 0; cycleK < matrixA.xDim; ++ cycleK){
          matrixC.data[cycleI * matrixA.xDim + cycleJ] += 
            matrixA.data[cycleI * matrixA.xDim + cycleK] *
            matrixB.data[cycleJ + cycleK * matrixB.xDim];
        }

      }
    }
  }
}

void omp_mat_mul_div(matrix matrixA, matrix matrixB, matrix matrixC){
  int dim = matrixA.xDim;
  int thread_id = 0;
  int num_per_thread = dim / PROCESSOR_NUM;
   
  // calculate the sum in parallel
  //omp_set_num_threads(thread_num);
  #pragma omp parallel shared (matrixA, matrixB, matrixC, num_per_thread)\
    private (thread_id)
  {
    thread_id = omp_get_thread_num();
    int start = num_per_thread * thread_id;
    int end = start + num_per_thread;
    //printf("TID: %d, %d - %d\n", thread_id, start, end);
  
    int cycleI = 0;
    int cycleJ = 0;
    int cycleK = 0;

    for(cycleI = start; cycleI < end; ++ cycleI){
      for(cycleJ = 0; cycleJ < matrixB.xDim; ++ cycleJ){
        matrixC.data[cycleI * dim + cycleJ] = 0.0;

        for(cycleK = 0; cycleK < dim; ++ cycleK){
          matrixC.data[cycleI * dim + cycleJ] += 
            matrixA.data[cycleI * matrixA.xDim + cycleK] *
            matrixB.data[cycleJ + cycleK * matrixB.xDim];
        }

      }
    }
  }
}


void serial_mat_mul(matrix matrixA, matrix matrixB, matrix matrixC){
  int cycleI = 0;
  int cycleJ = 0;
  int cycleK = 0;
  int dim = 0;

  // check whether the dimension of the three matrices match 
  // with each other to conduct the following multiplication
  if(matrixA.xDim != matrixB.yDim){
    printf("matrix A and matrix B's dimension do not match\n");
    return;
  }
  dim = matrixA.xDim;

  if((matrixA.xDim != matrixC.xDim) || (matrixA.yDim != matrixC.yDim)){
    printf("matrix A and matrix C's dimension do not match\n");
    return;
  }

  for(cycleI = 0; cycleI < matrixA.yDim; ++ cycleI){
    for(cycleJ = 0; cycleJ < matrixB.xDim; ++ cycleJ){
      
      matrixC.data[cycleI * dim + cycleJ] = 0.0;

      for(cycleK = 0; cycleK < dim; ++ cycleK){
        matrixC.data[cycleI * dim + cycleJ] += 
          matrixA.data[cycleI * matrixA.xDim + cycleK] *
          matrixB.data[cycleJ + cycleK * matrixB.xDim];
      }
    }
  }
}

void validate_result(matrix matrixA, matrix matrixB){
  int cycleI = 0;
  int cycleJ = 0;

  int match = 0;
  int mismatch = 0;
  int size = 0;

  // check whether the dimension of the three matrices match 
  // with each other to conduct the following multiplication
  if((matrixA.xDim != matrixB.xDim) || (matrixA.yDim != matrixB.yDim)){
    printf("matrix A and matrix B's dimension do not match\n");
    return;
  }

  size = matrixA.xDim * matrixA.yDim;
  
  for(cycleI = 0; cycleI < matrixA.yDim; ++ cycleI){
    for(cycleJ = 0; cycleJ < matrixA.xDim; ++ cycleJ){
      if(matrixA.data[cycleI * matrixA.xDim + cycleJ] != 
          matrixB.data[cycleI * matrixA.xDim + cycleJ]){
        ++ mismatch;
      }else{
        ++ match;
      }
    }
  }

  printf("%d numbers in total, %d matched, %d mismatched\n", size, match, mismatch);
}

void safe_exit(matrix ma, matrix mb, matrix mc, matrix me){
  SAFE_FREE(ma.data);
  SAFE_FREE(mb.data);
  SAFE_FREE(mc.data);
  SAFE_FREE(me.data);
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
