#ifndef fill_mc_cxx
#define fill_mc_cxx

#include <iostream>
#include <fstream>
#include <map>
using std::cout; 
using std::endl; 


#include "FillMonteCarlo.h"
#include "Tree.h"

#include "CommonTools.h"
#include "Corrections.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"

#include "TTree.h"
#include "TBranch.h"

FillMonteCarlo::FillMonteCarlo(h22Options *opts, Parameters *p, Parameters *ptight, 
			     Parameters *ploose) : GenericAnalysis(opts), fPars(p) {

  fFilter      = new ParticleFilter(fPars); 
  fFilterLoose = new ParticleFilter(ploose); 
  fFilterTight = new ParticleFilter(ptight); 

  recTree = new FlatTree(1); 
  recTree->SetupTree(); 

  genTree = new FlatTree(2); 
  genTree->SetupTree(); 

  localEventNumber = 0; 

  GSIM = true; 
}

FillMonteCarlo::~FillMonteCarlo(){

}

void FillMonteCarlo::FillGeneratedMeson(int index){
  
  TLorentzVector electron = event.GetGeneratedParticle(11); 
  TLorentzVector meson = event.GetGeneratedParticle(index); 
  
  int electronIndex = event.GetGeneratedParticleIndex(11); 
  int mesonIndex = event.GetGeneratedParticleIndex(index); 
 
  if (electronIndex>-1 && mesonIndex>-1){
    PhysicsEvent ev = builder.getPhysicsEvent(electron, meson);
      

    genTree->ele_theta = electron.Theta()*to_degrees; 
    genTree->ele_phi = electron.Phi()*to_degrees; 
    
    genTree->mes_theta = meson.Theta()*to_degrees; 
    genTree->mes_phi = meson.Phi()*to_degrees; 
    
    // correct helicity with waveplate already fixed
    genTree->hel      = event.ihel;
    genTree->meson_id = index; 
  
    // corrected vertex positions and betas 
    genTree->ele_vz = event.mcvz[electronIndex]; 
    genTree->mes_vz = event.mcvz[mesonIndex]; 
    
    // how do we calclate this, we have mctof
    float beta = event.mcp[mesonIndex]/sqrt(pow(event.mcp[mesonIndex],2)+pow(event.mcm[mesonIndex],2)); 
    genTree->mes_b = beta; 
    
    // This needs to be replaced. 
    // Not sure why, this comment is old 
    // and I don't see the problem.
    genTree->ele_p = event.mcp[electronIndex];
    
    // this gives uncorrected values of p
    //      ele_p = event.p[electronIndex]; 
    genTree->mes_p = event.mcp[mesonIndex];
    
    int charge = 0; 
    
    if (index > 0){
      genTree->charge = 1; 
    }  else {  
      genTree->charge = -1; 
    }
    
    genTree->x        = ev.x; 
    genTree->w        = ev.w; 
    genTree->y        = ev.y; 
    genTree->z        = ev.z; 
    genTree->qq       = ev.qq; 
    genTree->mm2      = ev.mm2; 
    genTree->phiH     = ev.phiHadron; 
    genTree->pt       = ev.pT; 
    genTree->tof_mass = pow(event.mcp[mesonIndex],2) * (1-pow(beta,2))/pow(beta,2); 
    
    // check number and fill 
    genTree->kinematicTree->Fill(); 
  }
  
}

