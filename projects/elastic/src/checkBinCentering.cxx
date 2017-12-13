// --------------------------------------------
//   doCrossSection.cxx
//   Feb 10, 2016
//   David Riser 
// --------------------------------------------

#include <iostream>
#include <vector>
using namespace std;

// --------------------------------------------
//   Root Libs 
// --------------------------------------------
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "THnSparse.h"

// --------------------------------------------
//   My Libs 
// --------------------------------------------
#include "BostedElasticWrapper.h"
#include "CommonTools.h"
#include "h22Option.h"

class Analysis{
public:
  Analysis(){ } 
  ~Analysis(){ } 

protected:
  const static int numberSectors   = 6;
  const static int numberThetaBins = 100; 
  const static int numberPhiBins   = 1; 

  vector<TH1D*>  histoContainer; 

  TH1D       *binCorr_theta;

  double               totalCharge;
  double               thetaMin;
  double               thetaMax;
  double               phiMin;  
  double               phiMax;  
  double               thetaStep;
  double               phiStep;  
  BostedElasticWrapper model;

public:
  void makeBinCenteringCorrections(int numberBins);
  void setupBinning(); 
  void save(string outputFilename); 
};

void Analysis::setupBinning(){
  thetaMin  = 22.5;
  thetaMax  = 50.0;
  phiMin    = -12.0; 
  phiMax    = 12.0;
  thetaStep = (thetaMax-thetaMin)/numberThetaBins; 
  phiStep   = (phiMax-phiMin)/numberPhiBins;   
}

void Analysis::makeBinCenteringCorrections(int numberBins){
  thetaStep = (thetaMax-thetaMin)/numberBins; 

  binCorr_theta = new TH1D(Form("binCorr_theta_bins%d",numberBins),"",numberBins,thetaMin,thetaMax); 
  for(int tBin=1; tBin<=numberBins; tBin++){
    double thetaLow = (tBin-1)*thetaStep+thetaMin;
    double thetaMid = (tBin-0.5)*thetaStep+thetaMin; 
    double thetaUp  = tBin*thetaStep+thetaMin;
    double low = model.getRadiatedValue(5.498,thetaLow,5.0/865.0, 1.1);
    double mid = model.getRadiatedValue(5.498,thetaMid, 5.0/865.0, 1.1);
    double up  = model.getRadiatedValue(5.498,thetaUp, 5.0/865.0, 1.1);
    double avg = (up+low)/2;

    binCorr_theta->SetBinContent(tBin,mid/avg);
    binCorr_theta->SetBinError(tBin,0.00);
    
    cout << " thetaLow = " << thetaLow << " thetaMid = " << thetaMid << " thetaUp = " << thetaUp << endl; 
  }

  histoContainer.push_back(binCorr_theta);
}

void Analysis::save(string outputFilename){
  TFile *out = new TFile(outputFilename.c_str(),"recreate"); 
  
  for(int h=0; h<histoContainer.size(); ++h){
    histoContainer[h]->Write();
  }

  out->Close();
}

int main(int argc, char *argv[]){

  h22Options *options = new h22Options(); 
  options->args["INPUT"].args = "UNSET";
  options->args["INPUT"].type = 1;
  options->args["INPUT"].name = "Input file with THnSparse events";
  options->set(argc, argv); 

  Analysis analysis; 
  analysis.setupBinning();
  analysis.makeBinCenteringCorrections(10);
  analysis.makeBinCenteringCorrections(50);
  analysis.makeBinCenteringCorrections(100);
  analysis.makeBinCenteringCorrections(500);
  analysis.makeBinCenteringCorrections(5000);
  analysis.save(options->args["OUT"].args); 
  return 0; 
}
