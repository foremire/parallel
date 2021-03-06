/*
	Assembly simple example
	
		This file demonstrates the use of Assembly Operations
		
	Writen by Daniel Orozco on April 7th 2010
	
	Permission to copy this file is granted as long as you preserve this message
	with any copies of the file.
	
*/


/******************************************************************************
						Included Files
*******************************************************************************/

#include <stdio.h>

#include "gettime.h"

/******************************************************************************
						Macros
*******************************************************************************/

#define SIZE 100
#define TEST_LOOP 1

/******************************************************************************
						Global Variables
*******************************************************************************/

double a[ SIZE ];
double b[ SIZE ];

/******************************************************************************
						Functions
*******************************************************************************/

void init_a( double *a, int N );
int asmloop( double *a, int N, int t, double *constants );	
int cloop( double *a, int N, int t );	

/******************************************************************************
						Functions
*******************************************************************************/


/*
	comparearrays
		This functions compares the accuracy of two computations.
		
	Parameters: None. 
		This function uses global variables.
	Return value:
		Zero when equal, 1 when not.

*/
int comparearrays( void )
{
	// ----------------------- Local Variables -------------------
	
		double sum;
		double difference;
		int i;
		
	// ----------------------- Initialization ------------------
	
		sum = 0;
		
	// ------------------------- Compare Arrays ------------------
	
		for ( i = 0; i < SIZE; i++ )
		{
			difference = a[ i ] - b[ i ];
			sum += difference * difference;
		}
		
		if ( sum > 0.001 )
		{
			printf( "Error: Difference is %lf\n",  sum );
			return( 1 );
		}
	
	// -------------------------- Return -------------------------
		return( 0 );
}
/*

	print_array prints the values of a
	
	Parameters:
		None. Everything is in global variables
	
	Return values: None
	

*/
void print_array( double * a )
{
	// ------------------------------ Local Variables --------------------------
		
		int i;
		
	// ---------------------------- Initialization -----------------------------
	
	// ---------------------------- Print a -----------------------------------
	
		for ( i = 0; i < SIZE; i++ )
		{
			printf( "%3.3f\t", (float) a[ i ] );
		}
		printf( "\n" );
	// ----------------------------- Return -------------------------------------
	
		return;
}

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

		double k[3];
		int r;
		int i;
		int smallsize;
		double dtmp;
		double startime;
		double totaltime;
		
		

	// ------------------- Init ---------------------------------
		
		k[ 0 ] = 1.0;
		k[ 1 ] = 0.43;
		k[ 2 ] = 8.5;
		smallsize = 1;

	// ------------------- Check for correctness ---------------------------------
	
	
		printf( "Now Checking for correctness\n" );
		init_a( a, SIZE );
		init_a( b, SIZE );
		print_array(a);
		
		printf( "First testing loop\n" );

		printf( "Now executing the C loop\n" );
		cloop( a, SIZE, smallsize );
		print_array(a);

		printf( "Now executing the Assembly loop\n" );
		init_a( a, SIZE );
		r = asmloop( a, SIZE, smallsize, k );
		print_array(a);
		
		
		//printf( "Return value = %d\t = 0x%X\n", r, r );
		//printf( "a = 0x%lX\n", ( unsigned long int ) &a[0] );
		
		printf( "Correctness tests\n" );
		for ( i = 1; i < 100000; i = i * 2 )
		{
			init_a( a, SIZE );
			init_a( b, SIZE );
			
			// Run both to compare results
			cloop( a, SIZE, i );
		        //print_array(a);
		
                        asmloop( b, SIZE, i, k );
		        //print_array(b);
			
			// Now compare the results.
			if ( comparearrays() )
			{
				printf( "-" );
                                return 0;
			}
			else
			{
				printf( "+" );
			}
		
		}
		
		printf( "\n" );
		
		printf( "Performance tests\n" );
		
		totaltime = 0;
		for ( i = 0; i < TEST_LOOP; i++ )
		{
			init_a( b, SIZE );
			startime = GetTime();
			asmloop( b, SIZE, 3000000, k );
			totaltime += GetTime() - startime;
		}
		printf( "ASM Average time: %4.3f seconds\n", ( float ) totaltime / (float)TEST_LOOP );
		
                totaltime = 0;
		for ( i = 0; i < TEST_LOOP; i++ )
		{
			init_a( b, SIZE );
			startime = GetTime();
			cloop( b, SIZE, 3000000);
			totaltime += GetTime() - startime;
		}
		printf( "C Average time: %4.3f seconds\n", ( float ) totaltime / (float)TEST_LOOP );
		
		
	
	// -------------------- Return --------------------------------------------
	
		return( 0 );

}
