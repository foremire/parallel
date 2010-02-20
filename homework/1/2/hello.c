/*
	Hello world program
	
		This is one of the traditional simple programs written in C
		
	
	Written by Daniel Orozco
	orozco@capsl.udel.edu
	January 28th, 2010
	
	Permission to use this file is granted as long as this message is 
	preserved along with any use of the file.

*/

/******************************************************************************
							Included Files
*******************************************************************************/							

	// Stdio is required to use the printf function
	#include <stdio.h>
	

/******************************************************************************
							Functions
*******************************************************************************/							

/*

	main
	
		This function prints the message "Hello World" and returns.

	Parameters:
	
		Although the traditional command line input is supported for this program,
		it is not used.
		
	Return Value:
	
		Zero (integer)
*/
int main( int argc, char *argv[] )
{
	
	// ----------------------------- Local Variables -------------------------
	
	// ----------------------------- Print Message ---------------------------
	
		printf( "Hello World!\n" );
	
	// ------------------------------ Return ---------------------------------
	
		return( 0 );
}
