#ifndef histos_h 
#define histos_h 

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

class Histos {
  
 public:
 Histos(std::string name, int index) : fName(name), fMesonIndex(index) {
    SetupBinning();
    Init(); 

    inputFile = new TFile(); 
  }
  
  ~Histos(){
    if (inputFile)
      if (inputFile->IsOpen()){
    	inputFile->Write(); 
    	inputFile->Close(); 
      }
  }

  int GetMesonIndex() const {
    return fMesonIndex; 
  }

  std::string GetName() const {
    return fName; 
  }

  // the order is always x, z, pt, phi
  TH1D *h1_phi[constants::NHEL][constants::MAX_BINS_X][constants::MAX_BINS_Q2][constants::MAX_BINS_Z][constants::MAX_BINS_PT]; 
  TH1D *h1_asym[constants::MAX_BINS_X][constants::MAX_BINS_Q2][constants::MAX_BINS_Z][constants::MAX_BINS_PT]; 

  void Fill(PhysicsEvent &ev, int h){
    int x   = 1+bins->GetXBins()->FindBin(ev.x);
    int q2  = 1+bins->GetQ2Bins()->FindBin(ev.qq);
    int z   = 1+bins->GetZBins()->FindBin(ev.z);
    int pt  = 1+bins->GetPtBins()->FindBin(ev.pT);
    int phi = 1+bins->GetPhiBins()->FindBin(ev.phiHadron); 

    if (IndexIsSafe(h, x, q2, z, pt, phi)){

	  h1_phi[h][0][0][z][0]  ->Fill(ev.phiHadron); 
	  h1_phi[h][0][0][z][pt] ->Fill(ev.phiHadron); 
	  h1_phi[h][0][q2][z][0] ->Fill(ev.phiHadron); 
	  h1_phi[h][0][q2][z][pt]->Fill(ev.phiHadron); 
	  h1_phi[h][x][0][z][pt] ->Fill(ev.phiHadron); 
	  h1_phi[h][x][q2][z][0] ->Fill(ev.phiHadron); 	  

	if (ev.z > constants::LOWER_Z && ev.z < constants::UPPER_Z){
	  h1_phi[h][0][0][0][0]->Fill(ev.phiHadron); 
	  
	  h1_phi[h][x][0][0][0] ->Fill(ev.phiHadron); 
	  h1_phi[h][0][q2][0][0]->Fill(ev.phiHadron); 	  
	  h1_phi[h][0][0][0][pt]->Fill(ev.phiHadron); 

	  h1_phi[h][x][0][0][pt]->Fill(ev.phiHadron); 
	  h1_phi[h][x][q2][0][0]->Fill(ev.phiHadron); 

	  h1_phi[h][x][q2][0][pt]->Fill(ev.phiHadron); 
	}

    }
  }

  void CalculateAsymmetry(){
    for(int i=0; i<bins->GetXBins()->GetNumber()+1; i++){
      for(int j=0; j<bins->GetQ2Bins()->GetNumber()+1; j++){
	for(int k=0; k<bins->GetZBins()->GetNumber()+1; k++){
	  for(int l=0; l<bins->GetPtBins()->GetNumber()+1; l++){
	    std::string currentName(Form("h1_asym_%s_x%d_q2%d_z%d_pt%d_%s", 
					 constants::Names::mesons[fMesonIndex].c_str(), 
					 i, j, k, l, fName.c_str())); 

	    h1_asym[i][j][k][l] = new TH1D(currentName.c_str(),currentName.c_str(), bins->GetPhiBins()->GetNumber(), 
					   bins->GetPhiBins()->GetMin(), bins->GetPhiBins()->GetMax());
	    
	  for (int b=1; b<=h1_phi[Helicity::kNegative][i][j][k][l]->GetXaxis()->GetNbins(); b++){	      
	    double plus  = h1_phi[Helicity::kPositive][i][j][k][l]->GetBinContent(b);
	    double minus = h1_phi[Helicity::kNegative][i][j][k][l]->GetBinContent(b);
	    double sum   = plus+minus;
	    double diff  = plus-minus;
	    double err   = sqrt((1-pow(diff/sum,2))/sum);
	    
	    if (sum > 0.0) {
	      h1_asym[i][j][k][l]->SetBinContent(b,diff/sum/constants::BEAM_POL);
	      h1_asym[i][j][k][l]->SetBinError(b, err);
	    }
	  }
	}
      }
      }
    }
  }
  
