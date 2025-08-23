#include <iostream>
#include<cmath>
#include <fstream>
using std::cout;
using std::ofstream;

/*Returns dx/dt for x, y, z at that step.*/
double f1(double x, double y, double z, double gamma)
{
    return y*(z - 1 + (x*x)) + gamma*x;
}

/*Returns dy/dt for x, y, z at that step.*/
double f2(double x, double y, double z, double gamma)
{
    return x*(3*z + 1 - x*x) + gamma*y;
}

/*Returns dz/dt for x, y, z at that step.*/
double f3(double x, double y, double z, double alpha)
{
    return -2*z*(alpha + x*y);
}

double k11, k21, k31, k41, k12, k22, k32, k42, k13, k23, k33, k43;
double n;

double t1 = 0.0;
double t2 = 90;

double t = t1;

double gamma = 0.01;
double alpha = 0.03;

double x_old = 0.0;
double y_old = 0.0;
double z_old = 0.0;

int main()
{
    ofstream myfile;
    myfile.open ("results.dat");

for (n = 1; n <= 100000000; n = n*10) //To determine appropriate
{                                      //n to get 3sf

    double x = -0.5;
    double y = 0;
    double z = 0.5;

    double h = (t2-t1)/n;

        for (int i=0; i<=n; i++)
        {

            k11 = h*f1(x, y, z, gamma);
            k12 = h*f2(x, y, z, gamma);
            k13 = h*f3(x, y, z, alpha);

            k21 = h*f1(x + k11/2, y + k12/2, z + k13/2, gamma);
            k22 = h*f2(x + k11/2, y + k12/2, z + k13/2, gamma);
            k23 = h*f3(x + k11/2, y + k12/2, z + k13/2, alpha);

            k31 = h*f1(x + k21/2, y + k22/2, z + k23/2, gamma);
            k32 = h*f2(x + k21/2, y + k22/2, z + k23/2, gamma);
            k33 = h*f3(x + k21/2, y + k22/2, z + k23/2, alpha);

            k41 = h*f1(x + k31, y + k32, z + k33, gamma);
            k42 = h*f2(x + k31, y + k32, z + k33, gamma);
            k43 = h*f3(x + k31, y + k32, z + k33, alpha);

            x = x + ((k11 + 2*k21 + 2*k31 + k41)/6);
            y = y + ((k12 + 2*k22 + 2*k32 + k42)/6);
            z = z + ((k13 + 2*k23 + 2*k33 + k43)/6);

            myfile << t << "\t" << x << "\t" << y << "\t" << z << "\n";

            t += h;
        }

    if ((fabs(x - x_old) < 0.01) && (fabs(y - y_old) < 0.01) && (fabs(z - z_old < 0.01)))
        {break;}            //Ie. if current and old value differ
                            //by less than 3sf then done

    myfile.close();
    x_old = x;   //If loop hasn't broken then current x, y, z used in
    y_old = y;      //next loop
    z_old = z;

    cout << "Step Number:\t" << n << "\n";
    cout << "x(90)=\t" << x << "\n";
    cout << "y(90) =\t" << y << "\n";
    cout << "z(90) =\t" << z << "\n";
}
    return 0;
}
