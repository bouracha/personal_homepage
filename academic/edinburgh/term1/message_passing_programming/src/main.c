/*
 * A simple serial solution to the Case Study exercise from the MP
 * course.  Note that this uses the alternative boundary conditions
 * that are appropriate for the assessed coursework.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#include "pgmio.h"
#include "functions.h"

#define PRINTFREQ  200
#define TESTFREQ  50

#define ndims 2


int main (int argc, char **argv)
{
  /* Read in image as argument and determine size */
  char *filename;
  filename = argv[1];
  int M, N;
  pgmsize(filename, &M, &N);

  /* MPI variables */
  MPI_Init(NULL, NULL);
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm grid_comm;
  MPI_Request request1, request2, request3, request4, request5, request6, request7, request8;
  double starttime, endtime;

  /* =================================================================================================
   *	START: Define our MPI Virtual Topology
   * ================================================================================================= */

  /* Set sizei and sizej to factors of MPI size */
  int dims [2]= {0, 0};
  MPI_Dims_create(size, 2, dims);
  int sizei = dims[0];
  int sizej = dims[1];

  /* Create virtual grid topology in 2D, periodic in x
   * and set to communicator 'grid_comm' and set coords */
  int periods [2]= {1, 0};
  int  coords [2];
  MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periods, 0, &grid_comm);
  MPI_Cart_coords(grid_comm, rank, ndims, coords);

  /* Now we set ranki and rankj to their grid topology */
  int ranki = coords[0];
  int rankj = coords[1];

  /* We use MPI Cart shifts to define four nearest neighbouring
   * processes to each process in our virtual MPI Topology   */
  int disp = 1;
  int directionx = 0;
  int directiony = 1;
  int rankleft, rankright, rankdown, rankup;
  MPI_Cart_shift(grid_comm, directionx, disp, &rankleft, &rankright);
  MPI_Cart_shift(grid_comm, directiony, disp, &rankdown, &rankup);
  //printf("Process %i calling a shift in directionx Rank_left = %i  Rank_right = %i\n", rank, rankleft, rankright);
  //printf("Process %i calling a shift in directiony Rank_down = %i  Rank_up = %i\n", rank, rankdown, rankup);
  
  /* =================================================================================================
   *	END: Define our cartesian virtual Topology
   * ================================================================================================= */



  /* =================================================================================================
   *    START: Define Number of pixels on each process and where they start/end
   * ===========================================================================
   * In the event that we have number of pixels not divisible by sizei (sizej). We shall 
   * increase the number of pixels on the first sizei-1 (or sizej-1) processes such that
   * all these arrays are of equal size. The last process shall have the remainder.
   * ================================================================================================= */

  int M_extended;
  int N_extended;
  /* This function sets M_extended (or N_extended) to a number divisible by p */
  padding (&M_extended, &N_extended, M, N, sizei, sizej);
  /* M_p, N_p shall be chunksize per process in the i and j direction respectively */
  int M_p = M_extended/sizei;
  int N_p = N_extended/sizej;
  /* We define the pixel at which each process starts/ends in direction i and j 
   * This will not work if M_extended - M >= M_p (N_extended - N >= N_p) which should
   * not be the case - only if a high and prime number of processes are chosen */
  int starti = ranki*M_p;
  int endi = (ranki+1)*M_p;
  if (endi > M)
  {
    M_p = M_p - (endi-M);
    endi = M;
  }
  if (endi < starti) 
  {
    printf("Bad number of processors in i direction!\n");
    exit(0);
  }
  int startj = (rankj*N_extended)/sizej;
  int endj = ((rankj+1)*N_extended)/sizej;
  if (endj > N)
  {
    N_p = N_p - (endj-N);
    endj = N;
  }
  if (endj < startj)
  {
    printf("Bad number of processors in j direction!\n"); 
    exit(0);
  }
  /* =================================================================================================
   *    END: Define Number of pixels on each process. 
   * ================================================================================================= */
 
  /* We must define a new datatype to send non-contiguous data */
  MPI_Datatype noncontiguous;
  MPI_Type_vector (M_p, 1, (N_p+2), MPI_FLOAT, &noncontiguous);
  MPI_Type_commit (&noncontiguous);

  float old[M_p+2][N_p+2], new[M_p+2][N_p+2], edge[M_p+2][N_p+2];
  float buf[M_p][N_p];
  float masterbuf [M][N];
  float finalbuf [M][N];

  int i, j;
  int iter = 1;

  /* =================================================================================================
   *	START: Read in data to masterbuf, this is done on all processes so they all have a copy.
   * ================================================================================================= */

  if (rank == 0)
  {
    printf("\nsizei: %d \nsizej: %d \n\n", sizei, sizej);
    printf("Processing %d x %d image\n", M, N);

    printf("\nReading <%s>\n", filename);
    printf("\n");
  }
  pgmread(filename, masterbuf, M, N);

  /* =================================================================================================
   *	END: Read in data to masterbuf.
   * ================================================================================================= */

  /* Set buf to appropriate values of masterbuf for each process */
  pack_buf (M_p, N_p, M, N, buf, masterbuf, starti, endi, startj, endj);

  for (i=1;i<M_p+1;i++)
    {
      for (j=1;j<N_p+1;j++)
	{
	  edge[i][j]=buf[i-1][j-1];
	}
    }

  for (i=0; i<M_p+2;i++)
    {
      for (j=0;j<N_p+2;j++)
	{
	  old[i][j]=255.0;
	}
    }

  Sawtooth_BC (M_p, N_p, old, M, rankj, sizej, starti, endi, startj, endj);


  /* Must enter tolerance < 1 from command line */
  float tolerance = atof(argv[2]);
  float Global_max = 1.0;
  float Global_pix_aver;

  starttime = MPI_Wtime();
  /* =================================================================================================
   * =================================================================================================
   *	START: Main Loop. This shall be the computationally expensive part of the code. START TIME.
   * ================================================================================================= 
   * ================================================================================================= */
  while (Global_max > tolerance)
  {

    /* =================================================================================================
     *	START: Halo Exchange.
     * ================================================================================================= */

    /* Halo swapping horizontally (contiguous data)*/
    MPI_Issend(&old[M_p][1], N_p, MPI_FLOAT, rankright, 0, MPI_COMM_WORLD, &request1);
    MPI_Irecv(&old[0][1], N_p, MPI_FLOAT, rankleft, 0, MPI_COMM_WORLD, &request2);
    MPI_Issend(&old[1][1], N_p, MPI_FLOAT, rankleft, 0, MPI_COMM_WORLD, &request3);
    MPI_Irecv(&old[M_p+1][1], N_p, MPI_FLOAT, rankright, 0, MPI_COMM_WORLD, &request4);

    /* Halo swapping vertically (using derived datatype for noncontiguous data)*/
    MPI_Issend(&old[1][N_p], 1, noncontiguous, rankup, 0, MPI_COMM_WORLD, &request5);
    MPI_Irecv(&old[1][0], 1, noncontiguous, rankdown, 0, MPI_COMM_WORLD, &request6);
    MPI_Issend(&old[1][1], 1, noncontiguous, rankdown, 0, MPI_COMM_WORLD, &request7);
    MPI_Irecv(&old[1][N_p+1], 1, noncontiguous, rankup, 0, MPI_COMM_WORLD, &request8);

    /* All Halo communication must be completed
       before applying next iteration  */
    MPI_Wait(&request1, MPI_STATUS_IGNORE);
    MPI_Wait(&request2, MPI_STATUS_IGNORE);
    MPI_Wait(&request3, MPI_STATUS_IGNORE);
    MPI_Wait(&request4, MPI_STATUS_IGNORE);
    MPI_Wait(&request5, MPI_STATUS_IGNORE);
    MPI_Wait(&request6, MPI_STATUS_IGNORE);
    MPI_Wait(&request7, MPI_STATUS_IGNORE);
    MPI_Wait(&request8, MPI_STATUS_IGNORE);

    /* =================================================================================================
     *	END: Halo Exchange.
     * ================================================================================================= */

    /* Perform main iteration */
    for (i=1;i<M_p+1;i++)
    {
      for (j=1;j<N_p+1;j++)
      {
	new[i][j]=0.25*(old[i-1][j]+old[i+1][j]+old[i][j-1]+old[i][j+1] - edge[i][j]);
      }
    }

    /* =================================================================================================
     *	START: Calculate Stopping Criteria and Pixel average at TESTFREQ.
     * ================================================================================================= */
    /* Calculate the Global Max: the biggest pixel change between old and new */
    /* Calculate Pixel Average */
    if (iter%TESTFREQ ==0)
    {
      float pix_aver = 0;
      float max = 0;
      /* Calulate max pixel change for last iteration
       * (and the pixel average) on each process */
      max_change (M_p, N_p, old, new, &max, &pix_aver);

      /* Get global pixel average on process 0 */
      MPI_Reduce(&pix_aver, &Global_pix_aver, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
      Global_pix_aver = Global_pix_aver/(M*N);

      /* All Reduce so that EVERY process knows it's time to exit the while loop */
      MPI_Allreduce(&max, &Global_max, 1, MPI_FLOAT, MPI_MAX, MPI_COMM_WORLD);
    }

    if (iter%PRINTFREQ ==0)
    {
      if (rank == 0)
      {
        printf("Iteration %d\n", iter);
        printf("Average pixel value %f\n", Global_pix_aver);
        printf("Global Max %f\n", Global_max);
      }
    }
    /* =================================================================================================
     *	END: Stopping Criteria tested at TESTFREQ. Print pixel average at PRINTFREQ.
     * ================================================================================================= */

    for (i=1;i<M_p+1;i++)
    {
      for (j=1;j<N_p+1;j++)
      {
	old[i][j]=new[i][j];
      }
    }

    iter++;
  }
  /* =================================================================================================
   * =================================================================================================
   *	END: Main Loop. END TIME
   * ================================================================================================= 
   * ================================================================================================= */
  endtime = MPI_Wtime();

  if (rank == 0)
  {
    printf("\nFinished %d iterations\n", iter-1);
    printf("Max Delta:%f\n", Global_max);
    printf("Average Pixel:%f\n", Global_pix_aver);
  }

  for (i=1;i<M_p+1;i++)
  {
    for (j=1;j<N_p+1;j++)
    {
      buf[i-1][j-1]=old[i][j];
    }
  }

  /* We want elements which are not altered on 'this' process
   * to be zero so we can do a global reduction. */
  for (i=0; i<M; i++)
  {
    for (j=0; j<N; j++)
    {
      masterbuf[i][j] = 0;
    }
  } 

  /* Set appropriate value of masterbuf to values of buf for each process */
  unpack_buf (M_p, N_p, M, N, buf, masterbuf, starti, endi, startj, endj);
  /* rank 0 will now be given the entire image by this reductions */
  MPI_Reduce(&masterbuf[0][0], &finalbuf[0][0], (M*N), MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);


  /* ==================================================================================================================
   *	START: FILE PRINTS used for speed tesing and verifying code works (see report). Not required for standard use.
   * ================================================================================================================== */
  
  /* Used to caluculate speedup and to verify
   * code worked by examing Global_max (max pixel change)
  if (rank == 0)
  {
    FILE *speed_file;
    speed_file = fopen ("data/speed.dat", "a");

    double time = endtime - starttime;
    fprintf(speed_file, "%i\t", size);
    fprintf(speed_file, "%lf\t", time);
    fprintf(speed_file, "%i\t", iter);
    fprintf(speed_file, "%f\t", Global_pix_aver);
    fprintf(speed_file, "%f\n", Global_max);
  }  
  */

  /* This was used to print finalbuf to a file
   * which was then compared to serial code to
   * ensure that they were identical *
  if (rank == 0)
  {
    FILE *data;
    data = fopen ("data/parallel64_512_100000.dat", "w");
    for (i=0; i<M; i++)
    {
      for (j=0; j<N; j++)
      {
        fprintf(data, "%f\t", finalbuf[i][j]);
      }
      fprintf(data, "\n");
    }
  }  */        

  /* Use to print time for different
   * values of TESTFREQ
  if (rank == 0)
  {
    float time = (endtime-starttime);
    FILE *testfreq;
    testfreq = fopen ("data/testfreq.dat", "a");  
    fprintf(testfreq, "%i\t", TESTFREQ);
    fprintf(testfreq, "%i\t", (iter-1));
    fprintf(testfreq, "%f\n", time);
  }  */

  /* =================================================================================================
   *	END: FILE PRINTS
   * ================================================================================================= */

  if (rank == 0)
  {
    filename="processedimage.pgm";
    printf("\nWriting <%s>\n", filename); 
    pgmwrite(filename, finalbuf, M, N);
    printf("Time: %lf\n", (endtime-starttime));
  }

  MPI_Finalize();
  return 0;
} 



