#include <iostream>
#include <fstream>
#include <vector>
using std::cout; 
using std::endl; 
using std::string; 

// Mine
#include "CommonTools.h"
#include "DBins.h"
#include "h22Option.h"
#include "GenericAnalysis.h"
#include "Parameters.h"
#include "ParticleFilter.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"

// Root
#include "TString.h"
#include "TLorentzVector.h"
#include "TH1.h"
#include "TH2.h"
#include "THnSparse.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TPad.h"
#include "TStyle.h"

#define DIM 5
vector<string> loadFilesFromList(string fileList, int numFiles);
vector<string> loadFilesFromCommandLine(h22Options *theseOpts, int numFiles);

class MyAnalysis : public GenericAnalysis {
public:
  MyAnalysis(Parameters *pars){ filter = new ParticleFilter(pars); } 
  ~MyAnalysis(){ }
  
  ParticleFilter *filter;
  PhysicsEventBuilder *builder;

  THnSparseI *events; 

  DBins *angleBins, *massBins, *wBins, *dAngleBins, *finalAngleBins; 

  void ProcessEvent();
  void Initialize();
  void Display();
  void Save(string fileName); 
};

// Example override of Init using 
// the methods of h22Reader runno()
// and GSIM const.
void MyAnalysis::Initialize(){
  filter->set_info(GetRunNumber(), GSIM);
  builder     = new PhysicsEventBuilder();

  angleBins      = new DBins(200, -12.0, 65);
  dAngleBins     = new DBins(200,  0, 360); 
  massBins       = new DBins(200, -1.0, 5.0);  
  wBins          = new DBins(200,   1.1, 3.3); 
  finalAngleBins = new DBins(200, -12.0, 80.0); 

  int num[DIM]    = {angleBins->number(), massBins->number(), wBins->number(), dAngleBins->number(), finalAngleBins->number()};
  double min[DIM] = {angleBins->min(),       massBins->min(),    wBins->min(), dAngleBins->min(),    finalAngleBins->min()};
  double max[DIM] = {angleBins->max(),       massBins->max(),    wBins->max(), dAngleBins->max(),    finalAngleBins->max()};

  events = new THnSparseI("events","events",DIM,num,min,max); 

  cout << "[MyAnalysis::Init] Initialization done" << endl;
}

// Example overwriting the event action 
void MyAnalysis::ProcessEvent(){  
  std::vector<TLorentzVector> protons, electrons; 
  electrons = filter->getVectorOfTLorentzVectors(event,   11);
  protons   = filter->getVectorOfTLorentzVectors(event, 2212);

  if ( !electrons.empty() && !protons.empty() ){
    for(int aProton=0; aProton < protons.size(); aProton++){
      PhysicsEvent physicsEvent = builder->getPhysicsEvent(electrons[0], protons[aProton]);
      double point[DIM] = {physicsEvent.finalState.Theta()*to_degrees, physicsEvent.mm2, 
			   physicsEvent.w,to_degrees*(electrons[0].Phi()-protons[aProton].Phi()), to_degrees*physicsEvent.finalState.Angle(electrons[0].Vect())};
      //      double point[DIM] = {physicsEvent.finalState.Theta()*to_degrees, physicsEvent.mm2, 
      //      			   physicsEvent.w,to_degrees*(electrons[0].Phi()-protons[aProton].Phi()), to_degrees*(physicsEvent.finalState.Theta()-electrons[0].Theta())};
      events->Fill(point); 
    }
  }

}

void MyAnalysis::Save(string fileName){
  TFile *outputFile = new TFile(fileName.c_str(),"recreate"); 
  events->Write(); 
  outputFile->Write();
  outputFile->Close();
}


int main(int argc, char *argv[]){

  h22Options *options = new h22Options();

  options->args["PARS"].args = "/u/home/dmriser/analysis-main/lists/parameters/data/data.pars";
  options->args["PARS"].type = 1;
  options->args["PARS"].name = "Parameter file";

  options->args["LIST"].args = "/u/home/dmriser/analysis-main/projects/dis2/allGoodEvents.dat";
  options->args["LIST"].type = 1;
  options->args["LIST"].name = "File list";

  options->set(argc, argv);

  Parameters *pars = new Parameters(); 
  pars->loadParameters(options->args["PARS"].args); 

  MyAnalysis analysis(pars);

  // Getting files
  vector<string> filesToProcess; 
  if (options->ifiles.empty()){
    filesToProcess = loadFilesFromList(options->args["LIST"].args, options->args["N"].arg);
  }
  else {
    filesToProcess = loadFilesFromCommandLine(options, options->args["N"].arg);
  }

  if( !filesToProcess.empty() ){
    for (int i=0; i<filesToProcess.size(); i++) { analysis.AddFile(filesToProcess[i]); } 
    analysis.RunAnalysis(1e9);
    analysis.Save(options->args["OUT"].args);
  }

  else {
    cout << "[MyAnalysis::Main] No files added to options->ifiles " << endl; 
  }


  return 0;
}

vector<string> loadFilesFromList(string fileList, int numFiles){
  vector<string> theseFiles;

  std::ifstream inputFile;
  inputFile.open(fileList.c_str());

  int ifile = 0; string line;
  while (getline(inputFile, line) && ifile < numFiles){
    theseFiles.push_back(line);
    ifile++;
  }

  inputFile.close();
  return theseFiles;
}

vector<string> loadFilesFromCommandLine(h22Options *theseOpts, int numFiles){
  vector<string> theseFiles;

  for(int ifile = 0; ifile < theseOpts->ifiles.size(); ifile++){
    theseFiles.push_back(theseOpts->ifiles[ifile]);
    ifile++;

    if (ifile == numFiles){ break; }
  }

  return theseFiles;
}
