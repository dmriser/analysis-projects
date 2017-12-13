#ifndef kinematic_tree_cxx
#define kinematic_tree_cxx

#include <iostream>
#include <fstream>
#include <map>
using std::cout; 
using std::endl; 


#include "KinematicTree.h"
#include "Tree.h"

#include "CommonTools.h"
#include "Corrections.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"

#include "TTree.h"
#include "TBranch.h"

KinematicTree::KinematicTree(h22Options *opts, Parameters *p, Parameters *ptight, 
			     Parameters *ploose, int type) : GenericAnalysis(opts), fPars(p) {

  fFilter      = new ParticleFilter(fPars); 
  fFilterLoose = new ParticleFilter(ploose); 
  fFilterTight = new ParticleFilter(ptight); 

  std::string path        = Global::Environment::GetAnalysisPath();
  std::string momCorrPath = Form("%s/momCorr/",path.c_str());
  fMomCorr                = new MomCorr_e1f(momCorrPath); 

  kinematicTree = new FlatTree(type); 
  kinematicTree->SetupTree(); 

  localEventNumber = 0; 
}

KinematicTree::~KinematicTree(){

}

void KinematicTree::CheckForMesonAndFill(int index){
  
  std::vector<int> mesons = fFilter->getVectorOfParticleIndices(event, index); 

  if (mesons.size() > 0){    

    // something will be filled 
    localEventNumber++; 
    kinematicTree->eventNumber = localEventNumber; 


    // we will be writing something to the tree
    int electronIndex = event.GetElectronIndex(); 
    int mesonIndex    = mesons[0];

      TLorentzVector electron = event.GetTLorentzVector(electronIndex, 11); 
      TLorentzVector meson    = event.GetTLorentzVector(mesonIndex,  index); 

      // apply momentum corrections only 
      // to our experimental data
      electron = fMomCorr->PcorN(electron, -1, 11);

      kinematicTree->ele_theta = electron.Theta()*to_degrees; 
      kinematicTree->ele_phi = electron.Phi()*to_degrees; 

      kinematicTree->mes_theta = meson.Theta()*to_degrees; 
      kinematicTree->mes_phi = meson.Phi()*to_degrees; 

      // build all the physics that we want to know about
      PhysicsEvent ev = builder.getPhysicsEvent(electron, meson);

      // correct helicity with waveplate already fixed
      kinematicTree->hel      = event.corr_hel;
      kinematicTree->meson_id = index; 

      // corrected vertex positions and betas 
      kinematicTree->ele_vz = event.corr_vz[electronIndex]; 
      kinematicTree->mes_vz = event.corr_vz[mesonIndex]; 
      kinematicTree->mes_b  = event.corr_b[mesonIndex]; 

      // This needs to be replaced. 
      // Not sure why, this comment is old 
      // and I don't see the problem.
      kinematicTree->ele_p = electron.P(); 

      // this gives uncorrected values of p
      //      ele_p = event.p[electronIndex]; 
      kinematicTree->mes_p = event.p[mesonIndex];
      kinematicTree->charge = event.q[mesonIndex]; 

      kinematicTree->mes_nphe = event.nphe[mesonIndex]; 
      kinematicTree->mes_etot = event.etot[mesonIndex]; 

      kinematicTree->x = ev.x; 
      kinematicTree->w = ev.w; 
      kinematicTree->y = ev.y; 
      kinematicTree->z = ev.z; 
      kinematicTree->qq = ev.qq; 
      kinematicTree->mm2 = ev.mm2; 
      kinematicTree->phiH = ev.phiHadron; 
      kinematicTree->pt   = ev.pT; 
      kinematicTree->tof_mass = pow(event.p[mesons[0]],2) * (1-pow(event.corr_b[mesons[0]],2))/pow(event.corr_b[mesons[0]],2); 

      kinematicTree->kinematicTree->Fill(); 
    }
}

void KinematicTree::ProcessEvent(){

  std::vector<int> electrons = fFilterLoose->getVectorOfParticleIndices(event, 11); 
  
  if (!electrons.empty()){
    event.SetElectronIndex(electrons[0]);
    fCorr.correctEvent(&event, GetRunNumber(), GSIM);

    std::map<std::string, bool> results_nom   = fFilter     ->eid_map(event, electrons[0]); 
    std::map<std::string, bool> results_tight = fFilterTight->eid_map(event, electrons[0]); 
    
    // loose is always passed by this point 
    kinematicTree->pass_sf  = -1; 
    kinematicTree->pass_dc1 = -1; 
    kinematicTree->pass_dc3 = -1; 
    kinematicTree->pass_vz  = -1; 
    kinematicTree->pass_ec  = -1; 

    if (results_nom["EC_SAMPLING"])  { kinematicTree->pass_sf = 0; }
    if (results_tight["EC_SAMPLING"]){ kinematicTree->pass_sf = 1; }

    if (results_nom["DC_R1_FID"])  { kinematicTree->pass_dc1 = 0; }
    if (results_tight["DC_R1_FID"]){ kinematicTree->pass_dc1 = 1; }

    if (results_nom["DC_R3_FID"])  { kinematicTree->pass_dc3 = 0; }
    if (results_tight["DC_R3_FID"]){ kinematicTree->pass_dc3 = 1; }

    if (results_nom["EC_FID"])  { kinematicTree->pass_ec = 0; }
    if (results_tight["EC_FID"]){ kinematicTree->pass_ec = 1; }

    if (results_nom["Z_VERTEX"])  { kinematicTree->pass_vz = 0; }
    if (results_tight["Z_VERTEX"]){ kinematicTree->pass_vz = 1; }
    
    //    CheckForMesonAndFill(211); 
    //    CheckForMesonAndFill(-211); 
    CheckForMesonAndFill(321); 
    CheckForMesonAndFill(-321); 
  }

}

void KinematicTree::Save(std::string outputFilename){
  kinematicTree->Save(outputFilename); 
}

#endif
