//
//  main.h
//  
//
//  Created by Zhuowei Si on 18/10/2015.
//
//

#ifndef ppmodel_h
#define ppmodel_h

/* This structure is used by main to communicate with parse_opt. */
typedef struct
{
  char * args[1];            /* File name */
  int output_interval;       /* Argument for -t */
  int verbose;               /* The -v flag */
  int output_files;          /* If there will be files written to disk */
  int output_stdout;         /* If there will be output written to stdout */
  char * config_file;        /* Argument for -c */
} args;

typedef struct
{
    enum {HARE, PUMA} type;
    double birth_rate;  // The birth rate of the animal
    double death_rate;  // The death rate of the animal
    double diff_rate;   // The diffusion rate of the animal
} animal;

typedef struct
{
    animal hare;
    animal puma;
    double tstep;         //the size of the time step (0.4).
    int verbose;
    int output_interval;
    int output_files;
    int output_stdout;
} simulation_parameters;

/* FUNCTIONS FOR LANDSCAPE AND INPUTS INITIALIZATION */
void density_initialse(int Nx, int Ny, double ** H, double ** P, int ** landscape);

int value_input(args arguments, simulation_parameters * parameters);

int inputs_detect(simulation_parameters parameters);

int boundary_detect(int Nx, int Ny);

void print_initial_values(args arguments, simulation_parameters parameters, int Nx, int Ny, int ** landscape);

/*  EQUATIONs IMPLEMENTATION AND OUTPUT  */
int write_rgb(FILE * fp, int r, int g, int b);

int rgb_hill_function(double value);

double update_animal(animal animal, double tstep, int neighbours, double current_density[], double other_density);

int update_simulation(int loop_number, simulation_parameters parameters, int Nx, int Ny, double ** H, double ** P, double ** tempH, double ** tempP, int ** landscape);

void ptr_swap(void ** ptr1, void ** ptr2);

int get_neighbour_count(int ** array, int current_x, int current_y);

#endif /* main_h */
