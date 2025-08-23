#include <iostream>
#include<cmath>
#include <fstream>
using std::cout;
using std::ofstream;


/* Equation to be solved: dx/dt = -(t^2)d^2x/dt^2 - 4x*/
double f2(double x_2i, double t, double x_0i)
{
    return -pow(t,2)*x_2i - 4*x_0i;
}



double k1, k2, k3, k4;
double n = 10000;

double x_0i = 1;
double x_1i = 0;
double x_2i = -1;

double t1 = 0;
double t2 = 20;

double h = (t2-t1)/n;
double t = t1;

int main()
{
    ofstream myfile;
    myfile.open ("results.dat");


    for (int i=0; i<n; i++)
    {

        x_0i = x_0i + h*x_1i;

        x_1i = x_1i + h*x_2i;

        x_2i = x_2i + h*f2(x_2i,t, x_0i);

        myfile << t << "\t" << x_0i << "\n";

        t += h;
    }

    myfile.close();

    cout << "x(20) = " << x_0i << "\n";

    return 0;
}
