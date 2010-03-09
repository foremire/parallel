#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

//global varialbes and definitions
#define OUTPUT_THRESHOLD 6

char * usage = "Usage: mm N\n";
char * matrix_size_error = "Invalid matrix size N = %d. It must be greater than 1.\n";
char * matrix_size_info = "Matrix Size N = %d\n";
char * malloc_error = "Malloc ERROR.\n";

void print_matrix(double * matrix, int matrixSize);

int main( int argc, char *argv[] )
{
  int matrixSize = 0;
  double * matrixA = NULL;
  double * matrixB = NULL;
  double * matrixC = NULL;
  
  struct timeval tv;

  int cycleI = 0;
  int cycleJ = 0;
  int cycleK = 0;

  if(argc < 2){
    puts(usage);
    exit(-1);
  }

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

  // initilize the matrix by random numbers between -1.00 and 1.00
  for(cycleI = 0; cycleI < matrixSize * matrixSize; ++ cycleI){
    matrixA[cycleI] = (double)rand() / ((double)(RAND_MAX)+ 1.00f) * 2.0f - 1.0f;
    matrixB[cycleI] = (double)rand() / ((double)(RAND_MAX)+ 1.00f) * 2.0f - 1.0f;
    matrixC[cycleI] = (double)rand() / ((double)(RAND_MAX)+ 1.00f) * 2.0f - 1.0f;
  }

  // do the calculation
  
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
