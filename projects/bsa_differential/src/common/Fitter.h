#ifndef fitter_h
#define fitter_h

#include "Bins.h"
#include "Constants.h"
#include "Fits.h"
#include "Histograms.h"

// set limit for percentage error to be 
// fit on the bins 
#define NO_FIT_LIMIT 0.06


class Fitter{
 public:
 Fitter() {

  }

 virtual void Fit(Histos *h, Fits *f){

 }
  
};

class BasicFitter : public Fitter {
 public:
  void Fit(Histos *h, Fits *f){

    for (int i=0; i<f->GetBinning()->GetXBins()->GetNumber()+1; i++){
      for(int j=0; j<f->GetBinning()->GetQ2Bins()->GetNumber()+1; j++){
	for(int k=0; k<f->GetBinning()->GetZBins()->GetNumber()+1; k++){
	  for(int l=0; l<f->GetBinning()->GetPtBins()->GetNumber()+1; l++){
	    
	    TH1D *histo = (TH1D*) h->h1_asym[i][j][k][l]->Clone();
	    
	    // stop large error points from being fit
	    for(int b=1; b<=histo->GetXaxis()->GetNbins(); b++){
	      if (fabs(histo->GetBinError(b)) > NO_FIT_LIMIT){
		histo->SetBinError(b, 0); 
	      }
	    }
	    
	    std::string current_name(Form("fit_asym_%s_x%d_q2%d_z%d_pt%d_%s", 
					  constants::Names::mesons[h->GetMesonIndex()].c_str(), 
					  i, j, k, l, f->GetName().c_str()));
	    
	    f->fit_asym[i][j][k][l] = new TF1(current_name.c_str(),"[0]*sin((3.14159/180.0)*x)",
					      f->GetBinning()->GetPhiBins()->GetMin(), 
					      f->GetBinning()->GetPhiBins()->GetMax());
	    histo->Fit(current_name.c_str(), "RWL"); 
	  }
	}
      }
    } 
  }
  
};

class BasicAllMomentFitter : public Fitter {
 public:
  void Fit(Histos *h, Fits *f){
    std::string fitFunction("[0]*sin((3.14159/180.0)*x)/(1+[1]*cos((3.14159/180.0)*x)+[2]*cos(2*(3.14159/180.0)*x))"); 
    
    for (int i=0; i<f->GetBinning()->GetXBins()->GetNumber()+1; i++){
      for(int j=0; j<f->GetBinning()->GetQ2Bins()->GetNumber()+1; j++){
	for(int k=0; k<f->GetBinning()->GetZBins()->GetNumber()+1; k++){
	  for(int l=0; l<f->GetBinning()->GetPtBins()->GetNumber()+1; l++){
	    std::string current_name(Form("fit_asym_%s_x%d_q2%d_z%d_pt%d_%s", 
					  constants::Names::mesons[h->GetMesonIndex()].c_str(), 
					  i, j, k, l, f->GetName().c_str()));
	    
	   TH1D *histo = (TH1D*) h->h1_asym[i][j][k][l]->Clone();
	   
	   // stop large error points from being fit
	   for(int b=1; b<=histo->GetXaxis()->GetNbins(); b++){
	     if (fabs(histo->GetBinError(b)) > NO_FIT_LIMIT){
	       histo->SetBinError(b, 0); 
	     }
	   }
   
	   
	   f->fit_asym[i][j][k][l] = new TF1(current_name.c_str(),fitFunction.c_str(),
					     f->GetBinning()->GetPhiBins()->GetMin(), 
					     f->GetBinning()->GetPhiBins()->GetMax());
	   histo->Fit(current_name.c_str(), "RWL");
	  }
	}
      }
    }
    
  }
};

#endif
