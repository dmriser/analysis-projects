#ifndef mc_loader_h
#define mc_loader_h

// c++ includes 
#include <iostream>
#include <vector>
using namespace std;

// my includes 
#include "CommonTools.h"
#include "h22Reader.h"
#include "HistogramLoader.h"
#include "NathanArchive.h"
#include "MomCorr.h"
#include "Parameters.h"
#include "ParticleFilter.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"
#include "PhysicsEventSelector.h"
#include "StandardHistograms.h"
#include "Histograms.h"

// root 
#include "TFile.h"
#include "TLorentzVector.h"
#include "THnSparse.h"

class MCLoader : public HistogramLoader{
 public:
  MCLoader(PhysicsEventSelector *eventCriteria, Parameters *pars,std::string outputFile, std::string saveOpts);
  ~MCLoader();

 protected:

  StandardHistograms *standardHistosRec; 
  StandardHistograms *standardHistosGen; 
  ElasticHistograms  *rec; 
  ElasticHistograms  *gen; 

  NathanEIDWrapper eID; 
  ParticleFilter  *filter;
  MomCorr_e1f     *momCorrector; 
  string           outputFilename; 

 protected:
  void Initialize();
  void ProcessEvent();
  void Save();
  bool EventHasGeneratedElectron(); 

};

MCLoader::MCLoader(PhysicsEventSelector *eventCriteria, Parameters *pars, std::string outputFile, std::string saveOpts) : HistogramLoader(eventCriteria, outputFile, saveOpts){
  outputFilename = outputFile;
  filter         = new ParticleFilter(pars);

  standardHistosRec = new StandardHistograms("rec",0); 
  standardHistosGen = new StandardHistograms("gen",0); 
  rec               = new ElasticHistograms("rec",0); 
  gen               = new ElasticHistograms("gen",0); 
  rec->Initialize(); 
  gen->Initialize(); 

  filter->GetSelector(11)->DisableByRegex("Samp"); 
  filter->GetSelector(11)->DisableByRegex("Charge"); 
  filter->GetSelector(11)->DisableByRegex("CC"); 
  filter->GetSelector(11)->DisableByRegex("Fid"); 
}

MCLoader::~MCLoader(){
  filter->GetSelector(11)->PrintSummary(); 
}

void MCLoader::Initialize(){

}

void MCLoader::ProcessEvent(){

  if(EventHasGeneratedElectron()){
    // -----------------------------------------------------
    //          Fill Generated
    // -----------------------------------------------------
    TLorentzVector genElectron = event.GetGeneratedParticle(11);
    PhysicsEvent   genEvent    = builder.getPhysicsEvent(genElectron); 
    int            genSector   = phiToSector(genElectron.Phi()*to_degrees);

    //    if(eventSelector->passes(genEvent) && genSector > -1){
    //    standardHistosGen->Fill(genEvent);
    //    gen              ->Fill(genEvent);
    //    }

    standardHistosGen->Fill(genEvent);
    gen              ->Fill(genEvent);

    // -----------------------------------------------------
    //          Fill Reconstructed 
    // -----------------------------------------------------
    vector<int> electrons = filter->getVectorOfParticleIndices(event, 11);
    if ( !electrons.empty() ){
      TLorentzVector electron   = event.GetTLorentzVector(electrons[0], 11);
      int sector                = event.dc_sect[electrons[0]]-1; 
      PhysicsEvent physicsEvent = builder.getPhysicsEvent(electron);
      
      if (eventSelector->passes(physicsEvent) && sector > -1) {
	standardHistosRec->Fill(event, electrons[0], physicsEvent); 
	rec              ->Fill(physicsEvent); 
      }
    } 
  }

}

bool MCLoader::EventHasGeneratedElectron(){
  for(int ipart=0; ipart<event.mcnentr; ipart++){
    if (event.mcid[ipart] == 11){
      return true;
    }
  }
  return false;
}

void MCLoader::Save(){
  TFile *outputFile = new TFile(outputFilename.c_str(), "RECREATE");
  standardHistosRec->Save(outputFile); 
  standardHistosGen->Save(outputFile); 
  rec->Save(outputFile); 
  gen->Save(outputFile); 
  outputFile->Close();
}

#endif
