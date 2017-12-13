#ifndef int_histos_h
#define int_histos_h

#include "Bins.h"
#include "Constants.h"
#include "Fits.h"
#include "Types.h"

#include "TF1.h"
#include "TFile.h"
#include "TH1.h"

class IntegratedHistos {
 public:
  
  // Used when they dont exist
 IntegratedHistos(Fits *f, std::string name, int index): fFits(f), fName(name), fMesonIndex(index) {
    CreateHistograms(); 
    fInputFile = new TFile(); 
  }

  //  Used when they are being loaded.
 IntegratedHistos(std::string infile, std::string name, int index) : fName(name), fMesonIndex(index){
    Load(infile);
  }

  ~IntegratedHistos(){
    if(fInputFile->IsOpen()){
      fInputFile->Write();
      fInputFile->Close();
    }
  }

  TH1D *h1_x[constants::MAX_BINS_Q2][constants::MAX_BINS_Z][constants::MAX_BINS_PT];
  TH1D *h1_q2[constants::MAX_BINS_Z][constants::MAX_BINS_PT][constants::MAX_BINS_X];
  TH1D *h1_z[constants::MAX_BINS_PT][constants::MAX_BINS_X][constants::MAX_BINS_Q2];
  TH1D *h1_pt[constants::MAX_BINS_X][constants::MAX_BINS_Q2][constants::MAX_BINS_Z];


  // assume nobody asks for weird values of x, z, pt 
  double GetAsymmetryX(int xbin, int q2bin, int zbin, int ptbin){
    return h1_x[q2bin][zbin][ptbin]->GetBinContent(xbin);
  }
  
  double GetStatErrorX(int xbin, int q2bin, int zbin, int ptbin){
    return h1_x[q2bin][zbin][ptbin]->GetBinError(xbin);
  }

  double GetAsymmetryZ(int xbin, int q2bin, int zbin, int ptbin){
    return h1_z[ptbin][xbin][q2bin]->GetBinContent(zbin);
  }
  
  double GetStatErrorZ(int xbin, int q2bin, int zbin, int ptbin){
    return h1_z[ptbin][xbin][q2bin]->GetBinError(zbin);
  }
  double GetAsymmetryPt(int xbin, int q2bin, int zbin, int ptbin){
    return h1_pt[xbin][q2bin][zbin]->GetBinContent(ptbin);
  }
  
  double GetStatErrorPt(int xbin, int q2bin, int zbin, int ptbin){
    return h1_pt[xbin][q2bin][zbin]->GetBinError(ptbin);
  }

  void Load(std::string infile){
    Bins *bins = Bins::GetInstance(fMesonIndex);
    fInputFile = TFile::Open(infile.c_str());
    
    // Initialize histograms for x
    for (int i=0; i<bins->GetQ2Bins()->GetNumber()+1; i++){
      for(int j=0; j<bins->GetZBins()->GetNumber()+1; j++){
	for(int k=0; k<bins->GetPtBins()->GetNumber()+1; k++){
	  h1_x[i][j][k] = (TH1D*) fInputFile->Get(Form("integrated/%s/h1_x_q2%d_z%d_pt%d_%s",
						       constants::Names::mesons[fMesonIndex].c_str(), 
						       i, j, k, fName.c_str()));
	}
      }
    }
    
    // Initialize histograms for q2
    for (int i=0; i<bins->GetZBins()->GetNumber()+1; i++){
      for(int j=0; j<bins->GetPtBins()->GetNumber()+1; j++){
	for(int k=0; k<bins->GetXBins()->GetNumber()+1; k++){
	  h1_q2[i][j][k] = (TH1D*) fInputFile->Get(Form("integrated/%s/h1_q2_z%d_pt%d_x%d_%s",
							constants::Names::mesons[fMesonIndex].c_str(), 
							i, j, k, fName.c_str()));
	}
      }
    }
    
    // Initialize histograms for z 
    for (int i=0; i<bins->GetPtBins()->GetNumber()+1; i++){
      for(int j=0; j<bins->GetXBins()->GetNumber()+1; j++){
	for(int k=0; k<bins->GetQ2Bins()->GetNumber()+1; k++){
	  h1_z[i][j][k] = (TH1D*) fInputFile->Get(Form("integrated/%s/h1_z_pt%d_x%d_q2%d_%s",
						       constants::Names::mesons[fMesonIndex].c_str(), 
						       i, j, k, fName.c_str()));
	}
      } 
    }
    
    // Initialize histograms for pt     
    for (int i=0; i<bins->GetXBins()->GetNumber()+1; i++){
      for(int j=0; j<bins->GetQ2Bins()->GetNumber()+1; j++){
	for(int k=0; k<bins->GetZBins()->GetNumber()+1; k++){
	  h1_pt[i][j][k] = (TH1D*) fInputFile->Get(Form("integrated/%s/h1_pt_x%d_q2%d_z%d_%s",
							constants::Names::mesons[fMesonIndex].c_str(), 
							i, j, k, fName.c_str()));
	}
      }
    }
  }
  