  void Save(std::string outfile, std::string saveOpts){
    TFile *out = new TFile(outfile.c_str(), saveOpts.c_str()); 
    
    out->mkdir(Form("phi/%s", constants::Names::mesons[fMesonIndex].c_str()));
    out->cd(Form("phi/%s/", constants::Names::mesons[fMesonIndex].c_str()));
    for(int i=0; i<bins->GetXBins()->GetNumber()+1; i++){
      for(int j=0; j<bins->GetQ2Bins()->GetNumber()+1; j++){
	for(int k=0; k<bins->GetZBins()->GetNumber()+1; k++){
	  for(int l=0; l<bins->GetPtBins()->GetNumber()+1; l++){
	    h1_phi[0][i][j][k][l]->Write(); 
	    h1_phi[1][i][j][k][l]->Write(); 
	  }
	}
      }
    }
      out->cd("/");
      
      
      out->mkdir(Form("asym/%s", constants::Names::mesons[fMesonIndex].c_str()));
      out->cd(Form("asym/%s/", constants::Names::mesons[fMesonIndex].c_str()));
      for(int i=0; i<bins->GetXBins()->GetNumber()+1; i++){
	for(int j=0; j<bins->GetQ2Bins()->GetNumber()+1; j++){
	  for(int k=0; k<bins->GetZBins()->GetNumber()+1; k++){
	    for(int l=0; l<bins->GetPtBins()->GetNumber()+1; l++){
	      h1_asym[i][j][k][l]->Write(); 
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
	    h1_phi[Helicity::kNegative][i][j][k][l] = (TH1D*) inputFile->Get(Form("phi/%s/h1_phi_%s_helMinus_x%d_q2%d_z%d_pt%d_%s", 
										  constants::Names::mesons[fMesonIndex].c_str(),
										  constants::Names::mesons[fMesonIndex].c_str(), 
										  i, j, k, l, fName.c_str()));
	    
	    h1_phi[Helicity::kPositive][i][j][k][l] = (TH1D*) inputFile->Get(Form("phi/%s/h1_phi_%s_helPlus_x%d_q2%d_z%d_pt%d_%s", 
										  constants::Names::mesons[fMesonIndex].c_str(), 
										  constants::Names::mesons[fMesonIndex].c_str(), 
										  i, j, k, l, fName.c_str()));
	    
	    h1_asym[i][j][k][l] = (TH1D*) inputFile->Get(Form("asym/%s/h1_asym_%s_x%d_q2%d_z%d_pt%d_%s", 
							      constants::Names::mesons[fMesonIndex].c_str(), 
							      constants::Names::mesons[fMesonIndex].c_str(), 
							      i, j, k, fName.c_str()));
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
	    h1_phi[Helicity::kNegative][i][j][k][l] = new TH1D(Form("h1_phi_%s_helMinus_x%d_q2%d_z%d_pt%d_%s", 
								    constants::Names::mesons[fMesonIndex].c_str(), i, j, k, l, 
								    fName.c_str()),"", bins->GetPhiBins()->GetNumber(), 
							       bins->GetPhiBins()->GetMin(), bins->GetPhiBins()->GetMax());
	    
	    h1_phi[Helicity::kPositive][i][j][k][l] = new TH1D(Form("h1_phi_%s_helPlus_x%d_q2%d_z%d_pt%d_%s", 
								    constants::Names::mesons[fMesonIndex].c_str(), i, j, k, l, 
								    fName.c_str()),"", bins->GetPhiBins()->GetNumber(), 
							       bins->GetPhiBins()->GetMin(), bins->GetPhiBins()->GetMax());
	    
	    h1_asym[i][j][k][l] = new TH1D(Form("h1_asym_%s_x%d_q2%d_z%d_pt%d_%s", 
						constants::Names::mesons[fMesonIndex].c_str(), i, j, k, l, 
						fName.c_str()),"", bins->GetPhiBins()->GetNumber(), 
					   bins->GetPhiBins()->GetMin(), bins->GetPhiBins()->GetMax());
	    
	  }
	}
      }
    }
  }  

  bool IndexIsSafe(int h, int x, int q2, int z, int pt, int phi){
    if (h>-1 && h<2)
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
