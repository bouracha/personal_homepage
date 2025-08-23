#include <iostream>
#include<cmath>
#include <fstream>
using std::cout;
using std::ofstream;

/*dx2/dt = -t^2 x2 +4 x*/
double f(double x_2i, double t, double x_0i)
{
    return -pow(t,2)*x_2i - 4*x_0i;
}

double k11, k21, k31, k41, k12, k22, k32, k42, k13, k23, k33, k43;

double t1 = 0;
double t2 = 20;

double x_old = 0;

int main()
{
    ofstream myfile;
    myfile.open ("results.dat");

for (int n = 1; n <= 10000000; n = n*10)       //loop determines appropriate
{                                           //value of n
    double h = (t2-t1)/(double)n;
    double t = t1;
    double x_0i = 1;
    double x_1i = 0;
    double x_2i = -1;

    for (int i=0; i<n; i++)
    {

        k11 = h*x_1i;
        k12 = h*x_2i;
        k13 = h*f(x_2i, t, x_0i);

        k21 = h*(x_1i + k12/2);
        k22 = h*(x_2i + k13/2);
        k23 = h*f(x_2i + k13/2, t + h/2, x_0i + k11/2);

        k31 = h*(x_1i + k22/2);
        k32 = h*(x_2i + k23/2);
        k33 = h*f(x_2i + k23/2, t + h/2, x_0i + k21/2);

        k41 = h*(x_1i + k32);
        k42 = h*(x_2i + k33);
        k43 = h*f(x_2i + k33, t + h, x_0i + k31);

        x_0i = x_0i + ((k11 + 2*k21 + 2*k31 + k41)/6);
        x_1i = x_1i + ((k12 + 2*k22 + 2*k32 + k42)/6);
        x_2i = x_2i + ((k13 + 2*k23 + 2*k33 + k43)/6);

        myfile << t << "\t" << x_0i << "\n";

        t += h;
    }

    if (fabs(x_0i - x_old) < 0.001)     //Loop checks if we've reaches
     {break;}                           //sufficient degree of accuracy
                                        //(gives at leasr 4sf)
    myfile.close();
    x_old = x_0i;

    cout << "x(20) = " << x_0i << "\tStep Number:\t" << n << "\n";
}

return 0;
}
