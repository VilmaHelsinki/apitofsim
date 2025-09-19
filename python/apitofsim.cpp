#include <iostream>
#include <stdlib.h>

#include <Eigen/Dense>
#include <nanobind/nanobind.h>
#include <nanobind/eigen/dense.h>

#include "skimmer_lib.h"

typedef Eigen::Matrix<float, Eigen::Dynamic, 6> SkimmerResult;

SkimmerResult skimmer(
  double T0,
  double P0,
  double rmax,
  double dc,
  double alpha_factor,
  double m,
  double ga,
  int N,
  int M,
  int resolution,
  double tolerance)
{
  int nwarnings = 0;
  std::ofstream warnings;
  warnings.open("warnings_skimmer.dat");
  warnings << std::scientific;
  Skimmer s = {
    T0,
    P0,
    rmax,
    dc,
    alpha_factor,
    m,
    ga,
    N,
    M,
    resolution,
    tolerance,
    nwarnings,
    warnings,
  };

  SkimmerResult result(resolution, 6);

  int i = 0;
  while (true)
  {
    s.next();
    auto r = s.get();
    if (r.has_value())
    {
      result(i, 0) = r->r;
      result(i, 1) = r->vel;
      result(i, 2) = r->T;
      result(i, 3) = r->P;
      result(i, 4) = r->rho;
      result(i, 5) = r->speed_of_sound;
      i++;
    }
    else
    {
      break;
    }
  }
  return result;
}

NB_MODULE(_apitofsim, m)
{
  m.def("skimmer", &skimmer);
}
