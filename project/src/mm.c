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

#define EVENT_SET_SIZE 4
long_long serial_values[EVENT_SET_SIZE];
long_long parallel_values[EVENT_SET_SIZE];

int serial_event_set;
int parallel_event_set;

ftype init_array[4];

int main( int argc, char *argv[] )
{
  matrix matrixA;
  matrix matrixB;
  matrix matrixC;
  matrix matrixCValid;

  struct timeval __start;
  double t_omp = 0.0f;
  double t_serial = 0.0f;


  int matrix_size = MATRIX_SIZE;

  // initialize PAPI
  int ret = PAPI_library_init( PAPI_VER_CURRENT );
  if ( ret != PAPI_VER_CURRENT ){
    printf(" Failed on init. Returned Value = %d != %d\n", ret, PAPI_VER_CURRENT );
    exit(-1);
  }
  
  init_matrix(&matrixA, matrix_size, matrix_size, TRUE);
  init_matrix(&matrixB, matrix_size, matrix_size, TRUE);
  init_matrix(&matrixC, matrix_size, matrix_size, FALSE);
  init_matrix(&matrixCValid, matrix_size, matrix_size, FALSE);

  // do it in parallel way
  gettimeofday(&__start, NULL);
  //omp_mat_mul_baseline(matrixA, matrixB, matrixC);
  //omp_mat_mul_div(matrixA, matrixB, matrixC);
  //omp_mat_mul_transpose(matrixA, matrixB, matrixC);
  //omp_mat_mul_transpose_sse(matrixA, matrixB, matrixC);
  //omp_mat_mul_transpose_sse_ppl(matrixA, matrixB, matrixC);
  omp_mat_mul_transpose_sse_ppl2(matrixA, matrixB, matrixC);
  t_omp = get_duration(__start);

  //sleep(2);

  // do it in serial way
  gettimeofday(&__start, NULL);
  serial_mat_mul(matrixA, matrixB, matrixCValid);
  t_serial = get_duration(__start);
  //t_serial = SERIAL_TIME;
 
  // report the result
  //report_result();
  printf("Serial Time:%3.6f\n", t_serial);
  printf("Parallel Time:%3.6f\n", t_omp);
  printf("Speed Up:%3.6f\n", t_serial/t_omp);
  
  // validate the result
  validate_result(matrixC, matrixCValid);

  SAFE_FREE(matrixA.data);
  SAFE_FREE(matrixB.data);
  SAFE_FREE(matrixC.data);
  SAFE_FREE(matrixCValid.data);
		
  return 0;
}

void init_matrix(matrix * mat, int xDim, int yDim, int random){
  struct timeval tv;
  int cycleI = 0;
  int cycleJ = 0;
  int size = 0;

  size = xDim * yDim;

  mat->data = malloc(size * sizeof(ftype));
  if(NULL == mat->data){
    puts(malloc_error);
    exit(-1);
  }
  memset(mat->data, 0, size * sizeof(ftype));
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
          (ftype)rand() / ((ftype)(RAND_MAX)+ 1.00) * 2.0 - 1.0;
      }
    }
  }
}

/*
 * Baseline version
 * *
 */
void omp_mat_mul_baseline(matrix matrixA, matrix matrixB, matrix matrixC){
  int cycleI = 0;
  int cycleJ = 0;
  int cycleK = 0;
  
  // init PAPI
  parallel_event_set = PAPI_NULL;
  
  if(PAPI_thread_init((unsigned long (*)(void)) (omp_get_thread_num)) != PAPI_OK){
    printf( "Error on thread_init\n" );
    exit(-1);
  }

  omp_set_num_threads(PROCESSOR_NUM);
  #pragma omp parallel shared (matrixA, matrixB, matrixC)\
    private (cycleI, cycleJ, cycleK)
  {
    /*
    if(PAPI_OK != PAPI_create_eventset(&parallel_event_set)){
      printf( "Error creating the event set\n" );
      exit(-1);
    }
    
    if(PAPI_OK != PAPI_add_event(parallel_event_set, PAPI_TOT_CYC)){
      printf( "Error on add_event\n" );
      exit(-1);
    }
    if(PAPI_OK != PAPI_add_event(parallel_event_set, PAPI_FP_OPS)){
      printf( "Error on add_event\n" );
      exit(-1);
    }
    */
   
    PAPI_start(parallel_event_set);

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
    PAPI_stop(parallel_event_set, parallel_values);
  }

  PAPI_cleanup_eventset(parallel_event_set);
  PAPI_destroy_eventset(&parallel_event_set);
}

