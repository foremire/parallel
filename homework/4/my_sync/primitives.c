/*
	Primitives test file.

	This file tests for the performance and reliability of synchronization operations.

	Writen by Daniel Orozco
	03/11/2010

	Permission to use this file is given as long as this message is preserved along any use of it.

*/

/**************************************************************************
				Included Files
***************************************************************************/

// Required for printf
#include <stdio.h>

// Required for pthreads
#include <pthread.h>

// Required for usleep
#include <unistd.h>

// Required for atoi and rand
#include <stdlib.h>

// Required for timing
#include <sys/time.h>

// Prototypes for this function
#include "primitives.h"

//Synchronization primitives
#include "my_sync.h"

/**************************************************************************
				Macros
***************************************************************************/

/**************************************************************************
				Type Definitions
***************************************************************************/


/**************************************************************************
				Global Variables
***************************************************************************/

// The synchronization variable
my_mutex_t gMutex;
my_barrier_t gBarrier;

// Timing and correctness information
double gElapsedTime;
int gCorrectTest;

// gN is a global variable with the number of times each test is done.
int gN;

// gP is a global variable with the number of processors to try.
int gP;


// A test variable is used.
int gTestVariable;

// And a global variable is used for a delay.
int gCounter;

// And also, for correctness, a permanent zero
volatile int gZero = 0;

/**************************************************************************
				Function Prototypes
***************************************************************************/


/**************************************************************************
				Functions
***************************************************************************/


/*
  main
    This is the driver for the application. Main uses the standard
    ansi C parameters

  Parameters from the command line:
    P number of processors to use.
    N number of writes to perform
  Return Value
    Zero
*/
int main( int argc, char *argv[] )
{
  // --------------------- Local Variables ------------------------
  int N;
  int P;

  // --------------------- Initialization -------------------------

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

  // Set as global variables.
  gP = P;
  gN = N;

  // Avoid optimizations with gZero
  gZero = 1;
  // Set a random seed
  srand( ( int ) GetTime() );
  gZero = 0;

  // Print the program parameters.
  printf( "Running with P=%d N=%d\n", gP, gN );

  // --------------------- Tests -------------------------

  // Lock Correctness
  RunTest( BarrierCorrectness, "Barrier Correctness" );

  // Lock Performance
  RunTest( BarrierPerformance, "Barrier Performance" );

  // Lock Correctness
  RunTest( LockCorrectness, "Lock Correctness" );

  // Lock Performance
  RunTest( LockPerformance, "Lock Performance" );

  // --------------------- Return ------------------------
  return( 0 );
}

/*
  RunTest
    This function runs a particular test for a number of times and reports the result.

  Parameters
    func        A function with the test
    str		A string with the test name

  Return Value
    None. This function prints the result

*/
void RunTest( void * ( * func)( void * ), char * str )
{
  // ------------------ Local Variables --------------------
  int testnum;
  int passedtests;
  double timesum;
  pthread_t *threads;
  ThreadParameters_t *threadparameters;

  // ------------------- Initialization -------------------
  passedtests = 0;
  timesum = 0;
  printf( "Now running [%s]\n", str );

  // ------------------- Run Tests ------------------------
  for ( testnum = 0; testnum < REPEATTESTS; testnum++ )
  {
    my_mutex_init( &gMutex );
    my_barrier_init( &gBarrier, gP );

    CreateThreads( &threads, gP, func, &threadparameters );
    JoinThreads( &threads, gP, &threadparameters );

    my_mutex_destroy( &gMutex );
    my_barrier_destroy( &gBarrier );

    timesum += gElapsedTime;
    if ( gCorrectTest )
    {
      passedtests++;
      printf( "+" ); 
      fflush( stdout );
    }
    else
    {
      printf( "-" );
      fflush( stdout );
    }
  }

  timesum /= REPEATTESTS;

  // ------------------- Print Outcome ---------------------
  printf( "\nTests passed: %d of %d. Average Time = %f (sec)\n", 
      passedtests, REPEATTESTS, timesum );
}

