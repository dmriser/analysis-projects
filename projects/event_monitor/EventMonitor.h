#ifndef EventMonitor_h
#define EventMonitor_h

#include <iostream>

 // Put your includes here 
#include "DBins.h" 
#include "h22Event.h" 
#include "h22Reader.h" 
#include "Parameters.h"
#include "ParticleFilter.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"

#include "THnSparse.h"

class EventMonitor : public h22Reader {
 public:
  EventMonitor(Parameters *params, bool isGen);
  ~EventMonitor();

  bool isGenerated;  
  THnSparseI *events;

  Parameters     *pars;
  ParticleFilter *filter;

  DBins xBins, yBins, pBins, 
    qqBins, wBins, thetaBins, 
    phiBins, nuBins;

  PhysicsEventBuilder builder; 
  
  void Loop();
  void ProcessEvent();
  void Save(std::string outputFilename);
};

#endif
