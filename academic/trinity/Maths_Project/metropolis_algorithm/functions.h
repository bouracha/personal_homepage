//Function prototypes
double Magnetization (int L, int array[L][L]);     //Function prototype
double Energy (double k_boltzman, double J, double T, int L, int array[L][L]);
double Pflip (double k_boltzman,double T,int a,int b,int c,int d,int e,double h,double J,float r,double *num_of_flips);   //Function prototype

double initialize_lattice (int L, int array[L][L]);
double starting_heat (double start_state, int L, int array[L][L]);
double direction (int dir, int i, int j, int L, int array[L][L]);


extern void ranlxs(float r[],int n);
extern void rlxs_init(int level,int seed);
extern int rlxs_size(void);
extern void rlxs_get(int state[]);
extern void rlxs_reset(int state[]);
