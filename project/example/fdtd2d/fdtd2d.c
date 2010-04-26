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

/*******************************************************************************
								Included Files
********************************************************************************/

	#include <stdlib.h>
	#include "globaldefs.h"
	#include <math.h>
	#include "fdtd2d.h"
	#include "pthread.h"
	#include "papi.h"
	
	
	// Required for timing
	#include <sys/time.h>


/*******************************************************************************
								Macros
********************************************************************************/

/*******************************************************************************
								Globals
********************************************************************************/

pthread_barrier_t gBarrier;

/*******************************************************************************
								Functions
********************************************************************************/


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

	Init2D
	
		This function initializes all the arrays to zeros. Must be called
		before any computation is made.
	
	Parameters:
		None
	
	Return Values:
		None
		
*/

void Init2D( void )
{
	// ------------------------ Local Variables -------------------------------
	
		int i; 			// For the loops
		int j; 			// For the loops
		int t; 			// For the loops
		
		// Constants:
		double beta0; 	
		double lambda;
		double Rb;
		double x;
		double sigma;
		
	 // --------------- Initialization of values -----------------

        // Calculation of constants

        beta0 = OMEGA * sqrt( EPSILON0 * MU0 );
        lambda = 2 * M_PI / beta0;
        delta_x = lambda / 20;
        delta_t = delta_x / LIGHTSPEED / 5;
        Rb = delta_t / ( MU *delta_x );


        global_sin_dt = sin( OMEGA * delta_t );
        global_cos_dt = cos( OMEGA * delta_t );

		global_sin_wt = 0;
		global_cos_wt = 1;
		
	
	// ------------------------ Initialization --------------------------------
	
		for ( i = 0; i < NSIZE; i++ )
		{
			for ( j = 0; j < NSIZE; j++ )
			{

				x = i - NSIZE / 2;
				x /= NSIZE / 2;
			
				sigma = x * x;
				sigma = sigma * sigma;
				
				
				// Init material properties
				globalSigma[ i ][ j ] = sigma;
				
				// Init Field components
				globalEz[ i ][ j ] = 0.0;
				globalHx[ i ][ j ] = 0.0;
				globalHy[ i ][ j ] = 0.0;
				
				
		    }

		}
		

	// ------------------------ Return ----------------------------------------
	
		return;
}

/*
	SerialFDTD2D
		
		Computes the Fdtd in a naive way.
		
	Parameters:
	
		None
		
	Return Value
	
		None
		
*/

void SerialFDTD2D( void )
{
	// ---------------------------- Local Variables ----------------------------
	
		double dHydx;
		double dHxdy;
		double dEzdx;
		double dEzdy;
		
		double inv_dx;		// Inverse of dx;
		double dtepsilon;	// dt over epsilon;
		double deltamu;		// dt over mu;
		
		int i, j, t;		// For the loops
		
		
	
	// ---------------------------- Initialization -----------------------------
	
		// Restore Sin( wt ) to its original value
		global_sin_wt = 0;
		global_cos_wt = 1;
		
		inv_dx = 1.0 / delta_x;
		dtepsilon = delta_t / EPSILON0;
		deltamu = delta_t / MU0;
		
		
		
	// --------------------------- Loops ---------------------------------------
	
		for ( t = 0; t < TIMESTEPS; t++ )
		{
		
			// Ez update
			for ( i = 1; i < NSIZE - 1; i++ )
			{
				for ( j = 1; j < NSIZE -1; j++ )
				{
					dHydx = - ( globalHy[ i - 1 ][ j ] - globalHy[ i ][ j ] ) * inv_dx;
					dHxdy = - ( globalHx[ i ][ j - 1 ] - globalHx[ i ][ j ] ) * inv_dx;
					globalEz[ i ][ j ] += dtepsilon * 
						( dHydx - dHxdy - globalSigma[ i ][ j ] * globalEz[ i ][ j ] );
						
					
				}
			}
			
			// Introduce source
			

			globalEz[ NSIZE / 2 ][ NSIZE / 2 ] = AMPLITUDE * global_sin_wt;
	
			UPDATE_SINWT

			
			// Hx, Hy
			for ( i = 1; i < NSIZE - 1; i++ )
			{
				for ( j = 1; j < NSIZE - 1; j++ )
				{
					dEzdx = ( globalEz[ i + 1 ][ j ] - globalEz[ i ][ j ] ) * inv_dx;
					dEzdy = ( globalEz[ i ][ j + 1 ] - globalEz[ i ][ j ] ) * inv_dx;
					
					globalHx[ i ][ j ] += deltamu * ( -dEzdy );
					globalHy[ i ][ j ] += deltamu * ( dEzdx );
					
				}
			}
			
			
			
		}
		
	// --------------------------- Print a sample value for testing purposes ----------------
	
		printf( "For testing purposes: %e\n", globalEz[ NSIZE / 2 - 10 ][ NSIZE / 2 + 17 ] );
	
	// --------------------------- Return --------------------------------------
		
		return;
	
}

