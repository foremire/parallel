#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

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
void omp_matrix_mul(matrix matrixA, matrix matrixB, matrix matrixC);
void serial_matrix_mul(matrix matrixA, matrix matrixB, matrix matrixC);
void print_matrix(matrix * mat);
double get_duration(struct timeval __start);
void validate_result(matrix * matrixA, matrix * matrixB);

int main( int argc, char *argv[] )
{
  matrix matrixA;
  matrix matrixB;
  matrix matrixC;
  matrix matrixCValid;

  int matrix_size = 0;
  int thread_num = 1;

  struct timeval __start;
  double Ti = 0.0f;

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
  
  gettimeofday(&__start, NULL);
  Ti = get_duration(__start);

  init_matrix(&matrixA, matrix_size, matrix_size, 1);

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

void omp_matrix_mul(matrix matrixA, matrix matrixB, matrix matrixC){
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

void safe_exit(matrix ma, matrix mb, matrix mc, matrix me){
  SAFE_FREE(ma.data);
  SAFE_FREE(mb.data);
  SAFE_FREE(mc.data);
  SAFE_FREE(me.data);
}

void print_matrix(matrix * mat){
  int cycleI = 0;
  int cycleJ = 0;
  double val = 0.0;

  for(cycleI = 0; cycleI < mat->yDim; ++ cycleI){
    for(cycleJ = 0; cycleJ < mat->xDim; ++ cycleJ){
      val = mat->data[cycleI * mat->xDim + cycleJ];
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
