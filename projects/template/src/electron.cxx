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
    tupleWriter.addFloat("q2"); 
    tupleWriter.addFloat("w"); 
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

      std::vector<int> electronIndices = event.GetVectorOfUserAnaParticleIndices(11); 
      if(!electronIndices.empty()){
	
	int electronIndex = electronIndices[0];
	event.SetElectronIndex(electronIndex);
	  
	TLorentzVector electron = event.GetTLorentzVector(electronIndex, 11); 
	PhysicsEvent ev = builder.getPhysicsEvent(electron);
	tupleWriter.setFloat("q2",           ev.qq);
	tupleWriter.setFloat("w",            ev.w);
	tupleWriter.writeEvent();
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
