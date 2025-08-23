#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include <argp.h>
#include "main.h"
#include "arralloc.h"
#include "ranlux.h"

#include "argp.c"

int main(int argc, char * argv[])
{
    /*****************          VARIABLES DEFINITION         *****************/
    int    i, j, outer;            //Index i, j & outer
    int    Nx, Ny;                 //The boundary of landscape
    int    loop = 1;               //Loop times of simulation
    FILE * fp;                     //FILE pointer
    char * landscapeFile;          //Char arrays for file names
    double initial_time, end_time; //The start & end time of simulation

    /* 2D arrays declared as double pointers
     * This is for the memory allocation
     */
    int ** landscape;

    /* H is the 2d array for the densities of the hare population
     * P is the same for the pumas
     * This tempH & tempP are used to receive the new densities when moving
     */
    double ** H, ** P, ** tempH, ** tempP;

    //The user must be able to set all the parameters in the equations.
    simulation_parameters parameters = {
                                         {HARE, 0.8, 0.4, 0.2},
                                         {PUMA, 0.2, 0.6, 0.2},
                                         0.4,
                                         0
                                       };

    args arguments; //Used to communicate with parse_opt

    /*****************  LANDSCAPE AND INPUTS INITIALIZATION  *****************/

    /***** Part 1: Initialise parameters and time step *****/
    // Set argument defaults
    arguments.config_file = NULL;
    arguments.verbose = 0;
    arguments.output_interval = -1;
    arguments.output_files = 1;
    arguments.output_stdout = 1;

    // Parse command line arguments using argp
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    // Read parameters from the config file
    value_input(arguments, &parameters);

    /***** Part 2: Initial landscape & Memory allocation *****/
    landscapeFile = arguments.args[0];
    fp = fopen(landscapeFile, "r");
    if (fp == NULL)
    {
        printf("Cannot open file %s.\n", landscapeFile);
        exit(0);
    }
    // Read the column and row
    fscanf(fp, "%d", &Nx);
    fscanf(fp, "%d", &Ny);

    // Check bourndary
    if (boundary_detect(Nx, Ny) == 0)
    {
        printf("Boundary out of range\n");
        exit(0);
    }

    // Memory allocation
    landscape   = (int **)    arralloc(sizeof(int),    2, Nx+2, Ny+2);
    H           = (double **) arralloc(sizeof(double), 2, Nx+2, Ny+2);
    P           = (double **) arralloc(sizeof(double), 2, Nx+2, Ny+2);
    tempH       = (double **) arralloc(sizeof(double), 2, Nx+2, Ny+2);
    tempP       = (double **) arralloc(sizeof(double), 2, Nx+2, Ny+2);

    // Read landscape data from file
    for (i = 1; i <= Nx; i++)
    {
        for (j = 1; j <= Ny; j++)
        {
                fscanf(fp, "%d", &landscape[i][j]);
        }
    }
    fclose(fp);

    /***** Part 3: Initialise densities of Puma and Hare *****/
    density_initialse(Nx, Ny, H, P, landscape);

    /* Print initial landscape & parameters */
    print_initial_values(arguments, parameters, Nx, Ny, landscape);
 
    /*****************  EQUATIONs IMPLEMENTATION AND OUTPUT  *****************/
    loop = (int) 500/parameters.tstep;
    if (parameters.verbose == 1) printf("Total loops: %d\n", loop);
    
    initial_time = omp_get_wtime();
    for(outer = 0; outer < loop; outer++) //Run the simulation from time 0 to 500
    {
	// Calculate densities of hares & pumas in next time step 
        update_simulation(outer, parameters, Nx, Ny, H, P, tempH, tempP, landscape);

	// Swap the new densities from temp arrays into H & P 
        ptr_swap((void **) &H, (void **) &tempH);
        ptr_swap((void **) &P, (void **) &tempP);

    }
    end_time = omp_get_wtime();

    printf("Time for simulation: %lf s\n", end_time-initial_time);
    
    /*****************      FREE MEMORY AND END PROGRAM      *****************/  
    if (parameters.verbose == 1) printf("Freeing memory\n");
    free(landscape);
    free(H);
    free(P);
    free(tempH);
    free(tempP);
    return 0;
}

