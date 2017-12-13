#ifndef tree_output_h 
#define tree_output_h 

// standard libs
#include <iostream>

// root includes 
#include "TBranch.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TTree.h"


class TreeOutput {
  
 public:
  TreeOutput(){
    tree = new TTree("events",""); 
    tree->SetDirectory(0); 

    b_id       = tree->Branch("id",       &id); 
    b_hel      = tree->Branch("hel",      &hel); 
    b_electron = tree->Branch("electron", &electron); 
    b_meson    = tree->Branch("meson",    &meson); 
  }

  TTree *tree; 

  // data 
  int             id; 
  int             hel; 
  TLorentzVector electron; 
  TLorentzVector meson; 

  // branches 
  TBranch *b_id; 
  TBranch *b_hel; 
  TBranch *b_electron; 
  TBranch *b_meson; 

  void Save(TFile *out){
    if (out->IsOpen()){
      tree->Write(); 
    }
  }

};

#endif 
