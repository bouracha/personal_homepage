#include <iostream>
#include<cmath>
#include <fstream>
#include <iomanip>

using std::cout;
using std::ofstream;

//Included header file of Planets
#include "planets.h"

using namespace std;

int main()
{
ofstream myfile;
myfile.open ("planets.dat");

//Initial conditions and mass
Planet planetArray[4];
planetArray[0] = Planet(2.0,-0.5,0.0,-0.94,0.65);
planetArray[1] = Planet(1.0,-0.60,-0.20,1.86,0.70);
planetArray[2] = Planet(1.0,0.50,0.10,-0.44,-1.40);
planetArray[3] = Planet(0.40,0.60,0.30,1.15,-1.50);

double t2 = 3;
int t1 = 0;
int n = 10000;  //10^6 steps gives correct to 3 sf
double h = (t2-t1)/double(n);

for (int k=0; k <= n; k++)
{
    //output to datafile
    myfile << setprecision(4)
    << planetArray[0].pos(0) << "\t" << planetArray[0].pos(1) << "\t"
    << planetArray[1].pos(0) << "\t" << planetArray[1].pos(1) << "\t"
    << planetArray[2].pos(0) << "\t" << planetArray[2].pos(1) << "\t"
    << planetArray[3].pos(0) << "\t" << planetArray[3].pos(1) << "\n";

    //Loop over each planet
    for (int i=0; i<=3; i++)
    {
        //First leap-frog update. We change the class values
        planetArray[i].pos(0) = planetArray[i].pos(0) + (h/2)*planetArray[i].vel(0);
        planetArray[i].pos(1) = planetArray[i].pos(1) + (h/2)*planetArray[i].vel(1);

        //Reset forces sum for each new planet i
        double ForceX_sum = 0;
        double ForceY_sum = 0;

        //Loop over interactions for each planet
        //and calculate magnitude and direction of total force on each planet
        for (int j=0; j<=3; j++)
        {
            double distX = (planetArray[i].pos(0) - planetArray[j].pos(0));
            double distY = (planetArray[i].pos(1) - planetArray[j].pos(1));

            double theta = atan(abs(distY/distX));

            double massProd = planetArray[i].mass() * planetArray[j].mass();

            double Force = massProd/(distX*distX+distY*distY);
            double ForceX = Force*cos(theta);
            if (distX > 0){ForceX = -ForceX;}
            double ForceY = Force*sin(theta);
            if (distY > 0){ForceY = -ForceY;}

            if (i == j)
            {//There should be no force of the planet on itself
                ForceX = 0;
                ForceY = 0;
            }

            ForceX_sum += ForceX;
            ForceY_sum += ForceY;
        }
        //Second leap frog update
        planetArray[i].vel(0) = planetArray[i].vel(0) + h*(ForceX_sum)/planetArray[i].mass();
        planetArray[i].vel(1) = planetArray[i].vel(1) + h*(ForceY_sum)/planetArray[i].mass();

        //Third leap frog update
        planetArray[i].pos(0) = planetArray[i].pos(0) + (h/2)*planetArray[i].vel(0);
        planetArray[i].pos(1) = planetArray[i].pos(1) + (h/2)*planetArray[i].vel(1);

        }
}

for (int k=0; k<=3;k++)
{
    cout << "Final position of planet" << k << ":\t" << planetArray[k].pos(0) << "\t" << planetArray[k].pos(1) << "\n";
}

}
