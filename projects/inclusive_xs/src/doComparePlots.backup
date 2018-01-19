
// --------------------------------
//          c++
// --------------------------------
#include <iostream>
#include <vector>
using std::cout; 
using std::endl; 
using std::vector; 
using std::string; 

// --------------------------------
//       my libs   
// --------------------------------
#include "h22Option.h"
#include "Printer.h"
#include "StandardHistograms.h"

// --------------------------------
//           root
// --------------------------------
#include "TFile.h"
#include "TH1.h"

int main(int argc, char *argv[]){

  h22Options *opts = new h22Options(); 

  opts->args["DATA"].args = "UNSET"; 
  opts->args["DATA"].type = 1; 
  opts->args["DATA"].name = "Input filename."; 
  opts->args["MC"].args = "UNSET"; 
  opts->args["MC"].type = 1; 
  opts->args["MC"].name = "Input filename."; 
  opts->args["STYLE"].args = "default"; 
  opts->args["STYLE"].type = 1; 
  opts->args["STYLE"].name = "Styling of histograms"; 
  opts->args["OUT"].args = "/volatile/clas12/dmriser/plots/inclusive/";
  opts->set(argc, argv);

  SimplePlot::Printer plotter; 
  SimplePlot::Tools   tools; 
  
  plotter.setPath(opts->args["OUT"].args);
  plotter.setStyle(opts->args["STYLE"].args);

  StandardHistograms dataHistos("dataPassed",1);
  StandardHistograms mcHistos("passedRec",1);
  
  dataHistos.Load(opts->args["DATA"].args);
  mcHistos.Load(opts->args["MC"].args);

  plotter.setOutputTitle("compareProtonMass");
  plotter.printSectorsCompare(&dataHistos.h1_wProtonMass[1], &mcHistos.h1_wProtonMass[1], "E1-F Inclusive Sample", "W (GeV/c^{2})", "Events");

  plotter.setOutputTitle("compareTheta");
  plotter.printSectorsCompare(&dataHistos.h1_ele_theta[1], &mcHistos.h1_ele_theta[1], "E1-F Inclusive Sample", "#theta", "Events");

  plotter.setOutputTitle("compareRelPhi");
  plotter.printSectorsCompare(&dataHistos.h1_ele_relPhi[1], &mcHistos.h1_ele_relPhi[1], "E1-F Inclusive Sample", "#phi_{Rel}", "Events");

  plotter.setOutputTitle("comparePhi");
  plotter.printSectorsCompare(&dataHistos.h1_ele_phi[1], &mcHistos.h1_ele_phi[1], "E1-F Inclusive Sample", "#phi", "Events");
  
  plotter.setOutputTitle("compareP");
  plotter.printSectorsCompare(&dataHistos.h1_ele_p[1], &mcHistos.h1_ele_p[1], "E1-F Inclusive Sample", "p (GeV/c)", "Events");
  
  plotter.setOutputTitle("compareRatioW");
  plotter.printSectorsCompareWithRatio(&dataHistos.h1_w[1], &mcHistos.h1_w[1], "E1-F Inclusive Sample", "W (GeV/c^{2})", "Events (Ratio)");
  
  plotter.setOutputTitle("compareW");
  plotter.printSectorsCompare(&dataHistos.h1_w[1], &mcHistos.h1_w[1], "E1-F Inclusive Sample", "W (GeV/c^{2})", "Events");
  
  plotter.setOutputTitle("compareRatioQ2");
  plotter.printSectorsCompareWithRatio(&dataHistos.h1_q2[1], &mcHistos.h1_q2[1], "E1-F Inclusive Sample", "Q^{2} (GeV^{2}/c^{2})", "Events (Ratio)");
  
  plotter.setOutputTitle("compareQ2");
  plotter.printSectorsCompare(&dataHistos.h1_q2[1], &mcHistos.h1_q2[1], "E1-F Inclusive Sample", "Q^{2} (GeV^{2}/c^{2})", "Events");
  
  plotter.setOutputTitle("compareRatioX");
  plotter.printSectorsCompareWithRatio(&dataHistos.h1_xbj[1], &mcHistos.h1_xbj[1], "E1-F Inclusive Sample", "x_{Bjorken}", "Events (Ratio)");
  
  plotter.setOutputTitle("compareX");
  plotter.printSectorsCompare(&dataHistos.h1_xbj[1], &mcHistos.h1_xbj[1], "E1-F Inclusive Sample", "x_{Bjorken}", "Events");

  return 0;
}
