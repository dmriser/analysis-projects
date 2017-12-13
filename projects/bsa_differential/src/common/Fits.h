#ifndef fit_h 
#define fit_h 

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
#include "Histograms.h"
#include "Types.h"

class Fits {
  
 public:
 Fits(Histos *h, std::string name, int index) : fHistos(h), fName(name), fMesonIndex(index) {
    SetupBinning();
    inputFile = new TFile(); 
  }

 Fits(Fits *f, std::string name) : fName(name) {
    inputFile = new TFile(); 

    bins        = f->GetBinning();
    fMesonIndex = f->GetMesonIndex(); 
    fHistos     = f->GetHistos(); 

    for(int i=0; i<bins->GetXBins()->GetNumber()+1; i++){
      for(int j=0; j<bins->GetQ2Bins()->GetNumber()+1; j++){
	for(int k=0; k<bins->GetZBins()->GetNumber()+1; k++){
	  for(int l=0; l<bins->GetPtBins()->GetNumber()+1; l++){
	    std::string current_name(Form("fit_asym_%s_x%d_q2%d_z%d_pt%d_%s", 
					  constants::Names::mesons[fMesonIndex].c_str(), 
					  i, j, k, l, fName.c_str()));
	    
	    fit_asym[i][j][k][l] = (TF1*) f->fit_asym[i][j][k][l]->Clone(); 
	    fit_asym[i][j][k][l]->SetName(current_name.c_str()); 
	    fit_asym[i][j][k][l]->SetTitle(current_name.c_str()); 
	  }
	}
      }
    }
  }
  
 Fits(std::string name, int index) : fName(name), fMesonIndex(index) {
    SetupBinning();
    inputFile = new TFile(); 
  }

  ~Fits(){
    if (inputFile)
      if (inputFile->IsOpen()){
    	inputFile->Write(); 
    	inputFile->Close(); 
      }
  }

  // the order is always x, z, pt, phi
  TF1 *fit_asym[constants::MAX_BINS_X][constants::MAX_BINS_Q2][constants::MAX_BINS_Z][constants::MAX_BINS_PT]; 
  
  void Save(std::string outfile, std::string saveOpts){
    TFile *out = new TFile(outfile.c_str(), saveOpts.c_str()); 
    
    out->mkdir(Form("fit/%s", constants::Names::mesons[fMesonIndex].c_str()));
    out->cd(Form("fit/%s/", constants::Names::mesons[fMesonIndex].c_str()));
    for(int i=0; i<bins->GetXBins()->GetNumber()+1; i++){
      for(int j=0; j<bins->GetQ2Bins()->GetNumber()+1; j++){
	for(int k=0; k<bins->GetZBins()->GetNumber()+1; k++){
	  for(int l=0; l<bins->GetPtBins()->GetNumber()+1; l++){
	    fit_asym[i][j][k][l]->Write(); 
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
	    fit_asym[i][j][k][l] = (TF1*) inputFile->Get(Form("fit/%s/fit_asym_%s_x%d_q2%d_z%d_pt%d_%s", 
							      constants::Names::mesons[fMesonIndex].c_str(), 
							      constants::Names::mesons[fMesonIndex].c_str(), 
							      i, j, k, l, fName.c_str()));
	    
	    std::cout << "[Fit::Load] Loaded " << fit_asym[i][j][k][l]->GetName() << std::endl;
	  }
	}
      }
    }
  }

  void Fit(){    
    for(int i=0; i<bins->GetXBins()->GetNumber()+1; i++){
      for(int j=0; j<bins->GetQ2Bins()->GetNumber()+1; j++){
	for(int k=0; k<bins->GetZBins()->GetNumber()+1; k++){
	  for(int l=0; l<bins->GetPtBins()->GetNumber()+1; l++){
	    std::string currentName(Form("fit_asym_%s_x%d_q2%d_z%d_pt%d_%s", 
					 constants::Names::mesons[fMesonIndex].c_str(), 
					 i, j, k, l, fName.c_str()));
    
	    fit_asym[i][j][k][l] = new TF1(currentName.c_str(),"[0]*sin((3.14159/180.0)*x)",
					bins->GetPhiBins()->GetMin(), bins->GetPhiBins()->GetMax());

	    fHistos->h1_asym[i][j][k][l]->Fit(currentName.c_str(), "RQ");
	  }
	}
      }
}
  }

  Bins *GetBinning() const {
    return bins; 
  }

  int GetMesonIndex() const {
    return fMesonIndex; 
  }

  Histos *GetHistos() const {
    return fHistos; 
  }

  std::string GetName() const {
    return fName; 
  }

 protected:
  std::string  fName; 
  int          fMesonIndex; 
  Histos      *fHistos;
  Bins        *bins; 
  TFile       *inputFile; 

  void SetupBinning(){
    bins = Bins::GetInstance(fMesonIndex); 
  }

};

#endif
