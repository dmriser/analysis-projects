
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

// --------------------------------
//           root
// --------------------------------
#include "TFile.h"
#include "TH1.h"

int main(int argc, char *argv[]){

  h22Options *opts = new h22Options(); 

  opts->args["INPUT"].args = "UNSET"; 
  opts->args["INPUT"].type = 1; 
  opts->args["INPUT"].name = "Input filename."; 
  opts->args["STYLE"].args = "default"; 
  opts->args["STYLE"].type = 1; 
  opts->args["STYLE"].name = "Styling of histograms"; 
  opts->args["OUT"].args = "/volatile/clas12/dmriser/plots/elastic/";
  opts->set(argc, argv);

  SimplePlot::Printer plotter; 
  SimplePlot::Tools   tools; 
  
  plotter.setPath(opts->args["OUT"].args);
  plotter.setStyle(opts->args["STYLE"].args);

  TFile *inputFile = TFile::Open(opts->args["INPUT"].args.c_str());
  
  if(inputFile->IsOpen()){
    TH1D *dataEvents[6]; 
    TH1D *acc[6]; 
    TH1D *ratio[6]; 
    TH1D *xs[6]; 
    TH1D *model = (TH1D*) inputFile->Get("model_theta"); 

    for(int h=0; h<6; h++){
      dataEvents[h] = (TH1D*) inputFile->Get(Form("dataEvents_thetaByPhi_sect%d_bin0",h));
      ratio[h]      = (TH1D*) inputFile->Get(Form("ratio_thetaByPhi_sect%d_bin0",h));
      xs[h]         = (TH1D*) inputFile->Get(Form("crossSection_thetaByPhi_sect%d_bin0",h));
//      acc[h]        = (TH1D*) inputFile->Get(Form("acc_thetaByPhi_sect%d_bin0",h));
    }

    plotter.setOutputTitle("data");
    plotter.printSectors(dataEvents, "E1-F Elastic Sample", "#theta", "Events");

//    plotter.setOutputTitle("acc");
//    plotter.printSectors(acc, "E1-F Elastic Sample", "#theta", "Rec/Gen");

    plotter.setStyle("lineMarker");
    plotter.setOutputTitle("ratio");
    plotter.printSectors(ratio, "E1-F Elastic Sample", "#theta", "Ratio");

    plotter.setOutputTitle("xs");
    plotter.printSectorsCompare(xs, model, "E1-F Elastic Sample", "#theta", "#muB");

  } else {
    cout << "[doPlots] Failed to open " << opts->args["INPUT"].args << endl;
  }

  inputFile->Close(); 

  return 0;
}
