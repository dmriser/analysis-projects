#ifndef histogram_loader_h
#define histogram_loader_h

// c++ includes 
#include <iostream>
#include <vector>
using namespace std; 

// my includes 
#include "h22Reader.h"
#include "HistogramLoader.h"
#include "PhysicsEventBuilder.h"
#include "PhysicsEventSelector.h"
#include "StatusBar.h"

class HistogramLoader : public h22Reader{
 public:
  HistogramLoader(PhysicsEventSelector *eventCriteria, std::string outputFile, std::string saveOpts);
  ~HistogramLoader();

 protected:
  // Put some histograms here
  std::string outputFilenameWithExtension; 
  std::string saveOption; 
  PhysicsEventSelector *eventSelector; 
  PhysicsEventBuilder   builder; 
  StatusBar             bar;   

 public:
  void Execute();

 public:
  virtual void Initialize();
  virtual void Loop();
  virtual void ProcessEvent(); /** We don't need to pass in the event because h22Reader has access to the events already. */
  virtual void Save();
};


HistogramLoader::HistogramLoader(PhysicsEventSelector *eventCriteria, string outputFile, string saveOpts){

  eventSelector = eventCriteria; 
  outputFilenameWithExtension = outputFile;
  saveOption = saveOpts; 

}

HistogramLoader::~HistogramLoader(){

}

void HistogramLoader::Execute(){
  Initialize();
  Loop();
  Save();
}

void HistogramLoader::Save(){
  // Just save to the file here. 
  TFile * outputFile = TFile::Open(outputFilenameWithExtension.c_str(), saveOption.c_str());
  outputFile->Write();
  outputFile->Close();
}

void HistogramLoader::Initialize(){
  // Here you initialize histograms. 
}

void HistogramLoader::Loop(){

  // Assumes that file lists are loaded in h22Reader. 
  if (GSIM == -1) { Init(); }
  
  int numberOfEntries = fchain->GetEntries();

  for (int iEntry = 0; iEntry < numberOfEntries; iEntry++) {
    if (iEntry%100000 == 0){ bar.PrintStatus((double)iEntry/numberOfEntries); } 

    GetEntry(iEntry); // Tell the fchain where to point 
    ProcessEvent();    
  }
  cout << endl; 
  eventSelector->summarize();
}

void HistogramLoader::ProcessEvent(){
  event.PrintEvent();
}

#endif