void FillMonteCarlo::CheckForMesonAndFill(int index){
  
  //  std::vector<int> mesons = fFilter->getVectorOfParticleIndices(event, index); 

  //  std::cout << "found electron, searcing for mesons" << std::endl; 

  std::pair<bool,int> trueParticle = event.GetGeneratedToReconstructedMapping(index); 

  if (trueParticle.first){    
    //    std::cout << "found mesons" << std::endl; 

    // we will be writing something to the tree
    int electronIndex = event.GetElectronIndex(); 
    int mesonIndex    = trueParticle.second; 
    
    TLorentzVector electron = event.GetTLorentzVector(electronIndex, 11); 
    TLorentzVector meson    = event.GetTLorentzVector(mesonIndex,  index); 
      
    recTree->ele_theta = electron.Theta()*to_degrees; 
    recTree->ele_phi = electron.Phi()*to_degrees; 
    
    recTree->mes_theta = meson.Theta()*to_degrees; 
    recTree->mes_phi = meson.Phi()*to_degrees; 
    
    // build all the physics that we want to know about
    PhysicsEvent ev = builder.getPhysicsEvent(electron, meson);
    
    // correct helicity with waveplate already fixed
    recTree->hel      = event.ihel;

    // we got the right answer
    if (EventHasGeneratedParticle(index)){
      recTree->meson_id = index; 
    }

    // we have to find the right answer
    // because event doesnt have what we found
    else {
      
      if (index == 211){
	if (EventHasGeneratedParticle(321)){
	  recTree->meson_id = 321;
	}
      } else if (index == 321){
	if (EventHasGeneratedParticle(211)){
	  recTree->meson_id = 211;
	}
      }
     
      else if (index == -211){
	if (EventHasGeneratedParticle(-321)){
	  recTree->meson_id = -321;
	}
      }

      else if (index == -321){
	if (EventHasGeneratedParticle(-211)){
	  recTree->meson_id = -211;
	}
      }
      
      else {
	recTree->meson_id = -999; 
      }

    }
    
    

    recTree->meson_id = index; 
    
    // corrected vertex positions and betas 
    recTree->ele_vz = event.vz[electronIndex]; 
    recTree->mes_vz = event.vz[mesonIndex]; 
    recTree->mes_b  = event.b[mesonIndex]; 
    
    // This needs to be replaced. 
    // Not sure why, this comment is old 
    // and I don't see the problem.
    recTree->ele_p = electron.P(); 
    
    // this gives uncorrected values of p
    //      ele_p = event.p[electronIndex]; 
    recTree->mes_p = event.p[mesonIndex];
    recTree->charge = event.q[mesonIndex]; 
    
    recTree->mes_nphe = event.nphe[mesonIndex];
    recTree->mes_etot = event.etot[mesonIndex];

    recTree->x = ev.x; 
    recTree->w = ev.w; 
    recTree->y = ev.y; 
    recTree->z = ev.z; 
    recTree->qq = ev.qq; 
    recTree->mm2 = ev.mm2; 
    recTree->phiH = ev.phiHadron; 
    recTree->pt   = ev.pT; 
    recTree->tof_mass = pow(event.p[mesonIndex],2) * (1-pow(event.corr_b[mesonIndex],2))/pow(event.corr_b[mesonIndex],2); 

    recTree->kinematicTree->Fill(); 
  }
}

bool FillMonteCarlo::EventHasGeneratedParticle(int code){
  for(int ipart=0; ipart<event.mcnentr; ipart++){
    if (event.mcid[ipart] == code){
      return true;
    }
  }
  return false;
}

void FillMonteCarlo::ProcessEvent(){

  for (int ipart=0; ipart<event.gpart; ipart++){
    event.corr_b[ipart] = event.b[ipart]; 
  }

  // sometihng always gets filled 
  // because of generated
  localEventNumber++; 
  recTree->eventNumber = localEventNumber; 
  genTree->eventNumber = localEventNumber; 

  genTree->pass_sf  = -1; 
  genTree->pass_dc1 = -1; 
  genTree->pass_dc3 = -1; 
  genTree->pass_vz  = -1; 
  genTree->pass_ec  = -1; 

  // electron 
  if (EventHasGeneratedParticle(11)){
    if (EventHasGeneratedParticle( 211)){ FillGeneratedMeson( 211); }
    if (EventHasGeneratedParticle(-211)){ FillGeneratedMeson(-211); }
    if (EventHasGeneratedParticle( 321)){ FillGeneratedMeson( 321); }
    if (EventHasGeneratedParticle(-321)){ FillGeneratedMeson(-321); }
  }

  // reconstructed business 
  std::vector<int> electrons = fFilterLoose->getVectorOfParticleIndices(event, 11);   
  if (!electrons.empty()){

    event.SetElectronIndex(electrons[0]);
    fCorr.correctEvent(&event, GetRunNumber(), GSIM);


    std::map<std::string, bool> results_nom   = fFilter     ->eid_map(event, electrons[0]); 
    std::map<std::string, bool> results_tight = fFilterTight->eid_map(event, electrons[0]); 
    
    // loose is always passed by this point 
    recTree->pass_sf  = -1; 
    recTree->pass_dc1 = -1; 
    recTree->pass_dc3 = -1; 
    recTree->pass_vz  = -1; 
    recTree->pass_ec  = -1; 

    if (results_nom["EC_SAMPLING"])  { recTree->pass_sf = 0; }
    if (results_tight["EC_SAMPLING"]){ recTree->pass_sf = 1; }

    if (results_nom["DC_R1_FID"])  { recTree->pass_dc1 = 0; }
    if (results_tight["DC_R1_FID"]){ recTree->pass_dc1 = 1; }

    if (results_nom["DC_R3_FID"])  { recTree->pass_dc3 = 0; }
    if (results_tight["DC_R3_FID"]){ recTree->pass_dc3 = 1; }

    if (results_nom["EC_FID"])  { recTree->pass_ec = 0; }
    if (results_tight["EC_FID"]){ recTree->pass_ec = 1; }

    if (results_nom["Z_VERTEX"])  { recTree->pass_vz = 0; }
    if (results_tight["Z_VERTEX"]){ recTree->pass_vz = 1; }
    
    // reconstructed mesons 
    CheckForMesonAndFill(211); 
    CheckForMesonAndFill(-211); 
    CheckForMesonAndFill(321); 
    CheckForMesonAndFill(-321); 
  }

  

}

void FillMonteCarlo::Save(std::string outputFilename){
  recTree->Save(Form("rec_%s",outputFilename.c_str())); 
  genTree->Save(Form("gen_%s",outputFilename.c_str())); 
}

#endif
