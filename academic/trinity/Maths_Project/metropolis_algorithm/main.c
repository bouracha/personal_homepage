#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#include "functions.h"

main ()
{
const int L = 10;
int max;

int i, j;
int array [L][L];
float random_array[L*L];
int a,b,c,d,e;
int t;
int z;
int seed = 1;

 double E;
 int E_local = 0;
 double M;
 double h;
 double T;
 double k_boltzman = 1;
 float r;
 double J;
 double beta;

double lower_bound;
double upper_bound;
double start_state;
double step_size;

FILE *netmag;
netmag = fopen ("netmag.dat", "w");

printf ("Ising: Metropolis Algorithm\n");
printf ("##################################################\n");
printf ("Please enter initial state: (old state, cold start, hot start):");
scanf ("%lf", &start_state);
printf ("Please enter a timestep number:");
scanf ("%d",&max);
printf ("Please enter a step size:");
scanf ("%lf",&step_size);
printf ("##################################################\n");
printf ("Please enter a lower bound for Temperature (beta):");
scanf ("%lf",&lower_bound);
printf ("Please enter an upper bound for Temperature (beta):");
scanf ("%lf",&upper_bound);
printf ("##################################################\n");
J = 1;
h = 0;

double num_of_flips = 0;

void Observables()
{
    E = 0;
    for (j=0;j<L;j++)
        for (i=0;i<L;i++)
    {{
        a= *(&array[i][j]);
        b = direction (1, i, j, L, array);  //right
        e = direction (4, i, j, L, array);  //up
        E_local = (a*b+a*e);
        E += (double)E_local;
    }}

    E = -1*(E/(2*L*L));
    M = Magnetization (L, array);

    fprintf(netmag, "%lf\t", M);
    fprintf(netmag, "%lf\t", E);
    fprintf(netmag, "%d\t", t);
    fprintf(netmag, "%lf\t", beta);
    fprintf(netmag, "%lf\n", num_of_flips);
}

void RandomNumbers()
{
    z = 0;
    rlxs_init(0,seed);
    seed++;
    ranlxs(random_array, L*L );
}

void oneMonteCarloStep ()
{
for (j=0;j<L;j++)
{
    for (i=0;i<L;i++)
    {

        r = random_array[z];
        z++;

        a= *(&array[i][j]);

        b = direction (1, i, j, L, array);
        c = direction (2, i, j, L, array);
        d = direction (3, i, j, L, array);
        e = direction (4, i, j, L, array);

        *(&array[i][j]) = Pflip (k_boltzman,T,a,b,c,d,e,h,J,r,&num_of_flips);
        }
      }
}

void PrintToScreen(){
    if (t == max)
    {
        printf("<e>:\t%lf", E);
        printf("\t<m>:\t%lf", M);
        printf("\tBeta:\t%lf", 1/(k_boltzman*T));
        printf("\n");
    }
}


initialize_lattice (L, array);

for (beta = lower_bound; beta <= upper_bound; beta = beta + step_size)
{
    T = 1/(k_boltzman*beta);
    starting_heat (start_state, L, array);

for (t=1;t<=max;t++)
{
    Observables();
    RandomNumbers();
    num_of_flips = 0;
    oneMonteCarloStep();
    num_of_flips = num_of_flips/(L*L);
    PrintToScreen();
}   //time loop
}   //temperature loop


}      //end of main
