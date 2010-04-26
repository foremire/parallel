/*

        Global Variable Definitions
        This file contains all external definitions used in the program.
        
        
        Written by Daniel Orozco
        May 14th 2009
        
*/

/*******************************************************************************
								Included Files
********************************************************************************/



/***************************************************************************
                                                Globals
***************************************************************************/

	
	double globalEz[ NSIZE ][ NSIZE ];
	double globalHx[ NSIZE ][ NSIZE ];
	double globalHy[ NSIZE ][ NSIZE ];
	
	double globalSigma[ NSIZE ][ NSIZE ];



	double delta_x;
	double delta_t;

    // Constants used to calculate sin( wt ) repeatedly
    double global_sin_dt;
    double global_cos_dt;

    double global_sin_wt;
    double global_cos_wt;

    // Timing information for the problem
    int gTimingPoints[ 2 ];