/*
  CreateThreads
    This function does all the thread creation.

  Parameters
    ppThreads: A pointer to a pointer to a pthread_t array.
    P: Number of threads
    func: The function to execute
    ppParameters: A pointer to a null pointer of type ThreadParameters_t

*/
void CreateThreads( pthread_t **ppThreads, int P, 
    void * ( * func)( void * ), ThreadParameters_t **ppParameters )
{
  // ------------------- Local Variables ---------------------
  int i;
  ThreadParameters_t *parameters;
  pthread_t *threads;

  // ------------------- Initialization ---------------------
  threads = *ppThreads = ( pthread_t * ) malloc( sizeof( pthread_t ) * P );
  parameters = *ppParameters = 
    ( ThreadParameters_t * ) malloc( sizeof( ThreadParameters_t ) * P );

  // Initialize the parameters
  for ( i = 0; i < P; i++ )
  {
    parameters[ i ].id = i;
  }


  // ------------------- Create Threads ---------------------

  // Create threads
  for ( i = 0; i < P-1; i++ )
  {
    pthread_create( &threads[ i ],
    NULL,	// Default attibutes
    func,	///void *(*start_routine)(void *),
    &parameters[ i ]);
  }

  // Timing: To help other threads complete creation, the
  // master thread will sleep.
  usleep( 150000 );

  // ------------------- Set the timer ----------------------

  // Start Counting Time
  gElapsedTime = GetTime();

  // ------------------- Use Current Thread -----------------
  func( &parameters[ P - 1 ] );
		
  // ------------------- Return -----------------------------
  return;
}

/*
  JoinThreads
    This function joins all threads spawned and measures termination time.

  Parameters
    ppThreads: A pointer to a pointer to a pthread_t array.
    P: Number of threads
    ppParameters: A pointer to a pointer of type ThreadParameters_t

  Return Value:
    None

*/
void JoinThreads( pthread_t **ppThreads, int P, ThreadParameters_t **ppParameters )
{
  // ------------------------ Local Variables ---------------------
  int i;
  ThreadParameters_t *parameters;
  pthread_t *threads;
  void *retvalue;

  // ------------------- Initialization ---------------------
  parameters = *ppParameters;
  threads = *ppThreads;

  // ------------------ Get Timing ------------------------
  // Tests start and end with a barrier, so taking
  // the time before join is not an error.
  gElapsedTime = GetTime() - gElapsedTime;

  // ------------------ Join Threads ------------------------

  gCorrectTest = 1;

  for ( i = 0; i < P-1; i++ )
  {
    pthread_join( threads[ i ], &retvalue );
    if ( retvalue == NULL )
    {
      gCorrectTest = 0;
    }
  }

  // ----------------- Free Memory -------------------------
  free( parameters );
  free( threads );
  *ppThreads = NULL;
  *ppParameters = NULL;

  // ----------------- Return -------------------------------
  return;
}

/*
  GetTime
    A function that returns the time using gettimeofday

  Parameters:
    None

  Return value:
    The number of seconds since a fixed (unknown) time in the past.
*/
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

/*
  LockPerformance:
    Tests the performance of a single lock, with no writings or readings.

  Parameters:

   parameters: A structure of type ThreadParameters_t with parameters

  Return Value:
    This test always succeeds. It always returns 1.

*/
void * LockPerformance( void *parameters )
{
  // ----------------------- Local Variables ------------------
  int i;
  int localN; // On the stack for performance.
  int j;
  ThreadParameters_t *par;

  // ---------------------- Initialization -------------------------
  par = ( ThreadParameters_t * ) parameters;
  localN = gN*15;

  my_barrier_wait( &gBarrier );

  // ---------------------- Run Tests -------------------------
  for ( j = 0; j < 10; j++ )
  {
    my_barrier_wait( &gBarrier );

    for ( i = 0; i < localN; i++ )
    {
      my_mutex_lock( &gMutex );
      my_mutex_unlock( &gMutex );
    }
  }

  // ---------------------- Barrier wait -------------------------
  my_barrier_wait( &gBarrier );

  // ----------------------- Always return success ---------------------
  return( ( void * ) 1 );
}

