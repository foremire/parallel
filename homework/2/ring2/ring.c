#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

//global varialbes and definitions
#define COMM_TAG 16314
#define BUFF_SIZE 16
#define RELAY_CYCLE 3

char * msg = "Hi";
char * msg_template = "Process number %d writes number %d\n";

int main( int argc, char *argv[] )
{
  int myrank = 0;
  int numprocesses = 0;
  int source = 0;
  int dest = 0;
  int cycle = 0;
  char buffer[BUFF_SIZE];
  MPI_Status status;

  // Init MPI
  MPI_Init( &argc, &argv );
                
  // Get num processes
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  // Get num of processes
  MPI_Comm_size(MPI_COMM_WORLD, &numprocesses);
 
  for(cycle = 0; cycle < RELAY_CYCLE; ++cycle){
    // start message relay ring from process 0
    if(0 == myrank){
      //it this is not the first cycle
      if(0 < cycle){
        source = numprocesses - 1;
        // receive the message from the last process in the previous cycle
        dest = numprocesses - 1;
        MPI_Recv(buffer, BUFF_SIZE, MPI_CHAR, source, COMM_TAG, MPI_COMM_WORLD, &status);
      }
      source = myrank;
      dest = source + 1;
      MPI_Send(msg, strlen(msg) + 1, MPI_CHAR, dest, COMM_TAG, MPI_COMM_WORLD);
      printf(msg_template, myrank, myrank);
      fflush(stdout);
      usleep(1000);
    }else{
      // or, it's a intermediate relay
      source = myrank - 1;
      dest = myrank + 1;
      // receive the message from the previus process
      MPI_Recv(buffer, BUFF_SIZE, MPI_CHAR, source, COMM_TAG, MPI_COMM_WORLD, &status);
      printf(msg_template, myrank, myrank);
      fflush(stdout);

      //relay the message to the next receiver
      if(dest < numprocesses){
        usleep(1000);
        MPI_Send(msg, strlen(msg) + 1, MPI_CHAR, dest, COMM_TAG, MPI_COMM_WORLD);
      }else if(2 > cycle && dest == numprocesses){
        dest = 0;
        usleep(1000);
        MPI_Send(msg, strlen(msg) + 1, MPI_CHAR, dest, COMM_TAG, MPI_COMM_WORLD);
      }
    }
  }
  
  // Finalize MPI
  MPI_Finalize();
  return 0;
}
