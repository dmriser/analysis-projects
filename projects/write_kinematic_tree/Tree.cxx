#ifndef tree_cxx
#define tree_cxx

#include <iostream>

#include "Tree.h"

#include "TBranch.h"
#include "TFile.h"
#include "TTree.h"

FlatTree::~FlatTree(){
}

void FlatTree::SetupTree(){
 
  // Create the structure of our kinematic tree 
  // and set the memory addresses of local vars.
  kinematicTree = new TTree("miniTree","Mininal Tree");
  kinematicTree->SetDirectory(0);

  b_qq          = kinematicTree->Branch("qq",           &qq);
  b_x           = kinematicTree->Branch("x",            &x);
  b_mes_b       = kinematicTree->Branch("mes_b",        &mes_b);
  b_x           = kinematicTree->Branch("w",            &w);
  b_y           = kinematicTree->Branch("y",            &y);
  b_z           = kinematicTree->Branch("z",            &z);
  b_mm2         = kinematicTree->Branch("mm2",          &mm2);
  b_hel         = kinematicTree->Branch("hel",          &hel);
  b_pt          = kinematicTree->Branch("pt",           &pt);
  b_ele_vz      = kinematicTree->Branch("ele_vz",       &ele_vz);
  b_mes_vz      = kinematicTree->Branch("mes_vz",       &mes_vz);
  b_ele_p       = kinematicTree->Branch("ele_p",        &ele_p);
  b_mes_p       = kinematicTree->Branch("mes_p",        &mes_p);
  b_ele_phi     = kinematicTree->Branch("ele_phi",      &ele_phi);
  b_mes_phi     = kinematicTree->Branch("mes_phi",      &mes_phi);
  b_ele_theta   = kinematicTree->Branch("ele_theta",    &ele_theta);
  b_mes_theta   = kinematicTree->Branch("mes_theta",    &mes_theta);
  b_pass_sf     = kinematicTree->Branch("pass_sf",      &pass_sf);
  b_pass_dc1    = kinematicTree->Branch("pass_dc1",     &pass_dc1);
  b_pass_dc3    = kinematicTree->Branch("pass_dc3",     &pass_dc3);
  b_pass_vz     = kinematicTree->Branch("pass_vz",      &pass_vz);
  b_pass_ec     = kinematicTree->Branch("pass_ec",      &pass_ec);
  b_meson_id    = kinematicTree->Branch("meson_id",     &meson_id);
  b_tof_mass    = kinematicTree->Branch("tof_mass2",    &tof_mass);
  b_phiH        = kinematicTree->Branch("phi_h",        &phiH);
  b_eventNumber = kinematicTree->Branch("event_number", &eventNumber); 
  b_dataType    = kinematicTree->Branch("data_type",    &dataType); 
  b_q           = kinematicTree->Branch("charge",       &charge);
  b_mes_etot    = kinematicTree->Branch("mes_etot",       &mes_etot);
  b_mes_nphe    = kinematicTree->Branch("mes_nphe",       &mes_nphe);
}

void FlatTree::Save(std::string outputFilename){
  TFile *outputFile = new TFile(outputFilename.c_str(), "recreate");
  kinematicTree->Write(); 
  outputFile   ->Close();
}

#endif
