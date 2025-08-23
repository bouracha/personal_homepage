#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include <argp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include "main.h"
#include "arralloc.h"
#include "ranlux.h"

/* Updates the simulation by a single time step
 *
 * Arguments: loop_number -> the current loop number
 *            parameters  -> the parameters for the simulation
 *            Nx          -> the maximum X value
 *            Ny          -> the maximum Y value
 *            H           -> array of the old densities for the hares
 *            P           -> array of the old densities for the pumas
 *            tempP       -> array of the new densities for the hares
 *            tempP       -> array of the new densities for the pumas
 *            landscape   -> array of the landscape
 * Returns: 0 on normal completion
 *          1 after writing to stdout
 *          2 after writing only to a file
 *          3 after writing a file and to stdout
 */
int update_simulation(int loop_number, simulation_parameters parameters, int Nx, int Ny, double ** H, double ** P, double ** tempH, double ** tempP, int ** landscape)
{
    int i, j, print = 0, fprint = 0;
    int count = 0;
    char PPMFile[30];
    double hare_average = 0.0, puma_average = 0.0;
    FILE * fp;
    struct stat st = {0};

    //Choose the frequency of printing averages and writing pics
    if (parameters.verbose == 1 && parameters.output_interval <= 0) 
    {
        if (loop_number % 100 == 0) print = 1;
        if (loop_number % 10 == 0) fprint = 2;
    }
    else if (parameters.output_interval <= 0)
    {
        if (loop_number % 250 == 0) print = 1;
        if (loop_number % 25 == 0) fprint = 2;
    }
    else
    {
        if (loop_number % parameters.output_interval == 0) fprint = 2;
        if (loop_number % parameters.output_interval == 0) print = 1;
    }
    if (parameters.output_files == 0)
    {
        fprint = 0;
    }
    if (parameters.output_stdout == 0)
    {
        print = 0;
    }

    //Bulid new PPM file to write data
    if (fprint != 0)
    {
        //Put all PPM pictures into the outputs file
        if (stat("./outputs", &st) == -1)
        {
            mkdir("./outputs", 0700);
        }
        sprintf(PPMFile, "./outputs/PPMPic%07d.ppm", loop_number);

        fp = fopen(PPMFile, "wb");
        if (fp == NULL)
        {
            printf("Cannot open file %s.\n", PPMFile);
            exit(0);
        }
        //wrtie "P3", "width height", "255"
        fprintf(fp, "P3\n%d %d\n255\n", Nx, Ny);
    }
    
    //Simulate grids of landscape one by one
    for (i = 1; i <= Nx; i++)
    {
        for (j = 1; j <= Ny; j++)
        {
            if (landscape[i][j] == 0)
            {
                //write blue into PPM file represent water
                if (fprint != 0) write_rgb(fp, 0, 0, 255);
            }
            else
            {
                //Get neighbour count of each land
                int neighbours = get_neighbour_count(landscape, i, j);

                //Put densities of itself and its neighbours into an array
                double hare_current_density[5] = {H[i][j], H[i-1][j], H[i+1][j], H[i][j-1], H[i][j+1]};
                double puma_current_density[5] = {P[i][j], P[i-1][j], P[i+1][j], P[i][j-1], P[i][j+1]};

                //Updates the densities in the simulation
                tempH[i][j] = update_animal(parameters.hare, parameters.tstep, neighbours, hare_current_density, puma_current_density[0]);
                tempP[i][j] = update_animal(parameters.puma, parameters.tstep, neighbours, puma_current_density, hare_current_density[0]);

                //Calculate the sum of hares and pumas
                count++;
                hare_average += tempH[i][j];
                puma_average += tempP[i][j];

                //write into PPM file
                if (fprint != 0) write_rgb(fp, (int)rgb_hill_function(tempP[i][j]), (int)rgb_hill_function(tempH[i][j]), 0);
            }
        }
        if (fprint != 0) fprintf(fp, "\n");
    }

    if (fprint != 0) fclose(fp);//close PPM file

    //Calculate the averages of hares and pumas
    hare_average /= (double) count;
    puma_average /= (double) count;
    if (print != 0)
        printf("Loop %d:\n\tAverage Density of:\n\t\tHares\t%lf\n\t\tPumas\t%lf\n", loop_number, hare_average, puma_average);
    
    return (print + fprint);
}

/* A hill function to change the density of an animal to
 * a value between 0 and 255.
 *
 * Arguments: value -> the density
 * Returns: an integer between 0 and 255.
 */
int rgb_hill_function(double value)
{
    return (255 * value/(1 + value));
}

