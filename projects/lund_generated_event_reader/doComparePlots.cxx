
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

  opts->args["FILE1"].args = "UNSET"; 
  opts->args["FILE1"].type = 1; 
  opts->args["FILE1"].name = "Input filename."; 
  opts->args["FILE2"].args = "UNSET"; 
  opts->args["FILE2"].type = 1; 
  opts->args["FILE2"].name = "Input filename."; 
  opts->args["STYLE"].args = "default"; 
  opts->args["STYLE"].type = 1; 
  opts->args["STYLE"].name = "Styling of histograms"; 
  opts->args["OUT"].args   = "/volatile/clas12/dmriser/plots/elastic/generated/";
  opts->set(argc, argv);

  SimplePlot::Printer plotter; 
  SimplePlot::Tools   tools; 
  
  plotter.setPath(opts->args["OUT"].args);
  plotter.setStyle(opts->args["STYLE"].args);

  StandardHistograms histos1("generatedEvents",1);
  StandardHistograms histos2("generatedEvents",1);
  
  histos1.Load(opts->args["FILE1"].args);
  histos2.Load(opts->args["FILE2"].args);

  plotter.setOutputTitle("compareProtonMass");
  plotter.printSectorsCompare(&histos1.h1_wProtonMass[1], &histos2.h1_wProtonMass[1], "E1-F Elastic Sample", "W (GeV/c^{2})", "Events");

  plotter.setOutputTitle("compareTheta");
  plotter.printSectorsCompare(&histos1.h1_ele_theta[1], &histos2.h1_ele_theta[1], "E1-F Elastic Sample", "#theta", "Events");

  plotter.setOutputTitle("compareRelPhi");
  plotter.printSectorsCompare(&histos1.h1_ele_relPhi[1], &histos2.h1_ele_relPhi[1], "E1-F Elastic Sample", "#phi_{Rel}", "Events");

  plotter.setOutputTitle("comparePhi");
  plotter.printSectorsCompare(&histos1.h1_ele_phi[1], &histos2.h1_ele_phi[1], "E1-F Elastic Sample", "#phi", "Events");
  
  plotter.setOutputTitle("compareP");
  plotter.printSectorsCompare(&histos1.h1_ele_p[1], &histos2.h1_ele_p[1], "E1-F Elastic Sample", "p (GeV/c)", "Events");
  
  plotter.setOutputTitle("compareW");
  plotter.printSectorsCompare(&histos1.h1_w[1], &histos2.h1_w[1], "E1-F Elastic Sample", "W (GeV/c^{2})", "Events");
  
  plotter.setOutputTitle("compareQ2");
  plotter.printSectorsCompare(&histos1.h1_q2[1], &histos2.h1_q2[1], "E1-F Elastic Sample", "Q^{2} (GeV^{2}/c^{2})", "Events");

  return 0;
}
