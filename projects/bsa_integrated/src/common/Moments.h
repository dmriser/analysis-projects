#ifndef moments_h 
#define moments_h 

// standard libs 
#include <iostream>
#include <map>

// this project 
#include "Constants.h"
#include "Types.h"

// h22 library 
#include "CommonTools.h"
#include "PhysicsEvent.h"

// root libs 
#include "TH1.h"
#include "TFile.h"

class Moments {

 public:
  Moments(){
    fMaxMoment = 12; 
    Init(); 
  }

  ~Moments(){
  }

  void SetMaxMoment(int max){
    fMaxMoment = max; 
    Init(); 
  }

  int GetMaxMoment() const {
    return fMaxMoment; 
  }

  void Fill(PhysicsEvent & ev, int hel){
    if (hel == helicity::plus) { fTotalPlus++;  }
    if (hel == helicity::minus){ fTotalMinus++; }

    for(int n=1; n<fMaxMoment+1; n++){
      FillCosMoment(ev, hel, n); 
      FillSinMoment(ev, hel, n); 
    }
  }


  void Save(TFile *out){
    MakeHistos(); 

    if(out && out->IsOpen()){
      out->mkdir("moments"); 
      out->cd("moments");
      cosMoments->Write(); 
      sinMoments->Write(); 
      out->cd("/"); 
    }
  }

  TH1F *cosMoments, *sinMoments; 

 protected:
  int fMaxMoment; 

  // holds moments up to the max number 
  std::map<int, double> fSinPlus; 
  std::map<int, double> fSinMinus; 
  std::map<int, double> fCosPlus; 
  std::map<int, double> fCosMinus; 

  long int fTotalPlus, fTotalMinus; 

  void Init(){
    fSinPlus .clear(); 
    fSinMinus.clear(); 
    fCosPlus .clear(); 
    fCosMinus.clear(); 

    fTotalPlus = 0; 
    fTotalMinus = 0; 
    for(int m=1; m<fMaxMoment+1; m++){
      fSinPlus[m]  = 0.0; 
      fSinMinus[m] = 0.0; 
      fCosPlus[m]  = 0.0; 
      fCosMinus[m] = 0.0; 
    }

    cosMoments = new TH1F("cosMoments", "cosMoments", fMaxMoment, 1, fMaxMoment); 
    sinMoments = new TH1F("sinMoments", "sinMoments", fMaxMoment, 1, fMaxMoment); 
  }

  void FillCosMoment(PhysicsEvent & ev, int hel, int moment){
    if(hel == helicity::plus){
      fCosPlus[moment] += cos(ev.phiHadron *to_radians *moment);
    } else if (hel == helicity::minus){
      fCosMinus[moment] += cos(ev.phiHadron *to_radians *moment);
    }
  }

  void FillSinMoment(PhysicsEvent & ev, int hel, int moment){
    if(hel == helicity::plus){
      fSinPlus[moment] += sin(ev.phiHadron *to_radians *moment);
    } else if (hel == helicity::minus){
      fSinMinus[moment] += sin(ev.phiHadron *to_radians *moment);
    }
  }

  void MakeHistos(){
    for(int m=1; m<fMaxMoment+1; m++){
      cosMoments->SetBinContent(m, (float) (fCosPlus[m]/fTotalPlus -fCosMinus[m]/fTotalMinus)/constants::beam_pol); 
      sinMoments->SetBinContent(m, (float) (fSinPlus[m]/fTotalPlus -fSinMinus[m]/fTotalMinus)/constants::beam_pol); 
    }
  }

};

#endif 