/*
	ParallelFDTD2D
		
		Computes the Fdtd with pthreads
		To do so, papi_mem_info was used.
		L2 cache is 4046KB: 
		L1 cache is 32KB
		
	Parameters:
	
		None
		
	Return Value
	
		None
		
*/
void ParallelFDTD2D( void )
{
	// ---------------------------- Local Variables ----------------------------
	
		pthread_t threads[NUMTHREADS];
		thread_params_t arguments[ NUMTHREADS ];
		void *retvalue;
		int i;
		double AccumulatedCounter;
		double StartTime;
		double EndTime;
		
		
		
	// ---------------------------- Initialization -----------------------------
	
		pthread_barrier_init( &gBarrier, NULL, NUMTHREADS );
		
		// Init arguments		
		for ( i = 0; i < NUMTHREADS; i++ )
		{
			arguments[ i ].id = i;
		}
		
		// Start Counting time
		StartTime = GetTime();
		
		for ( i = 0; i < NUMTHREADS - 1; i++ )
		{
			pthread_create( &threads[ i ], // the pointer
				NULL, // Attributes
				ThreadWrapper, // function
				( void * ) &arguments[ i ] // The arguments
				);
		}
		
	// --------------------- Locally execute thread -----------------------
	
		ThreadWrapper( ( void * ) &arguments[ NUMTHREADS - 1 ] );
		
	// ---------------------------- Join -----------------------------
	
		for ( i = 0; i < NUMTHREADS - 1; i++ )
		{
			pthread_join( threads[ i ], // the pointer
				&retvalue // Return value
				);
		}
		
		// Record Time
		EndTime = GetTime();
		
	// ----------------------------- Destroy Barrier ---------------------
	
		pthread_barrier_destroy( &gBarrier );
		
	// ------------------------------ Collect the result --------------------------
	
		AccumulatedCounter = 0.0;
		for ( i = 0; i < NUMTHREADS; i++ )
		{
			AccumulatedCounter += ( double ) arguments[ i ].values[ 0 ];
		}
		
		printf( "PAPI Accumulated counter:\n" );
		printf( "XxX %3.4e\n", AccumulatedCounter );
		printf( "Elapsed Time:\n" );
		printf( "TtT %3.4f\n", EndTime - StartTime );
		printf( "For testing purposes: %e\n", globalEz[ NSIZE / 2 - 10 ][ NSIZE / 2 + 17 ] );
		
	// ------------------------------ Return ------------------------------
	
		return;		
		
	
}

