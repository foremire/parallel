/*
	Primitives include file.


	Writen by Daniel Orozco
	03/11/2010

	Permission to use this file is given as long as this message is preserved along any use of it.

*/
#ifndef PRIMITIVESH
#define PRIMITIVESH
/**************************************************************************
				Included Files
***************************************************************************/


/**************************************************************************
				Macros
***************************************************************************/

#define REPEATTESTS 10
#define DELAYNUM 1000

/**************************************************************************
				Type Definitions
***************************************************************************/

typedef struct sthreadparameters
{
	int id;
} ThreadParameters_t;

/**************************************************************************
				Global Variables
***************************************************************************/


/**************************************************************************
				Function Prototypes
***************************************************************************/



/*
	RunTest

		This function runs a particular test for a number of times and reports the result.

	Parameters
		func	A function with the test
		str		A string with the test name

	Return Value
		None. This function prints the result

*/
void RunTest( void * ( * func)( void * ), char * str );


/*
	CreateThreads

		This function does all the thread creation.

	Parameters

		ppThreads: A pointer to a pointer to a pthread_t array.
		P: Number of threads
		func: The function to execute
		ppParameters: A pointer to a null pointer of type ThreadParameters_t

*/
void CreateThreads( pthread_t **ppThreads, int P, void * ( * func)( void * ), ThreadParameters_t **ppParameters );

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
void JoinThreads( pthread_t **ppThreads, int P, ThreadParameters_t **ppParameters );

/*
	GetTime
		A function that returns the time using gettimeofday

	Parameters:
		None

	Return value:
		The number of seconds since a fixed (unknown) time in the past.
*/
double GetTime( void );

/*
	LockPerformance:
		Tests the performance of a single lock, with no writings or readings.

	Parameters:

		parameters:
			A structure of type ThreadParameters_t with parameters

	Return Value:
		This test always succeeds. It always returns 1.

*/
void * LockPerformance( void *parameters );


/*
	LockCorrectness:
		Tests correct implementation of the lock

	Parameters:

		parameters:
			A structure of type ThreadParameters_t with parameters

	Return Value:
		

*/
void * LockCorrectness( void *parameters );


/*
	BarrierCorrectness:
		Tests correct implementation of barrier

	Parameters:

		parameters:
			A structure of type ThreadParameters_t with parameters

	Return Value:
		

*/
void * BarrierCorrectness( void *parameters );


/*
	BarrierPerformance:
		Tests the performance of a single barrier, with no writings or readings.

	Parameters:

		parameters:
			A structure of type ThreadParameters_t with parameters

	Return Value:
		This test always succeeds. It always returns 1.

*/
void * BarrierPerformance( void *parameters );

#endif