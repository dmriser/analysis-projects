#ifndef tree_h
#define tree_h

#include <iostream>
#include <fstream>

#include "TBranch.h"
#include "TLorentzVector.h"
#include "TTree.h"

class FlatTree {
 public:
 FlatTree(int type) : dataType(type){
    eventNumber = 0; 
  }

  ~FlatTree();
  
  // Structure of output tree
  TTree   *kinematicTree;
  TBranch *b_qq;
  TBranch *b_x;
  TBranch *b_mes_b;
  TBranch *b_y;
  TBranch *b_z;
  TBranch *b_w;
  TBranch *b_mm2;
  TBranch *b_hel;
  TBranch *b_meson_id;
  TBranch *b_tof_mass; 
  TBranch *b_pt;
  TBranch *b_phiH;
  TBranch *b_pass_sf;
  TBranch *b_pass_dc1;
  TBranch *b_pass_dc3;
  TBranch *b_pass_vz;
  TBranch *b_pass_ec;
  TBranch *b_ele_vz;
  TBranch *b_mes_vz;
  TBranch *b_ele_theta;
  TBranch *b_ele_phi;
  TBranch *b_ele_p;
  TBranch *b_mes_p;
  TBranch *b_mes_theta;
  TBranch *b_mes_phi;
  TBranch *b_eventNumber; 
  TBranch *b_dataType; 
  TBranch *b_q; 
  TBranch *b_mes_nphe;
  TBranch *b_mes_etot;


  Float_t qq; 
  Float_t x; 
  Float_t mes_b; 
  Float_t w; 
  Float_t y; 
  Float_t mm2; 
  Float_t z; 
  Float_t pt; 
  Float_t ele_p; 
  Float_t mes_p; 
  Float_t ele_phi; 
  Float_t mes_phi; 
  Float_t ele_theta; 
  Float_t mes_theta; 
  Float_t ele_vz; 
  Float_t mes_vz; 
  Float_t phiH;
  Float_t tof_mass; 
  Float_t mes_etot; 
  Int_t mes_nphe; 
  Int_t    meson_id; 
  Int_t    hel; 
  Int_t    pass_sf; 
  Int_t    pass_dc1; 
  Int_t    pass_dc3; 
  Int_t    pass_vz; 
  Int_t    pass_ec; 
  Int_t    dataType; 
  Int_t    eventNumber; 
  Int_t    charge; 

  void SetupTree();
  void Save(std::string outputFilename);
};

#endif
