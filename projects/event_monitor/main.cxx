#include <iostream>
#include <vector>
using namespace std;

#include "Parameters.h"
#include "EventMonitor.h"
#include "EventMonitor.cxx"

#include "h22Option.h"

vector<string> loadFilesFromList(string fileList, int numFiles);
vector<string> loadFilesFromCommandLine(h22Options * theseOpts, int numFiles);

int main(int argc, char * argv[]){

  if (argc < 2) { return 0; }
  
  // Setup command line options.
  h22Options opts; 
  opts.args["TYPE"].args = "rec";
  opts.args["TYPE"].type = 1;
  opts.args["TYPE"].name = "Data type";
  opts.args["PARS"].args = "/u/home/dmriser/mydoc/analysis/root_scripts/Analysis_v2/lists/data.pars";
  opts.args["PARS"].type = 1;
  opts.args["PARS"].name = "Parameter file";
  opts.args["LIST"].args = "UNSET";
  opts.args["LIST"].type = 1;
  opts.args["LIST"].name = "File list";
  opts.set(argc, argv);

  EventMonitor *Analyzer;
  bool isGen = false; 
  Parameters *pars = new Parameters();
  pars->loadParameters(opts.args["PARS"].args); 

  if (opts.args["TYPE"].args == "gen") { isGen = true; }
  Analyzer = new EventMonitor(pars, isGen);

  vector<string> fileList;
  if (opts.args["LIST"].args == "UNSET"){
    fileList = loadFilesFromCommandLine(&opts,opts.args["N"].arg);
  }
  else{
    fileList = loadFilesFromList(opts.args["LIST"].args,opts.args["N"].arg);
  } 

  for (int ifile=0; ifile<fileList.size(); ifile++){ Analyzer->AddFile(fileList[ifile]); } 
  Analyzer->Init(); 
  Analyzer->Loop(); 
  Analyzer->Save(opts.args["OUT"].args);

  delete Analyzer;
  return 0; 
	
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
