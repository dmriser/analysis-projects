
#ifndef mc_loader_h
#define mc_loader_h

// c++ includes 
#include <iostream>
#include <vector>
#include <cmath>
using namespace std; 

// my includes 
#include "BaseDISHistograms.h"
#include "h22Reader.h"
#include "HistogramLoader.h"
#include "NathanArchive.h"
#include "Parameters.h"
#include "ParticleFilter.h"
#include "PhysicsEventBuilder.h"
#include "PhysicsEventSelector.h"
#include "CommonTools.h"
#include "PhysicsEvent.h"
#include "StandardHistograms.h"

// root 
#include "TLorentzVector.h"
#include "TH2.h"

class MCLoader : public HistogramLoader{
 public:
  MCLoader(PhysicsEventSelector *eventCriteria, Parameters *pars, std::string outputFile, std::string saveOpts, std::string monteCarloType);
  ~MCLoader();

 protected:
  BaseDISHistograms recEvents;
  BaseDISHistograms genEvents;
  BaseDISHistograms recAndGenEvents;
  NathanEIDWrapper  eID; 
  ParticleFilter    *filter; 
  std::string       mcType; 
  
  StandardHistograms *allRecHistos; 
  StandardHistograms *passedRecHistos; 
  StandardHistograms *genHistos; 

 public:
  void Initialize();
  void ProcessEvent();
  void Save();
  void Rebin(int xFactor, int yFactor);
  bool EventHasGeneratedElectron();
};

MCLoader::MCLoader(PhysicsEventSelector *eventCriteria, Parameters *pars, std::string outputFile, std::string saveOpts, std::string monteCarloType) : HistogramLoader(eventCriteria, outputFile, saveOpts){
  mcType = monteCarloType; 
  filter = new ParticleFilter(pars);
}

MCLoader::~MCLoader(){
}

void MCLoader::Initialize(){
  recEvents.Init(Form("recEvents%s",mcType.c_str()),"Reconstructed MC Hits");
  genEvents.Init(Form("genEvents%s",mcType.c_str()),"Generated MC Events");
  recAndGenEvents.Init(Form("recAndGenEvents%s",mcType.c_str()),"Rec/Gen Same Bin Events");
  //  recAndGenEvents.Rebin2D(2,2);

  allRecHistos    = new StandardHistograms("allRec",   0); 
  passedRecHistos = new StandardHistograms("passedRec",0); 
  genHistos       = new StandardHistograms("gen",      0); 
}

// This is the core routine which conditionally fills histograms. 
void MCLoader::ProcessEvent(){

  if (EventHasGeneratedElectron()){
    // Deal with the generated first.
    TLorentzVector genElectron   = event.GetGeneratedParticle(11);   
    PhysicsEvent genPhysicsEvent = builder.getPhysicsEvent(genElectron); 
    genHistos->Fill(genPhysicsEvent);

    // Maybe here we should check that the track was reconstructed 
    // if (event.gpart > 0), but that would probably skew acceptance 
    // by stopping all particles which went through holes ect. so it 
    // doesn't seem like what we want to do. 
    int mcSector = event.GetMCSectorByPID(11);
    //    if (mcSector > -1 && mcSector < 7 && eventSelector->get_cut("Relative Phi Cut")->passes(genPhysicsEvent)) { 
    if (mcSector > -1 && mcSector < 7) { 
      genEvents.Fill(genPhysicsEvent, mcSector); 
    }
    
    // Dealing directly with histograms in base histograms here
    // is not ideal, think of a better way. 
    int genxByQQBin = recAndGenEvents.xByQQ[0]->FindBin(genPhysicsEvent.x, genPhysicsEvent.qq);
    int genwByQQBin = recAndGenEvents.wByQQ[0]->FindBin(genPhysicsEvent.w, genPhysicsEvent.qq);
    
    //  eID.set_info(GetRunNumber(),GSIM);
    //  int e_index = eID.get_electron(event);
    

    
    int e_index = filter->getByPID(event, 11);
    if (e_index > -123){
      TLorentzVector recElectron   = event.GetTLorentzVector(e_index, 11);
      int sector                   = event.dc_sect[e_index]; 
      PhysicsEvent recPhysicsEvent = builder.getPhysicsEvent(recElectron);
      
      allRecHistos->Fill(event, e_index, recPhysicsEvent);

      // Again doing this to fill coincident events recAndGen needed to calculate purity and
      // stability. 
      int recwByQQBin = recAndGenEvents.wByQQ[0]->FindBin(recPhysicsEvent.w, recPhysicsEvent.qq);
      if (eventSelector->passes(recPhysicsEvent) && sector > 0) {
	recEvents.Fill(recPhysicsEvent, sector);
	passedRecHistos->Fill(event, e_index, recPhysicsEvent);
      }
    }
  } // End of EventHasGeneratedElectron() 
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
  // You may want to recreate the file, but dont overwrite it by
  // doing "RECREATE" on the second 2 saves. 
  recEvents      .Save(outputFilenameWithExtension, saveOption);
  genEvents      .Save(outputFilenameWithExtension, "UPDATE");   
  recAndGenEvents.Save(outputFilenameWithExtension, "UPDATE");

  TFile *out = new TFile(outputFilenameWithExtension.c_str(),"UPDATE"); 
  allRecHistos   ->Save(out); 
  passedRecHistos->Save(out); 
  genHistos      ->Save(out); 
}

void MCLoader::Rebin(int xFactor, int yFactor){
  recEvents.Rebin2D(xFactor, yFactor);
  genEvents.Rebin2D(xFactor, yFactor);
  recAndGenEvents.Rebin2D(xFactor, yFactor);
}

#endif
