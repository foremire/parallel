#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

//global varialbes and definitions
#define OUTPUT_THRESHOLD 5

char * usage = "Usage: mm N\n";
char * matrix_size_error = "Invalid matrix size N = %d. It must be greater than 1.\n";
char * matrix_size_info = "Matrix Size N = %d\n";
char * malloc_error = "Malloc ERROR.\n";

int main( int argc, char *argv[] )
{
  int matrixSize = 0;
  int * matrixA = NULL;
  int * matrixB = NULL;
  int * matrixC = NULL;
  
  struct timeval tv;

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

  if(NULL == (matrixA = malloc( matrixSize * matrixSize * sizeof(int)))){
    puts(malloc_error);
    exit(-1);
  }
  if(NULL == (matrixB = malloc( matrixSize * matrixSize * sizeof(int)))){
    puts(malloc_error);
    free(matrixA);
    exit(-1);
  }
  if(NULL == (matrixC = malloc( matrixSize * matrixSize * sizeof(int)))){
    puts(malloc_error);
    free(matrixA);
    free(matrixB);
    exit(-1);
  }

  //initilize random generator
  gettimeofday(&tv, NULL);
  srand(tv.tv_sec * tv.tv_usec);

  free(matrixA);
  free(matrixB);
  free(matrixC);
  return 0;
}
