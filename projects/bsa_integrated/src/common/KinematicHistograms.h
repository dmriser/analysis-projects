#ifndef kin_histos_h
#define kin_histos_h

// standard includes 
#include <iostream>
#include <vector>

// this library includes
#include "DBins.h"
#include "PhysicsEvent.h"

// this project
#include "PhiFits.h"
#include "Config.h"
#include "Types.h"

// external 
#include "TDirectory.h"
#include "TFile.h"
#include "TKey.h"
#include "TList.h"
#include "TRegexp.h"
#include "TString.h"

class KinematicHistos {
  
 public:
  KinematicHistos(){
   

    fFile = new TFile(); 
  }

  ~KinematicHistos(){
    if(fFile){
      if(fFile->IsOpen()){
	fFile->Close();
      }
    }
    
  }

  void Init(std::map<int, DLineBins> bins,
	    std::string name){
    fName = name; 
    fBins = bins; 

    // there are more elegant ways, but im trying to 
    // be a physicist 
    histos[axis::x]  = new TH1F(Form("kin_x_%s", fName.c_str()), "", fBins[axis::x] .GetNumber(), fBins[axis::x] .GetLimits().data());
    histos[axis::q2] = new TH1F(Form("kin_q2_%s",fName.c_str()), "", fBins[axis::q2].GetNumber(), fBins[axis::q2].GetLimits().data());
    histos[axis::z]  = new TH1F(Form("kin_z_%s", fName.c_str()), "", fBins[axis::z] .GetNumber(), fBins[axis::z] .GetLimits().data());
    histos[axis::pt] = new TH1F(Form("kin_pt_%s",fName.c_str()), "", fBins[axis::pt].GetNumber(), fBins[axis::pt].GetLimits().data());
  }

  void Fill(PhysicsEvent & ev){
    histos[axis::x] ->Fill(ev.x);
    histos[axis::q2]->Fill(ev.qq);
    histos[axis::z] ->Fill(ev.z);
    histos[axis::pt]->Fill(ev.pT);
  }

  void Save(TFile *out){    
    if (out->IsOpen()){
      out->mkdir("kin");
      out->cd("kin");

      for(std::pair<int, TH1F*> p : histos){
	p.second->Write(); 
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
    
    histos[axis::x]  = (TH1F*) fFile->Get(Form("kin/kin_x_%s", title.c_str()));
    histos[axis::q2] = (TH1F*) fFile->Get(Form("kin/kin_q2_%s",title.c_str()));
    histos[axis::z]  = (TH1F*) fFile->Get(Form("kin/kin_z_%s", title.c_str()));
    histos[axis::pt] = (TH1F*) fFile->Get(Form("kin/kin_pt_%s",title.c_str()));

    std::cout << "[KinematicHistos::Load] loaded = " << title << " entries = " << histos[axis::x] ->GetEntries() << std::endl; 
    std::cout << "[KinematicHistos::Load] loaded = " << title << " entries = " << histos[axis::q2]->GetEntries() << std::endl; 
    std::cout << "[KinematicHistos::Load] loaded = " << title << " entries = " << histos[axis::z] ->GetEntries() << std::endl; 
    std::cout << "[KinematicHistos::Load] loaded = " << title << " entries = " << histos[axis::pt]->GetEntries() << std::endl; 
  }


  void BuildFromFits(PhiFits *f){
    BuildFromFits(f,0); 
  }

  void BuildFromFits(PhiFits *f, int par){
    fBins = f->GetBins();

    for(int index=0; index < fBins[axis::x].GetNumber(); index++){
      histos[axis::x]->SetBinContent(index+1, f->fits[axis::x][index]->GetParameter(par));
      histos[axis::x]->SetBinError(index+1,   f->fits[axis::x][index]->GetParError(par));
    }

    for(int index=0; index < fBins[axis::q2].GetNumber(); index++){
      histos[axis::q2]->SetBinContent(index+1, f->fits[axis::q2][index]->GetParameter(par));
      histos[axis::q2]->SetBinError(index+1,   f->fits[axis::q2][index]->GetParError(par));
    }

    for(int index=0; index < fBins[axis::z].GetNumber(); index++){
      histos[axis::z]->SetBinContent(index+1, f->fits[axis::z][index]->GetParameter(par));
      histos[axis::z]->SetBinError(index+1,   f->fits[axis::z][index]->GetParError(par));
    }

    for(int index=0; index < fBins[axis::pt].GetNumber(); index++){
      histos[axis::pt]->SetBinContent(index+1, f->fits[axis::pt][index]->GetParameter(par));
      histos[axis::pt]->SetBinError(index+1,   f->fits[axis::pt][index]->GetParError(par));
    }

  }

  std::map<int, DLineBins> GetBins() const {
    return fBins; 
  }

  // the histograms have index 
  // axis, heliciy 
  std::map<int, TH1F*> histos; 

 protected:
  std::string               fName; 
  std::map<int, DLineBins>  fBins; 

  TFile * fFile; 
};

#endif
