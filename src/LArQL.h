#ifndef LARQL_H
#define LARQL_H 1

#include <math.h>       /* exp */
#include "G4SystemOfUnits.hh"


double * LArQL(double ElectricField, int Granularity, double EnergyRange_Low, double EnergyRange_High);

double LArQL_MeanNumberPhotons(double energyDeposit, double stepWidth, double electricField);

double * Energy(int Granularity, double EnergyRange_Low, double EnergyRange_High);

#endif
