// Wolff cluster
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <time.h>
#include <random>


using namespace std;

double J = +1;                  // ferromagnetic coupling
int L;                    // number of spins in x and y
int N;                          // number of spins
int **s;                        // the spins
double T;                       // temperature
double beta;
double H = 0;                   // magnetic field
int steps;                      // number of Monte Carlo steps
double r;
int start_state;

default_random_engine randomGenerator(2);
uniform_real_distribution<double> random(0.0, 1.0);


void initialize ()
{
    s = new int* [L];
    for (int i = 0; i < L; i++)
        s[i] = new int [L];
    for (int i = 0; i < L; i++)
        {
        for (int j = 0; j < L; j++)
        {
            if (start_state == 2){
            s[i][j] = (random(randomGenerator)) < 0.5 ? +1 : -1; }  // hot start
            else {s[i][j] = 1;}
        }}
    steps = 0;
}

bool **cluster;                     // cluster[i][j] = true if i,j belongs
double addProbability;              // 1 - e^(-2J/kT)

void initializeClusterVariables() {

    // allocate 2-D array for spin cluster labels
    cluster = new bool* [L];
    for (int i = 0; i < L; i++)
        cluster[i] = new bool [L];

    // compute the probability to add a like spin to the cluster
    addProbability = 1 - exp(-2*J/T);
}

// declare functions to implement Wolff algorithm
void growCluster(int i, int j, int clusterSpin);
void tryAdd(int i, int j, int clusterSpin);

int oneMonteCarloStep() {

    double size = 0;
    // no cluster defined so clear the cluster array
    for (int i = 0; i < L; i++)
    for (int j = 0; j < L; j++)
        cluster[i][j] = false;

    // choose a random spin and grow a cluster
    int i = int((random(randomGenerator)) * L);
    int j = int((random(randomGenerator)) * L);
    growCluster(i, j, s[i][j]);

    for (int i = 0; i < L; i++)
    for (int j = 0; j < L; j++)
        size += cluster[i][j];

    return size;
    ++steps;
}

void growCluster(int i, int j, int clusterSpin) {

    // mark the spin as belonging to the cluster and flip it
    cluster[i][j] = true;
    s[i][j] = -s[i][j];

    // find the indices of the 4 neighbors
    // assuming periodic boundary conditions
    int iPrev = i == 0    ? L-1 : i-1;
    int iNext = i == L-1 ? 0    : i+1;
    int jPrev = j == 0    ? L-1 : j-1;
    int jNext = j == L-1 ? 0    : j+1;

    // if the neighbor spin does not belong to the
    // cluster, then try to add it to the cluster
    if (!cluster[iPrev][j])
        tryAdd(iPrev, j, clusterSpin);
    if (!cluster[iNext][j])
        tryAdd(iNext, j, clusterSpin);
    if (!cluster[i][jPrev])
        tryAdd(i, jPrev, clusterSpin);
    if (!cluster[i][jNext])
        tryAdd(i, jNext, clusterSpin);
}

void tryAdd(int i, int j, int clusterSpin) {
    if (s[i][j] == clusterSpin)
        if ((random(randomGenerator)) < addProbability)
            growCluster(i, j, clusterSpin);
}

double m;   //Magnetisation
double e;   //Energy

void initializeObservables()
{
    m = 0;
    e = 0;
}

//Calaculate observables magnetisation per site m and energy per site e
//These are the only measured observables and she will derive others from data
void measureObservables()
{
    m =0;
    e = 0;
    double temp;
    // observables are derived from the magnetic moment
    for (int i = 0; i < L; i++)
    for (int j = 0; j < L; j++)
    {{
        int iNext = i == L-1 ? 0    : i+1;
        int jNext = j == L-1 ? 0    : j+1;
            temp = s[i][j]*s[iNext][j] + s[i][j]*s[i][jNext];
            e += temp;
            m += s[i][j];
    }}
    m = fabs(m/(N));
    e = -1*(e/(2*N));
}



int main()
{
ofstream myfile;
myfile.open ("netmag.dat");

    cout << " Wolff Cluster Algorithm\n"
         << " #####################################################\n"
         << " Enter a value for L: ";
    cin >> L;
    cout << " Please enter initial state: (cold start, hot start): ";
    cin >> start_state;
    N = L * L;
    cout << " Enter number of Monte Carlo steps: ";
    int MCSteps;
    cin >> MCSteps;


    for (beta = 0.44068607932; beta <= 1; beta+=2)
    {
        initialize();
        initializeClusterVariables();

        for (int i = 1; i < MCSteps+1; i++)
        {
            measureObservables();
            myfile << m << "\t" << e <<"\t" << i << "\t" << beta << "\t" << oneMonteCarloStep()/double(N)  <<"\n";

            initializeObservables();
            measureObservables();
        }

        cout << "Magnetization:\t" << m << "\tEnergy Density:\t" << e << "\tBeta:" << beta << endl;
    }

    myfile.close();
}
