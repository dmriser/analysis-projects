
#ifndef base_dis_histograms_h
#define base_dis_histograms_h

// c++ 
#include <iostream>
using namespace std; 

// me 
#include "CommonTools.h"
#include "PhysicsEvent.h"

// root 
#include "TFile.h"
#include "TH2.h"
#include "TH3.h"
#include "THnSparse.h"

#define NDIM 4
 
class BaseDISHistograms{
 public:
  BaseDISHistograms();
  ~BaseDISHistograms();

  const static int numberOfXBins  = 200;
  const static int numberOfQQBins = 200; 
  const static int numberOfWBins  = 200;

  // c++ 11 Standrd 
  //  static constexpr double xMin  = 0.1;  static constexpr double xMax  = 0.6; 
  //  static constexpr double qqMin = 1.7;  static constexpr double qqMax = 4.2;
  //  static constexpr double wMin  = 1.1;  static constexpr double wMax  = 2.1; 

  const static double xMin  = 0.1;  const static double xMax  = 0.6; 
  const static double qqMin = 1.7;  const static double qqMax = 4.2;
  const static double wMin  = 1.1;  const static double wMax  = 2.1; 
  
  string baseName; 
  
  TFile * inputFile; 
  TH2I *xByQQ[7]; 
  TH2I *wByQQ[7]; 
  TH3I *pByTheta[7];

  // New thing
  THnSparseI *events; 

  void Init(std::string name, std::string title);
  void InitWithNewNumberOfBins(std::string name, std::string title, int xBins, int yBins);
  void Load(std::string filename, std::string name); 
  void Save(std::string filename, std::string option);
  void Fill(PhysicsEvent event, int sector); 
  void Rebin2D(int xFactor, int yFactor);
  
  // These were added for recAndGen coincident in the same bin,
  // there should be a better way to accomplish this.  Keep thinkin.
  void FillxByQQ(double x, double qq, int sector);
  void FillwByQQ(double w, double qq, int sector);
};

BaseDISHistograms::BaseDISHistograms(){
  inputFile = new TFile();
}

BaseDISHistograms::~BaseDISHistograms(){
  if (inputFile->IsOpen()){
    inputFile->Close();
  }
}

void BaseDISHistograms::Init(string name, string title){
  baseName = name; 

  for (int isect = 0; isect < 7; isect++){
    xByQQ[isect]    = new TH2I(Form("%s_xByQQ_s%d",name.c_str(),isect),Form("%s x vs. Q^{2} Sect. %d",title.c_str(),isect), numberOfXBins, xMin, xMax, numberOfQQBins, qqMin, qqMax);
    wByQQ[isect]    = new TH2I(Form("%s_wByQQ_s%d",name.c_str(),isect),Form("%s w vs. Q^{2} Sect. %d",title.c_str(),isect), numberOfWBins, wMin, wMax, numberOfQQBins, qqMin, qqMax);
    pByTheta[isect] = new TH3I(Form("%s_pByTheta_s%d",name.c_str(),isect),Form("%s P vs. #theta Sect. %d",title.c_str(),isect), 200, 0.5, 5.5, 200, 12, 65, numberOfQQBins, qqMin, qqMax); 
  }

  // Sparse that holds everything 
  // For THnSparseI Array 
  // 8-Dimensional Binning 
  // 0:sect 
  // 1:phi_rel 
  // 2:q2 
  // 3:w
  int numberBins[NDIM] = {  6,  10,  10,  40};
  double min[NDIM]     = {1.0, -20, 0.5, 1.1};
  double max[NDIM]     = {7.0,  20, 4.8, 3.0};
  events               = new THnSparseI(Form("%s_events",name.c_str()),Form("THnSparseI Events for %s",title.c_str()),NDIM,numberBins,min,max);

  // Unused but has to be here for the event that 
  // class is instantiated in loading mode. 
  inputFile = new TFile(); 
}

