#ifndef pid_histos_h 
#define pid_histos_h 

#include <iostream>
#include <vector>

#include "TFile.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
 
#include "CommonTools.h"
#include "DBins.h"
#include "h22Event.h"
#include "PhysicsEvent.h"

#include "Bins.h"
#include "Constants.h"
#include "Types.h"

class PidHistos {
  
 public:
 PidHistos(std::string name, int index) : fName(name), fMesonIndex(index) {
    SetupBinning();
    Init(); 
    
    inputFile = new TFile(); 
  }
  
  ~PidHistos(){
    if (inputFile)
      if (inputFile->IsOpen()){
    	inputFile->Write(); 
    	inputFile->Close(); 
      }
  }

  std::string GetName() const {
    return fName; 
  }

  // the order is always x, z, pt, phi
  TH2D *h2_p_b[constants::MAX_BINS_X][constants::MAX_BINS_Q2][constants::MAX_BINS_Z][constants::MAX_BINS_PT]; 
  TH1D *h1_tof_mass[constants::MAX_BINS_X][constants::MAX_BINS_Q2][constants::MAX_BINS_Z][constants::MAX_BINS_PT]; 

  void Fill(h22Event &event, PhysicsEvent &ev, int index){
    int x   = 1+bins->GetXBins()->FindBin(ev.x);
    int q2  = 1+bins->GetQ2Bins()->FindBin(ev.qq);
    int z   = 1+bins->GetZBins()->FindBin(ev.z);
    int pt  = 1+bins->GetPtBins()->FindBin(ev.pT);
    int phi = 1+bins->GetPhiBins()->FindBin(ev.phiHadron); 

    if (IndexIsSafe(x, q2, z, pt, phi)){
      h2_p_b[0][0][0][0]->Fill(event.p[index], event.corr_b[index]);

      h2_p_b[x][0][0][0]->Fill(event.p[index], event.corr_b[index]);
      h2_p_b[0][q2][0][0]->Fill(event.p[index], event.corr_b[index]);
      h2_p_b[0][0][z][0]->Fill(event.p[index], event.corr_b[index]);
      h2_p_b[0][0][0][pt]->Fill(event.p[index], event.corr_b[index]);

      h2_p_b[0][q2][z][pt]->Fill(event.p[index], event.corr_b[index]);
      h2_p_b[x][0][z][pt]->Fill(event.p[index], event.corr_b[index]);
      h2_p_b[x][q2][0][pt]->Fill(event.p[index], event.corr_b[index]);
      h2_p_b[x][q2][z][0]->Fill(event.p[index], event.corr_b[index]);

      h2_p_b[0][0][z][pt]->Fill(event.p[index], event.corr_b[index]);
      h2_p_b[x][0][0][pt]->Fill(event.p[index], event.corr_b[index]);
      h2_p_b[x][q2][0][0]->Fill(event.p[index], event.corr_b[index]);
      h2_p_b[0][q2][z][0]->Fill(event.p[index], event.corr_b[index]);

      // fill bin 
      h2_p_b[x][q2][z][pt]->Fill(event.p[index], event.corr_b[index]);

      double tof_mass = event.p[index] * sqrt((1-pow(event.corr_b[index],2))/pow(event.corr_b[index],2)); 
      h1_tof_mass[0][0][0][0] ->Fill(tof_mass);

      h1_tof_mass[x][0][0][0]  ->Fill(tof_mass);
      h1_tof_mass[0][q2][0][0] ->Fill(tof_mass);
      h1_tof_mass[0][0][z][0]  ->Fill(tof_mass);
      h1_tof_mass[0][0][0][pt] ->Fill(tof_mass);

      h1_tof_mass[0][0][z][pt]->Fill(tof_mass);
      h1_tof_mass[x][0][0][pt]->Fill(tof_mass);
      h1_tof_mass[x][q2][0][0]->Fill(tof_mass); 
      h1_tof_mass[0][q2][z][0]->Fill(tof_mass);

      // fill bin 
      h1_tof_mass[x][q2][z][pt]->Fill(tof_mass);
    }
  }

  
  void Save(std::string outfile, std::string saveOpts){
    TFile *out = new TFile(outfile.c_str(), saveOpts.c_str()); 

    out->mkdir(Form("pid/%s/", constants::Names::mesons[fMesonIndex].c_str()));
    out->cd(Form("pid/%s/",constants::Names::mesons[fMesonIndex].c_str()));
    for(int i=0; i<bins->GetXBins()->GetNumber()+1; i++){
      for(int j=0; j<bins->GetQ2Bins()->GetNumber()+1; j++){
	for(int k=0; k<bins->GetZBins()->GetNumber()+1; k++){
	  for(int l=0; l<bins->GetPtBins()->GetNumber()+1; l++){
	    h2_p_b[i][j][k][l]     ->Write(); 
	    h1_tof_mass[i][j][k][l]->Write(); 
	  }
	}
      }
    }
    
    out->cd("/");
    out->Close(); 
  }