/*
 * Baseline version 2
 * 
 */
void omp_mat_mul_div(matrix matrixA, matrix matrixB, matrix matrixC){
  int dim = matrixA.xDim;
  int thread_id = 0;
  int num_per_thread = dim / PROCESSOR_NUM;
   
  // calculate the sum in parallel
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

/*
 * tranpose matrixB so that it can be accessed in one row a time,
 * just to utilize the cacheline
 *
 */
void omp_mat_mul_transpose(matrix matrixA, matrix matrixB, matrix matrixC){
  int dim = matrixA.xDim;
  int thread_id = 0;
  int num_per_thread = dim / PROCESSOR_NUM;
   
  matrix matrixBT;
  init_matrix(&matrixBT, matrixB.xDim, matrixB.yDim, FALSE);

  int cycleI = 0;
  int cycleJ = 0;

  for(cycleI = 0; cycleI < matrixB.yDim; ++ cycleI){
    for(cycleJ = 0; cycleJ < matrixB.xDim; ++ cycleJ){
      matrixBT.data[cycleJ * matrixB.xDim + cycleI] =
        matrixB.data[cycleI * matrixB.xDim + cycleJ];
    }
  }

  omp_set_num_threads(PROCESSOR_NUM);
  //calculate the sum in parallel
  #pragma omp parallel shared (matrixA, matrixBT, matrixC, num_per_thread)\
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
            //matrixB.data[cycleJ + cycleK * matrixB.xDim];
            matrixBT.data[cycleJ * matrixBT.xDim + cycleK];
        }

      }
    }
  }
  
  SAFE_FREE(matrixBT.data);
}



/*
 * use both transpose and SSE
 *
 */
void omp_mat_mul_transpose_sse(matrix matrixA, matrix matrixB, matrix matrixC){
  int dim = matrixA.xDim;
  int thread_id = 0;
  int num_per_thread = dim / PROCESSOR_NUM;
   
  matrix matrixBT;
  init_matrix(&matrixBT, matrixB.xDim, matrixB.yDim, FALSE);

  int cycleI = 0;
  int cycleJ = 0;

  for(cycleI = 0; cycleI < matrixB.yDim; ++ cycleI){
    for(cycleJ = 0; cycleJ < matrixB.xDim; ++ cycleJ){
      matrixBT.data[cycleJ * matrixB.xDim + cycleI] =
        matrixB.data[cycleI * matrixB.xDim + cycleJ];
    }
  }

  omp_set_num_threads(PROCESSOR_NUM);
  //calculate the sum in parallel
  #pragma omp parallel shared (matrixA, matrixBT, matrixC, num_per_thread)\
    private (thread_id)
  {
    thread_id = omp_get_thread_num();
    int start = num_per_thread * thread_id;
    int end = start + num_per_thread;
    //printf("TID: %d, %d - %d\n", thread_id, start, end);
  
    int cycleI = 0;
    int cycleJ = 0;
    int cycleK = 0;

    v4sf acc;
    v4sf oprand_a;
    v4sf oprand_b;
    ftype imd_ret[SSE_LENGTH];

    for(cycleI = start; cycleI < end; ++ cycleI){
      for(cycleJ = 0; cycleJ < matrixB.xDim; ++ cycleJ){
        // set acc to 0
        acc = __builtin_ia32_loadups(init_array);
        
        for(cycleK = 0; cycleK < (dim - SSE_LENGTH + 1); cycleK += SSE_LENGTH){
          oprand_a = __builtin_ia32_loadups(
            &(matrixA.data[cycleI * dim + cycleK]));
          oprand_b = __builtin_ia32_loadups(
            &(matrixBT.data[cycleJ * dim + cycleK]));
          acc = __builtin_ia32_addps(acc, __builtin_ia32_mulps(oprand_a, oprand_b));
        }
        __builtin_ia32_storeups(imd_ret, acc);

        for(cycleK = 1; cycleK < SSE_LENGTH; ++ cycleK){
          imd_ret[0] += imd_ret[cycleK];
        }
        matrixC.data[cycleI * dim + cycleJ] = imd_ret[0];
      }
    }
  }
  
  SAFE_FREE(matrixBT.data);
}

