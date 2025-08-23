#include <iostream>
#include<cmath>
#include <iomanip>
#include <fstream> //In order to use ios::app
using std::cout;
using std::ofstream;

#include "Schrodinger.h"

//=======================================================================

double E_old = 0;

//==========================================================================

int main()
{
    cout << std::setprecision(5);

    for (double E = 0; E < 100; E=E++) //Checks ranges of E for y
    {                                   //first ran for a smaller increment to find energies
        if (RKsolve(E)/RKsolve(E_old) < 0)  //checks if y has a root between E and E+1
        {
            shooting (E_old, E);
        }
        E_old = E;
    }
}
