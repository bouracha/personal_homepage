class Planet
{
  private:
    double m;
    double x[2];
    double v[2];

  public:
    Planet(double mass=0, double x0=0, double x1=0, double v0=0, double v1=0)
    {
      m=mass;
      x[0] = x0; x[1] = x1;
      v[0] = v0; v[1] = v1;
    }
    double kinetic_energy() const
    {
      return 0.5*m*(v[0]*v[0] + v[1]*v[1]);
    }
//Give us access to mass without option to change it
    double mass() const {return m;}

//Function that enables us to change mass, and checks is possitive
    void set_mass(double mass) {if(mass >= 0.0) m=mass;}

    double& vel(int i)       { return v[i]; }
    double  vel(int i) const { return v[i]; }

    double& pos(int i)       { return x[i]; }
    double  pos(int i) const { return x[i]; }

};
