/*****************************************************************************
						Included Files
******************************************************************************/

#include <mpi.h>
#include <stdio.h>

// Required for strlen
#include <string.h>

// Required for usleep
#include <unistd.h>

/*****************************************************************************
						Functions
******************************************************************************/

int main( int argc, char *argv[] )
{
	// -------------------------- Local Variables ---------------------------

		int myrank;
		int numprocesses;
		int dest;
		int tag;
		int source;
		char buffer[ 15 ];
		MPI_Status status;

	//--------------------------- Initialization ---------------------------


		// InitMPI
		MPI_Init( &argc, &argv );

		// Get num processes
		MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

		// Get num processes
		MPI_Comm_size(MPI_COMM_WORLD, 
		&numprocesses);

	// ------------------------ Send and receive -----------------------------


		if ( myrank == 0 )
		{
			// Set the source of messages and the destination as 1
			// All communication is done with process 1
			source = 1;
			dest = 1;
			// The tag is chosen as 1023 arbitrarily. It could also be 0
			tag = 1023;

			MPI_Send( "hello", strlen( "hello" )+1,
			MPI_CHAR, dest, tag, MPI_COMM_WORLD );

			MPI_Recv( buffer, 15, MPI_CHAR, 
			source, tag, MPI_COMM_WORLD,
			&status );

			printf( "Received from %d: %s\n", source, buffer );

			// Flush and wait
			fflush( stdout );
			usleep( 500000 );

		}
		else
		{

			source = 0;
			dest = 0;
			tag = 1023;

			MPI_Recv( buffer, 15, MPI_CHAR, 
			source, tag, MPI_COMM_WORLD,
			&status );

			printf( "Received from %d: %s\n", source, buffer );

			// Flush and wait
			fflush( stdout );
			usleep( 500000 );


			MPI_Send( "Bye", strlen( "Bye" )+1,
			MPI_CHAR, dest, tag, MPI_COMM_WORLD );

		}


	// ------------------------------- Cleanup ------------------------------

		// Clean MPI
		MPI_Finalize();

	// -------------------------------- Return ------------------------------

		return( 0 );

}
