#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

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

// ----------------------------------------------
/* 
   Run file-by-file and remove events that 
   fall in bad events window, save file. 

 */
// ----------------------------------------------
class MyAnalysis : public GenericAnalysis {
public:
  MyAnalysis(Parameters *pars, h22Options *opts) : GenericAnalysis(opts) { filter = new ParticleFilter(pars); } 
  ~MyAnalysis(){ }
  

protected:
  ParticleFilter      *filter;
  PhysicsEventBuilder *builder; 
  TFile               *outputFile; 
  TChain              *clonedChain; 
  TTree               *clonedTree; 

  int         iBadWindow; 
  vector<int> badEventStart;
  vector<int> badEventEnd;

public:
  void ProcessEvent();
  void Initialize();
  void LoadBadEventsFile(); 
  void Save();
  bool SkimCriteriaPasses();
  bool EventIsSelected(); 
};

// Example override of Init using 
// the methods of h22Reader runno()
// and GSIM const.
void MyAnalysis::Initialize(){
  filter->set_info(GetRunNumber(), GSIM);
  builder = new PhysicsEventBuilder(); 

  outputFile  = new TFile(Form("/volatile/clas12/dmriser/data/meson/root/%d.root",GetRunNumber()),"RECREATE");
  clonedChain = (TChain*) fchain->CloneTree(0);
  clonedTree  = clonedChain->GetTree();

  std::cout << "[MyAnalysis::Init] Setting up filter with custom options! " << std::endl; 
  filter->GetSelector(11)->DisableByRegex("DC"); 
  filter->GetSelector(11)->DisableByRegex("CC"); 
  filter->GetSelector(11)->DisableByRegex("Samp"); 

  cout << "[MyAnalysis::Init] Initialization done" << endl;
}

void MyAnalysis::LoadBadEventsFile(){
  iBadWindow  = 0;
  ifstream eventQCFile(Form("/volatile/clas12/dmriser/analysis/e1f_analysis/badEvents/%d.txt",GetRunNumber()));
    
  string line, buffer;
  while(getline(eventQCFile,line)){
      istringstream iss(line);
      int istring = 0;

      while(iss){
	  iss >> buffer;
	  if(istring == 0){
	      int s = atoi(buffer.c_str());
	      badEventStart.push_back(s);
	    }
	  else if (istring == 1){
	      int e = atoi(buffer.c_str());
	      badEventEnd.push_back(e);
	    }
	  istring++;
      }
  }
  eventQCFile.close();

  cout << "[MyAnalysis::LoadBadEventsFile] Found " << badEventStart.size() << " bad event windows for run " << GetRunNumber() << endl;
}

void MyAnalysis::ProcessEvent(){  
  if(SkimCriteriaPasses() && EventIsSelected()){
    clonedTree->Fill();
  }
}

bool MyAnalysis::EventIsSelected(){
  std::vector<int> electronIndices = filter->getVectorOfParticleIndices(event, 11); 
  
  if (!electronIndices.empty()){
    
    // we should use loose parameters 
    TLorentzVector electron   = event.GetTLorentzVector(electronIndices[0], 11); 
    PhysicsEvent physicsEvent = builder->getPhysicsEvent(electron); 
    
    
    // is part of the dis region and the missing mass
    // cut kills hard pions coming from ep -> epi+N
    if (physicsEvent.qq > 1.0 && physicsEvent.w > 1.9 && physicsEvent.mm2 > 1.1){
      
      for (int ipart=0; ipart<event.gpart; ipart++){
	if (ipart != electronIndices[0]){
	  double tofMass = event.p[ipart]*sqrt((1-pow(event.b[ipart],2))/pow(event.b[ipart],2)); 
	  
	  // Very loose cuts around pi/k mass, remember 
	  // that this is not corrected 
	  if (tofMass > 0.0 && tofMass < 0.75){ 
	    return true; 
	  }

	}
      }      
    } 
  }
  else {
    return false; 
  }
  
  // we passed electron but not the rest 
  // dont save event 
  return false; 
}

bool MyAnalysis::SkimCriteriaPasses(){
  if (iBadWindow < badEventStart.size()){
    // Catch times when we passed the current window
    if (event.evntid > badEventEnd[iBadWindow]) iBadWindow++;
    
    // We're in a bad event region, dQ = 0
    if ((event.evntid > badEventStart[iBadWindow]) && (event.evntid < badEventEnd[iBadWindow])){
      return false;
    } else {
      return true;
    }
  }
  
  return true;
}

void MyAnalysis::Save(){
  TString fileName = fchain->GetCurrentFile()->GetName();
  cout << "[MyAnalysis::Save] Saving " << fileName << " Old Events = " << fchain->GetEntries() << " New Events = " << clonedTree->GetEntries() << endl; 


  outputFile->Write(); 
  outputFile->Close(); 
}


// -----------------------------------------------------------------
//               main 
// -----------------------------------------------------------------

int main(int argc, char *argv[]){

  h22Options *options = new h22Options();
  options->args["PARS"].args = "/u/home/dmriser/Analysis_v2/lists/dataLoose.pars";
  options->args["PARS"].type = 1;
  options->args["PARS"].name = "Parameter file";

  options->args["LIST"].args = "unset";
  options->args["LIST"].type = 1;
  options->args["LIST"].name = "File list";
  options->set(argc, argv);

  Parameters *pars = new Parameters(); 
  pars->loadParameters(options->args["PARS"].args); 

  vector<string> filesToProcess; 

  if (options->args["LIST"].args != "unset") { 
    filesToProcess = loadFilesFromList(options->args["LIST"].args, 10000);;
  } else {
    filesToProcess = options->ifiles; 
  }

  

  for (int i=0; i<filesToProcess.size(); i++) { 
    MyAnalysis *analysis = new MyAnalysis(pars, options);
    analysis->AddFile(filesToProcess[i]); 

    if (analysis->RunAnalysis()){
      //      analysis->Save();
    } else {
      cout << "[SkimOutBadEvents] Warning: File " << filesToProcess[i] << " failed to be skimmed. " << endl;
    }

    delete analysis; 
  } 

  return 0;
}

