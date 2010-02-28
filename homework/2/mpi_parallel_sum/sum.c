#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

//global varialbes and definitions
#define COMM_TAG 16314
#define TOTAL_NUMBER 1000
#define MAX_NUM 100
#define BUFF_SIZE 16
char * msg = "Hi";
char * msg_template = "Process number %d writes number %d\n";

int main( int argc, char *argv[] )
{
  int myrank = 0;
  int numprocesses = 0;
  int source = 0;
  int dest = 0;
  int sum = 0;
  int partial_sum = 0;
  int cycle = 0;
  int remainder = 0;
  int num_per_process = 0;

  char buffer[BUFF_SIZE];
  int num_array [TOTAL_NUMBER];
 
  MPI_Status status;

  struct timeval tv;

  // Init MPI
  MPI_Init( &argc, &argv );
                
  // Get num processes
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  // Get num of processes
  MPI_Comm_size(MPI_COMM_WORLD, &numprocesses);
 
  // Get how many numbers should be processed per process
  num_per_process = TOTAL_NUMBER / numprocesses;
  remainder = TOTAL_NUMBER % numprocesses;

  // start message relay ring from process 0
  if(0 == myrank){
    //initilize random generator
    gettimeofday(&tv, NULL);
    srand(tv.tv_sec * tv.tv_usec);
    //printf("RAND_MAX: %d\n", RAND_MAX);

    //initialize the numbers array
    for(cycle = 0; cycle < TOTAL_NUMBER; ++ cycle){
      //printf("rand: %d\n", rand());
      num_array[cycle] = rand() % MAX_NUM;
      //num_array[cycle] = cycle;
      //printf("%d-%d\n", cycle, num_array[cycle]);
    }

    //Notify the other processes that the random numbers are ready
    for(cycle = 1; cycle < numprocesses; ++ cycle){
      dest = cycle;
      MPI_Send(num_array, TOTAL_NUMBER, MPI_INT, dest, 
          COMM_TAG, MPI_COMM_WORLD);
    }
    
    // calculate the sum of numbers on their part
    for(cycle = 0; cycle < num_per_process + remainder; ++ cycle){
      sum += num_array[cycle];
    }

    //collect the results from other processes
    for(cycle = 1; cycle < numprocesses; ++ cycle){
      source = cycle;
      // wait for the random numbers to be ready
      MPI_Recv(&partial_sum, 1, MPI_INT, source, COMM_TAG, 
        MPI_COMM_WORLD, &status);
      sum += partial_sum;
    }
    printf("parallel sum: %d\n", sum);

    // check with the serial sum
    sum = 0;
    for(cycle = 0; cycle < TOTAL_NUMBER; ++ cycle){
      sum += num_array[cycle];
    }
    printf("serial sum: %d\n", sum);

  }else{
    source = 0;
    // wait for the random numbers to be ready
    MPI_Recv(num_array, TOTAL_NUMBER, MPI_INT, source, COMM_TAG, 
        MPI_COMM_WORLD, &status);
    // calculate the sum of numbers on their part
    partial_sum = 0;
    for(cycle = myrank * num_per_process + remainder; 
        cycle < (myrank + 1) * num_per_process + remainder; ++ cycle){
      partial_sum += num_array[cycle];
    }
    // send the partial sum back to process 1
    dest = 0;
    MPI_Send(&partial_sum, 1, MPI_INT, dest, COMM_TAG, MPI_COMM_WORLD);
  }
  
  // Finalize MPI
  MPI_Finalize();
  return 0;
}