/* Simple function to write RGB values to a file
 *
 * Arguments: fp -> file to write to
 *            r  -> red   (0-255), rep Puma
 *            g  -> green (0-255), rep Hare
 *            b  -> blue  (0-255), rep Water
 * Returns: Number of bytes written
 *          or -1 if out of range (0 - 255)
 */
int write_rgb(FILE * fp, int r, int g, int b)
{
    if (r > 255 || r < 0 || g > 255 || g < 0 || b > 255 || b < 0) return -1;
    return fprintf(fp, "%d %d %d ", r, g, b);
}

/* Updates the densities in the simulation
 *
 * Arguments: animal          -> the struct containing the information for the animal
 *            tstep           -> the time step of the simulation
 *            neighbours      -> the number of land neighbours
 *            current_density -> array of the densities of the current square and E, W, N, S neighbours
 *            other_density   -> the density of the other animal in the square
 * Returns:   the new density of the specified animal in the square
 */
double update_animal(animal animal, double tstep, int neighbours, double current_density[], double other_density)
{
    double density = 0.0, born, died, moved; 

    born     = tstep * animal.birth_rate * current_density[0];
    died     = tstep * animal.death_rate * current_density[0];
    moved    = tstep * animal.diff_rate * ((current_density[1] + current_density[2] + current_density[3] +
                                                current_density[4]) - (neighbours * current_density[0]));
    
    switch(animal.type)
    {
        case HARE:
            died *= other_density;
            break;
        case PUMA:
            born *= other_density;
	    break;
    }

    density = current_density[0] + born - died + moved;

    if (density < 0.0) return 0.0; //If densities < 0 then make it 0
    else return density;
}

/* Swap where the pointers are pointing to
 * Uses a normal swap with three variables
 *
 * Arguments: ptr1 -> first pointer to swap
 *            ptr2 -> second pointer to swap
 * Returns: Nothing
 */
void ptr_swap(void ** ptr1, void ** ptr2)
{
    void * ptr_temp;
    ptr_temp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = ptr_temp;
}

/* Should return the number of neighbours that are land
 * Possile values in the landscape are:
 *       0 -> water or 1 -> land
 * Adding the surrounding elements in the array should be enough
 *
 * Arguments: array     -> landscape to check for neighbours
 *            current_x -> Current x position
 *            current_y -> Current y position
 * Returns: the number of neighbours that are land
 */
int get_neighbour_count(int ** array, int current_x, int current_y)
{
    return array[current_x-1][current_y] + array[current_x+1][current_y] \
           + array[current_x][current_y-1] + array[current_x][current_y+1];
}

/* Detect whether the boundary is out of range
 * The ranges of landscape are:
 *      0 < Nx < 2000, 0 < Ny < 2000
 *
 * Arguments: Nx -> the maximum X value
 *            Ny -> the maximum Y value
 * Returns: If out of range return 0, else return 1
 */
int boundary_detect(int Nx, int Ny)
{
    if ( (Nx <= 0 || Nx >= 2000) || (Ny <= 0 || Ny >= 2000) )
        return 0;
    else return 1;
}

/* Initialise the densities of Hares and Pumas
 * Initial densities of H and P are in the range 0 to 5.0
 *
 * Arguments: Nx        -> the maximum X value
 *            Ny        -> the maximum Y value
 *            H         -> array of densities for Hare
 *            P         -> array of densities for Puma
 *            landscape -> array of the landscape
 * Returns: Nothing
 */
void density_initialse(int Nx, int Ny, double ** H, double ** P, int ** landscape)
{
    int   i, j;
    int   seed = (int) time(NULL);
    float dis = 5.0; //Used to generate random density
    float * random_array[2];

    random_array[0] = (float *)   arralloc(sizeof(float),  1, ((Nx+2) * (Ny+2)));
    random_array[1] = (float *)   arralloc(sizeof(float),  1, ((Nx+2) * (Ny+2)));
    
    rlxs_init(1, seed); // Initialise the random number generator
    ranlxs(random_array[0], (Nx+2)*(Ny+2)); //Generate decimals & put into ranndomArray
    ranlxs(random_array[1], (Nx+2)*(Ny+2)); //Generate decimals & put into ranndomArray

    for (i = 0; i <= Nx+1; i++)
    {
        for (j = 0; j <= Ny+1; j++)
        {
            //if is the halo (i or j is 0) or is water then set density 0
            if (landscape[i][j] == 0 || i == 0 || j == 0)
            {
                H[i][j] = 0.0;
                P[i][j] = 0.0;
            }
            else
            {
                H[i][j] = (double) (5.0 * random_array[0][(Nx+2)*i+j]);
                P[i][j] = (double) (5.0 * random_array[1][(Nx+2)*i+j]);
            }
        }
    }
    free(random_array[0]);
    free(random_array[1]);
}

