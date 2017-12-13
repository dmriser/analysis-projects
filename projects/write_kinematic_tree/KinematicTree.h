#ifndef kinematic_tree_h
#define kinematic_tree_h

#include <iostream>
#include <fstream>

#include "Corrections.h"
#include "h22Option.h"
#include "GenericAnalysis.h"
#include "MomCorr.h"
#include "Parameters.h"
#include "ParticleFilter.h"
#include "PhysicsEventBuilder.h"

#include "TBranch.h"
#include "TLorentzVector.h"
#include "TTree.h"

#include "Tree.h"

class KinematicTree : public GenericAnalysis {
 public:
  KinematicTree(h22Options *opts, Parameters *p, Parameters *ptight, Parameters *ploose, int type);
  ~KinematicTree();

  PhysicsEventBuilder builder;
  FlatTree *kinematicTree; 

 public:
  void ProcessEvent();
  void Save(std::string outputFilename);
  void CheckForMesonAndFill(int index); 

  ParticleFilter *getFilter() {
    return fFilter;
  }

 protected:
  int localEventNumber;

  MomCorr_e1f    *fMomCorr; 
  Corrections     fCorr; 
  ParticleFilter *fFilter; 
  ParticleFilter *fFilterTight; 
  ParticleFilter *fFilterLoose;
 
  Parameters     *fPars; 

};

#endif
