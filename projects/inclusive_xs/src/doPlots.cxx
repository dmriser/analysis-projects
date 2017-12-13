#include <iostream>
using std::cout; 
using std::endl; 
using std::string;

#include "common/DIS1DHistograms.h"
#include "h22Option.h"

// Root Things
#include "TH1.h"
#include "TH2.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TPad.h"
#include "TCanvas.h"

class Plot{
public:
  Plot(){ }
  ~Plot(){ }
};

int main(int argc, char *argv[]){

  h22Options *options = new h22Options();
  options->args["INPUT"].args = "unset"; 
  options->args["INPUT"].type = 1; 
  options->args["INPUT"].name = "Input file"; 
  options->set(argc, argv); 

  if (  options->args["INPUT"].args != "unset"){
    
  }
  else {
    cout << "Send input file with -INPUT=yourFile.root" << endl; 
  }

  return 0;
} 
