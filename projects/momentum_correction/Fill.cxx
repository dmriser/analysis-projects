#include <iostream>
#include <vector>
using std::cout; 
using std::endl; 

// Mine
#include "CommonTools.h"
#include "h22Option.h"
#include "GenericAnalysis.h"
#include "Parameters.h"
#include "ParticleFilter.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"
#include "MomCorr.h"

// Root
#include "TString.h"
#include "TLorentzVector.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TPad.h"
#include "TStyle.h"


class Fill : public GenericAnalysis {
public:
  Fill(Parameters *pars, h22Options *opts, std::string outputFile) : outputFilename(outputFile), GenericAnalysis(opts) { filter = new ParticleFilter(pars); } 
  ~Fill(){}
  
  ParticleFilter      *filter;
  PhysicsEventBuilder *builder;
  MomCorr_e1f         *momCorr;

  TH2I *pWBefore[6], *pWAfter[6];
  TH1I *deltaWBefore[6], *deltaWAfter[6]; 

  std::string outputFilename;

  void ProcessEvent();
  void Initialize();
  void Save();
};

void Fill::Initialize(){
  filter->set_info(GetRunNumber(), GSIM);
  builder     = new PhysicsEventBuilder();
  
  for(int s=0; s<6; s++){
    pWBefore[s]     = new TH2I(Form("pWBefore%d",s),"",100,0.5,5.5,100,0.65,1.25); 
    pWAfter[s]      = new TH2I(Form("pWAfter%d",s),"",100,0.5,5.5,100,0.65,1.25); 
    deltaWBefore[s] = new TH1I(Form("deltaWBefore%d",s),"",100,-0.4,0.4); 
    deltaWAfter[s]  = new TH1I(Form("deltaWAfter%d",s),"",100,-0.4,0.4); 
  }

  momCorr = new MomCorr_e1f("/u/home/dmriser/mydoc/analysis/root_scripts/Analysis_v2/momCorr/"); 

  cout << "[Fill::Init] Initialization done" << endl;
}

void Fill::ProcessEvent(){  
  std::vector<int> electrons          = filter->getVectorOfParticleIndices(event,11);
  std::vector<TLorentzVector> protons = filter->getVectorOfTLorentzVectors(event,2212); 

  if ( !electrons.empty() && !protons.empty() ){
    
    TLorentzVector electron = event.GetTLorentzVector(electrons[0],11);
    int s                   = event.dc_sect[electrons[0]] -1;
    
    if(s > -1){
      PhysicsEvent physicsEventBefore = builder->getPhysicsEvent(electron,protons[0]); 

      if(physicsEventBefore.mm2 > -0.1 && physicsEventBefore.mm2 < 0.1 && physicsEventBefore.w < 1.2){
	pWBefore[s]    ->Fill(electron.P(),physicsEventBefore.w);
	deltaWBefore[s]->Fill(physicsEventBefore.w - proton_mass); 
      }
      
      electron = momCorr->PcorN(electron, -1, 11);    
      PhysicsEvent physicsEventAfter = builder->getPhysicsEvent(electron,protons[0]); 
      if(physicsEventAfter.mm2 > -0.1 && physicsEventAfter.mm2 < 0.1 && physicsEventAfter.w < 1.2){
	pWAfter[s]    ->Fill(electron.P(),physicsEventAfter.w);
	deltaWAfter[s]->Fill(physicsEventAfter.w - proton_mass); 
      }
    }

  }
  
}

void Fill::Save(){
  TFile *outputFile = new TFile(outputFilename.c_str(),"RECREATE"); 
  
  for(int s=0; s<6; s++){
    pWBefore[s]    ->Write();
    pWAfter[s]     ->Write();
    deltaWBefore[s]->Write();
    deltaWAfter[s] ->Write();
  }

  outputFile->Close();
}

////////////////////////////////////////////////////////////////////////////////
/*

                                Start main code

 */
////////////////////////////////////////////////////////////////////////////////


int main(int argc, char *argv[]){

  h22Options *options = new h22Options();

  options->args["PARS"].args = "/u/home/dmriser/mydoc/analysis/root_scripts/Analysis_v2/lists/data.pars";
  options->args["PARS"].type = 1;
  options->args["PARS"].name = "Parameter file";
  options->set(argc, argv);

  Parameters *pars = new Parameters(); 
  pars->loadParameters(options->args["PARS"].args); 

  Fill analysis(pars, options, options->args["OUT"].args);

  if(options->ifiles.size() > 0){
    for (int i=0; i<options->ifiles.size(); i++) { analysis.AddFile(options->ifiles[i]); } 
    analysis.RunAnalysis();
  }

  else {
    cout << "[Fill::Main] No files added to options->ifiles " << endl; 
  }


  return 0;
}
