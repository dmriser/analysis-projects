#ifndef SLICEFITTER_CXX
#define SLICEFITTER_CXX

#include <iostream>
#include <vector>

#include "TF1.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TH2.h"

class SliceFitter {

public:
  SliceFitter(std::string fitFunction, int nSlices, double min, double max) : fFitFunction(fitFunction), 
									      fNumberSlices(nSlices), 
									      fxMin(min), fxMax(max) {
    SetupDefaultExpectedMean();
  }

  int GetNumberSlices() const{
    return fNumberSlices;
  }

  void SetNumberSlices(int value){
    fNumberSlices = value;
  }

  double GetXMin() const {
    return fxMin;
  }

  void SetXMin(double value) {
    fxMin = value;
  }

  double GetXMax() const {
    return fxMax;
  }

  void SetXMax(double value) {
    fxMax = value;
  }

  double GetUpperTolerance() const{
    return fUpperTolerance;
  }

  void SetUpperTolerance(double value){
    fUpperTolerance = value;
  }

  double GetLowerTolerance() const{
    return fLowerTolerance;
  }

  void SetLowerTolerance(double value){
    fLowerTolerance = value;
  }

  /** This function and the one below returns objects not references because we want to
      repeatedly call this fitting routine without overriding the
      previous results.  Therefor a copy is needed. */
  std::vector<TH1D> GetSlices() const {
    return fSlices;
  }

  std::vector<TF1> GetFits() const {
    return fFits;
  }

  void SetExpectedMean(std::string meanFunction){
    fExpectedMean = new TF1("fExpectedMean",meanFunction.c_str(),fxMin,fxMax);
  }

  void SetupDefaultExpectedMean(){
    if (fxMin > fxMax){
      std::cerr << "[SliceFitter::SliceFitter] Invalid bounds on fit, setting 0.0 -> 1.0! " << std::endl;
      fExpectedMean->SetRange(0.0, 1.0);
    } else {
      fExpectedMean = new TF1("fExpectedMean","[0]",fxMin,fxMax);
      fExpectedMean->SetParameter(0, 1.0);
    }

    fLowerTolerance = 0.8;
    fUpperTolerance = 1.2;
  }

  void Fit(TH2D *histo, std::string title){

    fSourceHisto = histo;

    fSlices.clear();
    fFits.clear(); 
    
    if (fSourceHisto){

      int xBins =   fSourceHisto->GetXaxis()->GetNbins();
      double xMin = fSourceHisto->GetXaxis()->GetBinLowEdge(1);
      double xMax = fSourceHisto->GetXaxis()->GetBinUpEdge(xBins);

      if ((xMin > fxMin) || (xMax < fxMax)){
        std::cerr << "[SliceFitter::Fit] Histogram bounds are inside of fit range, doing fits within range of histogram!" << std::endl;

        fxMin = xMin;
        fxMax = xMax;
      }

      int firstBin = fSourceHisto->GetXaxis()->FindBin(fxMin);
      int lastBin = fSourceHisto->GetXaxis()->FindBin(fxMax);
      int numberBinsInRange = lastBin-firstBin;

      // Setting up the correct number of slices if the user range is not
      // suitable.  The if statement actually is not needed, but makes it
      // more obvious what's going on.
      if (fNumberSlices > numberBinsInRange){
	fNumberSlices = numberBinsInRange; 
      }

      /*
	if (numberBinsInRange%fNumberSlices != 0){
        fNumberSlices -= numberBinsInRange%fNumberSlices;

        if (fNumberSlices < 1){
	std::cerr << "[SliceFitter::Fit] Error after adjusting number of slices, now negative or zero. " << std::endl;
	return;
        }
	}
      */

      std::cout << "[SliceFitter::Fit] Using number of slices = " << fNumberSlices << std::endl; 

      // start doing slices
      for (int i=0; i<fNumberSlices; ++i){
        std::string sliceTitle(Form("%s_slice%d",title.c_str(),i));
        std::string fitTitle(Form("%s_fit%d",title.c_str(),i));

        double xValue = i*(fxMax-fxMin)/fNumberSlices + fxMin;
        double xMid = xValue + (fxMax-fxMin)/fNumberSlices/2.0;
        int startBin = histo->GetXaxis()->FindBin(xValue);
        int stopBin = histo->GetXaxis()->FindBin(xValue + (fxMax-fxMin)/fNumberSlices);

        TF1 f(fitTitle.c_str(), fFitFunction.c_str(), fxMin, fxMax);
        f.SetTitle(fitTitle.c_str());
	f.SetParameter(1, fExpectedMean->Eval(xMid));
        f.SetRange(fLowerTolerance*fExpectedMean->Eval(xMid),
                   fUpperTolerance*fExpectedMean->Eval(xMid));

        TH1D h = (*histo->ProjectionY(fitTitle.c_str(),startBin,stopBin));
        h.SetTitle(sliceTitle.c_str());
        h.SetName(sliceTitle.c_str());
        h.Fit(fitTitle.c_str(), "RQ");

        fSlices.push_back(h);
        fFits.push_back(f);
      }
    } else {
      std::cerr << "[SliceFitter::Fit] Source histograms is a null pointer. " << std::endl;
    }
  }

