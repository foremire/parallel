#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

#include "multiqueue.h"

int main( int argc, char *argv[] )
{
  int N = 0;
  int P = 0;

  // Check for correct number of arguments
  if ( argc < 3 )
  {
    printf( "Usage:\nexecutable N P\n" );
    return( 0 );
  }

  // Get parameters
  N = atoi( argv[ 1 ] );
  P = atoi( argv[ 2 ] );

  // Set the minimum to 1
  P = ( P < 1 ) ? 1 : P;
  N = ( N < 1 ) ? 1 : N;

  // Set a random seed
  srand( ( int ) GetTime() );

  // Print the program parameters.
  printf( "Running with P=%d N=%d\n", P, N );

  return( 0 );
}

double GetTime( void )
{
  // ------------------- Local Variables ------------------
  struct timeval tp;
  double localtime;

  // ------------------- Get Time -------------------------
  gettimeofday( &tp, NULL );
  localtime = ( double ) tp.tv_usec;
  localtime /= 1e6;
  localtime += ( double ) tp.tv_sec;

  // ------------------- Return Time ----------------------
  return( localtime );
}

