void padding (int *M_padded, int *N_padded, int M, int N, int sizei, int sizej);

void pack_buf (int M_p, int N_p, int M, int N, float buf[M_p][N_p], float masterbuf[M][N], int starti, int endi, int startj, int endj);
void unpack_buf (int M_p, int N_p, int M, int N, float buf[M_p][N_p], float masterbuf[M][N], int starti, int endi, int startj, int endj);

float boundaryval(int i, int m);
void Sawtooth_BC (int M_p, int N_p, float old[M_p+2][N_p+2], int M, int rankj, int sizej, int starti, int endi, int startj, int endj);

void max_change (int M_p, int N_p, float old[M_p+2][N_p+2], float new[M_p+2][N_p+2], float *max, float *pix_aver);
