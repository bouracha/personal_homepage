#include <iostream>
#include<cmath>
#include <fstream>
#include <time.h>

using namespace std;

#include "XY_Field.h"

#define e 2.718281828459045235360287471352662497757       //Declare as global constant
#define pi 3.14159265358979323846       //Declare as global constant

#include <random>

default_random_engine randomGenerator(1);      //Seed number of 1
uniform_real_distribution<double> random(-1, 1);  //Range that random number will lie in

//This function completes one MC step which we have defined as an
//entire sweep of the lattice. Ie. metropolis condition for each site.
void OneMonteCarloStep (Field& theta, double beta, int t)
{
    double delta;   //PART 2
    int acceptance_counter = 0;
    //Nested for loops, to act on each lattice point in sequence
    for (int j = 0; j < theta.size() ; j++)
    {
        for (int i = 0; i < theta.size() ; i ++)
        {
            //double sij_new = pi*random(randomGenerator);       //PART 1 only
            delta = random(randomGenerator)*1.8;                //PART 2: this keeps the random number in range -1.8,1.8
            double sij_new = theta (i,j) + delta;           //PART 2: Proposed new theta at i,j
            double sij = theta (i,j);                       //Current theta at i,j

            //Due to cancelation this local change in S is the difference in the action between the
            //old state and the proposed new one
            double DeltaS = Calculate_S (sij_new, i, j, theta) - Calculate_S (sij, i, j, theta);

            double P = exp (-DeltaS*beta);
            //We accept change with probability P.
            if (P > abs(random(randomGenerator)))
            {
                theta (i,j) = sij_new;
                acceptance_counter ++;
            }
        }
    }
    double acceptance_fraction = (double)acceptance_counter/(theta.size()*theta.size());
    //Calculate observable and print to file
    Observable_C (beta, t, theta, acceptance_fraction, delta);
}



int main()
{
    //Initialise Field
    Field theta(32);

    //We run our program over a temperature range
    for (double beta = 0.0; beta <= 1.6; beta += 0.1)
    {
        //We shall make many steps, t, of the algorithm. EACH STEP ADDS ANOTHER STATE TO THE MARKOV CHAIN.
        //Some of these will be discarded as the system will not have reached equilibrium yet - DETAILED
        //BALANCE ENSURES THAT EACH STEP OF THE ALGORITHM BRINGS US CLOSER TO EQUILIBRIUM
        //The rest will be averaged to get a more accurate value of the observable.
        for (int t = 0; t <= 200; t++)
        {
            OneMonteCarloStep(theta, beta, t);
        }

        cout << "beta = " << beta << "\n";
    }
}
