#include <iostream>
#include<cmath>
#include <fstream>
#include <time.h>

using namespace std;

#include "Field.h"


int main()
{
//=========================================================
    Field phi(100);

    int l = phi.size();
    int upper_b = (l/10)*2;
    int lower_b = (l/10)*8; // = upper_a
    int lower_a = (l/10)*9;
    int left_b = (l/10)*7;
    int right_b = (l/10)*8;
    int left_a = (l/10)*1;
    int right_a = (l/10)*4;

//=============================================================

//start clock
clock_t t1,t2;
t1 = clock();

int steps =0;
bool verbose = false;
while (verbose == false)
{
    int flag = 0;
    flag += update (1, upper_b, 0, 0, 0, 0, phi);
    flag += update (upper_b, lower_b, left_b, right_b, 0, 0, phi);
    flag += update (lower_b, lower_b+1, left_a, right_a, left_b, right_b, phi);
    flag += update (lower_b +1, lower_a+1, left_a, right_a, 0, 0, phi);
    flag += update (lower_a + 1, l, 0, 0, 0, 0, phi);

    //Uncomment update bpoundary for neumann boundary conditions
    //flag += update_boundary (phi);

    steps ++;
    //Flag =0 implies that every point has converged within desired error.
    //Thus we now change our bool to break the while loop and exsit the program
    if (flag == 0)
    {
        cout << "Number of steps:\t" << steps << "\n";
        print_to_file (phi);
        verbose = true;

        //Stops clock
        t2 = clock();

        cout << "\nRun time: " << double(t2-t1)/CLOCKS_PER_SEC << "\n";
    }
}
}
