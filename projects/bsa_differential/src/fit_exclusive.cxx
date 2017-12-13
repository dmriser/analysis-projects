
#include <iostream>

#include "common/Fits.h"
#include "common/Fitter.h"
#include "common/Histograms.h"
#include "common/PIDHistograms.h"
#include "common/IntegratedHistograms.h"
#include "common/SignalBackground.h"
#include "common/Types.h"

#include "CommonTools.h"

int main(int nargs, char *args[]){

  
  if (nargs < 2){
    std::cout << " Please pass in the root file which contains asymmetry histograms. " << std::endl; 
    return 0;
  }
  
  std::string inputFile(args[1]); 

  Histos kp_hist("exclusive", Meson::kKaonPositive); 
  kp_hist.Load(inputFile);
  kp_hist.CalculateAsymmetry(); 
  kp_hist.Save(inputFile, "update"); 

  Histos km_hist("exclusive", Meson::kKaonNegative); 
  km_hist.Load(inputFile);
  km_hist.CalculateAsymmetry(); 
  km_hist.Save(inputFile, "update"); 

  Histos pp_hist("exclusive", Meson::kPionPositive); 
  pp_hist.Load(inputFile);
  pp_hist.CalculateAsymmetry(); 
  pp_hist.Save(inputFile, "update");

  Histos pm_hist("exclusive", Meson::kPionNegative); 
  pm_hist.Load(inputFile);
  pm_hist.CalculateAsymmetry(); 
  pm_hist.Save(inputFile, "update"); 

  Fits kp_fit(&kp_hist, "exclusive", Meson::kKaonPositive); 
  Fits km_fit(&km_hist, "exclusive", Meson::kKaonNegative); 
  Fits pp_fit(&pp_hist, "exclusive", Meson::kPionPositive); 
  Fits pm_fit(&pm_hist, "exclusive", Meson::kPionNegative); 

  // Uses just the A sin(phi) 
  BasicFitter fitter; 

  // Uses A sin(phi)/(1 + B cos(phi) + C cos(2phi))
  //  BasicAllMomentFitter fitter; 
  fitter.Fit(&kp_hist, &kp_fit); 
  fitter.Fit(&km_hist, &km_fit); 
  fitter.Fit(&pp_hist, &pp_fit); 
  fitter.Fit(&pm_hist, &pm_fit); 

  kp_fit.Save(inputFile, "update"); 
  km_fit.Save(inputFile, "update");
  pp_fit.Save(inputFile, "update");
  pm_fit.Save(inputFile, "update");

  // create and save integrated histograms 
  IntegratedHistos kp_integ(&kp_fit, "exclusive_base", Meson::kKaonPositive);  kp_integ.Save(inputFile, "update"); 
  //  IntegratedHistos km_integ(&km_fit, "exclusive_base", Meson::kKaonNegative);  km_integ.Save(inputFile, "update"); 
  IntegratedHistos pp_integ(&pp_fit, "exclusive_base", Meson::kPionPositive);  pp_integ.Save(inputFile, "update"); 
  //  IntegratedHistos pm_integ(&pm_fit, "exclusive_base", Meson::kPionNegative);  pm_integ.Save(inputFile, "update"); 

  PidHistos pp_pid("exclusive", Meson::kPionPositive);
  pp_pid.Load(inputFile);


  /* 
  SignalBackgroundFitter sb_fitter(&pp_pid, "exclusive", Meson::kPionPositive); 
  sb_fitter.Fit(0.0, 0.4, 0.7); 
  sb_fitter.CorrectAsymmetry(&pp_fit, &kp_fit); 
  sb_fitter.Save(inputFile, "update"); 

  IntegratedHistos *pp_corr_int = sb_fitter.GetPionHistos(); 
  IntegratedHistos *kp_corr_int = sb_fitter.GetKaonHistos(); 
  pp_corr_int->Save(inputFile, "update"); 
  kp_corr_int->Save(inputFile, "update"); 
  */ 

  return 0;
}
