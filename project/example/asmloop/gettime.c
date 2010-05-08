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


// Required for timing
#include <sys/time.h>


/**************************************************************************
				Macros
***************************************************************************/

/**************************************************************************
				Type Definitions
***************************************************************************/


/**************************************************************************
				Global Variables
***************************************************************************/


/**************************************************************************
				Function Prototypes
***************************************************************************/


/**************************************************************************
				Functions
***************************************************************************/


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

