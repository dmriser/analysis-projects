#ifndef EventMonitor_cxx
#define EventMonitor_cxx

#include <iostream>
#include <vector>
using namespace std;

#include "EventMonitor.h"

// Put your includes here 
#include "CommonTools.h"
#include "Parameters.h"
#include "ParticleFilter.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"
#include "h22Event.h" 
#include "h22Reader.h" 

#include "TFile.h"
#include "TLorentzVector.h"

 // Class Constructor/Destructor 
EventMonitor::EventMonitor(Parameters *params, bool isGen) : isGenerated(isGen), pars(params){ 
  

  builder = PhysicsEventBuilder();
  filter  = new ParticleFilter(pars); 

  xBins     = DBins(100,    0.0, 1.0);
  yBins     = DBins(100,    0.0, 1.0);
  pBins     = DBins(100,    0.0, 6.0);
  wBins     = DBins(100,    0.5, 3.8);
  qqBins    = DBins(100,    0.0, 5.0);
  thetaBins = DBins(100,   12.0, 60.0);
  phiBins   = DBins(100, -180.0, 180.0);
  nuBins    = DBins(100,      0, 5.5);

  int bins[8] = {xBins.number(), yBins.number(), pBins.number(),
		 wBins.number(), qqBins.number(), thetaBins.number(), phiBins.number(), nuBins.number()};

  double min[8] = {xBins.min(), yBins.min(), pBins.min(),
		   wBins.min(), qqBins.min(), thetaBins.min(), phiBins.min(), nuBins.min()};

  double max[8] = {xBins.max(), yBins.max(), pBins.max(),
		   wBins.max(), qqBins.max(), thetaBins.max(), phiBins.max(), nuBins.max()};

  events = new THnSparseI("events","events",8,bins,min,max);
}

EventMonitor::~EventMonitor(){ 

}

void EventMonitor::Loop(){
  
  // Event loop below. 
  Init(); 
  for(int ievent=0; ievent<GetEntries(); ievent++){
    GetEntry(ievent); 
    ProcessEvent(); 
    if (ievent%10000 == 0){ cout << "\r done "<< ievent << " of " << GetEntries() << flush; }
  } 

  cout << endl; 
}

void EventMonitor::ProcessEvent(){

  int e_index = -9;
  
  if (!isGenerated){ 
    for (int ipart=0; ipart<event.gpart; ipart++){
      //      if (event.id[ipart] == 11){ e_index = ipart; break; }
      vector<int> electrons = filter->getVectorOfParticleIndices(event, 11); 
      if ( !electrons.empty() ){ e_index = electrons[0]; }
    }
  }
  
  else if (isGenerated){ 
    for (int ipart=0; ipart<event.gpart; ipart++){
      if (event.mcid[ipart] == 11){ e_index = ipart; break; }
    }
  }


  if (e_index > -1) {
    TLorentzVector electron; 
    if (!isGenerated) { electron = TLorentzVector(event.cx[e_index]*event.p[e_index],
						  event.cy[e_index]*event.p[e_index],
						  event.cz[e_index]*event.p[e_index],
						  event.p[e_index]);
    }
    
    else { electron = event.GetGeneratedParticle(11); }
    PhysicsEvent physicsEvent = builder.getPhysicsEvent(electron);
    double dataPoint[8] = {physicsEvent.x, physicsEvent.y, electron.P(),
			   physicsEvent.w, physicsEvent.qq,electron.Theta()*to_degrees,electron.Phi()*to_degrees, physicsEvent.nu};
    events->Fill(dataPoint); 
  }
  
}

void EventMonitor::Save(string outputFilename){

  TFile *outputFile = new TFile(outputFilename.c_str(), "RECREATE");
  events     ->Write();
  outputFile ->Write();
  outputFile ->Close();

}

#endif
