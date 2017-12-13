
#include <iostream>

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
  
  std::string inputFile(args[1]); 

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

  Plotter gridPlotter; 
  gridPlotter.PlotGridZX(&kph, &kpf, Meson::kKaonPositive); 
  gridPlotter.PlotGridZX(&kmh, &kmf, Meson::kKaonNegative); 
  gridPlotter.PlotGridZX(&pph, &ppf, Meson::kPionPositive); 
  gridPlotter.PlotGridZX(&pmh, &pmf, Meson::kPionNegative); 

  gridPlotter.PlotGridZPt(&kph, &kpf, Meson::kKaonPositive); 
  gridPlotter.PlotGridZPt(&kmh, &kmf, Meson::kKaonNegative); 
  gridPlotter.PlotGridZPt(&pph, &ppf, Meson::kPionPositive); 
  gridPlotter.PlotGridZPt(&pmh, &pmf, Meson::kPionNegative); 


  IntegratedHistos kp_int(inputFile, "base", Meson::kKaonPositive); 
  IntegratedHistos km_int(inputFile, "base", Meson::kKaonNegative); 
  IntegratedHistos pp_int(inputFile, "base", Meson::kPionPositive); 
  IntegratedHistos pm_int(inputFile, "base", Meson::kPionNegative); 

  IntegratedPlotter plot_int;
  plot_int.Add(&kp_int, Meson::kKaonPositive); 
  plot_int.Add(&km_int, Meson::kKaonNegative); 
  plot_int.Add(&pp_int, Meson::kPionPositive); 
  plot_int.Add(&pm_int, Meson::kPionNegative); 

  plot_int.PlotX(); 
  plot_int.PlotZ(); 
  plot_int.PlotPt(); 
  plot_int.PlotXByZ(Meson::kPionPositive);
  plot_int.PlotXByZ(Meson::kKaonPositive);
  plot_int.PlotSummary(); 

  PidHistos pp_pid("test", Meson::kPionPositive);
  pp_pid.Load(inputFile);

  PidHistos pm_pid("test", Meson::kPionNegative);
  pm_pid.Load(inputFile);

  PidHistos kp_pid("test", Meson::kKaonPositive);
  kp_pid.Load(inputFile);

  PidHistos km_pid("test", Meson::kKaonNegative);
  km_pid.Load(inputFile);

  PidPlotter pidPlotter; 
  pidPlotter.PlotGridZPt(&pp_pid, Meson::kPionPositive); 
  pidPlotter.PlotGridZX(&pp_pid, Meson::kPionPositive); 
  pidPlotter.PlotGridZPt(&kp_pid, Meson::kKaonPositive); 
  pidPlotter.PlotGridZX(&kp_pid, Meson::kKaonPositive); 
  pidPlotter.PlotGridZPt(&pm_pid, Meson::kPionNegative); 
  pidPlotter.PlotGridZX(&pm_pid, Meson::kPionNegative); 
  pidPlotter.PlotGridZPt(&km_pid, Meson::kKaonNegative); 
  pidPlotter.PlotGridZX(&km_pid, Meson::kKaonNegative); 

  //  SignalBackgroundFitter signalFitter("test", Meson::kPionPositive);
  //  signalFitter.Load(inputFile); 

  //  pidPlotter.PlotGridZXWithFits(&kp_pid, &signalFitter, Meson::kKaonPositive); 

  return 0;
}
