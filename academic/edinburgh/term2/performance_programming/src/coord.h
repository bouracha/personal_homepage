/* $Id: coord.h,v 1.2 2002/01/08 12:32:48 spb Exp spb $
 * This file defines static array variables
 *
 *  Nbody		Number of particles
 *  Npair		Number of particle pairs
 *  pos			Position of the particles
 *  r       	distance of partice from central mass 
 *  vel			velocity of the particles
 *  f			Forces acting on each particle
 *  visc		viscosity coefficient for each particle
 *  mass		mass of each particle
 *  delta_pos	seperation vector for each particle pair
 *  delta_r		seperation for each particle pair
 *  collisions	number of collisions between particles
 */

#ifdef DECL
#define DEF
#else
#define DEF extern
#endif
#define Nbody 4*1024
#define  Npair ((Nbody*(Nbody-1))/2)

enum{ Xcoord=0, Ycoord, Zcoord, Ndim };


//OPTIMISATION: replaced dynamic arrays with static arrays
//OPTIMISATION: Padded arrays
DEF double pos[Ndim][Nbody+64], vel[Ndim][Nbody+64];
DEF double f[Ndim][Nbody+64], visc[Nbody+64], mass[Nbody+64], radius[Nbody+64];
//OPTIMISATION: Set delta_pos to size [3][Npair] rather than Nbody*Nbody
DEF double delta_pos[3][Npair];
DEF double r[Nbody+64];
DEF double delta_r[Nbody*Nbody+4096];
DEF double wind[Ndim];
DEF int collisions;


#define G 2.0
#define M_central 1000.0

void evolve(int Nstep, double dt);
