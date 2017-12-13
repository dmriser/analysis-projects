#ifndef data_loader_h
#define data_loader_h

// c++ includes 
#include <iostream>
#include <vector>
using namespace std;

// my includes 
//#include "BaseHistograms.h"
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

class DataLoader : public HistogramLoader{
 public:
  DataLoader(PhysicsEventSelector *eventCriteria, MomCorr_e1f * momCorr, Parameters *pars,std::string outputFile, std::string saveOpts);
  ~DataLoader();

 protected:

  StandardHistograms *standardHistos; 
  ElasticHistograms  *histos; 

  NathanEIDWrapper eID; 
  ParticleFilter  *filter;
  MomCorr_e1f     *momCorrector; 
  string           outputFilename; 

 protected:
  void Initialize();
  void ProcessEvent();
  void Save();

};

DataLoader::DataLoader(PhysicsEventSelector *eventCriteria, MomCorr_e1f *momCorr, Parameters *pars, std::string outputFile, std::string saveOpts) : HistogramLoader(eventCriteria, outputFile, saveOpts){
  momCorrector   = momCorr; 
  outputFilename = outputFile;
  filter         = new ParticleFilter(pars);

  standardHistos = new StandardHistograms("data",0); 
  histos         = new ElasticHistograms("data",0); 
  histos->Initialize(); 

  // Modify EID 
  //  filter->getSelector(11)->disable_by_regex("EC-"); 
  //  filter->getSelector(11)->disable_by_regex("CC"); 
  //  filter->getSelector(11)->disable_by_regex("Fid"); 
}

DataLoader::~DataLoader(){
  filter->GetSelector(11)->PrintSummary(); 
}

void DataLoader::Initialize(){

}

void DataLoader::ProcessEvent(){

  vector<int> electrons = filter->getVectorOfParticleIndices(event, 11);
  if ( !electrons.empty() ){
    TLorentzVector electron   = event.GetTLorentzVector(electrons[0],     11);
    electron                  = momCorrector->PcorN(electron, -1, 11); 
    int sector                = event.dc_sect[electrons[0]]; 
    PhysicsEvent physicsEvent = builder.getPhysicsEvent(electron);
    
    if (eventSelector->passes(physicsEvent) && sector > 0) {
      standardHistos->Fill(event, electrons[0], physicsEvent); 
      histos        ->Fill(physicsEvent); 
    }
  } 
}

void DataLoader::Save(){
  TFile *outputFile = new TFile(outputFilename.c_str(), "RECREATE");
  standardHistos->Save(outputFile); 
  histos        ->Save(outputFile); 
  outputFile->Close();
}

#endif
