
#include <iostream>

#include "common/Fits.h"
#include "common/Fitter.h"
#include "common/Histograms.h"
#include "common/PIDHistograms.h"
#include "common/IntegratedHistograms.h"
#include "common/SignalBackground.h"
#include "common/Types.h"

#include "CommonTools.h"
#include "Parameters.h"

int main(int nargs, char *args[]){

  
  if (nargs < 2){
    std::cout << " Please pass in the root file which contains asymmetry histograms. " << std::endl; 
    return 0;
  }
  
  std::string inputFile(args[1]); 

  Histos kp_hist("test", Meson::kKaonPositive); 
  kp_hist.Load(inputFile);
  kp_hist.CalculateAsymmetry(); 
  kp_hist.Save(inputFile, "update"); 

  Histos km_hist("test", Meson::kKaonNegative); 
  km_hist.Load(inputFile);
  km_hist.CalculateAsymmetry(); 
  km_hist.Save(inputFile, "update"); 

  Histos pp_hist("test", Meson::kPionPositive); 
  pp_hist.Load(inputFile);
  pp_hist.CalculateAsymmetry(); 
  pp_hist.Save(inputFile, "update");

  Histos pp_ex_hist("exclusive", Meson::kPionPositive); 
  pp_hist.Load(inputFile);
  pp_hist.CalculateAsymmetry(); 
  pp_hist.Save(inputFile, "update");

  Histos pm_hist("test", Meson::kPionNegative); 
  pm_hist.Load(inputFile);
  pm_hist.CalculateAsymmetry(); 
  pm_hist.Save(inputFile, "update"); 

  Fits kp_fit(&kp_hist, "test", Meson::kKaonPositive); 
  Fits km_fit(&km_hist, "test", Meson::kKaonNegative); 
  Fits pp_fit(&pp_hist, "test", Meson::kPionPositive); 
  Fits pp_ex_fit(&pp_ex_hist, "exclusive", Meson::kPionPositive); 
  Fits pm_fit(&pm_hist, "test", Meson::kPionNegative); 

  Fits kp_allfit(&kp_hist, "full", Meson::kKaonPositive); 
  Fits km_allfit(&km_hist, "full", Meson::kKaonNegative); 
  Fits pp_allfit(&pp_hist, "full", Meson::kPionPositive); 
  Fits pm_allfit(&pm_hist, "full", Meson::kPionNegative); 

  // Uses just the A sin(phi) 
  BasicFitter basicFitter; 
  basicFitter.Fit(&kp_hist, &kp_fit); 
  basicFitter.Fit(&km_hist, &km_fit); 
  basicFitter.Fit(&pp_hist, &pp_fit); 
  basicFitter.Fit(&pp_ex_hist, &pp_ex_fit); 
  basicFitter.Fit(&pm_hist, &pm_fit); 

  // Uses A sin(phi)/(1 + B cos(phi) + C cos(2phi))
  BasicAllMomentFitter notBasicFitter; 
  notBasicFitter.Fit(&kp_hist, &kp_allfit); 
  notBasicFitter.Fit(&km_hist, &km_allfit); 
  notBasicFitter.Fit(&pp_hist, &pp_allfit); 
  notBasicFitter.Fit(&pm_hist, &pm_allfit); 

  kp_fit.Save(inputFile, "update"); 
  km_fit.Save(inputFile, "update");
  pp_fit.Save(inputFile, "update");
  pp_ex_fit.Save(inputFile, "update");
  pm_fit.Save(inputFile, "update");

  kp_allfit.Save(inputFile, "update"); 
  km_allfit.Save(inputFile, "update");
  pp_allfit.Save(inputFile, "update");
  pm_allfit.Save(inputFile, "update");

  // create and save integrated histograms 
  IntegratedHistos kp_integ(&kp_fit, "base", Meson::kKaonPositive);  kp_integ.Save(inputFile, "update"); 
  IntegratedHistos km_integ(&km_fit, "base", Meson::kKaonNegative);  km_integ.Save(inputFile, "update"); 
  IntegratedHistos pp_integ(&pp_fit, "base", Meson::kPionPositive);  pp_integ.Save(inputFile, "update"); 
  IntegratedHistos pp_ex_integ(&pp_ex_fit, "exclusive", Meson::kPionPositive);  pp_ex_integ.Save(inputFile, "update"); 
  IntegratedHistos pm_integ(&pm_fit, "base", Meson::kPionNegative);  pm_integ.Save(inputFile, "update"); 

  PidHistos pp_pid("test", Meson::kPionPositive);
  pp_pid.Load(inputFile);

  /* 

  // parameter file is here 
  Parameters pars; 

  if (nargs == 3){
    pars.loadParameters(args[2]); 
  } else {
    std::string path = Global::Environment::GetAnalysisPath(); 
    path.append("/lists/data_tofmass.pars"); 
    pars.loadParameters(path); 
  }


  SignalBackgroundFitter sb_fitter(&pp_pid, "test", Meson::kPionPositive); 
  sb_fitter.Fit(pars.getParameter("PIP_TOFMASS_MIN").getValue(0), 
		pars.getParameter("KP_TOFMASS_MIN").getValue(0), 
		pars.getParameter("KP_TOFMASS_MAX").getValue(0)); 
  sb_fitter.CorrectAsymmetry(&pp_fit, &kp_fit); 
  sb_fitter.Save(inputFile, "update"); 

  IntegratedHistos *pp_corr_int = sb_fitter.GetPionHistos(); 
  IntegratedHistos *kp_corr_int = sb_fitter.GetKaonHistos(); 
  
  pp_corr_int->Save(inputFile, "update"); 
  kp_corr_int->Save(inputFile, "update"); 

  */ 

  return 0;
}
