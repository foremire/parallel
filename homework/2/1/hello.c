#include <mpi.h>
#include <stdio.h>

int main( int argc, char *argv[] )
{
  int myrank;
  int numprocesses;

  // InitMPI
  MPI_Init( &argc, &argv );

  // Get num processes
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  // Get num processes
  MPI_Comm_size(MPI_COMM_WORLD, 
		&numprocesses);

  // Print information
  printf( "Hello world from %d of %d\n", 
	  myrank, numprocesses );

  // Flush
  fflush( stdout );

  // Clean MPI
  MPI_Finalize();
  
  return( 0 );

}
