/* ########################################################################
   #                                                                      #
   #               David Riser, University of Connecticut                 #
   #                      September 24, 2016                              #
   #                                                                      #
   ######################################################################## */

#include <iostream>
#include <fstream>
using namespace std;

#include "h22Option.h"
#include "DInformation.h"
#include "MomCorr.h"
#include "PhysicsEventCut.h"
#include "PhysicsEventSelector.h"
#include "Parameters.h"

#include "common/DataLoader.h"
#include "common/MCLoader.h"
#include "common/FaradayCupLoader.h"
#include "common/Histograms.h"

int PrintUsage();
void configureCommandLineOptions(h22Options * theseOpts); 
vector<string> loadFilesFromList(string fileList, int numFiles);
vector<string> loadFilesFromCommandLine(h22Options * theseOpts, int numFiles);

int main(int argc, char * argv[]){

  // ------------------------ Boring Bits that make shit work -------------------------
  //
  // Check for improper usage of this code. 
  if (argc < 3){
    return PrintUsage();
  }

  // Collect command line options, configure ours and compare.
  h22Options *options = new h22Options(); 
  configureCommandLineOptions(options); 
  options->set(argc, argv);

  // ---------------------------- Setup Physics Options -------------------------------

  PhysicsEventSelector        *eventSelector = new PhysicsEventSelector();
  PhysicsEventCut_w           *w_cut = new PhysicsEventCut_w();

  w_cut->set_min( options->args["WMIN"].arg );
  w_cut->set_max( options->args["WMAX"].arg ); 

  eventSelector->add_cut(w_cut);
  eventSelector->enable_all();

  // ----------------------------------------------------------------------------------
  //  

  // Set local variables to command line flags. 
  string runMode             = options->args["TYPE"].args; 
  string fileList            = options->args["LIST"].args; 
  string outputFilename      = options->args["OUT"].args;
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

  // Start running the correct type
  if (runMode == "data"){
    MomCorr_e1f *momentumCorrection = new MomCorr_e1f("/u/home/dmriser/analysis-main/momCorr/");
//    MomCorr_e1f *momentumCorrection = new MomCorr_e1f("/Users/dmriser/Work/analysis-main/momCorr/");
    DataLoader loader(eventSelector, momentumCorrection, pars, outputFilename, "RECREATE");
    for (int ifile = 0; ifile < files.size(); ifile++) { loader.AddFile(files[ifile]); }
    loader.Execute();

    DInformation *runInformation = new DInformation();
    runInformation->load("/u/home/dmriser/analysis-main/lists/runs.info");
//    runInformation->load("/Users/dmriser/Work/analysis-main/lists/runs.info");

    FaradayCupAppender chargeAppender(runInformation);
    chargeAppender.AddFiles(files);
    chargeAppender.AppendToFile(outputFilename);
  }

  else if (runMode == "mc"){
    MCLoader loader(eventSelector, pars, outputFilename, "RECREATE");
    for (int ifile = 0; ifile < files.size(); ifile++) { loader.AddFile(files[ifile]); }
    loader.Execute();
  }

  else {
    cout << " TYPE not recognized: " << runMode << endl;
    return 0; 
  }


  return 0;
}

void configureCommandLineOptions(h22Options * theseOpts){

  theseOpts->args["N"].arg = 0;
  theseOpts->args["N"].type = 0;
  theseOpts->args["N"].name = "Number of files to process";

  theseOpts->args["WMIN"].arg = 0.6;
  theseOpts->args["WMIN"].type = 0;
  theseOpts->args["WMIN"].name = "W Min. ";
 
  theseOpts->args["WMAX"].arg = 1.1;
  theseOpts->args["WMAX"].type = 0;
  theseOpts->args["WMAX"].name = "W Min. ";
 
  theseOpts->args["TYPE"].args = "UNSET";
  theseOpts->args["TYPE"].type = 1;
  theseOpts->args["TYPE"].name = "Processing mode";
   
  theseOpts->args["LIST"].args = "UNSET";
  theseOpts->args["LIST"].type = 1;
  theseOpts->args["LIST"].name = "Process list of files";
  
  theseOpts->args["PARS"].args = "/u/home/dmriser/mydoc/analysis/root_scripts/analysis-main/lists/pars.dat";
  theseOpts->args["PARS"].type = 1;
  theseOpts->args["PARS"].name = "Parameter File";
  
}

vector<string> loadFilesFromList(string fileList, int numFiles){
  vector<string> theseFiles; 

  ifstream inputFile; 
  inputFile.open(fileList.c_str());

  int ifile = 0; string line;
  while (getline(inputFile, line) && ifile < numFiles){
    theseFiles.push_back(line);
    ifile++;
  }
  
  inputFile.close();
  return theseFiles; 
}

vector<string> loadFilesFromCommandLine(h22Options * theseOpts, int numFiles){
  vector<string> theseFiles; 

  for(int ifile = 0; ifile < theseOpts->ifiles.size(); ifile++){
    theseFiles.push_back(theseOpts->ifiles[ifile]);
    ifile++;

    if (ifile == numFiles){ break; }
  }

  return theseFiles; 
}

int PrintUsage(){

  cout << endl;
  cout << endl; 
  cout << "######################################################" << endl;
  cout << "#                                                    #" << endl;
  cout << "#    fillHistograms -> Used to fill DIS histograms   #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#  This code requires a set of files specified by    #" << endl;
  cout << "#  a list using -LIST=files.dat, or by wildcard      #" << endl;
  cout << "#  after the option flags.                           #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#  Set the number of files using the -N= flag, and   #" << endl;
  cout << "#  set the run type using the -TYPE= flag.           #" << endl; 
  cout << "#                                                    #" << endl;
  cout << "#  Valid types are data, mcnorad, mcrad.             #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#  Set the output file using the -OUT= flag.         #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#  Sample usage:                                     #" << endl;
  cout << "#  ./fillHistograms -N=1000 -LIST=goodFiles.dat      #" << endl;
  cout << "#   -OUT=pass1.root                                  #" << endl;
  cout << "#                                                    #" << endl;
  cout << "######################################################" << endl;
  cout << endl;
  cout << endl;
  
  return 0;
} 
