#include <iostream>

// h22 libs 
#include "CommonTools.h"
#include "h22Event.h"
#include "h22Reader.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"
#include "SimpleNTupleWriter.h"
#include "StatusBar.h"

// root 
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TLorentzVector.h"
#include "TStopwatch.h"
#include "TVector3.h"

class Analysis : public h22Reader {
public:  
  Analysis() {

    // setup structure of ntuple 
    tupleWriter.addFloat("vz");
    tupleWriter.addFloat("corr_vz");
    tupleWriter.addFloat("phi");
  }

  ~Analysis(){
  }

  void Loop(){

    // setup reader options 
    GSIM = false; 
    Init();
    
    StatusBar  stat; 
    TStopwatch timer; 
    timer.Reset();
    timer.Start();

    // for every event
    for(int ientry=0; ientry<GetEntries(); ientry++){
      GetEntry(ientry);

      for(int ipart=0; ipart<event.gpart; ipart++){
	if (event.q[ipart] < 0){
	  tupleWriter.setFloat("vz",      event.vz[ipart]);
	  tupleWriter.setFloat("corr_vz", event.corr_vz[ipart]);
	  tupleWriter.setFloat("phi",     event.GetPhi(ipart));
	  tupleWriter.writeEvent();
	}
      }

      if (ientry%10000 == 0){
	stat.PrintStatus(ientry, GetEntries()); 
      }
    }

    // summarize results 
    std::cout << std::endl;
    double loopTime  = timer.RealTime(); 
    double eventRate = GetEntries()/loopTime; 
    std::cout << "[GenericAnalysis::Loop] Total time: " 
	      << loopTime << " seconds, Event rate: " 
	      << eventRate << " events/sec. " << std::endl;

  }

  void Save(std::string outFile){
    TFile *out = new TFile(outFile.c_str(), "recreate");
    tupleWriter.save(out); 
    out->Close();
  }


protected:

  PhysicsEventBuilder      builder; 
  SimpleNTupleWriter       tupleWriter; 
};

int main(int argc, char *argv[]){

  if (argc > 1){
  // setup analysis 
  Analysis analysis; 

  // Setup Options
  h22Options opts;
  opts.args["LIST"].args = "UNSET";
  opts.args["LIST"].type = 1;
  opts.args["LIST"].name = "List of files";
  opts.set(argc,argv);
 
  std::vector<std::string> files; 
  if (opts.args["LIST"].args != "UNSET"){
    files = loadFilesFromList(opts.args["LIST"].args, 10000);
  } else {
    files = loadFilesFromCommandLine(&opts, 10000);
  }

  for (std::string f : files){
    analysis.AddFile(f); 
  }

  // run analysis loop
  analysis.Loop();
  analysis.Save(opts.args["OUT"].args);
  
  } else {
    std::cerr << "No files found." << std::endl; 
  }

  return 0; 
}