void BaseDISHistograms::InitWithNewNumberOfBins(string name, string title, int xBins, int yBins){
  baseName = name; 

  for (int isect = 0; isect < 7; isect++){
    xByQQ[isect]    = new TH2I(Form("%s_xByQQ_s%d",name.c_str(),isect),Form("%s x vs. Q^{2} Sect. %d",title.c_str(),isect), xBins, xMin, xMax, yBins, qqMin, qqMax);
    wByQQ[isect]    = new TH2I(Form("%s_wByQQ_s%d",name.c_str(),isect),Form("%s w vs. Q^{2} Sect. %d",title.c_str(),isect), xBins, wMin, wMax, yBins, qqMin, qqMax);
    pByTheta[isect] = new TH3I(Form("%s_pByTheta_s%d",name.c_str(),isect),Form("%s P vs. #theta Sect. %d",title.c_str(),isect), 200, 0.5, 5.5, 200, 12, 65, numberOfQQBins, qqMin, qqMax); 
  }

  // Unused but has to be here for the event that 
  // class is instantiated in loading mode. 
  inputFile = new TFile(); 
}

void BaseDISHistograms::Save(string filename, string option){

  TFile * file = TFile::Open(filename.c_str(),option.c_str());

  if (file->IsOpen()){
    for (int isect = 0; isect < 7; isect++){
      xByQQ[isect]   ->Write();
      wByQQ[isect]   ->Write();
      pByTheta[isect]->Write();
    }

    events->Write();
  } else {
    cout << "[BaseDISHistograms::Save] Error opening file... " << filename << endl; 
  }

  file->Write();
  file->Close();
}

void BaseDISHistograms::Load(string filename, string name){

  inputFile = TFile::Open(filename.c_str());
  baseName = name; 

  if (inputFile->IsOpen()){
    for (int isect = 0; isect < 7; isect++){
      xByQQ[isect]    = (TH2I*) inputFile->Get(Form("%s_xByQQ_s%d",   name.c_str(),isect));
      wByQQ[isect]    = (TH2I*) inputFile->Get(Form("%s_wByQQ_s%d",   name.c_str(),isect));
      pByTheta[isect] = (TH3I*) inputFile->Get(Form("%s_pByTheta_s%d",name.c_str(),isect));
    }

    events = (THnSparseI*) inputFile->Get(Form("%s_events",name.c_str()));
  } else {
    cout << "[BaseDISHistograms::Load] Error opening file... " << filename << endl; 
  }

  //  file->Close();
   
}

void BaseDISHistograms::Fill(PhysicsEvent event, int sector){
  // We assume that nobody sends us sector > 6 or sector < 1
  xByQQ[0]         ->Fill(event.x, event.qq);
  xByQQ[sector]    ->Fill(event.x, event.qq);
  wByQQ[0]         ->Fill(event.w, event.qq);
  wByQQ[sector]    ->Fill(event.w, event.qq);
  pByTheta[0]      ->Fill(event.detectedElectron.P(), event.detectedElectron.Theta()*to_degrees, event.qq); 
  pByTheta[sector] ->Fill(event.detectedElectron.P(), event.detectedElectron.Theta()*to_degrees, event.qq); 

  double relativePhi     = getRelativePhi(to_degrees*event.detectedElectron.Phi());
  double dataPoint[NDIM] = {(double)sector, relativePhi, event.qq, event.w};
  events->Fill(dataPoint);
}

void BaseDISHistograms::FillxByQQ(double x, double qq, int sector){
  xByQQ[0]      ->Fill(x, qq);
  xByQQ[sector] ->Fill(x, qq);
}

void BaseDISHistograms::FillwByQQ(double w, double qq, int sector){
  wByQQ[0]      ->Fill(w, qq);
  wByQQ[sector] ->Fill(w, qq);
}

void BaseDISHistograms::Rebin2D(int xFactor, int yFactor){
  for (int ihist = 0; ihist < 7; ihist++) {
    xByQQ[ihist]->Rebin2D(xFactor, yFactor);
    wByQQ[ihist]->Rebin2D(xFactor, yFactor); 
  }
}

#endif
