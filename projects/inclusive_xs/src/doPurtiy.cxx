/* ########################################################################
   #                                                                      #
   #               David Riser, University of Connecticut                 #
   #                         October 6, 2016                              #
   #                                                                      #
   ######################################################################## */
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#include "CommonTools.h"
#include "h22Option.h"

#include "common/BaseDISHistograms.h"
#include "common/DIS1DHistograms.h"
#include "common/FaradayCupLoader.h"

int PrintUsage();
void configureCommandLineOptions(h22Options * theseOpts); 

int main(int argc, char * argv[]){

  // Check for improper usage of this code. 
  if (argc < 3){
    return PrintUsage();
  }

  // Collect command line options, configure ours and compare.
  h22Options * options = new h22Options(); 
  configureCommandLineOptions(options); 
  options->set(argc, argv);

  string inputFilename = options->args["INPUT"].args; 
  string outputFilename = options->args["OUT"].args; 

  if (inputFilename != "UNSET"){

    const int numberConfigs = 8;
    int numberBins[numberConfigs] = {200, 100, 66, 50, 40, 34, 20, 10};

    BaseDISHistograms *recEvents2D[numberConfigs];
    BaseDISHistograms *genEvents2D[numberConfigs];
    BaseDISHistograms *recAndGenEvents2D[numberConfigs];
    DIS1DHistograms *recEvents[numberConfigs];
    DIS1DHistograms *genEvents[numberConfigs];
    DIS1DHistograms *recAndGenEvents[numberConfigs];
    DIS1DHistograms *purity[numberConfigs];
    DIS1DHistograms *stability[numberConfigs];

    for (int conf=0; conf<numberConfigs; conf++){
      string recTitle = Form("recEvents%dBins",numberBins[conf]);
      string genTitle = Form("genEvents%dBins",numberBins[conf]);
      string recAndGenTitle = Form("recAndGenEvents%dBins",numberBins[conf]);
      string purityTitle = Form("purity%dBins",numberBins[conf]);    
      string stabilityTitle = Form("stability%dBins",numberBins[conf]);    

      recEvents2D[conf] = new BaseDISHistograms();
      genEvents2D[conf] = new BaseDISHistograms();
      recAndGenEvents2D[conf] = new BaseDISHistograms();
      recEvents[conf] = new DIS1DHistograms();
      genEvents[conf] = new DIS1DHistograms();
      recAndGenEvents[conf] = new DIS1DHistograms();
      purity[conf] = new DIS1DHistograms();
      stability[conf] = new DIS1DHistograms();

      recEvents2D[conf]->Load(inputFilename.c_str(),recTitle.c_str());
      genEvents2D[conf]->Load(inputFilename.c_str(),genTitle.c_str());
      recAndGenEvents2D[conf]->Load(inputFilename.c_str(),recAndGenTitle.c_str());
      recEvents[conf]->Create(recEvents2D[conf]);
      genEvents[conf]->Create(genEvents2D[conf]);
      recAndGenEvents[conf]->Create(recAndGenEvents2D[conf]);
      recEvents[conf]->SetErrors();
      genEvents[conf]->SetErrors();
      recAndGenEvents[conf]->SetErrors();
      recEvents[conf]->Save(outputFilename.c_str(),"update");
      genEvents[conf]->Save(outputFilename.c_str(),"update");
      recAndGenEvents[conf]->Save(outputFilename.c_str(),"update");

      purity[conf]->CreateByDivision(recAndGenEvents[conf],recEvents[conf],purityTitle,purityTitle);
      purity[conf]->Save(outputFilename.c_str(),"update");
      stability[conf]->CreateByDivision(recAndGenEvents[conf],genEvents[conf],stabilityTitle,stabilityTitle);
      stability[conf]->Save(outputFilename.c_str(),"update");
    }
  }

  return 0;
}

void configureCommandLineOptions(h22Options * theseOpts){
  theseOpts->args["INPUT"].args = "UNSET";
  theseOpts->args["INPUT"].type = 1;
  theseOpts->args["INPUT"].name = "Processed file from fillHistograms";
}

int PrintUsage(){

  cout << endl;
  cout << endl; 
  cout << "######################################################" << endl;
  cout << "#                                                    #" << endl;
  cout << "#    doCrossSection -> Used to fill CS histograms    #" << endl;
  cout << "#     after fillHistograms has been run.             #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#   This code requires an input/output flagged       #" << endl;
  cout << "#   -INPUT=filename.root    -OUT=outputFile.root     #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#   You may also choose to specify -RADCORR=true     #" << endl;
  cout << "#   if your file contains radiative corrections.     #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#   You may also choose to specify -BINCORR=true     #" << endl;
  cout << "#   if your file contains bin centering corrections. #" << endl;
  cout << "#                                                    #" << endl;
  cout << "######################################################" << endl;
  cout << endl;
  cout << endl;
  
  return 0;
} 
