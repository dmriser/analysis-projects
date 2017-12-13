#ifndef phi_fits_h
#define phi_fits_h

// standard includes 
#include <iostream>
#include <vector>

// this library includes
#include "DBins.h"
#include "PhysicsEvent.h"

// this project
#include "Config.h"
#include "Types.h"

// external 
#include "TF1.h"
#include "TH1.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TKey.h"
#include "TList.h"
#include "TRegexp.h"
#include "TString.h"

class PhiFits {
  
 public:
  PhiFits(){
   
    fFile = new TFile(); 
  }

  ~PhiFits(){
    if(fFile){
      if(fFile->IsOpen()){
	fFile->Close();
      }
    }
    
  }

  void Init(std::map<int, DLineBins> bins,
	    std::string name, 
	    std::string function){

    fName = name; 
    fBins = bins; 
    fFunction = function; 

    // there are more elegant ways, but im trying to 
    // be a physicist 
    for(int b=0; b<fBins[axis::x].GetNumber(); b++){
      std::string title(Form("fit_x%d_%s", b, fName.c_str()));
      fits[axis::x].push_back(new TF1(title.c_str(), fFunction.c_str(), fBins[axis::phi].GetMin(), fBins[axis::phi].GetMax()));
    }

    for(int b=0; b<fBins[axis::q2].GetNumber(); b++){
      std::string title(Form("fit_q2%d_%s", b, fName.c_str()));
      fits[axis::q2].push_back(new TF1(title.c_str(), fFunction.c_str(), fBins[axis::phi].GetMin(), fBins[axis::phi].GetMax()));
    }

    for(int b=0; b<fBins[axis::z].GetNumber(); b++){
      std::string title(Form("fit_z%d_%s", b, fName.c_str()));
      fits[axis::z].push_back(new TF1(title.c_str(), fFunction.c_str(), fBins[axis::phi].GetMin(), fBins[axis::phi].GetMax()));
    }

    for(int b=0; b<fBins[axis::pt].GetNumber(); b++){
      std::string title(Form("fit_pt%d_%s", b, fName.c_str()));
      fits[axis::pt].push_back(new TF1(title.c_str(), fFunction.c_str(), fBins[axis::phi].GetMin(), fBins[axis::phi].GetMax())); 
    }
  }

  void Save(TFile *out){
    
    if (out->IsOpen()){
      out->mkdir("fit");
      out->cd("fit");

      for(int b=0; b<fBins[axis::x].GetNumber(); b++){
	fits[axis::x][b]->Write();
      }
      
      for(int b=0; b<fBins[axis::q2].GetNumber(); b++){
	fits[axis::q2][b]->Write();
      }
      
      for(int b=0; b<fBins[axis::z].GetNumber(); b++){
	fits[axis::z][b]->Write();
      }
      
      for(int b=0; b<fBins[axis::pt].GetNumber(); b++){
	fits[axis::pt][b]->Write();
      }
      
      out->Write();
      out->cd("/");
    }
  }

  // passing the bins means you have to have 
  // the config file but its ok.  it makes life
  // easier in the loading phase.
  void Load(std::map<int, DLineBins> bins, 
	    std::string inputFilenameWithExtension, 
	    std::string title){
    
    // setup things first 
    fBins = bins; 
    
    // setup file 
    fFile = TFile::Open(inputFilenameWithExtension.c_str());

    // load the business 
    for(int b=0; b<fBins[axis::x] .GetNumber(); b++){ fits[axis::x] .push_back( (TF1*) fFile->Get(Form("fit/fit_x%d_%s", b,title.c_str())) ); }
    for(int b=0; b<fBins[axis::q2].GetNumber(); b++){ fits[axis::q2].push_back( (TF1*) fFile->Get(Form("fit/fit_q2%d_%s",b,title.c_str())) ); }
    for(int b=0; b<fBins[axis::z] .GetNumber(); b++){ fits[axis::z] .push_back( (TF1*) fFile->Get(Form("fit/fit_z%d_%s", b,title.c_str())) ); }
    for(int b=0; b<fBins[axis::pt].GetNumber(); b++){ fits[axis::pt].push_back( (TF1*) fFile->Get(Form("fit/fit_pt%d_%s",b,title.c_str())) ); }

    std::cout << "[PhiFits::Load] Loaded fits = " << fits[axis::x] .size() 
	      << " for title = " << title << std::endl; 
    std::cout << "[PhiFits::Load] Loaded fits = " << fits[axis::q2] .size() 
	      << " for title = " << title << std::endl; 
    std::cout << "[PhiFits::Load] Loaded fits = " << fits[axis::z] .size() 
	      << " for title = " << title << std::endl; 
    std::cout << "[PhiFits::Load] Loaded fits = " << fits[axis::pt] .size() 
	      << " for title = " << title << std::endl; 
  }

  std::map<int, DLineBins> GetBins() const {
    return fBins; 
  }

  // the fits have index 
  // axis, heliciy 
  std::map<int, std::vector<TF1*> > fits; 

 protected:
  std::string               fName; 
  std::string               fFunction; 
  std::map<int, DLineBins>  fBins; 

  TFile * fFile; 
};

#endif
