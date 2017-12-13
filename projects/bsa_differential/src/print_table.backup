#include <iostream>
#include <iomanip>

#include "common/Bins.h"
#include "common/Histograms.h"
#include "common/Fits.h"
#include "common/Plotter.h"
#include "common/SignalBackground.h"

#include "CommonTools.h"

int main(int nargs, char *args[]){
  
  if (nargs < 2){
    std::cout << " Please pass in the root file which contains asymmetry histograms. " << std::endl; 
    return 0;
  }
  
  Bins *pionBins = Bins::GetInstance(Meson::kPionPositive); 

  std::string inputFile(args[1]); 

  IntegratedHistos kp_int(inputFile, "base", Meson::kKaonPositive); 
  IntegratedHistos km_int(inputFile, "base", Meson::kKaonNegative); 
  IntegratedHistos pp_int(inputFile, "base", Meson::kPionPositive); 
  IntegratedHistos pm_int(inputFile, "base", Meson::kPionNegative); 

  IntegratedHistos kp_int_corr(inputFile, "corr", Meson::kKaonPositive); 
  IntegratedHistos km_int_corr(inputFile, "corr", Meson::kKaonNegative); 
  IntegratedHistos pp_int_corr(inputFile, "corr", Meson::kPionPositive); 
  IntegratedHistos pm_int_corr(inputFile, "corr", Meson::kPionNegative); 

  //  SignalBackgroundFitter signalFitter("test", Meson::kPionPositive);
  //  signalFitter.Load(inputFile); 

  std::cout << std::setprecision(3); 

  std::cout.width(12); std::cout << "x_bin";  
  std::cout.width(12); std::cout << "z_bin";  
  std::cout.width(12); std::cout << "pt_bin";  
  std::cout.width(12); std::cout << "x_avg"; 
  std::cout.width(12); std::cout << "z_avg"; 
  std::cout.width(12); std::cout << "pt_avg"; 
  std::cout.width(12); std::cout << "A_pp";
  std::cout.width(12); std::cout << "A_pp_corr";
  std::cout.width(12); std::cout << "A_pp_stat";
  std::cout.width(12); std::cout << "A_kp";
  std::cout.width(12); std::cout << "A_kp_corr";
  std::cout.width(12); std::cout << "A_kp_stat" << std::endl; 
  

  for (int i=0; i<pionBins->GetXBins()->GetNumber()+1; i++){
    for (int j=0; j<pionBins->GetZBins()->GetNumber()+1; j++){
      for (int k=0; k<pionBins->GetPtBins()->GetNumber()+1; k++){

	std::cout << std::fixed; 
	std::cout.width(12); std::cout << i;  
	std::cout.width(12); std::cout << j;  
	std::cout.width(12); std::cout << k;  
	std::cout.width(12); std::cout << pionBins->GetXBins()->GetBin(i-1).GetCenter();  
	std::cout.width(12); std::cout << pionBins->GetZBins()->GetBin(j-1).GetCenter();  
	std::cout.width(12); std::cout << pionBins->GetPtBins()->GetBin(k-1).GetCenter();

	std::cout << std::scientific; 
	std::cout.width(12); std::cout << pp_int.GetAsymmetryX(i, j, k);
	std::cout.width(12); std::cout << pp_int_corr.GetAsymmetryX(i, j, k);
	std::cout.width(12); std::cout << pp_int.GetStatErrorX(i, j, k);
	std::cout.width(12); std::cout << kp_int.GetAsymmetryX(i, j, k);
	std::cout.width(12); std::cout << kp_int_corr.GetAsymmetryX(i, j, k);
	std::cout.width(12); std::cout << kp_int.GetStatErrorX(i, j, k) << std::endl; 


	//	std::cout.width(12); std::cout << signalFitter.GetPionSignal(i, j, k); 
	//	std::cout.width(12); std::cout << signalFitter.GetKaonSignal(i, j, k) << std::endl; 
      }
    }
  }

  /*


  Histos kph("test", Meson::kKaonPositive);
  kph.Load(inputFile);

  Histos kmh("test", Meson::kKaonNegative);
  kmh.Load(inputFile);

  Histos pph("test", Meson::kPionPositive);
  pph.Load(inputFile);

  Histos pmh("test", Meson::kPionNegative);
  pmh.Load(inputFile);

  Fits kpf("test", Meson::kKaonPositive);
  kpf.Load(inputFile);

  Fits kmf("test", Meson::kKaonNegative);
  kmf.Load(inputFile);

  Fits ppf("test", Meson::kPionPositive);
  ppf.Load(inputFile);

  Fits pmf("test", Meson::kPionNegative);
  pmf.Load(inputFile);

  PidHistos pp_pid("test", Meson::kPionPositive);
  pp_pid.Load(inputFile);

  PidHistos pm_pid("test", Meson::kPionNegative);
  pm_pid.Load(inputFile);

  PidHistos kp_pid("test", Meson::kKaonPositive);
  kp_pid.Load(inputFile);

  PidHistos km_pid("test", Meson::kKaonNegative);
  km_pid.Load(inputFile);
  */

  return 0;
}
