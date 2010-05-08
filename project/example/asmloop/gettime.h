/*
	Primitives include file.


	Writen by Daniel Orozco
	03/11/2010

	Permission to use this file is given as long as this message is preserved along any use of it.

*/
#ifndef GETTIMEH
#define GETTIMEH
/**************************************************************************
				Included Files
***************************************************************************/


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

/*
	GetTime
		A function that returns the time using gettimeofday

	Parameters:
		None

	Return value:
		The number of seconds since a fixed (unknown) time in the past.
*/
double GetTime( void );

#endif
