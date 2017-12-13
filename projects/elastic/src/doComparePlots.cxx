
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
  opts->args["OUT"].args = "/volatile/clas12/dmriser/plots/elastic/";
  opts->set(argc, argv);

  SimplePlot::Printer plotter; 
  SimplePlot::Tools   tools; 
  
  plotter.setPath(opts->args["OUT"].args);
  plotter.setStyle(opts->args["STYLE"].args);

  StandardHistograms dataHistos("data",1);
  StandardHistograms mcHistos("rec",1);
  
  dataHistos.Load(opts->args["DATA"].args);
  mcHistos.Load(opts->args["MC"].args);

  plotter.setOutputTitle("compareProtonMass");
  plotter.printSectorsCompare(&dataHistos.h1_wProtonMass[1], &mcHistos.h1_wProtonMass[1], "E1-F Elastic Sample", "W (GeV/c^{2})", "Events");

  plotter.setOutputTitle("compareTheta");
  plotter.printSectorsCompare(&dataHistos.h1_ele_theta[1], &mcHistos.h1_ele_theta[1], "E1-F Elastic Sample", "#theta", "Events");

  plotter.setOutputTitle("compareRelPhi");
  plotter.printSectorsCompare(&dataHistos.h1_ele_relPhi[1], &mcHistos.h1_ele_relPhi[1], "E1-F Elastic Sample", "#phi_{Rel}", "Events");

  plotter.setOutputTitle("comparePhi");
  plotter.printSectorsCompare(&dataHistos.h1_ele_phi[1], &mcHistos.h1_ele_phi[1], "E1-F Elastic Sample", "#phi", "Events");
  
  plotter.setOutputTitle("compareP");
  plotter.printSectorsCompare(&dataHistos.h1_ele_p[1], &mcHistos.h1_ele_p[1], "E1-F Elastic Sample", "p (GeV/c)", "Events");
  
  plotter.setOutputTitle("compareW");
  plotter.printSectorsCompare(&dataHistos.h1_w[1], &mcHistos.h1_w[1], "E1-F Elastic Sample", "W (GeV/c^{2})", "Events");
  
  plotter.setOutputTitle("compareQ2");
  plotter.printSectorsCompare(&dataHistos.h1_q2[1], &mcHistos.h1_q2[1], "E1-F Elastic Sample", "Q^{2} (GeV^{2}/c^{2})", "Events");

  return 0;
}
