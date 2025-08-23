#include <iostream>
#include<cmath>
#include <fstream>
using std::cout;
using std::ofstream;

/* Equation to be solved: dx/dt = -3x(t^2) */
double f(double x, double t)
{
    return -3*x*t*t;
}

double k1, k2, k3, k4;

double t1 = 0;
double t2 = 1;

double x_old = 0;

int main()
{
ofstream myfile;
myfile.open ("results.dat");

for (int n = 1; n <= 10000000; n = n*10)       //loop determines appropriate
{                                           //value of n
    double h = (t2-t1)/n;
    double x = 1;
    double t = t1;

    for (int i=0; i<n; i++)
    {
        k1 = h*f(x,t);
        k2 = h*f(x + k1/2, t + h/2);            //4th order Runge Kutta
        k3 = h*f(x + k2/2, t + h/2);
        k4 = h*f(x + k3, t + h);
        x = x + ((k1 + 2*k2 + 2*k3 + k4)/6);

        myfile << t << "\t" << x << "\n";       //Ran again once we've
                                                //decided on value for
                                                //n and results are plotted

        t += h;
    }

    if (fabs(x - x_old) < 0.0001) //Loop checks if we've reached sufficient
        {break;}                //degree of accuracy. Gives at least 3 sf
                                //for part one and 4 for part 2.
    myfile.close();
    x_old = x;


    cout << "x(1) = " << x << "\tStep Number:\t" << n << "\n";
}

return 0;
}