/*
	ThreadWrapper
		This thread does the PAPI counting and the advertising of the papi 
		results.
		
	Parametes:
		The same as required by pthreads
	
	Return Values:
		As returned by PAPI
		
*/
void * ThreadWrapper( void * par )
{
	// -------------------------- Local Variables ----------------------------
	
		PAPI_thread_id_t papithreads[ 10 ];
		int numthreads;
		int ret;
		thread_params_t *parameters;
		int tid;
		int i;
	
	// -------------------------- Initialization ----------------------------
	
		parameters = ( thread_params_t * ) par;
		tid = parameters->id;
	
		// printf( "Thread %d created!\n",  tid ); fflush( stdout );
		
	// ----------------------Papi  Initialization ----------------------------------

		for ( i = 0; i < 5; i++ )
		{
			parameters->values[ i ] = 0;
		}
		
		parameters->EventSet = PAPI_NULL;

		// printf( "Thread %d: Initializing Papi\n", tid );
		
		if ( tid == 0 )
		{

			ret = PAPI_library_init( PAPI_VER_CURRENT );
			if ( ret != PAPI_VER_CURRENT )
			{
				printf("Thread %d: Failed on init. Returned Value = %d != %d\n",
					tid, ret, PAPI_VER_CURRENT );
				exit( 1 );
			}
		}
		
		pthread_barrier_wait( &gBarrier );
		
		ret = PAPI_thread_init( pthread_self );
		if ( ret != PAPI_OK )
		{
			printf( "Error on PAPI_thread_init\n" );
			exit( 1 );
		}
		
		/*
		ret = PAPI_register_thread( );
		if ( ret != PAPI_OK )
		{
			printf( "Error on PAPI_register_thread\n" );
		}
		*/
		
		pthread_barrier_wait( &gBarrier );
		
		numthreads = 10;
		PAPI_list_threads( papithreads, &numthreads );
		if ( ret != PAPI_OK )
		{
			printf( "T %d: Error on PAPI_list_threads\n", tid );
			exit( 1 );
		}
		
		/*
		printf( "Thread %d reports a total of %d threads\n",
			tid,
			 numthreads ); 
			 
		fflush( stdout );
		*/
		
	// -------------------- Create the PAPI Event Set ------------------------

		ret = PAPI_create_eventset( &parameters->EventSet );
		if ( ret != PAPI_OK )
		{
			printf( "T %d: Error creating the event set. Error = %d\n", tid, ret );
			exit( 1 );
		}
		
		ret = PAPI_add_event( parameters->EventSet, (PAPI_TOT_CYC) ); 
		
		if ( ret != PAPI_OK ) 
		{	
			printf( "Thread %d: Error on add_event\n", tid );	
			exit( 1 );
		}		
		
	// --------------------- Start the Counters ----------------------------------
	
		ret = PAPI_start( parameters->EventSet );
		if ( ret != PAPI_OK )
		{
			printf( "Error %d on start papi from Thread %d\n", ret, tid );
			fflush( stdout );
		}
		
		
	
	// -------------------------- Call the function -------------------------
	
		pthread_barrier_wait( &gBarrier );
	
		fdtdv4( parameters->id );
	
	// --------------------------- Collect the PAPI counter -----------------
	
		ret = PAPI_stop( parameters->EventSet, parameters->values );
		if ( ret != PAPI_OK )
		{
			printf( "Error %d on collect PAPI_stop from thread %d\n", ret, tid );
			fflush( stdout );
			exit( 1 );
		}
		
		pthread_barrier_wait( &gBarrier );
		
		
		
		// Advertise it
		/*
		printf( "Thread %d gets PAPI counter = %3.4e\n", tid, ( double ) parameters->values[ 0 ] );
		fflush( stdout );
		*/
	
	// --------------------------- return -----------------------------------
	
		return( NULL );
	
}


