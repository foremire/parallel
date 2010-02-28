#include <mpi.h>
#include <stdio.h>

//global varialbes and definitions
#define COMM_TAG 16314
#define BUFF_SIZE 16
char * msg = "Hi";
char * msg_template = "Process number %d writes number %d";


int main( int argc, char *argv[] )
{
  int myrank = 0;
  int numprocesses = 0;
  int source = 0;
  int dest = 0;
  char buffer[BUFF_SIZE];

  // Init MPI
  MPI_Init( &argc, &argv );
                
  // Get num processes
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  // Get num of processes
  MPI_Comm_size(MPI_COMM_WORLD, &numprocesses);
  
  // start message relay ring from process 0
  if(0 == myrank){
    source = 0;
    dest = source + 1;
    MPI_Send(msg, strlen(msg) + 1, MPI_CHAR, dest, COMM_TAG, MPI_COMM_WORLD);
    printf(msg_template, myrank, myrank);
  }else{
    source = myrank - 1;
    dest = myrank + 1;
    MPI_Recv(buffer, BUFF_SIZE, MPI_CHAR, source, COMM_TAG, MPI_COMM_WORLD);
    printf(msg_template, myrank, myrank);

    //relay the message to the next receiver
    if(dest < numprocesses){
      MPI_Send(msg, strlen(msg) + 1, MPI_CHAR, dest, COMM_TAG, MPI_COMM_WORLD);
    }
  }
  
  // Finalize MPI
  MPI_Finalize();
  return 0;
}
