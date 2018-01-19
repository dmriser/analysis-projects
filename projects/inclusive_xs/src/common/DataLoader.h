#ifndef data_loader_h
#define data_loader_h

// c++ includes 
#include <iostream>
#include <vector>
using namespace std;

// my includes 
#include "BaseDISHistograms.h"
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

// cern root 
#include "TLorentzVector.h"

class DataLoader : public HistogramLoader {
 public:
  DataLoader(PhysicsEventSelector *eventCriteria, MomCorr_e1f *momCorr, Parameters *pars, std::string outputFile, std::string saveOpts);
  ~DataLoader();

 protected:
  BaseDISHistograms dataEvents;
  NathanEIDWrapper    eID; 
  MomCorr_e1f        *momCorrector; 
  ParticleFilter     *filter; 
  StandardHistograms *passedHistos; 
  StandardHistograms *allHistos; 

 protected:
  long int eventsProcessed; 

  void Initialize();
  void ProcessEvent();
  void Save();

};

DataLoader::DataLoader(PhysicsEventSelector *eventCriteria, MomCorr_e1f * momCorr, Parameters *pars, std::string outputFile, std::string saveOpts) : HistogramLoader(eventCriteria, outputFile, saveOpts){
  momCorrector = momCorr; 
  filter       = new ParticleFilter(pars);
}

DataLoader::~DataLoader(){
  cout << "[DataLoader::~DataLoader] Destroying data loader with events processed: " << eventsProcessed << endl; 
}

void DataLoader::Initialize(){
  dataEvents.Init("dataEvents","Data Hits");
  eventsProcessed = 0;

  passedHistos = new StandardHistograms("dataPassed",0); 
  allHistos    = new StandardHistograms("dataAll",   0); 

  /*
  filter->getSelector(11)->disable_by_name("DC Region 3 Fid Cut");
  filter->getSelector(11)->disable_by_name("EC-U Cut");
  filter->getSelector(11)->disable_by_name("EC-V Cut");
  filter->getSelector(11)->disable_by_name("EC-W Cut");
  */

}

void DataLoader::ProcessEvent(){
  eventsProcessed++;
  //  eID.set_info(GetRunNumber(),GSIM);
  //  int e_index = eID.get_electron(event);

  filter->set_info(GetRunNumber(), GSIM);
  int e_index = filter->getByPID(event, 11);
  if (e_index > -123){
    TLorentzVector electron(event.cx[e_index]*event.p[e_index],
			    event.cy[e_index]*event.p[e_index],
			    event.cz[e_index]*event.p[e_index],
			    event.p[e_index]);

    electron                  = momCorrector->PcorN(electron, -1, 11); 
    int sector                = event.dc_sect[e_index]; 
    PhysicsEvent physicsEvent = builder.getPhysicsEvent(electron);

    allHistos->Fill(event, e_index, physicsEvent); 

    if (eventSelector->passes(physicsEvent) && sector > 0) {
      dataEvents.Fill(physicsEvent, sector); 
      passedHistos->Fill(event, e_index, physicsEvent); 
    }

  } 
}

void DataLoader::Save(){
  dataEvents.Save(outputFilenameWithExtension, saveOption);

  // Reopen and update with standard histos, 
  // probably a better way to do it. 
  TFile *out = new TFile(outputFilenameWithExtension.c_str(), "update"); 
  allHistos   ->Save(out); 
  passedHistos->Save(out); 
  out->Close(); 
}

#endif