  TGraphErrors GetGraphMu(std::string title){
    if (!fSlices.empty() && !fFits.empty()){
      std::vector<double> x;
      std::vector<double> xError;
      std::vector<double> y;
      std::vector<double> yError;

      for (int i=0; i<fSlices.size(); i++){
	x.push_back(fxMin + i*(fxMax-fxMin)/fNumberSlices);
	xError.push_back(0.00);
	y.push_back(fFits[i].GetParameter(1));
	yError.push_back(fFits[i].GetParError(1));
      }

      TGraphErrors g(x.size(), x.data(), y.data(), xError.data(), yError.data());
      g.SetTitle(Form("%s_mu",title.c_str()));
      g.SetName(Form("%s_mu",title.c_str()));

      return g;
    } else {
      std::cerr << "[SliceFitter::GetGraphMu] Failed trying to retrieve MU graph with empty fits or slices. " << std::endl;
      return TGraphErrors();
    }
  }

  TGraphErrors GetGraphSigma(std::string title){
    if (!fSlices.empty() && !fFits.empty()){
      std::vector<double> x;
      std::vector<double> xError;
      std::vector<double> y;
      std::vector<double> yError;

      for (int i=0; i<fSlices.size(); i++){
	x.push_back(fxMin + i*(fxMax-fxMin)/fNumberSlices);
	xError.push_back(0.00);
	y.push_back(fFits[i].GetParameter(2));
	yError.push_back(fFits[i].GetParError(2));
      }

      TGraphErrors g(x.size(), x.data(), y.data(), xError.data(), yError.data());
      g.SetTitle(Form("%s_sigma",title.c_str()));
      g.SetName(Form("%s_sigma",title.c_str()));

      return g;
    } else {
      std::cerr << "[SliceFitter::GetGraphMu] Failed trying to retrieve SIGMA graph with empty fits or slices. " << std::endl;
      return TGraphErrors();
    }
  }

  TF1 GetFitToMu(std::string fitFunction, std::string title, double fitMin, double fitMax){
    TGraphErrors g = GetGraphMu(title); 
  
    // setup proper bounds 
    if (fitMin < fxMin){
      std::cout << "[SliceFitter::GetFitToMu] fitMin < function min, setting same! " << std::endl; 
      fitMin = fxMin; 
    } 
    if (fitMax > fxMax){
      std::cout << "[SliceFitter::GetFitToMu] fitMax > function max, setting same! " << std::endl; 
      fitMax = fxMax; 
    } 

    std::string fitName(Form("%s_slicesMuFit",title.c_str())); 
    TF1 f(fitName.c_str(),fitFunction.c_str(), fitMin, fitMax); 
    g.Fit(fitName.c_str(),"RQ"); 

    return f; 
  }

  TF1 GetFitToMu(std::string fitFunction, std::string title){
    return GetFitToMu(fitFunction, title, fxMin, fxMax); 
  }

  TF1 GetFitToSigma(std::string fitFunction, std::string title, double fitMin, double fitMax){
    TGraphErrors g = GetGraphSigma(title); 
  
    // setup proper bounds 
    if (fitMin < fxMin){
      std::cout << "[SliceFitter::GetFitToMu] fitMin < function min, setting same! " << std::endl; 
      fitMin = fxMin; 
    } 
    if (fitMax > fxMax){
      std::cout << "[SliceFitter::GetFitToMu] fitMax > function max, setting same! " << std::endl; 
      fitMax = fxMax; 
    } 

    std::string fitName(Form("%s_slicesSigmaFit",title.c_str())); 
    TF1 f(fitName.c_str(),fitFunction.c_str(), fitMin, fitMax); 
    g.Fit(fitName.c_str(),"RQ"); 

    return f; 
  }

  TF1 GetFitToSigma(std::string fitFunction, std::string title){
    return GetFitToSigma(fitFunction, title, fxMin, fxMax); 
  }

protected:
  std::string fFitFunction; 
  int fNumberSlices;
  double fxMin, fxMax;

  /** Describes the upper and lower bounds on the expected mean to fit between.
    The fit will be setup fit->SetRange(fLowerTolerance*fExpectedMean->Eval(x),
    fUpperTolerance*fExpectedMean->Eval(x)) */
  double fUpperTolerance, fLowerTolerance;

  TH2D                *fSourceHisto;
  std::vector<TH1D>    fSlices;
  std::vector<TF1>     fFits;

  TF1                *fExpectedMean;
};

#endif
