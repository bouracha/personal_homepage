/*  Simple molecular dynamics code.
 *  $Id: MD-c.c,v 1.2 2002/01/31 16:43:14 spb Exp spb $
 *
 * This program implements:
 *     long range inverse square forces between particles. F = G * m1*m2 / r**2
/*
 *     viscosity term     F = -u V
 * If 2 particles approach closer than Size we flip the direction of the
 * interaction force to approximate a collision.
 *
 * Coordinates are relative to a large central mass and the entire system is moving relative to the
 * viscous media.
 * If 2 particles approach closer than Size we flip the direction of the
 * interaction force to approximate a collision.
 *
 * This program was developed as part of a code optimisation course
 * and is therefore deliberately inefficient.
 *
 */
#include <math.h>
#include "coord.h"



void evolve(int count,double dt){
int  step;
int i,j,k,l;
double Size, force_calculation1, force_calculation2, force_calculation3;
/*
 * Loop over timesteps.
 */
      for(step = 1;step<=count;step++){
        printf("timestep %d\n",step);
        printf("collisions %d\n",collisions);



//OPTIMISATION 1a: Fused 3 loops over Ndim
//that compiler failed to fuse
//OPTIMISATION 1b: Fused 2 loops over Nbody 
//that compiler failed to fuse
//OPTIMSATION 1c: Moved inner most for loop to be
//outermost for more efficient contiguous memory access 
//OPTIMISATION 2: Inlined visc_force wind_force and add_norm functions and 
//switched the order of the loops. This was not done to gain the optimisation
//due to inlining as minfo informed us that the compiler did this automatically
//Instead this enabled us to fuse 6 loops fter 1a 1b and 1c.
//OPTIMISATION 3: We could then further add the calculation of viscosity and wind
//force to one line to calculate the resulting force vector.

        for(i = 0; i < Nbody; i++)
        {
          /* calculate distance from central mass */
	  r[i] = ((pos[0][i] * pos[0][i]) + (pos[1][i] * pos[1][i])+(pos[2][i] * pos[2][i]));
	  r[i] = sqrt(r[i]);
	}

//OPTIMISATION 4: We pulled this section out of the above loop. This is
//'counter-fusion' but gives better performance because of better memory
//access in the calculate central force line (it's not limited by the data
//locality issues in calculating r). Here we have sacraficed the
//potential benefit of loop fusion for more efficient memory access
//our sacrafices mitigate the performance increase however
//OPTIMISATION 5: Loop unrolled manually
        for(i = 0; i < Nbody; i++)
	{
            /* calculate central force */
	  f[0][i] = -visc[i]*(vel[0][i] + wind[0]) - (G*mass[i]*M_central*pos[0][i])/(pow(r[i],3));
	  f[1][i] = -visc[i]*(vel[1][i] + wind[1]) - (G*mass[i]*M_central*pos[1][i])/(pow(r[i],3));
	  f[2][i] = -visc[i]*(vel[2][i] + wind[2]) - (G*mass[i]*M_central*pos[2][i])/(pow(r[i],3));
	}

/* calculate pairwise separation of particles */
//OPTIMSATION: Moved inner most for loop to be
//outermost for more efficient contiguous memory access 
	for(l = 0; l < Ndim; l++)
	{
          k = 0;
          for(i = 0; i < Nbody; i++)
	  {
            for(j = i + 1; j < Nbody; j++)
	    {
              delta_pos[l][k] = pos[l][i] - pos[l][j];
              k = k + 1;
            }
          }
        }

/* calculate norm of seperation vector */
//OPTIMISATION 1: The loop within add_norm is over Npair thus inlinging this
//function allowed us to switch the order of the loops and hence fuse it with
//2 other loops. Though the compiler (with compiler flag -Mipa=fast,inline) 
//inline functions automatically it could not inline and fuse as we have done.
//OPTIMISATION 2: Inner loop was completely unrolled, Minfo revealed that the compiler
//did this optimisation itself, however the compiler only attempts to unroll the 
//innermost loop. We expect manually unrolling this inner loop will allow th compiler
// a chance to unroll the outer loop
        for(i = 0; i < Npair; i++)
	{
          delta_r[i] = ((delta_pos[0][i]*delta_pos[0][i]) + (delta_pos[1][i] * delta_pos[1][i]) + (delta_pos[2][i] * delta_pos[2][i]));
    	  delta_r[i] = sqrt(delta_r[i]);  
        }

/*
 * add pairwise forces.
 */
//OPTIMSATION 1: Moved inner most for loop to be
//outermost for more efficient contiguous memory access 
//OPTIMSATION 2a: Moved inner most for loop to be
//outermost for more efficient contiguous memory access 
//for loops over pos and vel which were in independent loops
//OPTIMISATION 2b: Now these two nested loops have the same form
//as the add pairwise forces loop and so loops were fused (double fusion)
//OPTIMISATION 3: There were 4 calls to force in the inner loop. We set this
//to a new variable 'force calculation' so we only need to call force once
//OPTIMISATION 4: Unrolled over Ndim required declaring force_calculation 1, 2 and 3
          k = 0;
          for(i=0;i<Nbody;i++)
	  {
            for(j=i+1;j<Nbody;j++)
	    {
              Size = radius[i] + radius[j];
            
	      /*  flip force if close in */
              force_calculation1 = (G*mass[i]*mass[j]*delta_pos[0][k])/(pow(delta_r[k],3.0));
              force_calculation2 = (G*mass[i]*mass[j]*delta_pos[1][k])/(pow(delta_r[k],3.0));
              force_calculation3 = (G*mass[i]*mass[j]*delta_pos[2][k])/(pow(delta_r[k],3.0));
              if( delta_r[k] >= Size )
	      {
                f[0][i] = f[0][i] - force_calculation1;
                f[1][i] = f[1][i] - force_calculation2;
                f[2][i] = f[2][i] - force_calculation3;

                f[0][j] = f[0][j] + force_calculation1;
                f[1][j] = f[1][j] + force_calculation2;
                f[2][j] = f[2][j] + force_calculation3;
              } 
	      else
	      {
                f[0][i] = f[0][i] + force_calculation1;
                f[1][i] = f[1][i] + force_calculation2;
                f[2][i] = f[2][i] + force_calculation3;

                f[0][j] = f[0][j] - force_calculation1;
                f[1][j] = f[1][j] - force_calculation2;
                f[2][j] = f[2][j] - force_calculation3;
		  
		collisions++;
              }
              k = k + 1;
            }

            /* update positions */
            pos[0][i] = pos[0][i] + dt * vel[0][i];
            pos[1][i] = pos[1][i] + dt * vel[1][i];
            pos[2][i] = pos[2][i] + dt * vel[2][i];
            /* update velocities */
            vel[0][i] = vel[0][i] + dt * (f[0][i]/mass[i]);
            vel[1][i] = vel[1][i] + dt * (f[1][i]/mass[i]);
            vel[2][i] = vel[2][i] + dt * (f[2][i]/mass[i]);
          }

      }

}