  void Save(std::string outfile, std::string mode){
    TFile *out = new TFile(outfile.c_str(), mode.c_str()); 
    
    std::string dir(Form("integrated/%s/", constants::Names::mesons[fMesonIndex].c_str()));
    out->mkdir(dir.c_str()); 
    out->cd(dir.c_str()); 

    Bins *bins = Bins::GetInstance(fMesonIndex);
    
    for (int i=0; i<bins->GetQ2Bins()->GetNumber()+1; i++){
      for(int j=0; j<bins->GetZBins()->GetNumber()+1; j++){
	for(int k=0; k<bins->GetPtBins()->GetNumber()+1; k++){
	  h1_x[i][j][k]->Write(); 
	}
      }
    }

    for (int i=0; i<bins->GetZBins()->GetNumber()+1; i++){
      for(int j=0; j<bins->GetPtBins()->GetNumber()+1; j++){
	for(int k=0; k<bins->GetXBins()->GetNumber()+1; k++){
	  h1_q2[i][j][k]->Write();
	}
      }
    }

    for (int i=0; i<bins->GetPtBins()->GetNumber()+1; i++){
      for(int j=0; j<bins->GetXBins()->GetNumber()+1; j++){
	for(int k=0; k<bins->GetQ2Bins()->GetNumber()+1; k++){
	  h1_z[i][j][k]->Write(); 
	} 
      }
    }

    for (int i=0; i<bins->GetXBins()->GetNumber()+1; i++){
      for(int j=0; j<bins->GetQ2Bins()->GetNumber()+1; j++){
	for(int k=0; k<bins->GetZBins()->GetNumber()+1; k++){
	  h1_pt[i][j][k]->Write(); 
	}
      }
    }

    out->cd("");
    out->Close(); 
  }

protected:
  std::string fName; 
  int         fMesonIndex; 
  Fits       *fFits; 
  TFile      *fInputFile; 

  void CreateHistograms(){
    
    for (int i=0; i<fFits->GetBinning()->GetQ2Bins()->GetNumber()+1; i++){
      for(int j=0; j<fFits->GetBinning()->GetZBins()->GetNumber()+1; j++){
	for(int k=0; k<fFits->GetBinning()->GetPtBins()->GetNumber()+1; k++){
	  h1_x[i][j][k] = new TH1D(Form("h1_x_q2%d_z%d_pt%d_%s", i, j, k, fName.c_str()), "", 
				   fFits->GetBinning()->GetXBins()->GetNumber(), 
				   fFits->GetBinning()->GetXBins()->GetLimits().data());
	  
	// iterate on the bins in x, adding them to the histogram 
	  for (int l=1; l<fFits->GetBinning()->GetXBins()->GetNumber()+1; l++){
	    h1_x[i][j][k]->SetBinContent(l, fFits->fit_asym[k][i][j][l]->GetParameter(0)); 	  
	    h1_x[i][j][k]->SetBinError(l, fFits->fit_asym[k][i][j][l]->GetParError(0)); 
	  }
	}
      }
    }
    
    for (int i=0; i<fFits->GetBinning()->GetZBins()->GetNumber()+1; i++){
      for(int j=0; j<fFits->GetBinning()->GetPtBins()->GetNumber()+1; j++){
	for(int k=0; k<fFits->GetBinning()->GetXBins()->GetNumber()+1; k++){
	  h1_q2[i][j][k] = new TH1D(Form("h1_q2_z%d_pt%d_x%d_%s", i, j, k, fName.c_str()), "", 
				    fFits->GetBinning()->GetQ2Bins()->GetNumber(), 
				    fFits->GetBinning()->GetQ2Bins()->GetLimits().data());
	  
	// iterate on the bins in q2, adding them to the histogram 
	  for (int l=1; l<fFits->GetBinning()->GetQ2Bins()->GetNumber()+1; l++){
	    h1_q2[i][j][k]->SetBinContent(l, fFits->fit_asym[k][l][i][j]->GetParameter(0)); 	  
	    h1_q2[i][j][k]->SetBinError(l,   fFits->fit_asym[k][l][i][j]->GetParError(0)); 
	  }
	}
      }
    }
    
    for (int i=0; i<fFits->GetBinning()->GetPtBins()->GetNumber()+1; i++){
      for(int j=0; j<fFits->GetBinning()->GetXBins()->GetNumber()+1; j++){
	for(int k=0; k<fFits->GetBinning()->GetQ2Bins()->GetNumber()+1; k++){
	  h1_z[i][j][k] = new TH1D(Form("h1_z_pt%d_x%d_q2%d_%s", i, j, k, fName.c_str()), "", 
				   fFits->GetBinning()->GetZBins()->GetNumber(), 
				   fFits->GetBinning()->GetZBins()->GetLimits().data());
	  
	// iterate on the bins in q2, adding them to the histogram 
	  for (int l=1; l<fFits->GetBinning()->GetZBins()->GetNumber()+1; l++){
	    h1_z[i][j][k]->SetBinContent(l, fFits->fit_asym[j][k][l][i]->GetParameter(0)); 	  
	    h1_z[i][j][k]->SetBinError(l, fFits->fit_asym[j][k][l][i]->GetParError(0)); 
	  }
	}
      }
    }
    
    for (int i=0; i<fFits->GetBinning()->GetXBins()->GetNumber()+1; i++){
      for(int j=0; j<fFits->GetBinning()->GetQ2Bins()->GetNumber()+1; j++){
	for(int k=0; k<fFits->GetBinning()->GetZBins()->GetNumber()+1; k++){
	  h1_pt[i][j][k] = new TH1D(Form("h1_pt_x%d_q2%d_z%d_%s", i, j, k, fName.c_str()), "", 
				    fFits->GetBinning()->GetPtBins()->GetNumber(), 
				    fFits->GetBinning()->GetPtBins()->GetLimits().data());
	  
	// iterate on the bins in pt, adding them to the histogram 
	  for (int l=1; l<fFits->GetBinning()->GetPtBins()->GetNumber()+1; l++){
	    h1_pt[i][j][k]->SetBinContent(l, fFits->fit_asym[i][j][k][l]->GetParameter(0)); 	  
	    h1_pt[i][j][k]->SetBinError(l, fFits->fit_asym[i][j][k][l]->GetParError(0)); 
	  }
	}
      }
    }


  }

};


#endif
