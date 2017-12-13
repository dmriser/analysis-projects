#include <iostream>
#include <fstream>
#include <vector>

#include "TChain.h"
#include "TTree.h"
#include "TFile.h"
#include "TBranch.h"
#include "TString.h"
#include "TRegexp.h"

#include "StatusBar.h"

namespace Tools{

  class RunNumberStripper{
  public:

    static int Strip(std::string file){
      int run = 0; 
      TString filename(file.c_str());
      TRegexp runno_regex("[1-9][0-9][0-9][0-9][0-9]");
      TString srunno = filename( runno_regex );
      run = srunno.Atoi();
      return run; 
    }

    static std::vector<std::string> GetFilesFromList(int run, std::string file){
      std::vector<std::string> files; 

      std::ifstream in; 
      in.open(file.c_str());

      if (in.is_open()){
	std::string line; 

	while(getline(in, line)){
	  int currentRun = Strip(line);
	  if (currentRun == run){ 
	    files.push_back(line); 
	  }
	}
      } else {
	std::cerr << "Couldn't open file list!" << std::endl; 
      }

      in.close(); 

      return files; 
    }


  };

  class WavePlate {
  public:
    
    static int GetStatus(int run){
      int status = 0; 

      if (InRange(run, 37658, 38091)){ status =  1; }
      if (InRange(run, 38092, 38113)){ status = -1; }
      if (InRange(run, 38114, 38120)){ status =  1; }
      if (InRange(run, 38121, 38130)){ status = -1; }
      if (InRange(run, 38131, 38131)){ status =  1; }
      if (InRange(run, 38132, 38136)){ status = -1; }
      if (InRange(run, 38137, 38142)){ status =  1; }
      if (InRange(run, 38143, 38193)){ status = -1; }
      if (InRange(run, 38194, 38198)){ status =  1; }
      if (InRange(run, 38199, 38199)){ status = -1; }
      if (InRange(run, 38200, 38202)){ status =  1; }
      if (InRange(run, 38203, 38203)){ status = -1; }
      if (InRange(run, 38204, 38206)){ status =  1; }
      if (InRange(run, 38207, 38264)){ status = -1; }
      if (InRange(run, 38265, 38289)){ status =  1; }
      if (InRange(run, 38290, 38299)){ status = -1; }
      if (InRange(run, 38300, 38547)){ status =  1; }
      if (InRange(run, 38548, 38657)){ status = -1; }
      if (InRange(run, 38658, 38666)){ status =  1; }
      if (InRange(run, 38667, 38680)){ status = -1; }
      if (InRange(run, 38681, 38751)){ status =  1; }

      return status; 
    }

    static bool InRange(int x, int low, int high){
      return (x >= low) && (x <=  high);
    }
  };
}

class h10Reader {
public:
  h10Reader() {
    chain = new TChain("h10"); 
    SetupBranches(); 
  }

  ~h10Reader(){
    chain->Delete(); 
  }

  TChain *chain; 
  Int_t  event; 
  Char_t  hel; 
  TBranch *b_event; 
  TBranch *b_hel; 

  void SetupBranches() {
    chain->SetBranchAddress("evntid", &event, &b_event);
    chain->SetBranchAddress("evthel", &hel, &b_hel);
 }

  void AddFile(std::string f){
    chain->AddFile(f.c_str()); 
  }

  int GetEntries() const {
    return chain->GetEntries(); 
  }

  void GetEntry(int i) {
    chain->GetEntry(i);
  }

};

class h22Reader {
public:
  h22Reader() {
    chain = new TChain("h22"); 
    SetupBranches(); 
  }

  ~h22Reader(){
    chain->Delete(); 
  }

  TChain *chain; 
  Int_t  event; 
  Char_t  hel; 

  TBranch *b_event; 
  TBranch *b_hel; 

  void SetupBranches() {
    chain->SetBranchAddress("evntid", &event, &b_event);
    chain->SetBranchAddress("ihel", &hel, &b_hel);
 }

