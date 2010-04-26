/*

        Global Variable Definitions
        This file contains all external definitions used in the program.
        
        
        Written by Daniel Orozco
        May 14th 2009
        
*/
#ifndef GLOBALDEFSH
#define GLOBALDEFSH
/***************************************************************************
                                                CONSTANTS
***************************************************************************/

// Problem specific constants
#define Eo                              (100)
#define FREQUENCY               (10e9)


// Physical constants
#define EPSILON0                (8.8542e-12)
#define MU0                     ( 4*(M_PI)*1e-7 )
#define LIGHTSPEED              (2.99792458e8)

#define MU_R                    1
#define EPSILON_R               1
#define SIGMA                   0

// computed spatial parameters
#define MU                              ( (MU_R) * (MU0) )
#define EPSILON                 ( (EPSILON0) * (EPSILON_R) )

//Wave characteristics
#define OMEGA                   ( 2 * (M_PI) * (FREQUENCY) )


/*******************************************************************************
								Included Files
********************************************************************************/




/***************************************************************************
                                                Macros
***************************************************************************/
	#define UPDATE_SINWT \
		{ \
			double sin_tmp; \
			sin_tmp = global_sin_wt * global_cos_dt + ( global_cos_wt ) * global_sin_dt; \
			global_cos_wt = global_cos_wt * global_cos_dt - ( global_sin_wt ) * global_sin_dt; \
			global_sin_wt = sin_tmp; \
		}

/*
	Inserted from the makefile
	
	#define NSIZE 300
	#define AMPLITUDE 100
	
	#define TIMESTEPS 200
*/

	#define GRIDSIZE 10

/***************************************************************************
                                                Globals
***************************************************************************/


	extern double globalEz[ NSIZE ][ NSIZE ];
	extern double globalHx[ NSIZE ][ NSIZE ];
	extern double globalHy[ NSIZE ][ NSIZE ];
	
	extern double globalSigma[ NSIZE ][ NSIZE ];

	
	extern double delta_x;
	extern double delta_t;


    // Constants used to calculate sin( wt ) repeatedly
    extern double global_sin_dt;
    extern double global_cos_dt;

    extern double global_sin_wt;
    extern double global_cos_wt;

    // Timing information for the problem
    extern int gTimingPoints[ 2 ];



		
#endif
