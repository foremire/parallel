#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <mpi.h>

//global varialbes and definitions
#define OUTPUT_THRESHOLD 6
#define BUFF_SIZE 16
#define TOTAL_NUMBER 1000

char * usage = "Usage: mm N\n";
char * matrix_size_error = "Invalid matrix size N = %d. It must be greater than 1.\n";
char * matrix_size_info = "Matrix Size N = %d\n";
char * malloc_error = "Malloc ERROR.\n";

char * msg = "Hi";
char * msg_template = "Process number %d writes number %d\n";

void print_matrix(double * matrix, int matrixSize);

int main( int argc, char *argv[] )
{
  int matrixSize = 0;
  double * matrixA = NULL;
  double * matrixB = NULL;
  double * matrixC = NULL;
  
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
 
  // Get how many numbers should be processed per process
  num_per_process = TOTAL_NUMBER / numprocesses;
  remainder = TOTAL_NUMBER % numprocesses;

  // get the matrix from the command line options list
  matrixSize = atoi(argv[1]);
  if(1 > matrixSize){
    printf(matrix_size_error, matrixSize);
    exit(-1);
  }
  printf(matrix_size_info, matrixSize);

  if(NULL == (matrixA = malloc( matrixSize * matrixSize * sizeof(double)))){
    puts(malloc_error);
    exit(-1);
  }
  if(NULL == (matrixB = malloc( matrixSize * matrixSize * sizeof(double)))){
    puts(malloc_error);
    free(matrixA);
    exit(-1);
  }
  if(NULL == (matrixC = malloc( matrixSize * matrixSize * sizeof(double)))){
    puts(malloc_error);
    free(matrixA);
    free(matrixB);
    exit(-1);
  }

  //initilize random generator
  gettimeofday(&tv, NULL);
  srand(tv.tv_sec * tv.tv_usec);

  gettimeofday(&__start, NULL);
  // initilize the matrix by random numbers between -1.00f and 1.00f
  for(cycleI = 0; cycleI < matrixSize * matrixSize; ++ cycleI){
    matrixA[cycleI] = (double)rand() / ((double)(RAND_MAX)+ 1.00) * 2.0 - 1.0;
    matrixB[cycleI] = (double)rand() / ((double)(RAND_MAX)+ 1.00) * 2.0 - 1.0;
  }
  gettimeofday(&__end, NULL);
  t_end = (__end.tv_sec + (__end.tv_usec/1000000.0));
  t_start = (__start.tv_sec + (__start.tv_usec/1000000.0));
  Ti = t_end - t_start;

  gettimeofday(&__start, NULL);
  // do the matrix multiplication
  for(cycleI = 0; cycleI < matrixSize; ++ cycleI){
    for(cycleJ = 0; cycleJ < matrixSize; ++ cycleJ){
      matrixC[cycleI * matrixSize + cycleJ] = 0.0f;
      for(cycleK = 0; cycleK < matrixSize; ++ cycleK){
        matrixC[cycleI * matrixSize + cycleJ] += 
          matrixA[cycleI * matrixSize + cycleK] *
          matrixB[cycleK * matrixSize + cycleJ];
      }
    }
  }
  gettimeofday(&__end, NULL);
  t_end = (__end.tv_sec + (__end.tv_usec/1000000.0));
  t_start = (__start.tv_sec + (__start.tv_usec/1000000.0));
  Tc = t_end - t_start;

  Tt = Ti + Tc;
  
  // if the matrix size is below the threshold, output the result
  if(matrixSize < OUTPUT_THRESHOLD){
    printf("MatrixA:\n");
    print_matrix(matrixA, matrixSize);
    printf("MatrixB:\n");
    print_matrix(matrixB, matrixSize);
    printf("MatrixC:\n");
    print_matrix(matrixC, matrixSize);
  }

  free(matrixA);
  free(matrixB);
  free(matrixC);
  // Finalize MPI
  MPI_Finalize();

  printf("Ti: %fs\n", Ti);
  printf("Tc: %fs\n", Tc);
  printf("Tt: %fs\n", Tt);
  return 0;
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