/*
  LockCorrectness:
    Tests correct implementation of the lock

  Parameters:
    parameters: A structure of type ThreadParameters_t with parameters

  Return Value:

*/
void * LockCorrectness( void *parameters )
{
	
  // ----------------------- Local Variables ------------------
  int i;
  int localN; // On the stack for performance.
  int tmp;
  int j;
  int expectedresult;
  int delaysteps;
		
  ThreadParameters_t *par;


  // ---------------------- Initialization -------------------------
  par = ( ThreadParameters_t * ) parameters;
  localN = gN*30;
  gTestVariable = 0;
  gCounter = 0;

  my_barrier_wait( &gBarrier );

  // ---------------------- Run Tests -------------------------
  for ( i = 0; i < localN; i++ )
  {
    my_mutex_lock( &gMutex );
    tmp = gTestVariable;
    delaysteps = rand() % DELAYNUM;
    for ( j = 0; j < delaysteps; j++ )
    {
      tmp += gZero;
    }
    tmp++;
    gTestVariable = tmp;
    my_mutex_unlock( &gMutex );
  }

  // ---------------------- Barrier wait -------------------------
  my_barrier_wait( &gBarrier );

  // ----------------------- Check for sucess ---------------------
  expectedresult = localN * gP;

  // validate the result
  if ( gTestVariable != expectedresult )
  {
    return( NULL );
  }

  return( ( void * ) 1 );
}

/*
  BarrierCorrectness:
    Tests correct implementation of barrier

  Parameters:
    parameters: A structure of type ThreadParameters_t with parameters

  Return Value:

*/
void * BarrierCorrectness( void *parameters )
{
	
  // ----------------------- Local Variables ------------------

  int i;
  int localN; // On the stack for performance.
  int tmp;
  int j;
  int delaysteps;
  int goodbarrier;
		
  ThreadParameters_t *par;


  // ---------------------- Initialization -------------------------

  par = ( ThreadParameters_t * ) parameters;
  localN = gN*6;
  gTestVariable = 0;
  gCounter = 0;
  goodbarrier = 1;

  my_barrier_wait( &gBarrier );

  // ---------------------- Run Tests -------------------------
  for ( i = 0; i < localN; i++ )
  {
    // Read a shared variable
    tmp = 0;
    delaysteps = rand() % DELAYNUM;
    for ( j = 0; j < delaysteps; j++ )
    {
      tmp += gZero;
    }
    tmp++;

    // Synchronize: Everybody reads the same variable
    my_barrier_wait( &gBarrier );

    // Atomically increment the variable after a random delay...
    delaysteps = rand() % DELAYNUM;
    for ( j = 0; j < delaysteps; j++ )
    {
      tmp += gZero;
    }
  
    // Atomically add the value to the test variable
    __sync_fetch_and_add( &gTestVariable, tmp );

    // Check for successful add from all threads.
    my_barrier_wait( &gBarrier );

    if ( gTestVariable != ( ( i + 1 ) * gP ) )
    {
      goodbarrier = 0;
    }

  }

  // ---------------------- Barrier wait -------------------------
  my_barrier_wait( &gBarrier );

  // ----------------------- Check for sucess ---------------------
  if ( goodbarrier != 1 )
  {
    return( NULL );
  }

  return( ( void * ) 1 );
}

/*
  BarrierPerformance:
    Tests the performance of a single barrier, with no writings or readings.

  Parameters:
    parameters: A structure of type ThreadParameters_t with parameters

  Return Value:
    This test always succeeds. It always returns 1.

*/
void * BarrierPerformance( void *parameters )
{
  // ----------------------- Local Variables ------------------

  int localN; // On the stack for performance.
  int i;
  ThreadParameters_t *par;

  // ---------------------- Initialization -------------------------
  par = ( ThreadParameters_t * ) parameters;
  localN = gN*10;
  my_barrier_wait( &gBarrier );

  // ---------------------- Run Tests -------------------------
  for ( i = 0; i < localN; i++ )
  {
    my_barrier_wait( &gBarrier );
  }

  // ---------------------- Barrier wait -------------------------
  my_barrier_wait( &gBarrier );

  // ----------------------- Always return success ---------------------
  return( ( void * ) 1 );
}
