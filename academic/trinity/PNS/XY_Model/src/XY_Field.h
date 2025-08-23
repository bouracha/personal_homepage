class Field
{
  private:
    double* data;
    int     n;
    int index(int x, int y) const { return (x+n)%n + n*((y+n)%n); } // Lexigraphical ordering with Periodic Boundary conditions
  public:
// constructor - takes one argument, which is grid spacing
    Field(int ngrid)  : n(ngrid)
    {
      data = new double[(ngrid)*(ngrid)];
      int l=(ngrid)*(ngrid);
      for (int i=0;i<l;i++) data[i]=1.3; // Initialise field whatever, makes no difference
    }
// destructor
    ~Field()
    {
      delete [] data;
    }

// Access a site
    double& operator() (int x, int y)       { return data[index(x,y)];}
    double  operator() (int x, int y) const { return data[index(x,y)];}

    int size() const { return n;}
};

//This calculates the value of the LOCAL contribution to S ie. action due to the links with the nearest neighbours
double Calculate_S (double sij, int i, int j, Field& theta)
{
    double S = 0;
    S = (1 - cos( sij - theta (i+1,j) ) );
    S += (1 - cos( sij - theta (i-1,j) ) );
    S += (1 - cos( sij - theta (i,j+1) ) );
    S += (1 - cos( sij - theta (i,j-1) ) );

    return S;
}

void Observable_C (double beta, int t, Field& theta, double acceptance_fraction, double delta)
{
    ofstream myfile;
    myfile.open ("results.dat", std::ios_base::app);
    double C = 0;
    for (int j = 0; j < theta.size() ; j++)
    {
        for (int i = 0; i < theta.size() ; i ++)
        {
            int i_new =  (i + theta.size()/2)%10;
            C += cos ( theta (i,j) - theta (i_new, j) );
        }
    }
    //We compute the average of C throughout the lattice
    C /= theta.size()*theta.size();
    myfile << beta << "\t" << t << "\t" << C << "\t" << acceptance_fraction << "\t" << delta << "\n";
}
