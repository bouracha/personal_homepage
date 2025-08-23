#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* This function sets M_padded (or N_padded) to a size divisible by p */
void padding (int *M_padded, int *N_padded, int M, int N, int sizei, int sizej)
{
  if (M%sizei != 0)
  {
    *M_padded = M + sizei - M%sizei;
  }
  else
  {
    *M_padded = M;
  }
  if (N%sizej != 0)
  {
    *N_padded = N + sizej - N%sizej;
  }
  else
  {
    *N_padded = N;
  }
}


  /* Set buf to appropriate values of masterbuf for each process */
void pack_buf (int M_p, int N_p, int M, int N, float buf[M_p][N_p], float masterbuf[M][N], int starti, int endi, int startj, int endj)
{
  int i, j;
  int x = 0;
  int y = 0;
  for (i = starti; i < endi; i++)
  {
    y = 0;
    for (j = startj; j < endj; j++)
    {
      buf [x][y] = masterbuf[i][j];
      y++;
    }
  x++;
  }
}


  /* Set appropriate values of masterbuf to values of buf for each process */
void unpack_buf (int M_p, int N_p, int M, int N, float buf[M_p][N_p], float masterbuf[M][N], int starti, int endi, int startj, int endj)
{
  int i, j;
  int x = 0;
  int y = 0;
  for (i = starti; i < endi; i++)
  {
    y = 0;
    for (j = startj; j < endj; j++)
    {
      masterbuf[i][j] = buf [x][y];
      y++;
    }
  x++;
  }
}

/* Used in Sawtooth_BC*/
float boundaryval(int i, int m)
{
  float val;

  val = 2.0*((float)(i-1))/((float)(m-1));
  if (i >= m/2+1) val = 2.0-val;
  
  return val;
}


/* Calculate Sawtooth BC */
void Sawtooth_BC (int M_p, int N_p, float old[M_p+2][N_p+2], int M, int rankj, int sizej, int starti, int endi, int startj, int endj)
{
  float val;
  int i;
  /* Set fixed boundary conditions on bottom edge */
  if (rankj == 0)
  {
    int x = 1;
    for (i = starti + 1; i <= endi; i++)
    {
      val = boundaryval(i, M);

      old[x][0]   = 255.0*val;
      x++;
    }
  }
  /* Set fixed boundary conditions on the top edge */
  if (rankj == (sizej - 1))
  {
    int x = 1;
    for (i = starti + 1; i <= endi; i++)
    {
      val = boundaryval(i, M);

      old[x][N_p+1] = 255.0*(1.0-val);
      x++;
    } 
  }

}

 
/* Calulate max pixel change for last iteration (and the pixel average) on each process */
void max_change (int M_p, int N_p, float old[M_p+2][N_p+2], float new[M_p+2][N_p+2], float *max, float *pix_aver)
{
  int i, j;
  float delta = 0;
  for (i=1;i<M_p+1;i++)
  {
    for (j=1;j<N_p+1;j++)
    {
      *pix_aver += new[i][j];
      delta = fabs(new[i][j] - old[i][j]);
      if(delta > *max)
      {
         *max = delta;
      }
    }
  }  
}





