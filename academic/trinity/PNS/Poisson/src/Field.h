class Field
{
private:
    double* data;
    int n;
    int index(int x, int y) const {return x + (n+1)*y;} //converts to lexicographical ordering
public:
    //Constructor-takes one argument, which is grid spacing
    Field(int ngrid) : n(ngrid)     //:n(ngrid) sets n = ngrid
    {
        data = new double[(ngrid+1)*(ngrid+1)];
        int l = (ngrid+1)*(ngrid+1);
        for (int i=0;i<l;i++) data[i]=0.0; // Initialise field to 0 everywhere

        for (int j = (n*(n+1)/10)*8; j <= (n*(n+1)/10)*9; j += (n+1))
        {for (int i= j + 1*n/10; i <= j + 4*n/10; i++) data[i]=1.0;}

        for (int j = (n*(n+1)/10)*2; j <= (n*(n+1)/10)*8; j += (n+1))
        {for (int i= j + 7*n/10; i <= j + 8*n/10; i++) data[i]=-1.0;}
    }
    //destructor. gets called automatically when variable falls out of scope
    ~Field()    //if we called destructor without delete data we'd delete our data but no return memory to computer
    {
        delete [] data;
    }
// Access a site
    double& operator() (int x, int y)       { return data[index(x,y)];}
    double  operator() (int x, int y) const { return data[index(x,y)];}

    int size() const { return n;}
};

//=======================================================================================================

//Function to update points. Avoids specified boxes by two conditional statements
int update (int j_lower, int j_upper, int con1_left, int con1_right, int con2_left, int con2_right, Field& phi)
{
    double w =0.915;
    int flag = 0;
    for (int j = j_lower; j < j_upper; j++)
    {
        for (int i = 1; i < phi.size(); i ++)
        {
            if (i < con1_left || (i > con1_right && (i < con2_left || i > con2_right)))
            {
                double phi_prev = phi(i,j);
                phi(i,j) = (phi(i-1,j) + phi(i,j-1) + phi(i+1,j) + phi(i,j+1))/4;   //Update phi
                phi(i,j) = (1+w)*phi(i,j) - w*phi_prev; //SOR
                if (abs(phi(i,j) - phi_prev) > 1.0e-3*abs(phi(i,j))){flag += 1;}    //Check for convergence of point
            }
        }
    }
    return flag;
}

//======================================================================================================

int update_boundary (Field& phi)
{
    int flag = 0;
    for (int j = 0; j <= phi.size(); j++)
    {
        for (int i = 0; i <= phi.size(); i ++)
        {
            double phi_prev = phi(i,j);
            if (j==0)           {phi(i,j) = (4*phi(i,j+1)-phi(i,j+2))/3;}
            if (j==phi.size())  {phi(i,j) = (4*phi(i,j-1)-phi(i,j-2))/3;}
            if (i==0)           {phi(i,j) = (4*phi(i+1,j)-phi(i+2,j))/3;}
            if (i==phi.size())  {phi(i,j) = (4*phi(i-1,j)-phi(i-2,j))/3;}
            if (abs(phi(i,j) - phi_prev) > 1.0e-3*abs(phi(i,j))){flag += 1;}
        }
    }
    return flag;
}

//===============================================================================================

void print_to_file (Field& phi)
{
    cout.precision(3);
    ofstream myfile;
    myfile.open ("results.dat", std::ios_base::app);

    cout << "Phi(1/2,1/2)=\t" << phi(phi.size()/2, phi.size()/2) << "\n";

    for (int j = 0; j <= phi.size(); j++)
    {
        for (int i = 0; i <= phi.size(); i ++)
        {
            //myfile << i << "\t" << phi.size()-j << "\t" << phi(i,j) << "\n";
        }
    }
}
