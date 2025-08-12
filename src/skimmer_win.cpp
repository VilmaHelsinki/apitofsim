#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <complex>
#include <string.h>
#include "utils.h"

#define eV 1.602176565e-19
#define boltzmann 1.38064852e-23

// Define Pi if it is not already defined
#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

using namespace std;

double secant_step(double x0, double x1, double f0, double f1)
{
  return (x0*f1-x1*f0)/(f1-f0);
}

double f(double x, double * c)
{
  return pow(abs((c[0]/(c[1]-c[2]*x*x))),c[3])-c[4]*x;
}

// Find lower solution --> a=0 (subsonic flow), find upper solution --> a=1 (supersonic flow)
double solve_eqn(double c[5], double v0, double v1, double tolerance, int N, int M, int a, int &nwarnings, ofstream &warnings)
{
  double mesh;
  double v;
  double v2=0.0;
  double f0;
  double f1;

  mesh=(v1-v0)/N;

  // find solution for supersonic flow
  if(a == 1)
  {
    for(int i=0; i< N; i++)
    {
      v=v0+mesh*i;
      f0=f(v,c);
      if(f0>0)
      {
        v0=v;
        v1=v+mesh;
        break;
      }
    }
  }

  // find solution for subsonic flow
  else
  {
    for(int i=0; i< N; i++)
    {
      v=v0-mesh*i;
      f0=f(v,c);
      if(f0>0)
      {
        v0=v-mesh;
        v1=v;
        break;
      }
    }
    
  }
  for(int i=0; i<M; i++)
  {
    f0=f(v0,c);
    f1=f(v1,c);
    if(abs((v1-v0)/v0)<tolerance)
    {
      v2=v1;
      return v2;
      break;
    }
    v2=secant_step(v0,v1,f0,f1);
    v0=v1;
    v1=v2;
  }
  nwarnings++;
  warnings << "tolerance not reached at " << c[4] << endl;
  return v2;
}

int main()
{
  double tolerance;
  double m;
  double k=1.380648e-23;
  double T0;
  double P0;
  double r;
  double vc;
  double v_alert;
  double ga;
  double dc;
  double c[5];
  double alpha;
  double alpha_factor;
  double rmax;
  double mesh;
  double vel;
  double T;
  double P;
  double rho;
  double speed_of_sound;
  int N; // number of iterations in finding location of solution in solve_eqn
  int M; // number of iterations in solve_eqn
  int resolution; // number of solved points
  int nwarnings=0;
  char file_output[150];

  ofstream warnings;
  ofstream skimmer;

  warnings.open("warnings_skimmer.dat");
  warnings << std:: scientific;

  // Reading from input
  read_config(
    std::cin,
    nullptr,
    nullptr,
    (int*)nullptr,
    nullptr,
    nullptr,
    &T0,
    &P0,
    (double*)nullptr,
    nullptr,
    &rmax,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    &dc,
    &alpha_factor,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    &m,
    &ga,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    file_output,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    &N,
    &M,
    &resolution,
    &tolerance
  );
  
  skimmer.open(file_output);
  skimmer << std:: scientific << "#Distance_Velocity_Temperature_Pressure_GasMassDensity_SpeedOfSound"<<endl;

  double rho0=m*P0/k/T0;
  alpha=alpha_factor*M_PI;

  vc=sqrt(2.0*ga*k*T0/(m*(ga+1)));
  v_alert=sqrt(2.0*k*ga*T0/(m*(ga-1)));
  
  c[1]=ga*k*T0/m;
  c[0]=c[1] - 0.5*(ga-1.0)*vc*vc;
  c[2]=0.5*(ga-1.0);
  c[3]=1.0/(ga-1.0);
  
  r=1.0e-3;
  c[4]=pow(dc+r*tan(alpha),2.0)/(vc*dc*dc);
  
  mesh=rmax/resolution;
  //mesh = 1.0e3/NN;
  cout << "###" << endl;
  cout << "Evaluating gas physical quantities in the skimmer..." << endl << endl;
  for(int i=0; i<resolution; i++)
  {
    r=mesh*i;
    c[4]=pow(dc+r*tan(alpha),2.0)/(vc*dc*dc);
    vel=solve_eqn(c, vc, v_alert, tolerance, N, M, 1, nwarnings, warnings);
    T=T0-0.5*vel*vel*m/k*(ga-1.0)/ga;
    P=P0*pow(T/T0 , ga/(ga-1.0));
    rho=rho0*pow(T/T0, 1/(ga-1));
    speed_of_sound=sqrt(ga*k*T/m);
    skimmer << r << " " << vel << " " << T << " " << P << " " << rho << " " << speed_of_sound  << endl;
    //if(i%1000==0 and i>0) cout << std::defaultfloat << 100.0*i/resolution << "%" << endl;
    //cout << mesh * i << " " << f(mesh*i,c)<< endl;
  }
  //cout << "100%" << endl;

  if(nwarnings>0) cout << nwarnings << " warnings have been generated: check the file warnings_skimmer.dat" << endl;

  cout << "END OF COMPUTATION" << endl << endl;
  cout << "OUTPUT" << endl << file_output << endl;
  cout << "###" << endl;

  warnings.close();
  skimmer.close();
  return 0;
}

