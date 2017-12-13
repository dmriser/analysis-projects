#ifndef fill_mc_h
#define fill_mc_h

#include <iostream>
#include <fstream>

#include "Corrections.h"
#include "h22Option.h"
#include "GenericAnalysis.h"
#include "Parameters.h"
#include "ParticleFilter.h"
#include "PhysicsEventBuilder.h"

#include "TBranch.h"
#include "TLorentzVector.h"
#include "TTree.h"

#include "Tree.h"

class FillMonteCarlo : public GenericAnalysis {
 public:
  FillMonteCarlo(h22Options *opts, Parameters *p, Parameters *ptight, Parameters *ploose);
  ~FillMonteCarlo();

  PhysicsEventBuilder builder;
  FlatTree           *recTree; 
  FlatTree           *genTree; 

 public:
  void ProcessEvent();
  void Save(std::string outputFilename);
  void CheckForMesonAndFill(int index); 
  void FillGeneratedMeson(int index);
  bool EventHasGeneratedParticle(int pid); 

  ParticleFilter *getFilter() {
    return fFilter;
  }

 protected:
  Corrections     fCorr; 
  ParticleFilter *fFilter; 
  ParticleFilter *fFilterTight; 
  ParticleFilter *fFilterLoose;
 
  Parameters     *fPars; 
  int localEventNumber; 
};

#endif