/* Check whether the variables are valid or not.
 * The range of any kind of rate is:
 *             0 < rate < 1
 * The range of time step is:
 *          0 < time step < 500
 *
 * Arguments: parameters -> initial varibles of hares & pumas
 * Returns: If invalid return 0, else return 1
 */
int inputs_detect(simulation_parameters parameters)
{
    if(parameters.hare.birth_rate < 0 || parameters.hare.birth_rate > 1 || \
       parameters.hare.death_rate < 0 || parameters.hare.death_rate > 1 || \
       parameters.puma.birth_rate < 0 || parameters.puma.birth_rate > 1 || \
       parameters.puma.death_rate < 0 || parameters.puma.death_rate > 1 || \
       parameters.hare.diff_rate < 0 || parameters.hare.diff_rate > 1 || \
       parameters.puma.diff_rate < 0 || parameters.puma.diff_rate > 1 || \
       parameters.tstep < 0 || parameters.tstep > 500) return 0;
	return 1;
}

/* Load config file from config.conf
 *
 * Arguments: arguments  -> data for parse_opt
 *            parameters -> parameters of hares & pumas
 * Returns: If invalid return 0, else return 1
 */
int value_input(args arguments, simulation_parameters * parameters)
{

    FILE *fp;
    if (arguments.config_file) //If file exists then read parameters from file
    {   
        fp = fopen(arguments.config_file, "r");
        fscanf(fp, "%lf", &(parameters->hare.birth_rate));
        fscanf(fp, "%lf", &(parameters->hare.death_rate));
        fscanf(fp, "%lf", &(parameters->puma.birth_rate));
        fscanf(fp, "%lf", &(parameters->puma.death_rate));
        fscanf(fp, "%lf", &(parameters->hare.diff_rate));
        fscanf(fp, "%lf", &(parameters->puma.diff_rate));
        fscanf(fp, "%lf", &(parameters->tstep));
        fclose(fp);
        //Detect whether parameters out of range
    	if (inputs_detect(*parameters) == 0)
    	{
    	    printf("Inputs Error.\n");
    	    return 0;
    	}
    }
    else
    {
        parameters->hare.birth_rate  = 0.08;    //the birth rate of hares (0.08).
        parameters->hare.death_rate  = 0.04;    //the predation rate at which pumas eat hares (0.04).
        parameters->puma.birth_rate  = 0.02;    //the birth rate of pumas per one hare eaten (0.02).
        parameters->puma.death_rate  = 0.06;    //the puma mortality rate (0.06).
        parameters->hare.diff_rate   = 0.2;     //the diffusion rates for hares (0.2).
        parameters->puma.diff_rate   = 0.2;     //the diffusion rates for pumas (0.2).
        parameters->tstep            = 0.4;     //the size of the time step (0.4).
    }
    parameters->verbose = arguments.verbose;
    parameters->output_interval = arguments.output_interval;
    parameters->output_files = arguments.output_files;
    parameters->output_stdout = arguments.output_stdout;
    return 1;
}

/* Print the initial landscape and parameters
 *
 * Arguments: arguments  -> data for parse_opt
 *            parameters -> parameters of hares & pumas
 *            Nx         -> the maximum X value
 *            Ny         -> the maximum Y value
 *            landscape  -> array of the landscape
 * Returns: Nothing
 */
void print_initial_values(args arguments, simulation_parameters parameters, \
                          int Nx, int Ny, int ** landscape)
{
    int i, j;
    if (parameters.verbose == 1)
    {
        if (Nx > 100 || Ny > 100)
        {
            printf("Landscape too big to print\n");
        }
        else
        {
            printf("Initial Landscape:\n");
            for (i = 0; i <= Nx+1; i++)
            {
                for (j = 0; j <= Ny+1; j++)
                {
                    printf("%d", landscape[i][j]);
                }
                printf("\n");
            }
        }
        printf("\nConfig file: %s\n", arguments.config_file);
        printf("Parameters:\n");
        printf("  Hare:\n");
        printf("    birth_rate = %lf\n", parameters.hare.birth_rate);
        printf("    death_rate = %lf\n", parameters.hare.death_rate);
        printf("     diff_rate = %lf\n", parameters.hare.diff_rate);
        printf("  Puma:\n");
        printf("    birth_rate = %lf\n", parameters.puma.birth_rate);
        printf("    death_rate = %lf\n", parameters.puma.death_rate);
        printf("     diff_rate = %lf\n", parameters.puma.diff_rate);
        printf("  tstep = %lf\n", parameters.tstep);
        printf("  verbose = %d\n", parameters.verbose);
        printf("  output interval = %d\n", parameters.output_interval);
        printf("  output files = %d\n", parameters.output_files);
        printf("  output stdout = %d\n", parameters.output_stdout);
    }
}