/*

	fdtdv2 This function computes fdtd in a naive way using threads.
	
	The implementation deliberately uses a bad partitioning scheme.
	
	Parameters:
		threadid: The threadid
		
	Return values: None
	
*/
void fdtdv2( int threadid )
{
	// ---------------------------- Local Variables ----------------------------
	
		double dHydx;
		double dHxdy;
		double dEzdx;
		double dEzdy;
		
		double inv_dx;		// Inverse of dx;
		double dtepsilon;	// dt over epsilon;
		double deltamu;		// dt over mu;
		
		int i, j, t;		// For the loops
		
		
		
	
	// ---------------------------- Initialization -----------------------------
	
		// Restore Sin( wt ) to its original value
		if ( threadid == 0 )
		{
			global_sin_wt = 0;
			global_cos_wt = 1;
		}
		
		inv_dx = 1.0 / delta_x;
		dtepsilon = delta_t / EPSILON0;
		deltamu = delta_t / MU0;
		
		//printf( "Thread %d created!\n", threadid );
		
	// --------------------------- Loops ---------------------------------------
	
		for ( t = 0; t < TIMESTEPS; t++ )
		{
		
			// Ez update
			//for ( i = 1; i < NSIZE - 1; i++ )
		  for ( i = 1 + threadid; i < NSIZE- 1; i+= NUMTHREADS )
			{
				for ( j = 1; j < NSIZE -1; j++ )
				{
					dHydx = - ( globalHy[ i - 1 ][ j ] - globalHy[ i ][ j ] ) * inv_dx;
					dHxdy = - ( globalHx[ i ][ j - 1 ] - globalHx[ i ][ j ] ) * inv_dx;
					globalEz[ i ][ j ] += dtepsilon * 
						( dHydx - dHxdy - globalSigma[ i ][ j ] * globalEz[ i ][ j ] );
						
					
				}
			}
			
			
			// Introduce source
			if ( threadid == 0 )
			{
				globalEz[ NSIZE / 2 ][ NSIZE / 2 ] = AMPLITUDE * global_sin_wt;
	
				UPDATE_SINWT
			}
			
			pthread_barrier_wait( &gBarrier );

			
			// Hx, Hy
			//for ( i = 1; i < NSIZE - 1; i++ )
			for ( i = 1 + threadid; i < NSIZE- 1; i+= NUMTHREADS )
			{
				for ( j = 1; j < NSIZE - 1; j++ )
				  //for ( j = 1 + threadid; j < NSIZE -1; j += NUMTHREADS )
				{
					dEzdx = ( globalEz[ i + 1 ][ j ] - globalEz[ i ][ j ] ) * inv_dx;
					dEzdy = ( globalEz[ i ][ j + 1 ] - globalEz[ i ][ j ] ) * inv_dx;
					
					globalHx[ i ][ j ] += deltamu * ( -dEzdy );
					globalHy[ i ][ j ] += deltamu * ( dEzdx );
					
				}
			}
			pthread_barrier_wait( &gBarrier );
			
			
			
		}
	
	// --------------------------- Return --------------------------------------
		
		return;
	
}


/*

	fdtdv3 Partitioned FDTD2D
	
	
	Parameters:
		threadid: The threadid
		
	Return values: None
	
*/
void fdtdv3( int threadid )
{
	// ---------------------------- Local Variables ----------------------------
	
		double dHydx;
		double dHxdy;
		double dEzdx;
		double dEzdy;
		
		double inv_dx;		// Inverse of dx;
		double dtepsilon;	// dt over epsilon;
		double deltamu;		// dt over mu;
		
		int i, j, t;		// For the loops
		int tidx;
		int tidy;
		int startx;
		int starty;
		int endx;
		int endy;
		int sizex;
		int sizey;
		
		
		
	// ---------------------------- Initialization -----------------------------
	
		// Restore Sin( wt ) to its original value
		// Partition in blocks of 2 by 4;
		if ( threadid == 0 )
		{
			global_sin_wt = 0;
			global_cos_wt = 1;
		}
		
		tidx = threadid % 4;
		tidy = threadid / 4;
		
		// Partition the problem
		sizex = ( NSIZE / 4  );
		sizey = ( NSIZE / 2  );
		
	// ------------------------ Compute the regions for each thread -----------
		
		startx = sizex * tidx;
		starty = sizey * tidy;
		
		endx = startx + sizex;
		endy = starty + sizey;
		
	// ---------------- Check the ranges ----------------------------------
		
		startx = ( startx <= 0 ) ? 1 : startx;
		endx = ( endx > NSIZE - 1 ) ? NSIZE - 1 : endx;

		starty = ( starty <= 0 ) ? 1 : starty;
		endy = ( endy > NSIZE - 1 ) ? NSIZE - 1 : endy;
		
		inv_dx = 1.0 / delta_x;
		dtepsilon = delta_t / EPSILON0;
		deltamu = delta_t / MU0;
		
		/*
		printf( "T%d [%d - %d][%d - %d]\n", threadid,
			startx, endx, starty, endy );
		*/
	// --------------------------- Loops ---------------------------------------
	
		for ( t = 0; t < TIMESTEPS; t++ )
		{
		
			// Ez update
			for ( i = startx; i < endx; i++ )
			{
				for ( j = starty; j < endy; j++ )
				//for ( j = 1 + threadid; j < NSIZE -1; j += NUMTHREADS )
				{
					dHydx = - ( globalHy[ i - 1 ][ j ] - globalHy[ i ][ j ] ) * inv_dx;
					dHxdy = - ( globalHx[ i ][ j - 1 ] - globalHx[ i ][ j ] ) * inv_dx;
					globalEz[ i ][ j ] += dtepsilon * 
						( dHydx - dHxdy - globalSigma[ i ][ j ] * globalEz[ i ][ j ] );
						
					
				}
			}
			
			
			// Introduce source
			if ( 
				( startx <= NSIZE/2 ) && ( endx > NSIZE / 2 ) && 
				( starty <= NSIZE/2 ) && ( endy > NSIZE / 2 ) 
				)
			{
				globalEz[ NSIZE / 2 ][ NSIZE / 2 ] = AMPLITUDE * global_sin_wt;
	
				UPDATE_SINWT
			}
			
			pthread_barrier_wait( &gBarrier );

			
			// Hx, Hy
			for ( i = startx; i < endx; i++ )
			{
				for ( j = starty; j < endy; j++ )
				{
					dEzdx = ( globalEz[ i + 1 ][ j ] - globalEz[ i ][ j ] ) * inv_dx;
					dEzdy = ( globalEz[ i ][ j + 1 ] - globalEz[ i ][ j ] ) * inv_dx;
					
					globalHx[ i ][ j ] += deltamu * ( -dEzdy );
					globalHy[ i ][ j ] += deltamu * ( dEzdx );
					
				}
			}
			pthread_barrier_wait( &gBarrier );
			
			
			
		}
	
	// --------------------------- Return --------------------------------------
		
		return;
	
}

