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
#include <papi.h>

/******************************************************************************
						Macros
*******************************************************************************/

#define SIZE 10000000

/******************************************************************************
						Global Variables
*******************************************************************************/

double a[ SIZE ];

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
		int EventSet;
		long_long values[ 2 ];
		int ret;
		double cycles;
		double flop;
		double mflops;
		double time;
		
		
	// ---------------------- Initialization ----------------------------------

		values[ 0 ] = 0;
		values[ 1 ] = 0;
		EventSet = PAPI_NULL;

		printf( "Initializing Papi\n" );

		ret = PAPI_library_init( PAPI_VER_CURRENT );
		if ( ret != PAPI_VER_CURRENT )
		{
			printf(" Failed on init. Returned Value = %d != %d\n", ret, PAPI_VER_CURRENT );
			return( 1 );
		}
		
	// -------------------- Create the PAPI Event Set ------------------------

		ret = PAPI_create_eventset( &EventSet );
		if ( ret != PAPI_OK )
		{
		printf( "Error creating the event set\n" );
		return( 1 );
		}
		
	// ---------------------- Add events to be measured -----------------------
	

		ret = PAPI_add_event( EventSet, PAPI_TOT_CYC );
		if ( ret != PAPI_OK )
		{
			printf( "Error on add_event\n" );
			return( 1 );
		}
		
		
		ret = PAPI_add_event( EventSet, PAPI_FP_OPS );
		if ( ret != PAPI_OK )
		{
			printf( "Error on add_event\n" );
			return( 1 );
		}
		
		
		
	// ----------------------- Start Counting ---------------------------------
	
		// Inform the user the program is about to run
		printf( "Now running the program...\n" );
		
		ret = PAPI_start( EventSet );
		if ( ret != PAPI_OK )
		{
		printf( "Error on start\n" );
		return( 0 );
		}
		
	// ------------------------ Run the program -------------------------------


		x = 0.3;
		y = 0.7;
		a[ 0 ] = 78;

		for ( i = 1; i < SIZE ; i++ )
		{
		a[ i ] = a[ i - 1 ] * 0.5 + x;
		x = x*y;
		}
		
	// ------------------------- Stop the Counters ----------------------------

		ret = PAPI_stop( EventSet, values );
		if ( ret != PAPI_OK )
		{
		printf( "Error on accum\n" );
		return( 0 );
		}
		
	// ------------------------- Present results ------------------------------


		cycles = ( double ) values[ 0 ];
		time = cycles / 2.33e9;
		flop = ( double ) values[ 1 ];
		mflops = flop / time / 1.0e6;
		

		printf( "Total Cycles (Millions) = %3.3f\n", cycles / 1.0e6 );
		printf( "Total Time (Seconds) = %3.3f\n", time );
		printf( "Total Flops (Millions) = %3.3f\n", flop / 1.0e6 );
		printf( "MFLOPS = %3.3f\n", mflops );
		
	// -------------------------- Cleanup of the PAPI library ------------------
	
		PAPI_cleanup_eventset( EventSet );
		PAPI_destroy_eventset( &EventSet );

	// ------------------------ Return ---------------------------------------

		return ( 0 );
}


