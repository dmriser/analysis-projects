/* ########################################################################
   #                                                                      #
   #               David Riser, University of Connecticut                 #
   #                      October 14, 201  6                              #
   #                                                                      #
   ######################################################################## */

#include <iostream>
#include <fstream>
using namespace std;

#include "h22Option.h"
#include "DInformation.h"
#include "MomCorr.h"
#include "Parameters.h"
#include "PhysicsEventCut.h"
#include "PhysicsEventSelector.h"

#include "common/DataLoader.h"
#include "common/FaradayCupLoader.h"
#include "common/MCLoader.h"

int PrintUsage();
void configureCommandLineOptions(h22Options * theseOpts); 

int main(int argc, char * argv[]){

  // ---------------------------- Setup Physics Options -------------------------------

  PhysicsEventSelector * eventSelector = new PhysicsEventSelector();
  PhysicsEventCut_w * w_cut = new PhysicsEventCut_w();
  PhysicsEventCut_y * y_cut = new PhysicsEventCut_y();
  PhysicsEventCut_qq * qq_cut = new PhysicsEventCut_qq();

  w_cut->set_min( 1.10 );
  w_cut->set_max( 99.9 ); 
  y_cut->set_min( 0.00 );
  y_cut->set_max( 0.7 ); 
  qq_cut->set_min( 1.00 );
  qq_cut->set_max( 99.9 ); 

  eventSelector->add_cut(w_cut);
  eventSelector->add_cut(y_cut);
  eventSelector->add_cut(qq_cut); 
  eventSelector->enable_all();

  // ----------------------------------------------------------------------------------
  //  
  // ------------------------ Boring Bits that make shit work -------------------------
  //
  // Check for improper usage of this code. 
  if (argc < 3){
    return PrintUsage();
  }

  // Collect command line options, configure ours and compare.
  h22Options * options = new h22Options(); 
  configureCommandLineOptions(options); 
  options->set(argc, argv);

  // Set local variables to command line flags. 
  string fileList = options->args["LIST"].args; 
  string outputFilename = options->args["OUT"].args;
  int numberOfFilesToProcess = options->args["N"].arg; 

  Parameters *pars = new Parameters();
  pars->loadParameters(options->args["PARS"].args); 

  // Check for no files, then setup the list of files to process correctly. 
  if (numberOfFilesToProcess == 0){ cout << " No files/list detected. " << endl; return 0; }
  vector<string> files;

  if (fileList != "UNSET") {
    files = loadFilesFromList(fileList, numberOfFilesToProcess);
  } else {
    files = loadFilesFromCommandLine(options, numberOfFilesToProcess);
  }

  // -----------------------------------------------
  //     User Parameters For Binning 
  // -----------------------------------------------

  const int NCONF           = 8; 
  const int oldNumberOfBins = 200;
  int numberWBins[NCONF]    = {80, 35, 35, 35, 30, 30, 30, 20};
  int numberQQBins[NCONF]   = {20, 10,  6,  4, 10,  6,  4,  6};
  string name[NCONF]        = {"80Bins20Bins","35Bins10Bins","35Bins6Bins","35Bins4Bins","30Bins10Bins","30Bins6Bins","30Bins4Bins","20Bins6Bins"};

  // -----------------------------------------------
  //                  Main Loop  
  // -----------------------------------------------

  for(int c=0; c<NCONF; c++){
    int wRebinFactor  = (int) oldNumberOfBins/numberWBins[c];
    int qqRebinFactor = (int) oldNumberOfBins/numberQQBins[c];

    cout << "[Main] Starting loop for " << name[c] << endl;
    cout << "[Main] wRebinFactor:  " << wRebinFactor << endl;     
    cout << "[Main] qqRebinFactor: " << qqRebinFactor << endl;     

    MCLoader *loader = new MCLoader(eventSelector, pars, outputFilename, "UPDATE",name[c]);
    for (int ifile = 0; ifile < files.size(); ifile++) { loader->AddFile(files[ifile]); }
    loader->Initialize();
    loader->Rebin(wRebinFactor,qqRebinFactor);
    loader->Loop();
    loader->Save();  
    delete loader; 
    cout << "[Main] Finished loop for " << name[c] << endl;
  }

  return 0;
}

// -----------------------------------------------
//      Useful Functions 
// -----------------------------------------------

void configureCommandLineOptions(h22Options * theseOpts){

  theseOpts->args["N"].arg = 0;
  theseOpts->args["N"].type = 0;
  theseOpts->args["N"].name = "Number of files to process";
  
  theseOpts->args["LIST"].args = "UNSET";
  theseOpts->args["LIST"].type = 1;
  theseOpts->args["LIST"].name = "Process list of files";
  
  theseOpts->args["PARS"].args = "/u/home/dmriser/mydoc/analysis/root_scripts/analysis-main/lists/keppelRad.pars";
  theseOpts->args["PARS"].type = 1;
  theseOpts->args["PARS"].name = "Parameter file";
  
}

int PrintUsage(){

  cout << endl;
  cout << endl; 
  cout << "######################################################" << endl;
  cout << "#                                                    #" << endl;
  cout << "#    fillPurityHistograms -> fills purity histograms #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#  This code requires a set of files specified by    #" << endl;
  cout << "#  a list using -LIST=files.dat, or by wildcard      #" << endl;
  cout << "#  after the option flags.                           #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#  Set the number of files using the -N= flag        #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#  Set the output file using the -OUT= flag.         #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#  Sample usage:                                     #" << endl;
  cout << "#  ./fillPurityHistograms -N=1000                    #" << endl;
  cout << "#   -LIST=goodFiles.dat                              #" << endl;
  cout << "#   -OUT=pass1.root                                  #" << endl;
  cout << "#                                                    #" << endl;
  cout << "######################################################" << endl;
  cout << endl;
  cout << endl;
  
  return 0;
} 
