/*
	PAPI Simple Example
	
		This file demonstrates the use of the PAPI library
		
	Writen by Daniel Orozco on April 7th 2010
	
	Permission to copy this file is granted as long as you preserve this message
	with any copies of the file.
	
*/


/******************************************************************************
						Included Files
*******************************************************************************/

#include <stdio.h>

#include <stdlib.h>

#include "papi.h"

#include "fdtd2d.h"

#include "pthread.h"

/******************************************************************************
						Macros
*******************************************************************************/


#define ADDEVENT( x )	ret = PAPI_add_event( gEventSet, (x) ); \
	if ( ret != PAPI_OK ) {	printf( "Error on add_event\n" );	return( 1 );	}

/******************************************************************************
						Global Variables
*******************************************************************************/


/******************************************************************************
						Functions
*******************************************************************************/

/*
	main
	
		This is the main driver of the program. It writes to a loop and
		then presents results.
		
		The Papi library is used to measure a variety of properties in the
		program.
		
		Parameters and return values:
			As defined by ANSI C
*/

int main( int argc, char*argv[] )
{

	// ------------------- Local Variables ------------------------------------

		int i;
		double x;
		double y;
		long_long values[ 5 ];
		long_long tmpll;
		int ret;
		double cycles;
		double flop;
		double mflops;
		double time;
		long int event;
		char *pEnd; // Required by strtol
		
		
		
		
	// ------------------------ Run the program -------------------------------
	
	
		Init2D();
		
		//SerialFDTD2D();
		
		ParallelFDTD2D();


		

	// ------------------------ Return ---------------------------------------

		return ( 0 );
}


