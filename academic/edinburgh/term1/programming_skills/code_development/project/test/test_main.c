#include <stdio.h>
#include <CUnit/Basic.h>

#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../src/main.h"
#include "../src/arralloc.h"

#define ARR_SIZE_X 5
#define ARR_SIZE_Y 5
#define DELTA 0.000001

/* Pointer to the file used by the tests. */
static FILE* test_file = NULL;
int ** test_landscape; 
double ** H;
double ** P;
double ** tempH;
double ** tempP;

/* Initialize global variables and open files before tests are run */
int init_suite()
{
    test_file = fopen("temp.txt", "w+");
    test_landscape = (int **) arralloc(sizeof(int), 2, (ARR_SIZE_X+2), (ARR_SIZE_Y+2));

    H  = (double **) arralloc(sizeof(double), 2, (ARR_SIZE_X+2), (ARR_SIZE_Y+2));
    P  = (double **) arralloc(sizeof(double), 2, (ARR_SIZE_X+2), (ARR_SIZE_Y+2));
    tempH  = (double **) arralloc(sizeof(double), 2, (ARR_SIZE_X+2), (ARR_SIZE_Y+2));
    tempP  = (double **) arralloc(sizeof(double), 2, (ARR_SIZE_X+2), (ARR_SIZE_Y+2));
    
    if (test_file == NULL || test_landscape == NULL || H == NULL|| P == NULL || tempH == NULL || tempP == NULL)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

/* Free memory and close files after tests are run*/
int clean_suite()
{
    if (fclose(test_file) != 0)
    {
        return -1;
    }
    else
    {
        test_file = NULL;
        free(test_landscape);
        free(H);
        free(P);
        free(tempH);
        free(tempP);
        return 0;
    }
}

void test_update_simulation()
{
    struct stat st = {0};
    simulation_parameters parameters;
    args arguments;
    int return_value = -1;

    printf("\n");

    arguments.config_file = NULL;
    arguments.verbose = 1;
    arguments.output_files = 1;
    arguments.output_stdout = 1;
    arguments.output_interval = -1;

    value_input(arguments, &parameters);
    density_initialse(ARR_SIZE_X, ARR_SIZE_Y, H, P, test_landscape);

    return_value = update_simulation(0, parameters, ARR_SIZE_X, ARR_SIZE_Y, H, P, tempH, tempP, test_landscape);
    
    /*Make sure folder exists after simulations have been run */
    
    CU_ASSERT(stat("./outputs", &st) != -1);
    CU_ASSERT(return_value == 3);

    parameters.output_stdout = 0;
    parameters.output_files = 1;
    return_value = update_simulation(0, parameters, ARR_SIZE_X, ARR_SIZE_Y, H, P, tempH, tempP, test_landscape);
    CU_ASSERT(return_value == 2);

    parameters.output_stdout = 1;
    parameters.output_files  = 0;
    return_value = update_simulation(0, parameters, ARR_SIZE_X, ARR_SIZE_Y, H, P, tempH, tempP, test_landscape);
    CU_ASSERT(return_value == 1);

    parameters.output_stdout = 0;
    parameters.output_files  = 0;
    return_value = update_simulation(0, parameters, ARR_SIZE_X, ARR_SIZE_Y, H, P, tempH, tempP, test_landscape);
    CU_ASSERT(return_value == 0);
}

void test_write_rgb()
{
    /* Tests how many bytes were written to the file */
    
    CU_ASSERT(write_rgb(test_file,   1,   1,   1) ==  6);
    CU_ASSERT(write_rgb(test_file,   1,  10,   1) ==  7);
    CU_ASSERT(write_rgb(test_file,   1,  10,  10) ==  8);
    CU_ASSERT(write_rgb(test_file,  10,  10,  10) ==  9);
    CU_ASSERT(write_rgb(test_file, 100, 100, 100) == 12);
    CU_ASSERT(write_rgb(test_file,  -1,   1,   1) == -1);
    CU_ASSERT(write_rgb(test_file, 256,   1,   1) == -1);
    CU_ASSERT(write_rgb(test_file,   1,  -1,   1) == -1);
    CU_ASSERT(write_rgb(test_file,   1, 256,   1) == -1);
    CU_ASSERT(write_rgb(test_file,   1,   1,  -1) == -1);
    CU_ASSERT(write_rgb(test_file,   1,   1, 256) == -1);
}

void test_rgb_hill_function()
{
    double value;
    int answer;
    
    /* Check that value returned by the rgb_hill_function is correct and that it lies between 0 and 255 */
    
    value = 254.0;
    answer = 255 * value/(1 + value);
    CU_ASSERT(rgb_hill_function(value) == answer);
    CU_ASSERT(rgb_hill_function(value) < 255 && rgb_hill_function(value) > 0)
    
    value = 0.0;
    answer = 255 * value/(1 + value);
    CU_ASSERT(rgb_hill_function(value) == answer);
    CU_ASSERT(rgb_hill_function(value) < 255 && rgb_hill_function(value) > 0)
    
    value = 100.0;
    answer = 255 * value/(1 + value);
    CU_ASSERT(rgb_hill_function(value) == answer);
    CU_ASSERT(rgb_hill_function(value) < 255 && rgb_hill_function(value) > 0)
    
    value = 9.0;
    answer = 255 * value/(1 + value);
    CU_ASSERT(rgb_hill_function(value) == answer);
    CU_ASSERT(rgb_hill_function(value) < 255 && rgb_hill_function(value) > 0)
}

void test_update_animal()
{
    animal test_hare = {HARE, 0.8, 0.4, 0.2},
           test_puma = {PUMA, 0.8, 0.4, 0.2};
    double tstep     = 0.01,
           current_density[5] = {1.2, 0.6, 3.4, 0.7, 2.8},
           other_density = 0.6;
    int    neighbours = 4;

    CU_ASSERT_DOUBLE_EQUAL(update_animal(test_hare, tstep, neighbours, current_density, other_density), 1.21212, DELTA);
    CU_ASSERT_DOUBLE_EQUAL(update_animal(test_puma, tstep, neighbours, current_density, other_density), 1.20636, DELTA);
    tstep = 1;
    CU_ASSERT_DOUBLE_EQUAL(update_animal(test_hare, tstep, neighbours, current_density, other_density), 2.412, DELTA);
    CU_ASSERT_DOUBLE_EQUAL(update_animal(test_puma, tstep, neighbours, current_density, other_density), 1.836, DELTA);
    tstep = 0.01;
    neighbours = 0;
    
    current_density[1] = 0.0;
    current_density[2] = 0.0;
    current_density[3] = 0.0;
    current_density[4] = 0.0;
    CU_ASSERT_DOUBLE_EQUAL(update_animal(test_hare, tstep, neighbours, current_density, other_density), 1.20672, DELTA);
    CU_ASSERT_DOUBLE_EQUAL(update_animal(test_puma, tstep, neighbours, current_density, other_density), 1.20096, DELTA);
}

void test_ptr_swap()
{
    int a = 10,
        b = 20;
    int * a_ptr = &a,
        * b_ptr = &b,
        * a_ptr_backup = a_ptr,
        * b_ptr_backup = b_ptr; 
    
    /* Check if assigned values are what they should be */

    CU_ASSERT(a == 10);
    CU_ASSERT(b == 20);
    CU_ASSERT(*a_ptr == 10);
    CU_ASSERT(*b_ptr == 20);
    CU_ASSERT(a_ptr == a_ptr_backup);
    CU_ASSERT(b_ptr == b_ptr_backup);

    ptr_swap((void **) &a_ptr, (void **) &b_ptr);
    
    /* Check if values have ben swapped */

    CU_ASSERT(a == 10);
    CU_ASSERT(b == 20);
    CU_ASSERT(*a_ptr == 20);
    CU_ASSERT(*b_ptr == 10);
    CU_ASSERT(a_ptr == b_ptr_backup);
    CU_ASSERT(b_ptr == a_ptr_backup);
}

void test_get_neighbour_count()
{
    int i, j;
 
    /* Initialize array from which neighbours will be counted */
    
    for (i = 0; i <  (ARR_SIZE_X+2); i++)
    {
        for (j = 0; j < (ARR_SIZE_X+2); j++)
        {
            if ((j + i) % 3 == 0) test_landscape[i][j] = 1;
        }
    }
    
    /* Check if sum of neighbours is what it should be */
 
    CU_ASSERT(get_neighbour_count(test_landscape, 1, 1) == 2);
    CU_ASSERT(get_neighbour_count(test_landscape, 1, 2) == 0);
    CU_ASSERT(get_neighbour_count(test_landscape, 2, 4) == 0);
    CU_ASSERT(get_neighbour_count(test_landscape, 3, 4) == 2);
    CU_ASSERT(get_neighbour_count(test_landscape, 4, 1) == 2);
    CU_ASSERT(get_neighbour_count(test_landscape, 5, 1) == 0);
}
 
void test_density_initialse()
{
    int i, j;
    
    /* Initialize test landscape array */
                 
    for (i = 0; i < (ARR_SIZE_X+2); i++)
    {
        for (j = 0; j < (ARR_SIZE_Y+2); j++)
        {
            test_landscape[i][j] = 0;
        }
    }
    
    density_initialse(ARR_SIZE_X, ARR_SIZE_Y, H, P, test_landscape);
    
    /* Check if halo values are zero in hare and puma density arrays */

    CU_ASSERT(P[0][0] == 0.0);
    CU_ASSERT(H[0][0] == 0.0);
    CU_ASSERT(P[4][4] == 0.0);
    CU_ASSERT(H[4][4] == 0.0);
    CU_ASSERT(P[0][3] == 0.0);
    CU_ASSERT(H[4][2] == 0.0);
    
    /* Initialize test landscape array differently */
    
    for (i = 1; i <= ARR_SIZE_X; i++)
    {
        for (j = 1; j <= ARR_SIZE_Y; j++)
        {
            test_landscape[i][j] = 1;
        }
    }
    
    density_initialse(ARR_SIZE_X, ARR_SIZE_Y, H, P, test_landscape);

    /* Check if density of hares and pumas within the landscape lies between 0 and 5 */
    
    CU_ASSERT(P[1][1] >= 0.0 && P[1][1] < 5.0);
    CU_ASSERT(H[1][1] >= 0.0 && P[1][1] < 5.0);
    CU_ASSERT(P[2][2] >= 0.0 && P[2][2] < 5.0);
    CU_ASSERT(H[2][2] >= 0.0 && P[2][2] < 5.0);
    CU_ASSERT(P[1][3] >= 0.0 && P[1][3] < 5.0);
    CU_ASSERT(H[1][3] >= 0.0 && P[1][3] < 5.0);
}

void test_inputs_detect()
{
    simulation_parameters parameters;
    
    /* Assign some values to puma and hare parameters */
    
    parameters.hare.birth_rate = 0.5;
    parameters.hare.death_rate = 0.5;
    parameters.puma.birth_rate = 0.5;
    parameters.puma.death_rate = 0.5;
    parameters.hare.diff_rate = 0.5;
    parameters.puma.diff_rate = 0.5;
    parameters.tstep = 10;
    parameters.verbose = 1;
    parameters.output_interval = 10;
    parameters.output_files = 1;
    parameters.output_stdout = 1;
    
    /* Check if parameter values are valid */
    
    CU_ASSERT(inputs_detect(parameters) == 1);
    
    parameters.hare.birth_rate = 0.0;
    CU_ASSERT(inputs_detect(parameters) == 1);
    
    /* Make sure function returns 0 even if one value is invalid */
    
    parameters.hare.birth_rate = 0.5;
    parameters.puma.death_rate = 1.1;
    CU_ASSERT(inputs_detect(parameters) == 0);
    
    parameters.puma.death_rate = 0.5;
    parameters.puma.diff_rate = -1.0;
    CU_ASSERT(inputs_detect(parameters) == 0);
    
    parameters.puma.diff_rate = 0.5;
    parameters.tstep = 1000;
    CU_ASSERT(inputs_detect(parameters) == 0);
}

void test_boundary_detect()
{
    /* Check that array dimension size does not exceed 2000*2000 */
    
    CU_ASSERT(boundary_detect(10,10) == 1);
    CU_ASSERT(boundary_detect(100,100) == 1);
    CU_ASSERT(boundary_detect(1000,1000) == 1);
    CU_ASSERT(boundary_detect(2000,2000) == 0);
    CU_ASSERT(boundary_detect(10000,10000) == 0);
    
    /* 0 and negative numbers should fail */
    
    CU_ASSERT(boundary_detect(0,0) == 0);
    CU_ASSERT(boundary_detect(-10,-10) == 0);
    CU_ASSERT(boundary_detect(10,-10) == 0);
    CU_ASSERT(boundary_detect(-10,100) == 0);
}

void test_value_input()
{
    args arguments;
    simulation_parameters parameters;
    char path[19] = "./test/config.test";

    parameters.hare.birth_rate = 0.5;
    parameters.hare.death_rate = 0.5;
    parameters.puma.birth_rate = 0.5;
    parameters.puma.death_rate = 0.5;
    parameters.hare.diff_rate = 0.5;
    parameters.puma.diff_rate = 0.5;
    parameters.tstep = 10;

    /* If no configuration file exists, set parameter values to default values */
    
    arguments.config_file = NULL;
    arguments.verbose = 1;
    arguments.output_files = 1;
    arguments.output_interval = -1;
    arguments.output_stdout = 1;

    value_input(arguments, &parameters);

    CU_ASSERT(parameters.hare.birth_rate == 0.08);
    CU_ASSERT(parameters.hare.death_rate == 0.04);
    CU_ASSERT(parameters.puma.birth_rate == 0.02);
    CU_ASSERT(parameters.puma.death_rate == 0.06);
    CU_ASSERT(parameters.hare.diff_rate == 0.2);
    CU_ASSERT(parameters.puma.diff_rate == 0.2);
    CU_ASSERT(parameters.tstep == 0.4);
    CU_ASSERT(parameters.verbose == 1);
    CU_ASSERT(parameters.output_files == 1);
    CU_ASSERT(parameters.output_interval == -1);
    CU_ASSERT(parameters.output_stdout == 1);
    
    /* Set parameters values to values in configuration files if it exits */

    arguments.config_file = path;
    arguments.verbose = 0;
    arguments.output_files = 0;
    arguments.output_interval = 100;
    arguments.output_stdout = 0;

    value_input(arguments, &parameters);

    CU_ASSERT(parameters.hare.birth_rate == 0.1);
    CU_ASSERT(parameters.hare.death_rate == 0.9);
    CU_ASSERT(parameters.puma.birth_rate == 0.8);
    CU_ASSERT(parameters.puma.death_rate == 0.9);
    CU_ASSERT(parameters.hare.diff_rate == 0.2);
    CU_ASSERT(parameters.puma.diff_rate == 0.2);
    CU_ASSERT(parameters.tstep == 0.1);
    CU_ASSERT(parameters.verbose == 0);
    CU_ASSERT(parameters.output_files == 0);
    CU_ASSERT(parameters.output_interval == 100);
    CU_ASSERT(parameters.output_stdout == 0);
}

void test_print_initial_values()
{
    /* Test if initial values are as expected */
    
    args arguments;
    simulation_parameters parameters;

    arguments.config_file = NULL;
    arguments.verbose = 1;
    arguments.output_files = 1;
    arguments.output_interval = -1;
    arguments.output_stdout = 1;
    value_input(arguments, &parameters);
    
    /* Test if verbose output is given depending on the parameter value */

    parameters.verbose = 1;
    printf("\nVerbose = 1:\n");
    print_initial_values(arguments, parameters, ARR_SIZE_X, ARR_SIZE_Y, test_landscape);
    printf("\npassed\n");

    parameters.verbose = 0; 
    printf("\nVerbose = 0:\n"); 
    print_initial_values(arguments, parameters, ARR_SIZE_X, ARR_SIZE_Y, test_landscape);
}


int main()
{
    CU_pSuite pSuite = NULL;
    
    /* Initializing CUnit test directory */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();
    
    /* Adding a suite to the registry */
    pSuite = CU_add_suite("Population.c", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Adding tests to suites */
    if ((NULL == CU_add_test(pSuite, "boundary_detect", test_boundary_detect)) ||
        (NULL == CU_add_test(pSuite, "ptr_swap", test_ptr_swap)) ||
        (NULL == CU_add_test(pSuite, "write_rgb", test_write_rgb)) ||
        (NULL == CU_add_test(pSuite, "update_animal", test_update_animal)) ||
        (NULL == CU_add_test(pSuite, "get_neighbour_count", test_get_neighbour_count)) ||
        (NULL == CU_add_test(pSuite, "density_initialse", test_density_initialse)) ||
        (NULL == CU_add_test(pSuite, "inputs_detect", test_inputs_detect)) ||
        (NULL == CU_add_test(pSuite, "update_simulation", test_update_simulation)) ||
        (NULL == CU_add_test(pSuite, "value_input", test_value_input)) ||
        (NULL == CU_add_test(pSuite, "print_initial_values", test_print_initial_values)) ||
        (NULL == CU_add_test(pSuite, "rgb_hill_function", test_rgb_hill_function)))
    {
        CU_cleanup_registry();
       return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
