#include <iostream>
#include <fstream>
#include <vector>
using std::cout; 
using std::endl; 
using std::string; 

// Mine
#include "CommonTools.h"
#include "DInformation.h"
#include "h22Option.h"
#include "GenericAnalysis.h"
#include "Parameters.h"
#include "ParticleFilter.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"
#include "PhysicsEventCut.h"
#include "PhysicsEventSelector.h"

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
#include "TObject.h"

class MyAnalysis : public GenericAnalysis {
public:
  MyAnalysis(Parameters *pars, DInformation *info, h22Options *op) : GenericAnalysis(op) { filter = new ParticleFilter(pars); runInfo = info; } 
  ~MyAnalysis(){ }
  
  ParticleFilter       *filter;
  PhysicsEventBuilder  *builder;
  PhysicsEventSelector *eventSelector;
  PhysicsEventCut_y    *yCut;
  PhysicsEventCut_qq   *qqCut;

  DInformation         *runInfo;

  int numberOfEvents, runNumber;
  double charge;

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
  builder       = new PhysicsEventBuilder();
  eventSelector = new PhysicsEventSelector();

  // Physics Cuts 
  yCut  = new PhysicsEventCut_y();
  qqCut = new PhysicsEventCut_qq(); 

  yCut ->set_min(0.0);
  yCut ->set_max(0.7);
  qqCut->set_min(1.5);
  qqCut->set_max(4.5);
  
  eventSelector->add_cut(yCut);
  eventSelector->add_cut(qqCut);

  numberOfEvents = 0;
  runNumber      = GetRunNumber();
  charge         = 0.0; 

  cout << "[MyAnalysis::Init] Initialization done" << endl;
}

// Example overwriting the event action 
void MyAnalysis::ProcessEvent(){  
  std::vector<TLorentzVector> electrons = filter->getVectorOfTLorentzVectors(event,   11);

  if ( !electrons.empty() ){
    PhysicsEvent physicsEvent = builder->getPhysicsEvent(electrons[0]);

    // Add this event to the total.
    if (eventSelector->passes(physicsEvent)){
      numberOfEvents++;
    }
  }
  

}

void MyAnalysis::Save(string fileName){
  TFile *outputFile = new TFile(fileName.c_str(),"recreate"); 

  TTree *outputTree = new TTree("fCup","Faraday Cup Tree"); 
  outputTree->SetDirectory(0);

  TBranch *runnoBranch  = outputTree->Branch("runno",&runNumber); 
  TBranch *chargeBranch = outputTree->Branch("dQ",&charge); 
  TBranch *eventsBranch = outputTree->Branch("dN",&numberOfEvents); 

  charge = (double) runInfo->info[runNumber].dQ/runInfo->info[runNumber].daq_scale;  

  outputTree->Fill();
  outputTree->Write();
  outputFile->Write();
  outputFile->Close();
}


int main(int argc, char *argv[]){

  h22Options *options = new h22Options();

  options->args["PARS"].args = "/u/home/dmriser/mydoc/analysis/root_scripts/analysis-main/lists/data.pars";
  options->args["PARS"].type = 1;
  options->args["PARS"].name = "Parameter file";

  options->args["LIST"].args = "/u/home/dmriser/mydoc/analysis/root_scripts/analysis-main/projects/dis2/allGoodEvents.dat";
  options->args["LIST"].type = 1;
  options->args["LIST"].name = "File list";

  options->set(argc, argv);

  Parameters *pars = new Parameters(); 
  pars->loadParameters(options->args["PARS"].args); 

  DInformation *info = new DInformation(); 
  info->load("/u/home/dmriser/mydoc/analysis/root_scripts/analysis-main/lists/runsNew.info");

  if(info->has_information()){

    MyAnalysis analysis(pars, info, options);

    if( !options->ifiles.empty() ){
      analysis.AddFile(options->ifiles[0]); 
      analysis.RunAnalysis();
      analysis.Save(options->args["OUT"].args);
    }
    
    else {
      cout << "[MyAnalysis::Main] No files added to options->ifiles " << endl; 
    }
  }
  
  else {
    cout << "[MyAnalysis::Main] Information file failed to load " << endl;
  }
  
  return 0;
}