/*

	fdtdv4 Tiled FDTD2D
	
	
	Parameters:
		threadid: The threadid
		
	Return values: None
	
*/
void fdtdv4( int threadid )
{
	// ---------------------------- Local Variables ----------------------------
	
		double dHydx;
		double dHxdy;
		double dEzdx;
		double dEzdy;
		
		double inv_dx;		// Inverse of dx;
		double dtepsilon;	// dt over epsilon;
		double deltamu;		// dt over mu;
		
		int i, j, t;		// For the loops
		int tidx;
		int tidy;
		int startx;
		int starty;
		int endx;
		int endy;
		int sizex;
		int sizey;
		int blocksize;
		int numblocksx;
		int numblocksy;
		int blockidx;
		int blockidy;
		
		
		
	// ---------------------------- Initialization -----------------------------
	
		// Restore Sin( wt ) to its original value
		// Partition in blocks of 2 by 4;
		if ( threadid == 0 )
		{
			global_sin_wt = 0;
			global_cos_wt = 1;
		}
		
		tidx = threadid % 4;
		tidy = threadid / 4;
		
		// Partition the problem
		sizex = ( NSIZE / 4  );
		sizey = ( NSIZE / 2  );
		
	// ------------------------ Compute the regions for each thread -----------
		
		startx = sizex * tidx;
		starty = sizey * tidy;
		
		endx = startx + sizex;
		endy = starty + sizey;
		
	// ---------------- Check the ranges ----------------------------------

		// The initial boundary is disabled to achieve good performance by having
		// aligned blocks.
		//startx = ( startx <= 0 ) ? 1 : startx;
		//starty = ( starty <= 0 ) ? 1 : starty;

		endy = ( endy > NSIZE - 1 ) ? NSIZE - 1 : endy;
		endx = ( endx > NSIZE - 1 ) ? NSIZE - 1 : endx;
		
		inv_dx = 1.0 / delta_x;
		dtepsilon = delta_t / EPSILON0;
		deltamu = delta_t / MU0;
		
		/*
		printf( "T%d [%d - %d][%d - %d]\n", threadid,
			startx, endx, starty, endy );
		*/
		
	// --------------------------- Blocking ---------------------------------------
	/*
		Blocking is another name for tiling.
		
		There are 4096K bytes of cache in L2
		Among 8 processors: 512KB for each.
		
		Three arrays are being read and written:
		512K/3 = 170K
		
		Double numbers take 8 Bytes:21250 numbers approx.
		and the size of a local block:
		145 x 145 double numbers or less.
		
		The cache line is 64 bytes long, so the cache should be a multiple of 8
		and there has to be memory left for other tasks such as the stack and others.
		
		So we use blocks of 128, which is a multiple of 8
		
		The blocks must be aligned so the start is not shifted yet.
		
		The blocks start at startx and end at endx.
		
		
	*/
		blocksize = 32;
		//blocksize = 128;
		numblocksx = ( endx - startx + ( blocksize - 1 ) ) / blocksize;
		numblocksy = ( endy - starty + ( blocksize - 1 ) ) / blocksize;
		
		
	// --------------------------- Loops ---------------------------------------
	
		for ( t = 0; t < TIMESTEPS; t++ )
		{
		
			for ( blockidx = 0; blockidx < numblocksx; blockidx++ )
			{
				for ( blockidy = 0; blockidy < numblocksy; blockidy++ )
				{
					int istart;
					int jstart;
					
					int iend;
					int jend;
					
					istart = blockidx * blocksize + startx;
					jstart = blockidy * blocksize + starty;
					
					iend = istart + blocksize;
					jend = jstart + blocksize;
					
					
					// Fix the boundaries
					istart = ( istart <= 0 ) ? 1 : istart;
					jstart = ( jstart <= 0 ) ? 1 : jstart;
					
					iend = ( iend > endx ) ? endx : iend;
					jend = ( jend > endy ) ? endy : jend;
					
					
					
		
					// Ez update
					for ( i = istart; i < iend; i++ )
					{
						for ( j = jstart; j < jend; j++ )
						{
							dHydx = - ( globalHy[ i - 1 ][ j ] - globalHy[ i ][ j ] ) * inv_dx;
							dHxdy = - ( globalHx[ i ][ j - 1 ] - globalHx[ i ][ j ] ) * inv_dx;
							globalEz[ i ][ j ] += dtepsilon * 
								( dHydx - dHxdy - globalSigma[ i ][ j ] * globalEz[ i ][ j ] );
						
					
						}
					}
					
				}
			}
			
			
			// Introduce source
			if ( 
				( startx <= NSIZE/2 ) && ( endx > NSIZE / 2 ) && 
				( starty <= NSIZE/2 ) && ( endy > NSIZE / 2 ) 
				)
			{
				globalEz[ NSIZE / 2 ][ NSIZE / 2 ] = AMPLITUDE * global_sin_wt;
	
				UPDATE_SINWT
			}
			
			pthread_barrier_wait( &gBarrier );

			
			// Hx, Hy
			for ( blockidx = 0; blockidx < numblocksx; blockidx++ )
			{
				for ( blockidy = 0; blockidy < numblocksy; blockidy++ )
				{
					int istart;
					int jstart;
					
					int iend;
					int jend;
					
					istart = blockidx * blocksize + startx;
					jstart = blockidy * blocksize + starty;
					
					iend = istart + blocksize;
					jend = jstart + blocksize;
					
					
					// Fix the boundaries
					istart = ( istart <= 0 ) ? 1 : istart;
					jstart = ( jstart <= 0 ) ? 1 : jstart;
					
					iend = ( iend > endx ) ? endx : iend;
					jend = ( jend > endy ) ? endy : jend;
					
					
					
		
					// Ez update
					for ( i = istart; i < iend; i++ )
					{
						for ( j = jstart; j < jend; j++ )
						{
							dEzdx = ( globalEz[ i + 1 ][ j ] - globalEz[ i ][ j ] ) * inv_dx;
							dEzdy = ( globalEz[ i ][ j + 1 ] - globalEz[ i ][ j ] ) * inv_dx;
					
							globalHx[ i ][ j ] += deltamu * ( -dEzdy );
							globalHy[ i ][ j ] += deltamu * ( dEzdx );
					
						}
					}
				}
			}
			
			pthread_barrier_wait( &gBarrier );
			
			
			
		}
	
	// --------------------------- Return --------------------------------------
		
		return;
	
}

