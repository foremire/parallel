/*

        FDTD 2D
        This file contains the computational code for fdtd2d
        
        Written by Daniel Orozco
        May 14th 2009

	Created by Daniel Orozco
	CAPSL Research Group
	www.capsl.udel.edu
	May14th, 2009
	
	Permission to use this file for any reason is provided as long as
	this message is included.

        
*/

#ifndef FDTD2DH
#define FDTD2DH

/*******************************************************************************
								Included Files
********************************************************************************/

	#include "globaldefs.h"
	#include <stdio.h>
	#include "papi.h"

/*******************************************************************************
								Macros
********************************************************************************/

#define NUMTHREADS 8

/*******************************************************************************
								Type Definitions
********************************************************************************/

typedef struct thread_params_s
{
	int id;
	long_long values[ 5 ];
	int EventSet;
}
thread_params_t;

/*******************************************************************************
								Globals
********************************************************************************/


/*******************************************************************************
								Functions
********************************************************************************/


/*

	Init2D
	
		This function initializes all the arrays to zeros. Must be called
		before any computation is made.
	
	Parameters:
		None
	
	Return Values:
		None
		
*/

void Init2D( void );



/*
	SerialFDTD2D
		
		Computes the Fdtd in a naive way.
		
	Parameters:
	
		None
		
	Return Value
	
		None
		
*/

void SerialFDTD2D( void );




/*
	FDTD2D_v2
		
		Computes the Fdtd with pthreads
		To do so, papi_mem_info was used.
		L2 cache is 4046KB
		L1 cache is 32KB
		
	Parameters:
	
		None
		
	Return Value
	
		None
		
*/
void FDTD2D_v2( void );



/*
	ThreadWrapper
		This thread does the PAPI counting and the advertising of the papi 
		results.
		
	Parametes:
		The same as required by pthreads
	
	Return Values:
		As returned by PAPI
		
*/
void * ThreadWrapper( void * par );

/*

	fdtdv2 This function computes fdtd in a naive way using threads.
	
	The implementation deliberately uses a bad partitioning scheme.
	
	Parameters:
		threadid: The threadid
		
	Return values: None
	
*/
void  fdtdv2( int threadid );


/*

	fdtdv3 Tiled FDTD2D
	
	
	Parameters:
		threadid: The threadid
		
	Return values: None
	
*/
void  fdtdv3( int threadid );


/*

	fdtdv4 Tiled FDTD2D
	
	
	Parameters:
		threadid: The threadid
		
	Return values: None
	
*/
void fdtdv4( int threadid );


#endif
