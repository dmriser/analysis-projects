#ifndef faraday_cup_appender_h
#define faraday_cup_appender_h

// c++ includes 
#include <iostream>
#include <vector>
using namespace std; 

// my includes 
#include "DInformation.h"
#include "FaradayCupLoader.h"

// cern root libs
#include "TFile.h"
#include "TH1.h"
#include "TString.h"
#include "TRegexp.h"

class FaradayCupAppender{
 public:
  FaradayCupAppender(DInformation * runInfo);
  ~FaradayCupAppender();

 protected:
  DInformation * runInfoDatabase; 
  double totalCharge; 
  TH1D * totalChargeHistogram; 
  
 public:
  void AddFiles(std::vector<std::string> dataFiles); 
  void AppendToFile(std::string outputFilenameWithExtension); 
};

FaradayCupAppender::FaradayCupAppender(DInformation * runInfo){
  runInfoDatabase = runInfo;
  totalCharge = 0;
  totalChargeHistogram = new TH1D("totalCharge","total charge uC",10,0,1);
}

FaradayCupAppender::~FaradayCupAppender(){
  delete runInfoDatabase; 
}

  
void FaradayCupAppender::AddFiles(std::vector<std::string> dataFiles){

  if ( !runInfoDatabase->has_information() ){
    std::cout << "[FaradayCupAppender::AddFiles] Error opening database for load! " << std::endl;
    return;
  }

  TRegexp regexForRunNumber("[1-9][0-9][0-9][0-9][0-9]");
  for (int ifile = 0; ifile<dataFiles.size(); ifile++)
    {
      TString file(dataFiles[ifile].c_str());
      TString srunno = file(regexForRunNumber);
      int runno      = srunno.Atoi();
      double charge  = (double) runInfoDatabase->info[runno].dQ/runInfoDatabase->info[runno].daq_scale;
      totalCharge += charge; 
    }

  // Load into histogram for saving. 
  for (int b=0; b<10; b++) {
    totalChargeHistogram->SetBinContent(1+b,totalCharge);
    totalChargeHistogram->SetBinError(1+b,0.0);
  }
}

void FaradayCupAppender::AppendToFile(std::string outputFilenameWithExtension){
  TFile * outputFile = TFile::Open(outputFilenameWithExtension.c_str(), "UPDATE");
  totalChargeHistogram->Write();
  outputFile->Write();
  outputFile->Close();
} 

class FaradayCupQuickLoader{
 public:
  FaradayCupQuickLoader(){

  }

  ~FaradayCupQuickLoader(){

  }

  double openFileGetCharge(string file){
    TFile * inputFile = TFile::Open(file.c_str());
    TH1D * chargeHistogram = (TH1D*) inputFile->Get("totalCharge");
    double charge = chargeHistogram->GetBinContent(1);
    inputFile->Close();
    return charge; 
  }


};

#endif

