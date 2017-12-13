#ifndef fitter_h 
#define fitter_h 

// standard 
#include <iostream>

// root 
#include "TF1.h"
#include "TH1.h"
#include "TVirtualFitter.h"

// this projcet
#include "PhiFits.h"
#include "PhiHistograms.h"
#include "Types.h"

// h22
#include "DBins.h"

class Fitter {

 public:
  Fitter(){

  }

  ~Fitter(){

  }

  virtual std::pair<PhiFits, PhiHistos> Fit(PhiHistos *histos, 
					    std::string title,
					    std::string function, 
					    std::string options){
    PhiFits   fits; 
    PhiHistos errHistos; 

    // setup 
    fits.Init(histos->GetBins(), title, function);

    // might want to change the phi bins to be 
    // much finer to get smooth confidence intervals 
    std::string confidenceLevelTitle(Form("%s_confidenceInterval",title.c_str()));
    errHistos.Init(histos->GetBins(), confidenceLevelTitle);

    // loop over histograms and fit 
    for(std::pair<int, std::vector<TH1F*>> p : histos->histos){
      for(int index=0; index < p.second.size(); index++){
	histos->histos[p.first][index]->Fit(fits.fits[p.first][index]->GetName(), options.c_str());
	TVirtualFitter::GetFitter()->GetConfidenceIntervals(errHistos.histos[p.first][index]);
      }
    }

    // give the data back 
    std::pair<PhiFits, PhiHistos> p; 
    p.first  = fits;
    p.second = errHistos; 
    return p; 
  }

};

#endif
