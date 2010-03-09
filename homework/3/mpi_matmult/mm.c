#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <mpi.h>

//global varialbes and definitions
#define OUTPUT_THRESHOLD 6
#define BUFF_SIZE 16

#define SAVE_FREE(addr)\
  if(NULL != addr){\
    free(addr);\
    addr = NULL;\
  }

typedef struct _matrix{
  double * data;
  int xDim;
  int yDim;
  int start;
  int end;
}matrix;

char * usage = "Usage: mm N\n";
char * matrix_size_error = "Invalid matrix size N = %d. It must be greater than 1.\n";
char * matrix_size_info = "Matrix Size N = %d\n";
char * malloc_error = "Malloc ERROR.\n";

char * msg = "Hi";
char * msg_template = "Process number %d writes number %d\n";
char * msg_no_task = "Process %d exit without no task.\n";

void print_matrix(double * matrix, int matrixSize);

int main( int argc, char *argv[] )
{
  int matrixSize = 0;
  double * matrixA = NULL;
  double * matrixB = NULL;
  double * matrixC = NULL;
  double * matrixExchange = NULL;
  
  struct timeval tv;
  
  struct timeval __start;
  struct timeval  __end;
  double t_start = 0.0f;
  double t_end = 0.0f;
  double Ti = 0.0f;
  double Tc = 0.0f;
  double Tt = 0.0f;

  int cycleI = 0;
  int cycleJ = 0;
  int cycleK = 0;
  int remainder = 0;
  int num_per_process = 0;
  int range_start = 0;
  int range_end = 0;
  int range_len = 0;

  int myrank = 0;
  int numprocesses = 0;
  int source = 0;
  int dest = 0;
  MPI_Status status;
  
  char buffer[BUFF_SIZE];

  if(argc < 2){
    puts(usage);
    exit(-1);
  }
  
  // Init MPI
  MPI_Init( &argc, &argv );
                
  // Get num processes
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  // Get num of processes
  MPI_Comm_size(MPI_COMM_WORLD, &numprocesses);

  // get the matrix from the command line options list
  matrixSize = atoi(argv[1]);
  if(1 > matrixSize){
    printf(matrix_size_error, matrixSize);
    MPI_Finalize();
    exit(-1);
  }
  if(0 == myrank){
    printf(matrix_size_info, matrixSize);
  }
  
  // Make sure the data is evenly distributed between the processes
  num_per_process = matrixSize / numprocesses;
  remainder = matrixSize % numprocesses;
  if(0 == remainder){
    range_start = myrank * num_per_process;
  }else{
    range_start = myrank * (num_per_process + 1);
    range_start = (myrank > remainder) ? range_start - 1 : range_start;
  }
  range_end = (myrank > remainder - 1) ? range_start + num_per_process
    : range_start + num_per_process + 1;
  range_len = range_end - range_start;

  //printf("Rank: %d\t Start: %d\t End: %d\n", myrank, range_start, range_end);
  //fflush(stdout);
  //usleep(1000);
  //exit(0);

  // These is no task for the current process, exit gracefully
  if(0 == range_len){
    printf(msg_no_task, myrank);
    MPI_Finalize();
    exit(0);
  }
  matrixA = malloc( matrixSize * range_len * sizeof(double));
  matrixB = malloc( matrixSize * range_len * sizeof(double));
  matrixC = malloc( matrixSize * range_len * sizeof(double));
  matrixExchange = malloc( matrixSize * (num_per_process + 1) * sizeof(double));

  if(!(matrixA && matrixB && matrixC && matrixExchange)){
    puts(malloc_error);
    SAVE_FREE(matrixA);
    SAVE_FREE(matrixB);
    SAVE_FREE(matrixC);
    SAVE_FREE(matrixExchange);
    MPI_Finalize();
    exit(-1);
  }

  memset(matrixA, 0, matrixSize * range_len * sizeof(double));
  memset(matrixB, 0, matrixSize * range_len * sizeof(double));
  memset(matrixC, 0, matrixSize * range_len * sizeof(double));
  memset(matrixExchange, 0, matrixSize * (num_per_process + 1) * sizeof(double));

  //initilize random generator
  gettimeofday(&tv, NULL);
  srand(tv.tv_sec * tv.tv_usec);

  gettimeofday(&__start, NULL);
  // initilize the matrix by random numbers between -1.00f and 1.00f
  for(cycleI = 0; cycleI < range_len; ++ cycleI){
    for(cycleJ = 0; cycleJ < matrixSize; ++ cycleJ){
      matrixA[cycleI * matrixSize + cycleJ] = 
        (double)rand() / ((double)(RAND_MAX)+ 1.00) * 2.0 - 1.0;
      matrixB[cycleI * matrixSize + cycleJ] = 
        (double)rand() / ((double)(RAND_MAX)+ 1.00) * 2.0 - 1.0;
    }
  }
  gettimeofday(&__end, NULL);
  t_end = (__end.tv_sec + (__end.tv_usec/1000000.0));
  t_start = (__start.tv_sec + (__start.tv_usec/1000000.0));
  Ti = t_end - t_start;

  gettimeofday(&__start, NULL);
  // do the matrix multiplication

  gettimeofday(&__end, NULL);
  t_end = (__end.tv_sec + (__end.tv_usec/1000000.0));
  t_start = (__start.tv_sec + (__start.tv_usec/1000000.0));
  Tc = t_end - t_start;

  Tt = Ti + Tc;
  
  // if the matrix size is below the threshold, output the result
  if(matrixSize < OUTPUT_THRESHOLD){
    //printf("MatrixA:\n");
    //print_matrix(matrixA, matrixSize);
    //printf("MatrixB:\n");
    //print_matrix(matrixB, matrixSize);
    //printf("MatrixC:\n");
    //print_matrix(matrixC, matrixSize);
  }

  //printf("Ti: %fs\n", Ti);
  //printf("Tc: %fs\n", Tc);
  //printf("Tt: %fs\n", Tt);

  SAVE_FREE(matrixA);
  SAVE_FREE(matrixB);
  SAVE_FREE(matrixC);
  SAVE_FREE(matrixExchange);

  // Finalize MPI
  MPI_Finalize();
  return 0;
}