  void AddFile(std::string f){
    chain->AddFile(f.c_str()); 
  }

  int GetEntries() const {
    return chain->GetEntries(); 
  }

  void GetEntry(int i) {
    chain->GetEntry(i);
  }

};

int main(int argc, char *argv[]){

  if(argc < 2){
    std::cout << "./print FILE1(h22) " << std::endl; 
    return 0;
  }

  std::string file_h22(argv[1]);
  std::string h10List("h10_files.dat"); 
  std::string hd_msg("[HelConverter::Main] "); 

  int run = Tools::RunNumberStripper::Strip(file_h22);
  int wavePlate = Tools::WavePlate::GetStatus(run);
  std::vector<std::string> files_h10 = Tools::RunNumberStripper::GetFilesFromList(run, h10List); 

  // creating output filename 
  std::string outfile(Form("/volatile/clas12/dmriser/analysis/e1f_analysis/corr_hel/%d.root", run)); 

  std::cout << hd_msg << "For run = " << run << " wave-plate is " << wavePlate << std::endl; 
  std::cout << hd_msg << "Output set to " << outfile << std::endl; 
  std::cout << hd_msg << "Running for " << file_h22 << std::endl; 

  // the file for h22 will be an entire run 
  // but the file for h10 will be just one stub
  // we need to grab all matching stubs from the 
  // h10 folder 
  h22Reader reader22;  
  reader22.AddFile(file_h22); 

  h10Reader reader10;  
  for (int f=0; f<files_h10.size(); f++){
    reader10.AddFile(files_h10[f]); 
    std::cout << hd_msg << " Running for " << files_h10[f] << std::endl; 
  }

  // new stuff 
  TFile * newNtuple = new TFile(outfile.c_str(),"recreate");
  TChain * newChain = (TChain*) reader22.chain->CloneTree(0);
  TTree * newTree   = newChain->GetTree();
  
  Int_t corrHel;
  TBranch *b_corrHel; 

  b_corrHel = newTree->Branch("corr_hel", &corrHel);

  StatusBar status; 

  int currentEvent10 = 0; 
  int currentEvent22 = 0; 
  int skippedEvent10 = 0; 
  int skippedEvent22 = 0; 
  int entriesProcessed = 0; 

  int maxEntries = reader10.GetEntries(); 
  if (reader22.GetEntries() > maxEntries){
    maxEntries = reader22.GetEntries(); 
  }

  while (currentEvent10 < reader10.GetEntries() && currentEvent22 < reader22.GetEntries()){
    entriesProcessed++; 
    if (entriesProcessed%50000 == 0) { 
      status.PrintStatus(entriesProcessed, maxEntries); 
    }

    reader10.GetEntry(currentEvent10); 
    reader22.GetEntry(currentEvent22); 

    if (reader10.event == reader22.event){

      /* 
      std::cout.width(10); std::cout << reader10.event; 
      std::cout.width(10); std::cout << (int) reader10.hel; 
      std::cout.width(10); std::cout << reader22.event; 
      std::cout.width(10); std::cout << (int) reader22.hel << std::endl; 
      */ 

      // this is temporarily wrong 
      corrHel = (int) (reader10.hel * wavePlate); 
      newTree->Fill(); 

      currentEvent10++; 
      currentEvent22++; 
    } else if (reader10.event > reader22.event){
      currentEvent22++; 
      skippedEvent22++; 
    } 
    else if (reader10.event < reader22.event){
      currentEvent10++; 
      skippedEvent10++; 
    }
  }

  std::cout << "Skipped events 10: " << skippedEvent10 << std::endl; 
  std::cout << "Skipped events 22: " << skippedEvent22 << std::endl; 
  std::cout << "Events 10: " << reader10.GetEntries() << std::endl; 
  std::cout << "Events 22: " << reader22.GetEntries() << std::endl; 

  newTree->Write(); 
  newNtuple->Close(); 

  return 0;
}
