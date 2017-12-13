#ifndef mm_histos_h
#define mm_histos_h

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
#include "TDirectory.h"
#include "TFile.h"
#include "TKey.h"
#include "TList.h"
#include "TRegexp.h"
#include "TString.h"

class MissingMassHistos {
  
 public:
  MissingMassHistos(){
    fNumBins   = 100; 
    fThreshold = 0.1; 

    fFile = new TFile(); 
  }

  ~MissingMassHistos(){
    if(fFile){
      if(fFile->IsOpen()){
	fFile->Close();
      }
    }
    
  }

  void Init(Config config,
	    std::string name){
    fName = name; 
    fBins = config.axes; 

    // build left and right window 
    float leftLimit  = (1-fThreshold)*config.cuts["missing_mass"]["min"];
    float rightLimit = (1+fThreshold)*config.cuts["missing_mass"]["max"];

    float wideLeft  = 0.60; 
    float wideRight = 2.00; 

    wideHist = new TH1F(Form("missingMass_wide_%s",fName.c_str()),"",200,wideLeft,wideRight);
    zoomHist = new TH1F(Form("missingMass_zoom_%s",fName.c_str()),"",fNumBins,leftLimit,rightLimit);

    // there are more elegant ways, but im trying to 
    // be a physicist
    for(int b=0; b<fBins[axis::x].GetNumber(); b++){
      std::string title(Form("missingMass_wide_x%d_%s", b, fName.c_str()));

      wide[axis::x].push_back(new TH1F(title.c_str(), 
				       title.c_str(),
				       100, wideLeft, wideRight));

      title = Form("missingMass_zoom_x%d_%s", b, fName.c_str());
      zoom[axis::x].push_back(new TH1F(title.c_str(), 
				       title.c_str(),
				       fNumBins, leftLimit, rightLimit));
    }

    for(int b=0; b<fBins[axis::q2].GetNumber(); b++){
      std::string title(Form("missingMass_wide_q2%d_%s", b, fName.c_str()));

      wide[axis::q2].push_back(new TH1F(title.c_str(), 
					title.c_str(),
					100, wideLeft, wideRight));
      title = Form("missingMass_zoom_q2%d_%s", b, fName.c_str());
      zoom[axis::q2].push_back(new TH1F(title.c_str(), 
					title.c_str(),
					fNumBins, leftLimit, rightLimit));
    }

    for(int b=0; b<fBins[axis::z].GetNumber(); b++){
      std::string title(Form("missingMass_wide_z%d_%s", b, fName.c_str()));

      wide[axis::z].push_back(new TH1F(title.c_str(), 
				       title.c_str(),
				       100, wideLeft, wideRight));
      title = Form("missingMass_zoom_z%d_%s", b, fName.c_str());
      zoom[axis::z].push_back(new TH1F(title.c_str(), 
				       title.c_str(),
				       fNumBins, leftLimit, rightLimit));
    }

    for(int b=0; b<fBins[axis::pt].GetNumber(); b++){
      std::string title(Form("missingMass_wide_pt%d_%s", b, fName.c_str()));

      wide[axis::pt].push_back(new TH1F(title.c_str(), 
					title.c_str(),
					100, wideLeft, wideRight));
      title = Form("missingMass_zoom_pt%d_%s", b, fName.c_str());
      zoom[axis::pt].push_back(new TH1F(title.c_str(), 
					title.c_str(),
					fNumBins, leftLimit, rightLimit));
    }
  }

  void Fill(PhysicsEvent & ev){
    std::map<int,int> indices;
    

    // What bin does this event belong in?
    indices[axis::x]  = fBins[axis::x] .FindBin(ev.x); 
    indices[axis::q2] = fBins[axis::q2].FindBin(ev.qq); 
    indices[axis::z]  = fBins[axis::z] .FindBin(ev.z); 
    indices[axis::pt] = fBins[axis::pt].FindBin(ev.pT); 

    // Is this a safe bin?
    if(IsSafe(indices)){
      double mm = sqrt(ev.mm2);

      wideHist->Fill(mm);
      zoomHist->Fill(mm);

      // Houston we have liftoff.
      wide[axis::x] [indices[axis::x]] ->Fill(mm);
      wide[axis::q2][indices[axis::q2]]->Fill(mm);
      wide[axis::z] [indices[axis::z]] ->Fill(mm);
      wide[axis::pt][indices[axis::pt]]->Fill(mm);

      zoom[axis::x] [indices[axis::x]] ->Fill(mm);
      zoom[axis::q2][indices[axis::q2]]->Fill(mm);
      zoom[axis::z] [indices[axis::z]] ->Fill(mm);
      zoom[axis::pt][indices[axis::pt]]->Fill(mm);
    }

  }

