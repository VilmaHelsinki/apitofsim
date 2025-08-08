#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <complex>
#include <string.h>

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
double solve_eqn(double * c, double v0, double v1, double tolerance, int N, int M, int a, int &nwarnings, ofstream &warnings)
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
  double * c;
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
  char garb[100];
  char file_output[150];

  ofstream warnings;
  ofstream skimmer;

  warnings.open("warnings_skimmer.dat");
  warnings << std:: scientific;

  // Reading from input
  cin >> garb;
  cin >> garb >> garb;   // Cluster_charge_sign
  cin >> garb >> garb;   // Atomic_mass_cluster
  cin >> garb >> garb;   // Atomic_mass_first_product
  cin >> garb >> garb;   // Atomic_mass_second_product
  cin >> T0 >> garb;   // Temperature_(K)
  cin >> P0 >> garb;   // Pressure_first_chamber(Pa)
  cin >> garb >> garb;   // Pressure_second_chamber(Pa)
  cin >> garb >> garb;   // Length_of_1st_chamber_(meters)
  cin >> rmax >> garb;   // Length_of_skimmer_(meters)
  cin >> garb >> garb;   // Length_between_skimmer_and_front_quadrupole_(meters)
  cin >> garb >> garb;   // Length_between_front_quadrupole_and_back_quadrupole_(meters)
  cin >> garb >> garb;   // Length_between_back_quadrupole_and_2nd_skimmer_(meters)
  cin >> garb >> garb;   // Voltage0_(Volt)
  cin >> garb >> garb;   // Voltage1_(Volt)
  cin >> garb >> garb;   // Voltage2_(Volt)
  cin >> garb >> garb;   // Voltage3_(Volt)
  cin >> garb >> garb;   // Voltage4_(Volt)
  cin >> garb >> garb;   // Number_of_realizations
  cin >> dc >> garb;   // Radius_at_smallest_cross_section_skimmer_(m)
  cin >> alpha_factor >> garb;   // Angle_of_skimmer_(multiple_of_PI)
  cin >> garb >> garb; // Fragmentation_energy
  cin >> garb >> garb;   // Energy_max_for_density_of_states_(Kelvin)
  cin >> garb >> garb; // Energy_max_for_rate_constant_(Kelvin)
  cin >> garb >> garb;   // Energy_resolution_(Kelvin)
  cin >> garb >> garb;   // Gas_molecule_radius
  cin >> m >> garb;   // Gas_molecule_mass_(kg)
  cin >> ga >> garb;   // Adiabatic_index
  cin >> garb >> garb;       // DC_quadrupole
  cin >> garb >> garb;       // AC_quadrupole
  cin >> garb >> garb;  // Radiofrequency_quadrupole
  cin >> garb >> garb;   // half-distance_between_quadrupole_rods
  cin >> file_output >> garb;   // Output_file_skimmer
  cin >> garb >> garb;   // file_vibrational_temperatures_cluster
  cin >> garb >> garb;   // file_vibrational_temperatures_first_product
  cin >> garb >> garb;   // file_vibrational_temperatures_second_product
  cin >> garb >> garb;   // file_rotational_temperatures_cluster
  cin >> garb >> garb;   // file_rotational_temperatures_first_product
  cin >> garb >> garb;   // file_rotational_temperatures_second_product
  cin >> garb >> garb;   // file_electronic_energy_cluster
  cin >> garb >> garb;   // file_electronic_energy_first_product
  cin >> garb >> garb;   // file_electronic_energy_second_product
  cin >> garb >> garb;   // output_file_density_cluster
  cin >> garb >> garb;   // output_file_density_first_product
  cin >> garb >> garb;   // output_file_density_second_product
  cin >> garb >> garb;   // output_file_density_combined_products
  cin >> garb >> garb;   // output_file_rate_constant
  cin >> garb >> garb;   // output_file_probabilities
  cin >> N >> garb;   // Number_of_iterations_in_solving_equation
  cin >> M >> garb;   // Number_of_iterations_in_solving_equation2
  cin >> resolution >> garb;   // Number_of_solved_points
  cin >> tolerance >> garb;   // Tolerance_in_solving_equation                               
  
  skimmer.open(file_output);
  skimmer << std:: scientific << "#Distance_Velocity_Temperature_Pressure_GasMassDensity_SpeedOfSound"<<endl;

  double rho0=m*P0/k/T0;
  alpha=alpha_factor*M_PI;

  c=new double [5];

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
  delete c;
  return 0;
}

