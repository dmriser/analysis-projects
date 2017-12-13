#ifndef data_loader_mva_h
#define data_loader_mva_h

// c++ includes 
#include <iostream>
#include <vector>
using namespace std;

// my includes 
#include "BaseDISHistograms.h"
#include "h22Reader.h"
#include "HistogramLoader.h"
#include "NathanArchive.h"
#include "MomCorr.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"
#include "PhysicsEventSelector.h"

// root 
#include "TLorentzVector.h"
#include "TMVA/Reader.h"

class DataLoaderMVA : public HistogramLoader{
 public:
  DataLoaderMVA(PhysicsEventSelector *eventCriteria, MomCorr_e1f * momCorr, std::string outputFile, std::string saveOpts);
  ~DataLoaderMVA();

 protected:
  BaseDISHistograms dataEvents;
  NathanEIDWrapper eID; 
  MomCorr_e1f * momCorrector; 
  TMVA::Reader *TMVAReader;

  //  Float_t input1, input2, input3; 
  Float_t input1, input2, input3, input4, input5; 

 protected:
  void Initialize();
  void ProcessEvent();
  void Save();

};
/*
#endif
#ifndef data_loader_cxx
#define data_loader_cxx
*/

DataLoaderMVA::DataLoaderMVA(PhysicsEventSelector *eventCriteria, MomCorr_e1f * momCorr, std::string outputFile, std::string saveOpts) : HistogramLoader(eventCriteria, outputFile, saveOpts){
  momCorrector = momCorr; 

  TMVAReader = new TMVA::Reader("");
  TMVAReader->AddVariable("electron.Theta()", &input1); 
  TMVAReader->AddVariable("w",                &input2); 
  TMVAReader->AddVariable("qq",               &input3); 
  TMVAReader->AddVariable("x",                &input4); 
  TMVAReader->AddVariable("y",                &input5); 

  TMVAReader->BookMVA("SVM","/u/home/dmriser/mydoc/analysis/root_scripts/Analysis_v2/projects/dis2/src/common/weights/TMVAClassification_SVM.weights.xml"); 
  //  TMVAReader->BookMVA("MLP","/u/home/dmriser/mydoc/analysis/root_scripts/Analysis_v2/projects/dis2/src/common/weights/TMVAClassification_MLP.weights.xml"); 

}

DataLoaderMVA::~DataLoaderMVA(){
  // Anything to do here? 
}

void DataLoaderMVA::Initialize(){
  dataEvents.Init("dataEvents","Data Hits");

  /* 
  cout << "[DataLoaderMVA] Warning: Running with electron ID strictness alterations. " << endl;
  eID.cc_fid_strict = 9;
  eID.dc_r1_strict = 9;
  eID.dc_r3_strict = 9;
  */ 

}

// This is the core routine which conditionally fills histograms. 
void DataLoaderMVA::ProcessEvent(){

  eID.set_info(runno(),GSIM);
  int e_index = eID.get_electron(event);
  if (e_index > -123){
    TLorentzVector electron(event.cx[e_index]*event.p[e_index],
			    event.cy[e_index]*event.p[e_index],
			    event.cz[e_index]*event.p[e_index],
			    event.p[e_index]);

    electron = momCorrector->PcorN(electron, -1, 11); 

    int sector = event.dc_sect[e_index]; 
    PhysicsEvent physicsEvent = builder.getPhysicsEvent(electron);

    input1 = electron.Theta(); 
    input2 = physicsEvent.w; 
    input3 = physicsEvent.qq;
    input4 = physicsEvent.x;
    input5 = physicsEvent.y;

    double MVAValue = TMVAReader->EvaluateMVA("SVM");
    //    double MVAValue = TMVAReader->EvaluateMVA("MLP");

    //    if (eventSelector->passes(physicsEvent) && MVAValue > 0.85) {
    if (MVAValue > 0.65) {
      dataEvents.Fill(physicsEvent, sector); 
    }

  } 
}

void DataLoaderMVA::Save(){
  dataEvents.Save(outputFilenameWithExtension, saveOption);
}

#endif
