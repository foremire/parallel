#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <omp.h>

// macro
#define OUTPUT_THRESHOLD 7
#define TRUE 1
#define FALSE 0

#define SAFE_FREE(addr)\
  if(NULL != addr){\
    free(addr);\
    addr = NULL;\
  }

//global varialbes and definitions
typedef struct _matrix{
  double * data;
  int xDim;
  int yDim;
}matrix;

// commen messages
char * usage = "Usage: mm P N\n";
char * thread_num_error = "Invalid thread number P = %d. It must be greater than 0.\n";
char * matrix_size_error = "Invalid matrix size N = %d. It must be greater than 1.\n";
char * matrix_size_info = "Matrix Size N = %d\n";
char * malloc_error = "malloc() ERROR.\n";

// function declaration
void init_matrix(matrix * mat, int xDim, int yDim, int random);
void omp_matrix_mul(matrix matrixA, matrix matrixB, matrix matrixC, int thread_num);
void serial_matrix_mul(matrix matrixA, matrix matrixB, matrix matrixC);
void print_matrix(matrix mat);
double get_duration(struct timeval __start);
void validate_result(matrix matrixA, matrix matrixB);

int main( int argc, char *argv[] )
{
  matrix matrixA;
  matrix matrixB;
  matrix matrixC;
  matrix matrixCValid;

  int matrix_size = 0;
  int thread_num = 1;

  struct timeval __start;
  double t_omp = 0.0f;
  double t_serial = 0.0f;

  if(argc < 3){
    puts(usage);
    exit(-1);
  }
  
  // get the matrix from the command line options list
  thread_num = atoi(argv[1]);
  if(0 > thread_num){
    printf(thread_num_error, thread_num);
    exit(-1);
  }

  matrix_size = atoi(argv[2]);
  if(1 > matrix_size){
    printf(matrix_size_error, matrix_size);
    exit(-1);
  }
 
  // make sure the thread_num is always less than or euqal to matrix_size
  if(thread_num > matrix_size){
    thread_num = matrix_size;
  }
  
  init_matrix(&matrixA, matrix_size, matrix_size, TRUE);
  init_matrix(&matrixB, matrix_size, matrix_size, TRUE);
  init_matrix(&matrixC, matrix_size, matrix_size, FALSE);
  init_matrix(&matrixCValid, matrix_size, matrix_size, FALSE);

  // do it in parallel way
  gettimeofday(&__start, NULL);
  omp_matrix_mul(matrixA, matrixB, matrixC, thread_num);
  t_omp = get_duration(__start);
  printf("omp time: %.6fs\n", t_omp);
 
  // do it in serial way
  gettimeofday(&__start, NULL);
  serial_matrix_mul(matrixA, matrixB, matrixCValid);
  t_serial = get_duration(__start);
  printf("serial time: %.6fs\n", t_serial);

  // validate the result
  validate_result(matrixC, matrixCValid);

  if(matrix_size < OUTPUT_THRESHOLD){
    puts("matrix A:\n");
    print_matrix(matrixA);
    
    puts("matrix B:\n");
    print_matrix(matrixB);
    
    puts("matrix C:\n");
    print_matrix(matrixC);
  }

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

void omp_matrix_mul(matrix matrixA, matrix matrixB, matrix matrixC, int thread_num){
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

  int num_per_thread = 0;
  int remainder = 0;
  int range_start = 0;
  int range_end = 0;
  int range_len = 0;
  int thread_id = 0;
  
  // calculate the sum in parallel
  omp_set_num_threads(thread_num);
#pragma omp parallel private (thread_id, num_per_thread, remainder, range_start, range_end, range_len, cycleI, cycleJ, cycleK) shared (matrixC)
  {
    // get the thread id
    thread_id = omp_get_thread_num();
    
    // Make sure the data is evenly distributed between the processes
    num_per_thread = dim / thread_num;
    remainder = dim % thread_num;
    if(0 == remainder){
      range_start = thread_id * num_per_thread;
    }else{
      range_start = thread_id * (num_per_thread + 1);
      range_start = (thread_id > remainder) ? range_start - thread_id + remainder 
        : range_start;
    }
    range_end = (thread_id > remainder - 1) ? range_start + num_per_thread
      : range_start + num_per_thread + 1;
    range_len = range_end - range_start;

    printf("thread_id: %d, range_start: %d, range_end: %d\n", 
        thread_id, range_start, range_end);

    for(cycleI = range_start; cycleI < range_end; ++ cycleI){
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


void serial_matrix_mul(matrix matrixA, matrix matrixB, matrix matrixC){
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

void print_matrix(matrix mat){
  int cycleI = 0;
  int cycleJ = 0;
  double val = 0.0;

  for(cycleI = 0; cycleI < mat.yDim; ++ cycleI){
    for(cycleJ = 0; cycleJ < mat.xDim; ++ cycleJ){
      val = mat.data[cycleI * mat.xDim + cycleJ];
      if(val >= 0.0){
        printf("+%f\t", val);
      }else{
        printf("%f\t", val);
      }
    }
    printf("\n");
  }
  fflush(stdout);
  usleep(1000);
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