/*
 * use transpose, SSE and pipeline( Unrolling + Scheduling )
 * L2 cache: 4096 KB per CPU, 8192 KB total
 * L1 cache: 32 KB per core
 * Intel64: 128 bit SIDM instructions referencing XMM register can access 16
 * XMM registers in 64-bit model
 */
void omp_mat_mul_transpose_sse_ppl(matrix matrixA, matrix matrixB, matrix matrixC){
  int dim = matrixA.xDim;
  int thread_id = 0;
  int num_per_thread = dim / PROCESSOR_NUM;
   
  matrix matrixBT;
  init_matrix(&matrixBT, matrixB.xDim, matrixB.yDim, FALSE);

  int cycleI = 0;
  int cycleJ = 0;

  for(cycleI = 0; cycleI < matrixB.yDim; ++ cycleI){
    for(cycleJ = 0; cycleJ < matrixB.xDim; ++ cycleJ){
      matrixBT.data[cycleJ * matrixB.xDim + cycleI] =
        matrixB.data[cycleI * matrixB.xDim + cycleJ];
    }
  }

  omp_set_num_threads(PROCESSOR_NUM);
  //calculate the sum in parallel
  #pragma omp parallel shared (matrixA, matrixBT, matrixC, num_per_thread)\
    private (thread_id)
  {
    thread_id = omp_get_thread_num();
    int start = num_per_thread * thread_id;
    int end = start + num_per_thread;
    //printf("TID: %d, %d - %d\n", thread_id, start, end);
  
    int cycleI = 0;
    int cycleJ = 0;
    int cycleK = 0;

    v4sf acc_0;
    v4sf acc_1;
    v4sf acc_2;
    v4sf acc_3;
    
    v4sf oprand_a;

    v4sf oprand_b_0;
    v4sf oprand_b_1;
    v4sf oprand_b_2;
    v4sf oprand_b_3;

    ftype imd_ret_0[SSE_LENGTH];
    ftype imd_ret_1[SSE_LENGTH];
    ftype imd_ret_2[SSE_LENGTH];
    ftype imd_ret_3[SSE_LENGTH];

    for(cycleI = start; cycleI < end; ++ cycleI){
      for(cycleJ = 0; cycleJ < matrixB.xDim - 3; cycleJ += 4){
        // set acc to 0
        acc_0 = __builtin_ia32_loadups(init_array);
        acc_1 = __builtin_ia32_loadups(init_array);
        acc_2 = __builtin_ia32_loadups(init_array);
        acc_3 = __builtin_ia32_loadups(init_array);
        
        for(cycleK = 0; cycleK < (dim - SSE_LENGTH + 1); cycleK += SSE_LENGTH){
          oprand_a = __builtin_ia32_loadups(
            &(matrixA.data[cycleI * dim + cycleK]));

          oprand_b_0 = __builtin_ia32_loadups(
            &(matrixBT.data[cycleJ * dim + cycleK]));
          oprand_b_1 = __builtin_ia32_loadups(
            &(matrixBT.data[(cycleJ + 1) * dim + cycleK]));
          oprand_b_2 = __builtin_ia32_loadups(
            &(matrixBT.data[(cycleJ + 2) * dim + cycleK]));
          oprand_b_3 = __builtin_ia32_loadups(
            &(matrixBT.data[(cycleJ + 3) * dim + cycleK]));
          
          acc_0 = __builtin_ia32_addps(acc_0, __builtin_ia32_mulps(oprand_a, oprand_b_0));
          acc_1 = __builtin_ia32_addps(acc_1, __builtin_ia32_mulps(oprand_a, oprand_b_1));
          acc_2 = __builtin_ia32_addps(acc_2, __builtin_ia32_mulps(oprand_a, oprand_b_2));
          acc_3 = __builtin_ia32_addps(acc_3, __builtin_ia32_mulps(oprand_a, oprand_b_3));

        }
        __builtin_ia32_storeups(imd_ret_0, acc_0);
        __builtin_ia32_storeups(imd_ret_1, acc_1);
        __builtin_ia32_storeups(imd_ret_2, acc_2);
        __builtin_ia32_storeups(imd_ret_3, acc_3);

        for(cycleK = 1; cycleK < SSE_LENGTH; ++ cycleK){
          imd_ret_0[0] += imd_ret_0[cycleK];
          imd_ret_1[0] += imd_ret_1[cycleK];
          imd_ret_2[0] += imd_ret_2[cycleK];
          imd_ret_3[0] += imd_ret_3[cycleK];
        }
        matrixC.data[cycleI * dim + cycleJ] = imd_ret_0[0];
        matrixC.data[cycleI * dim + cycleJ + 1] = imd_ret_1[0];
        matrixC.data[cycleI * dim + cycleJ + 2] = imd_ret_2[0];
        matrixC.data[cycleI * dim + cycleJ + 3] = imd_ret_3[0];
      }
    }
  }
  
  SAFE_FREE(matrixBT.data);
}