void matrix_mul(matrix matrixA, matrix matrixB, matrix matrixC){
  int cycleI = 0;
  int cycleJ = 0;
  int cycleK = 0;
  int dim = 0;

  // check whether the dimension of the three matrices match 
  // with each other to conduct the following multiplication
  if(matrixA.xDim != matrixB.yDim){
    printf("matrix A and matrix B dimension does not match\n");
    return;
  }
  dim = matrixA.xDim;

  if((matrixA.xDim != matrixC.xDim) || (matrixA.yDim != matrixC.yDim)){
    printf("matrix A and matrix C dimension does not match\n");
    return;
  }

  for(cycleI = 0; cycleI < matrixA.yDim; ++ cycleI){
    for(cycleJ = 0; cycleJ < matrixB.xDim; ++ cycleJ){
      matrixC.data[cycleI * dim + cycleJ + matrixC.start] = 0.0;
      for(cycleK = 0; cycleK < dim; ++ cycleK){
        matrixC.data[cycleI * dim + cycleJ + matrixC.start] += 
          matrixA.data[cycleI * dim + cycleK] *
          matrixB.data[cycleJ * dim + cycleK];
      }
    }
  }
}

void init_matrix(matrix mat, double * data, int xDim, int yDim){

}

void print_matrix(double * matrix, int matrixSize){
  int cycleI = 0;
  int cycleJ = 0;

  for(cycleI = 0; cycleI < matrixSize; ++ cycleI){
    for(cycleJ = 0; cycleJ < matrixSize; ++ cycleJ){
      if(matrix[cycleI * matrixSize + cycleJ] > 0.0f){
        printf("+%f\t", matrix[cycleI * matrixSize + cycleJ]);
      }else{
        printf("%f\t", matrix[cycleI * matrixSize + cycleJ]);
      }
    }
    printf("\n");
  }
}