  // is the index safe ?
  bool IsSafe(std::map<int,int> indices){
    for(std::pair<int,int> index : indices){
      if( (index.second < 0) || (index.second >= fBins[index.first].GetNumber()) ){
	return false; 
      }
    }

    return true;
  }

  void Save(TFile *out){
    
    if (out->IsOpen()){
      out->mkdir("missingMass");
      out->cd("missingMass");

      wideHist->Write();
      zoomHist->Write();

      for(int b=0; b<fBins[axis::x].GetNumber(); b++){
	wide[axis::x][b]->Write();
	zoom[axis::x][b]->Write();
      }
      
      for(int b=0; b<fBins[axis::q2].GetNumber(); b++){
	wide[axis::q2][b]->Write();
	zoom[axis::q2][b]->Write();
      }
      
      for(int b=0; b<fBins[axis::z].GetNumber(); b++){
	wide[axis::z][b]->Write();
	zoom[axis::z][b]->Write();
      }
      
      for(int b=0; b<fBins[axis::pt].GetNumber(); b++){
	wide[axis::pt][b]->Write();
	zoom[axis::pt][b]->Write();
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

    // everything goes here 
    wideHist = (TH1F*) fFile->Get(Form("missingMass/missingMass_wide_%s",title.c_str()));
    zoomHist = (TH1F*) fFile->Get(Form("missingMass/missingMass_zoom_%s",title.c_str()));

    // load the business 
    for(int b=0; b<fBins[axis::x] .GetNumber(); b++){ 
      wide[axis::x] .push_back( (TH1F*) fFile->Get(Form("missingMass/missingMass_wide_x%d_%s", b,title.c_str())) ); 
      zoom[axis::x] .push_back( (TH1F*) fFile->Get(Form("missingMass/missingMass_zoom_x%d_%s", b,title.c_str())) ); 
    }

    for(int b=0; b<fBins[axis::q2] .GetNumber(); b++){ 
      wide[axis::q2] .push_back( (TH1F*) fFile->Get(Form("missingMass/missingMass_wide_q2%d_%s", b,title.c_str())) ); 
      zoom[axis::q2] .push_back( (TH1F*) fFile->Get(Form("missingMass/missingMass_zoom_q2%d_%s", b,title.c_str())) ); 
    }

    for(int b=0; b<fBins[axis::z] .GetNumber(); b++){ 
      wide[axis::z] .push_back( (TH1F*) fFile->Get(Form("missingMass/missingMass_wide_z%d_%s", b,title.c_str())) ); 
      zoom[axis::z] .push_back( (TH1F*) fFile->Get(Form("missingMass/missingMass_zoom_z%d_%s", b,title.c_str())) ); 
    }

    for(int b=0; b<fBins[axis::pt] .GetNumber(); b++){ 
      wide[axis::pt] .push_back( (TH1F*) fFile->Get(Form("missingMass/missingMass_wide_pt%d_%s", b,title.c_str())) ); 
      zoom[axis::pt] .push_back( (TH1F*) fFile->Get(Form("missingMass/missingMass_zoom_pt%d_%s", b,title.c_str())) ); 
    }

    std::cout << "Missing mass loaded = " << wideHist->GetEntries() << std::endl;     
    std::cout << "Missing mass loaded = " << zoomHist->GetEntries() << std::endl; 
  }

  std::map<int, DLineBins> GetBins() const {
    return fBins; 
  }

  void SetNumBins(int n){
    fNumBins = n; 
  }
  
  int GetNumBins() const {
    return fNumBins; 
  }

  void SetThreshold(float t){
    fThreshold = t; 
  }

  float GetThreshold() const {
    return fThreshold; 
  }

  // the histograms have index 
  // axis, heliciy 
  TH1F *wideHist, *zoomHist; 
  std::map<int, std::vector<TH1F*> > wide; 
  std::map<int, std::vector<TH1F*> > zoom; 

 protected:
  int   fNumBins; 
  float fThreshold; 

  std::string               fName; 
  std::map<int, DLineBins>  fBins; 

  TFile * fFile; 
};

#endif