/*
 * use transpose, SSE and pipeline( Unrolling + Scheduling + Sofeware Pileple )
 * L2 cache: 4096 KB per CPU, 8192 KB total
 * L1 cache: 32 KB per core
 * Intel64: 128 bit SIDM instructions referencing XMM register can access 16
 * XMM registers in 64-bit model
 */
void omp_mat_mul_transpose_sse_ppl2(matrix matrixA, matrix matrixB, matrix matrixC){
  int dim = matrixA.xDim;
  int thread_id = 0;
  int num_per_thread = dim / PROCESSOR_NUM;
   
  matrix matrixBT;
  init_matrix(&matrixBT, matrixB.xDim, matrixB.yDim, FALSE);

  int cycleI = 0;
  int cycleJ = 0;

  for(cycleI = 0; cycleI < matrixB.yDim; ++ cycleI){
    for(cycleJ = 0; cycleJ < matrixB.xDim; ++ cycleJ){
      matrixBT.data[cycleJ * matrixB.xDim + cycleI] =
        matrixB.data[cycleI * matrixB.xDim + cycleJ];
    }
  }

  omp_set_num_threads(PROCESSOR_NUM);
  //calculate the sum in parallel
  #pragma omp parallel shared (matrixA, matrixBT, matrixC, num_per_thread)\
    private (thread_id)
  {
    thread_id = omp_get_thread_num();
    int start = num_per_thread * thread_id;
    int end = start + num_per_thread;
    //printf("TID: %d, %d - %d\n", thread_id, start, end);
  
    int cycleI = 0;
    int cycleJ = 0;
    int cycleK = 0;

    // accumulator register
    v4sf acc_0;
    v4sf acc_1;

    // group 0
    v4sf oprand_a_0;
    v4sf oprand_b_0_0;
    v4sf oprand_b_0_1;

    // group 1
    v4sf oprand_a_1;
    v4sf oprand_b_1_0;
    v4sf oprand_b_1_1;

    ftype imd_ret_0[SSE_LENGTH];
    ftype imd_ret_1[SSE_LENGTH];

    int is_odd = 0;

    for(cycleI = start; cycleI < end; ++ cycleI){
      for(cycleJ = 0; cycleJ < matrixB.xDim - 1; cycleJ += 2){
        // set acc to 0
        acc_0 = __builtin_ia32_loadups(init_array);
        acc_1 = __builtin_ia32_loadups(init_array);
    
        // prefetch the data for the first run
        oprand_a_0 = __builtin_ia32_loadups(
          &(matrixA.data[cycleI * dim]));
        oprand_b_0_0 = __builtin_ia32_loadups(
          &(matrixBT.data[cycleJ * dim]));
        oprand_b_0_1 = __builtin_ia32_loadups(
          &(matrixBT.data[(cycleJ + 1) * dim]));

        for(cycleK = SSE_LENGTH; cycleK < (dim - SSE_LENGTH + 1); cycleK += SSE_LENGTH){
          is_odd = cycleK / 4;
          is_odd = is_odd % 2;
            
          if(is_odd){
            // prefetch the data for the next run
            oprand_a_1 = __builtin_ia32_loadups(
              &(matrixA.data[cycleI * dim + cycleK]));
            oprand_b_1_0 = __builtin_ia32_loadups(
              &(matrixBT.data[cycleJ * dim + cycleK]));
            oprand_b_1_1 = __builtin_ia32_loadups(
              &(matrixBT.data[(cycleJ + 1) * dim + cycleK]));

            // calculate the sum for the current run
            acc_0 = __builtin_ia32_addps(acc_0,
                __builtin_ia32_mulps(oprand_a_0, oprand_b_0_0));
            acc_1 = __builtin_ia32_addps(acc_1, 
                __builtin_ia32_mulps(oprand_a_0, oprand_b_0_1));
          }else{
            // prefetch the data for the next run
            oprand_a_0 = __builtin_ia32_loadups(
              &(matrixA.data[cycleI * dim + cycleK]));
            oprand_b_0_0 = __builtin_ia32_loadups(
              &(matrixBT.data[cycleJ * dim + cycleK]));
            oprand_b_0_1 = __builtin_ia32_loadups(
              &(matrixBT.data[(cycleJ + 1) * dim + cycleK]));

            // calculate the sum for the current run
            acc_0 = __builtin_ia32_addps(acc_0,
                __builtin_ia32_mulps(oprand_a_1, oprand_b_1_0));
            acc_1 = __builtin_ia32_addps(acc_1, 
                __builtin_ia32_mulps(oprand_a_1, oprand_b_1_1));
          }
        }
            
        // calculate the sum for the last run
        if(!is_odd){
          acc_0 = __builtin_ia32_addps(acc_0,
            __builtin_ia32_mulps(oprand_a_0, oprand_b_0_0));
          acc_1 = __builtin_ia32_addps(acc_1, 
            __builtin_ia32_mulps(oprand_a_0, oprand_b_0_1));
        }else{
          acc_0 = __builtin_ia32_addps(acc_0,
            __builtin_ia32_mulps(oprand_a_1, oprand_b_1_0));
          acc_1 = __builtin_ia32_addps(acc_1, 
            __builtin_ia32_mulps(oprand_a_1, oprand_b_1_1));
        }

        __builtin_ia32_storeups(imd_ret_0, acc_0);
        __builtin_ia32_storeups(imd_ret_1, acc_1);

        for(cycleK = 1; cycleK < SSE_LENGTH; ++ cycleK){
          imd_ret_0[0] += imd_ret_0[cycleK];
          imd_ret_1[0] += imd_ret_1[cycleK];
        }
        matrixC.data[cycleI * dim + cycleJ] = imd_ret_0[0];
        matrixC.data[cycleI * dim + cycleJ + 1] = imd_ret_1[0];
      }
    }
  }
  
  SAFE_FREE(matrixBT.data);
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

  // init PAPI
  serial_event_set = PAPI_NULL;
  int ret = PAPI_create_eventset(&serial_event_set);
  if ( ret != PAPI_OK ){
    printf( "Error creating the event set\n" );
    exit(-1);
  }
  
  ret = PAPI_add_event(serial_event_set, PAPI_TOT_CYC);
  if ( ret != PAPI_OK ){
    printf( "Error on add_event\n" );
    exit(-1);
  }
  ret = PAPI_add_event(serial_event_set, PAPI_FP_OPS);
  if ( ret != PAPI_OK ){
    printf( "Error on add_event\n" );
    exit(-1);
  }

  PAPI_start(serial_event_set);
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
  PAPI_stop(serial_event_set, serial_values);
  PAPI_cleanup_eventset(serial_event_set);
  PAPI_destroy_eventset(&serial_event_set);
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
      if(matrixA.data[cycleI * matrixA.xDim + cycleJ] -
          matrixB.data[cycleI * matrixA.xDim + cycleJ] > CORRECT_THRESHOLD){
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

void report_result(){
  double cycles_p = ( double ) parallel_values[ 0 ];
  double time_p = cycles_p / 2.33e9;
  double flop_p = ( double ) parallel_values[ 1 ];
  double mflops_p = flop_p / time_p / 1.0e6;

  printf( "\nParallel Version:\n");
  printf( "Total Cycles (Millions) = %3.3f\n", cycles_p / 1.0e6 );
  printf( "Total Time (Seconds) = %3.3f\n", time_p );
  printf( "Total Flops (Millions) = %3.3f\n", flop_p / 1.0e6 );
  printf( "MFLOPS = %3.3f\n", mflops_p );
  
  double cycles_s = ( double ) serial_values[ 0 ];
  double time_s = cycles_s / 2.33e9;
  double flop_s = ( double ) serial_values[ 1 ];
  double mflops_s = flop_s / time_s / 1.0e6;

  printf( "\nSerial Version:\n");
  printf( "Total Cycles (Millions) = %3.3f\n", cycles_s / 1.0e6 );
  printf( "Total Time (Seconds) = %3.3f\n", time_s );
  printf( "Total Flops (Millions) = %3.3f\n", flop_s / 1.0e6 );
  printf( "MFLOPS = %3.3f\n", mflops_s );
  
  double speed_up = time_s / time_p;
  printf( "\nSpeed up = %3.3f\n", speed_up );
}
