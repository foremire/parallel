/*
	Assembly simple loop
	
		This file demonstrates the use of Assembly Operations
		
	Writen by Daniel Orozco on May 4th 2010
	
	Permission to copy this file is granted as long as you preserve this message
	with any copies of the file.
	
*/


/******************************************************************************
						Included Files
*******************************************************************************/


/******************************************************************************
						Macros
*******************************************************************************/


/******************************************************************************
						Global Variables
*******************************************************************************/


/******************************************************************************
						Functions
*******************************************************************************/


/*
	init_a sets the values of a to zeros.
	
	Parameters:
		a: a pointer to an array of double numbers.
		N: The size of a.
		
	Return values
		None
		
*/


void init_a( double *a, int N )
{
	// ------------------- Local Variables ------------------------------------
	
		int j;
	
	// -------------------- The loop ------------------------------------------
	
		for ( j = 1; j < N-1; j++ )
		{
			a[ j ] = 0;
		}
	
	
	// -------------------- Return --------------------------------------------
	
		return;

}


/*
	cloop: This loop computes a simple loop that uses very little memory.
	This loop is designed to use little memory.
	
	Parameters:
		a: a pointer to an array of double numbers.
		N: The size of a.
		t: The number of times the operation is done.
		
	Return values
		This function returns zero.
		
*/


int cloop( double *a, int N, int t )
{

	// ------------------- Local Variables ------------------------------------
	
		int i;
		int j;
	
	// -------------------- The loop ------------------------------------------
	
		for ( i = 0; i < t; i++ )
		{
			for ( j = 1; j < N-1; j++ )
			{
				a[ j ] = ( a[ j - 1 ] + a[ j ] + j ) * 0.43;
			}
		}
	
	
	// -------------------- Return --------------------------------------------
	
		return( 0 );

}

/*
	asmloop: This loop computes a simple loop that uses very little memory.
	This loop is designed to use little memory.
	The only difference is that this loop is written in assembly.
	
	Parameters:
		a: a pointer to an array of double numbers.
		N: The size of a.
		t: The number of times the operation is done.
		
	Return values
		This function returns zero.
		
*/

/*
int asmloop( double *a, int N, int t )
{
	// ------------------------- Local Variables --------------------

	// ------------------------- Return value ----------------------

		// 43 is shown to see how return values are sent.
		return( 43 );
}
*/