  void Load(std::string file){
    inputFile = TFile::Open(file.c_str()); 

	for(int i=0; i<bins->GetXBins()->GetNumber()+1; i++){
	  for(int j=0; j<bins->GetQ2Bins()->GetNumber()+1; j++){
	    for(int k=0; k<bins->GetZBins()->GetNumber()+1; k++){
	      for(int l=0; l<bins->GetPtBins()->GetNumber()+1; l++){
		h2_p_b[i][j][k][l] = (TH2D*) inputFile->Get(Form("pid/%s/h2_p_b_%s_x%d_q2%d_z%d_pt%d_%s", 
								 constants::Names::mesons[fMesonIndex].c_str(), 
								 constants::Names::mesons[fMesonIndex].c_str(), 
								 i, j, k, l, fName.c_str()));
		
		h1_tof_mass[i][j][k][l] = (TH1D*) inputFile->Get(Form("pid/%s/h1_tofmass_%s_x%d_q2%d_z%d_pt%d_%s", 
								  constants::Names::mesons[fMesonIndex].c_str(),  
								  constants::Names::mesons[fMesonIndex].c_str(), 
								  i, j, k, l, fName.c_str()));
	      }
	    }
	  }
	}
  }

 protected:
  std::string  fName; 
  int          fMesonIndex; 
  Bins        *bins; 
  TFile       *inputFile; 

  void SetupBinning(){
    bins = Bins::GetInstance(fMesonIndex); 
  }

  void Init(){    
      for(int i=0; i<bins->GetXBins()->GetNumber()+1; i++){
	for(int j=0; j<bins->GetQ2Bins()->GetNumber()+1; j++){
	  for(int k=0; k<bins->GetZBins()->GetNumber()+1; k++){
	    for(int l=0; l<bins->GetPtBins()->GetNumber()+1; l++){
	      h2_p_b[i][j][k][l] = new TH2D(Form("h2_p_b_%s_x%d_q2%d_z%d_pt%d_%s", 
						 constants::Names::mesons[fMesonIndex].c_str(), 
						 i, j, k, l, fName.c_str()), "", 
					    100, 0.5, 3.0, 100, 0.2, 1.1);
	      
	      h1_tof_mass[i][j][k][l] = new TH1D(Form("h1_tofmass_%s_x%d_q2%d_z%d_pt%d_%s", 
						      constants::Names::mesons[fMesonIndex].c_str(), 
						      i, j, k, l, fName.c_str()), "", 100, 0.0, 1.0);
	    }
	  }
	}      
      }
  }
  
  bool IndexIsSafe(int x, int q2, int z, int pt, int phi){
    if (x > 0)
      if (q2 > 0)
	if (z > 0)
	  if (pt > 0)
	    if (phi > 0)
	      return true;
    
    return false; 
  }

};

#endif
