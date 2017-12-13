#ifndef lund_gen_reader_cxx
#define lund_gen_reader_cxx

#include "LundGeneratedEventReader.h"

// C++ libs 
#include <iostream>
#include <cmath>
using namespace std; 

// My Libs 
#include "CommonTools.h"
#include "LundEvent.h"
#include "LundReader.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"
#include "StandardHistograms.h"
#include "StatusBar.h"

// CERN Root Libs
#include "TLorentzVector.h"

LundGeneratedEventReader::LundGeneratedEventReader(){
  outputFilenameWithExtension = "out.dat"; 

  histos  = new StandardHistograms("generatedEvents",0); 
  builder = new PhysicsEventBuilder(); 
}

LundGeneratedEventReader::~LundGeneratedEventReader(){

}

void LundGeneratedEventReader::SetBeamEnergy(double beam){
  TLorentzVector electron(0, 0, beam, beam);
  TLorentzVector proton(0, 0, 0, proton_mass);

  builder = new PhysicsEventBuilder(electron, proton);
}

void LundGeneratedEventReader::Execute(int numberOfEvents){
  Initialize();
  Loop(numberOfEvents);
  //  Save();
}

void LundGeneratedEventReader::Initialize(){
}

void LundGeneratedEventReader::Loop(int numberOfEvents){

  int currentEvent = 0;

  // No complicated conditional fill conditions, because 
  // we expect generated events only. 
  while(HasEvent() && currentEvent<numberOfEvents){

    TLorentzVector electron   = GetEvent().GetParticle(0).GetTLorentzVector(); 
    TLorentzVector part1      = GetEvent().GetParticle(1).GetTLorentzVector(); 
    PhysicsEvent physicsEvent = builder->getPhysicsEvent(electron, part1);
    int sector                = phiToSector(electron.Phi() *to_degrees); 

    histos->Fill(physicsEvent); 

    if (currentEvent%1000 == 0) { bar.PrintStatus(currentEvent, numberOfEvents); }

    currentEvent++;
  }

}

void LundGeneratedEventReader::Save(std::string outputName){
  TFile *out = new TFile(outputName.c_str(),"recreate"); 
  histos->Save(out); 
  out->Close(); 
}

#endif 
