#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

//global varialbes and definitions
#define OUTPUT_THRESHOLD 5

char * usage = "Usage: mm N\n";

int main( int argc, char *argv[] )
{
  int matrixSize = 0;
  int * matrixA = NULL;
  int * matrixB = NULL;
  int * matrixC = NULL;

  if(argc < 2){
    puts(usage);
  }

  // get the matrix from the command line options list
  matrixSize = atoi(argv[1]);


  //initilize random generator
  gettimeofday(&tv, NULL);
  srand(tv.tv_sec * tv.tv_usec);
  num_array[cycle] = rand() % MAX_NUM;

  free(matrixA);
  free(matrixB);
  free(matrixC);
  return 0;
}
