#ifndef lund_to_text_cxx
#define lund_to_text_cxx

#include "LundToText.h"

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

// CERN Root Libs
#include "TLorentzVector.h"

LundToText::LundToText(){
  outputFilenameWithExtension = "out.dat"; 
}

LundToText::~LundToText(){
  // Anything to do here? 
}

void LundToText::Execute(int numberOfEvents){
  Initialize();
  Loop(numberOfEvents);
  Save();
}

void LundToText::Initialize(){
  // do something here? 
}

void LundToText::Loop(int numberOfEvents){

  int currentEvent = 0;

  // No complicated conditional fill conditions, because 
  // we expect generated events only. 
  while(HasEvent() && currentEvent<numberOfEvents){
    TLorentzVector electron = GetEvent().GetParticle(0).GetTLorentzVector(); 
    PhysicsEvent physicsEvent = builder.getPhysicsEvent(electron);
    int sector = 1+floor((180.0 + electron.Phi()*to_degrees)/60.0);
    if (sector > 0 && sector < 7) { 
      // do something with the event 
      cout.width(14); cout << electron.X();
      cout.width(14); cout << electron.Y();
      cout.width(14); cout << electron.Z();
      cout.width(14); cout << electron.Theta() * to_degrees;
      cout.width(14); cout << electron.Phi() * to_degrees;
      cout.width(14); cout << physicsEvent.x; 
      cout.width(14); cout << physicsEvent.y; 
      cout.width(14); cout << physicsEvent.w; 
      cout.width(14); cout << physicsEvent.nu; 
      cout.width(14); cout << physicsEvent.qq << endl; 
    }



    currentEvent++;
  }

}

void LundToText::Save(){
  // save events 
}

#endif 
