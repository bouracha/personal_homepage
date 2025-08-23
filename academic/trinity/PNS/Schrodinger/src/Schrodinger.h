 //d2y/dx2 = (V(x) - E)*y
double f(double x, double y, double E)
{
    return (-100*(x-0.8)*(x-0.9)*(x-1)*(x-1) - E)*y;
}

//==============================================================

// Solve using runge kutta
double RKsolve(double E,  bool verbose=false)
{
    std::ofstream myfile;
    myfile.open ("results.dat", std::ios_base::app);
    double k11, k21, k31, k41, k12, k22, k32, k42;
    double x1 = 0;
    double x2 = 1;
    double y_old = 0;
    double Area = 0;
    double n = 10000; //set to 10e07 to give correct to 5 sf
    double h = (x2-x1)/double(n);
    double y[2] = {0.0, 1.0};
    double x = x1;
    double* normalised_array;
    double* x_array;
    normalised_array = new double  [int(n)];
    x_array = new double [int(n)];

    //================================
    for (int i=0; i<n; i++)
        {
        //Runge Kutta method
        k11 = h*y[1];
        k12 = h*f(x, y[0], E);

        k21 = h*(y[1] + k12/2);
        k22 = h*f(x + h/2, y[0] + k11/2, E);

        k31 = h*(y[1] + k22/2);
        k32 = h*f(x + h/2, y[0] + k21/2, E);

        k41 = h*(y[1] + k32);
        k42 = h*f(x + h, y[0] + k31/2, E);

        y[0] = y[0] + ((k11 + 2*k21 + 2*k31 + k41)/6);
        y[1] = y[1] + ((k12 + 2*k22 + 2*k32 + k42)/6);

        x += h;

        //If true we calculate area using trapezoidal rule
        //and write y to an array that shall be later normalised
        if (verbose) {
            double A;
            if (i == 0 || i == n-1)
                {
                    A = (h/2)*(y[0]*y[0]);
                }
            else {
                A = h*(y[0]*y[0]);
                }
            Area += A;
            normalised_array [i] = y[0];
            x_array [i] = x;
            }
        }//end of runge kutta loop

    //===================================

    //if true normalised values of y and write to file
    if (verbose)   {
        for (int j = 0; j<n;j++)
        {
            normalised_array [j] *= 1/sqrt(Area);
            myfile << x_array [j] << "\t" << normalised_array [j] << "\n";
            //Uncomment two lines below and comment ONE above to plot probability distribution.
            //normalised_array [j] *= normalised_array [j]/Area;
            //myfile << x_array [j] << "\t" << normalised_array [j] << "\n";
        }
            }
    //clean up: delete space
    delete [] normalised_array;
    delete [] x_array;
    //return final value of y to determine if y(1)
    return y[0];
}

//=====================================================================

int energy_number = 0; //which energy we have 0=groundstate

//====================================================================

// Use bisection to find the value of E that gives y(1) = 0
    void shooting(double E_lo, double E_hi)
    {
    double b_lo = RKsolve(E_lo);
    double b_hi = RKsolve(E_hi);
    double E;


  if (b_lo/b_hi > 0.)
  {
    std::cerr << "Error - function at end-points has same sign\n";
    return;
  }

//E will be correct to 1*10^-10 if n is high! Set to 10 since this is cheap as is just bisection
    while (b_lo/b_hi < 0.0 && E_hi-E_lo > 1.0e-10)
        {
        E = 0.5*(E_lo + E_hi); 	// set E to the midpoint
        double b_E = RKsolve(E);
        if (b_E/b_lo > 0.0) 	// If b(E) and b(E_lo) have same sign - move to E_lo
        {  E_lo = E; b_lo = b_E; }
        else 			// b(E) and b(E_hi) have same sign - move to E_hi
        { E_hi = E; b_hi = b_E; }
        }

    RKsolve(E, true);
    cout << "E" << energy_number << " is in range:\t" << E_lo << " - " << E_hi <<  "\n";
    //note: energy range only as accurate as step size n
    energy_number ++;
}


