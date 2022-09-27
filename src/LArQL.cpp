#include "LArQL.h"
#include <iostream>

double * LArQL(double ElectricField, int Granularity = 30, double EnergyRange_Low = 0.0, double EnergyRange_High = 10.0) {
	std::cerr << "THIS SHIT IS BROKEN - FIX THE FORMULAS TO RETURN THE ACTUAL YIELD - NOT TOTAL AMOUNT OF PHOTONS" << std::endl;
	std::cerr << " ALSO - THE OVERALL FORMULA IS WRONG :) " << std::endl;
	std::cerr << " CHECK THE QPIXLS FOR THE CORRECT IMPLEMENTATION " << std::endl;
	return 0;
	  double A = 0.800;
	  double k = 0.0486;
	  double Wion = 23.6;
	  double plAr = 1.39;
	  double Nex_Ni = 0.29;
	  double RFactor = 0.479;
	  double p1 = 2.15157266e-05;
	  double p2 = -3.98850458e+00;
	  double p3 = 1.38343421e+00;
	  double p4 = 1.99195217e-06;
	  double alpha = 0.032;
	  double beta_ = 0.008258;

   double * LArQL_Array = new double[Granularity];
   double EnergyRange_Step = (EnergyRange_High - EnergyRange_Low) / (Granularity-1 );
   for (int i = 0; i < Granularity; i++) {
      double Energy = EnergyRange_Low + i * EnergyRange_Step;
      if(Energy< 1.0) Energy = 1.0;
       double QChi = p1 / (p2 + exp(p3 + p4*Energy));
       double Qcorr = exp(-ElectricField / (alpha*log(Energy) + beta_));
      double LArQL_Value = (1e6/Wion)*(1+Nex_Ni - QChi*Qcorr - A/(1+k*Energy/(plAr*ElectricField)));
      LArQL_Array[i] = LArQL_Value*Energy;
   }
   return LArQL_Array;
}

double LArQL_MeanNumberPhotons(double energyDeposit, double stepWidth, double electricField){
	std::cerr << "THIS SHIT IS BROKEN - FIX THE FORMULAS TO RETURN THE ACTUAL YIELD - NOT TOTAL AMOUNT OF PHOTONS" << std::endl;
	std::cerr << " ALSO - THE OVERALL FORMULA IS WRONG :) " << std::endl;
	std::cerr << " CHECK THE QPIXLS FOR THE CORRECT IMPLEMENTATION " << std::endl;
	return 0;

	  double A = 0.800;
	  double k = 0.0486;
	  double Wion = 23.6;
	  double plAr = 1.39;
	  double Nex_Ni = 0.29;
	  double RFactor = 0.479;
	  double p1 = 2.15157266e-05;
	  double p2 = -3.98850458e+00;
	  double p3 = 1.38343421e+00;
	  double p4 = 1.99195217e-06;
	  double alpha = 0.032;
	  double beta_ = 0.008258;
	  double dEdx = energyDeposit/stepWidth;
	  //std::cout << "dEdx = " << dEdx << std::endl;
	  if(dEdx< 1.0) dEdx = 1.0;
       	  double QChi = p1 / (p2 + exp(p3 + p4*dEdx));
          double Qcorr = exp(-electricField / (alpha*log(dEdx) + beta_));
          double LArQL = (1e6/Wion)*(1+Nex_Ni - QChi*Qcorr - A/(1+k*dEdx/(plAr*electricField)));
	  return LArQL;
}

double * Energy(int Granularity, double EnergyRange_Low, double EnergyRange_High){
   double * Energy_Array = new double[Granularity];
   double EnergyRange_Step = (EnergyRange_High - EnergyRange_Low) / (Granularity-1 );
   for (int i = 0; i < Granularity; i++) {
      double Energy = EnergyRange_Low + i * EnergyRange_Step;
      Energy_Array[i] = Energy*MeV;
   }
   return Energy_Array;
}
