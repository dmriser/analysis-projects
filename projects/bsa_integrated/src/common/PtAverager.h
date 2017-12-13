#ifndef pt_averager_h 
#define pt_averager_h 

// c++ standard libraries 
#include <iostream>

// this project 
#include "Config.h"
#include "KinematicHistograms.h"
#include "Types.h"

// root includes 
#include "TH1.h"

class PtAverager {

 public:
  
 PtAverager(Config c) : fConfig(c) {

    // passing the first element of a vector
    // with .data() is like using an array pointer
    average = new TH1F("average_pt", "", 
		       fConfig.axes[axis::z].GetNumber(),      
		       fConfig.axes[axis::z].GetLimits().data()
		       );

    counts = new TH1F("counts", "", 
		      fConfig.axes[axis::z].GetNumber(),      
		      fConfig.axes[axis::z].GetLimits().data()
		      );
  }

  ~PtAverager(){
    if(fFile && fFile->IsOpen()){
      fFile->Write();
      fFile->Close();
    }
  }

  void Fill(PhysicsEvent & event){
    int bin = fConfig.axes[axis::z].FindBin(event.z);

    if (IsSafe(bin, axis::z)){
      counts->Fill(event.z);
      average->Fill(event.z, event.pT);
    }
  }

  bool IsSafe(int index, int axis){
    return (index > -1 && index < fConfig.axes[axis].GetNumber());
  }

  void Save(TFile *out){
    if (out->IsOpen()){

      // setup directory 
      out->cd("/");
      out->mkdir("average_pt");
      out->cd("average_pt");

      // save both so we can do the averaging
      // after merging jobs 
      counts->Write();
      average->Write();

      // go back to top 
      out->cd("/");
    }
  }

  void Load(std::string filename){
    fFile = TFile::Open(filename.c_str());
    
    if(fFile && fFile->IsOpen()){
      average = (TH1F*) fFile->Get("average_pt/average_pt");
      counts =  (TH1F*) fFile->Get("average_pt/counts");

      std::cout << "[PtAverager::Load] average = " << average->GetEntries() << std::endl; 
      std::cout << "[PtAverager::Load] counts = " << counts ->GetEntries() << std::endl; 
    }
  }

  void CalculateAverage(){
    average->Sumw2();
    counts ->Sumw2();

    for(int b=1; b<=average->GetXaxis()->GetNbins(); b++){
      float numer = average->GetBinContent(b);
      float denom = counts ->GetBinContent(b);

      float err = sqrt((numer/pow(denom,2)) + (pow(numer,2)/pow(denom,3)));
      average->SetBinContent(b, numer/denom);
      average->SetBinError(b, err);
    }
  }

  void ScaleKinematicHistograms(KinematicHistos *kh){
    kh->histos[axis::z]->Divide(average);

    std::cout << "[PtAverager::ScaleKinematicHistograms] Just divided that! " << std::endl; 
  }

  TH1F *counts, *average; 

 protected:
  Config fConfig; 

  TFile *fFile; 

};

#endif
