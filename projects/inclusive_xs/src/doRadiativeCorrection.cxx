/* ########################################################################
   #                                                                      #
   #               David Riser, University of Connecticut                 #
   #                         October 27, 2016                             #
   #                                                                      #
   #                       Updated Dec 20, 2016                           #
   ######################################################################## */

#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

#include "CommonTools.h"
#include "h22Option.h"
#include "KeppelRadReader.h"

#include "common/BaseDISHistograms.h"
#include "common/DIS1DHistograms.h"
#include "common/RadiativeCorrectionLoader.h"

int PrintUsage();
void configureCommandLineOptions(h22Options *theseOpts); 
vector<string> loadFilesFromList(string fileList);

int main(int argc, char * argv[]){

  // ------------- Physics Options -------------
  int numberOfXBins = 35; // x and W 
  int numberOfYBins = 10; // Q^{2}

  // Check for improper usage of this code. 
  if (argc < 3){ return PrintUsage(); }

  // Collect command line options, configure ours and compare.
  h22Options *options = new h22Options(); 
  configureCommandLineOptions(options); 
  options->set(argc, argv);

  int numberOfEvents    = options->args["N"].arg;
  string inputRadList   = options->args["RAD"].args; 
  string inputNoRadList = options->args["NORAD"].args; 
  string outputFilename = options->args["OUT"].args; 

  if (inputRadList != "UNSET" && inputNoRadList != "UNSET"){

    // Generate 2-D Histograms for both radiaited and unradiated events and 
    // save them in the output file. 
    RadiativeCorrectionLoader radLoader("radEvents","Radiated Gen. Events",outputFilename,"recreate");
    vector<string> radFiles = loadFilesFromList(inputRadList);
    for (int currentFile=0; currentFile<radFiles.size(); ++currentFile) { radLoader.AddFile(radFiles[currentFile]); }
    radLoader.Initialize();
    radLoader.Execute(numberOfEvents);
    radLoader.Save();

    RadiativeCorrectionLoader noRadLoader("noRadEvents","Gen. Events",outputFilename,"update");
    vector<string> noRadFiles = loadFilesFromList(inputNoRadList);
    for (int currentFile=0; currentFile<noRadFiles.size(); ++currentFile) { noRadLoader.AddFile(noRadFiles[currentFile]); }
    noRadLoader.Initialize();
    noRadLoader.Execute(numberOfEvents);
    noRadLoader.Save();

  } else {
    return PrintUsage();
  }

  return 0;
}

void configureCommandLineOptions(h22Options *theseOpts){
  theseOpts->args["RAD"].args = "UNSET";
  theseOpts->args["RAD"].type = 1;
  theseOpts->args["RAD"].name = "Radiaited list";

  theseOpts->args["NORAD"].args = "UNSET";
  theseOpts->args["NORAD"].type = 1;
  theseOpts->args["NORAD"].name = "Unradiated list";
}

vector<string> loadFilesFromList(string fileList){
  vector<string> theseFiles;

  ifstream inputFile;
  inputFile.open(fileList.c_str());

  string line;
  while (getline(inputFile, line)){
    theseFiles.push_back(line);
  }

  inputFile.close();
  return theseFiles;
}

int PrintUsage(){

  cout << endl;
  cout << endl; 
  cout << "######################################################" << endl;
  cout << "#                                                    #" << endl;
  cout << "#    doRadiativeCorrection                           #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#   This code requires an input/output flagged       #" << endl;
  cout << "#   -RAD=rad.dat -RAD=norad.dat                      #" << endl;
  cout << "#       -OUT=out/radCorr.root                        #" << endl;
  cout << "#                                                    #" << endl;
  cout << "######################################################" << endl;
  cout << endl;
  cout << endl;
  
  return 0;
} 
