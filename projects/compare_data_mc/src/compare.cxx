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
    tupleWriter.addInt("q");
    tupleWriter.addInt("pid");
    tupleWriter.addFloat("p"); 
    tupleWriter.addFloat("dp"); 
  }

  ~Analysis(){
  }

  void Loop(int eventsToProcess){

    // Setup the reader options. 
    Init();
    
    StatusBar  stat; 
    TStopwatch timer; 
    timer.Reset();
    timer.Start();

    // Set the maximum number of events to process
    // to be the number of entries if the user requested more. 
    eventsToProcess = (eventsToProcess > GetEntries() ? GetEntries() : eventsToProcess);

    // For every event
    for(int ientry=0; ientry < eventsToProcess; ientry++){
      GetEntry(ientry);

      // Process a MC event. 
      if (GSIM) {
	std::map<int, int> particleMapping;

	for (int genParticleIndex = 0; genParticleIndex < event.mcnentr; genParticleIndex++) {
	  for (int recParticleIndex = 0; recParticleIndex < event.gpart; recParticleIndex++) {

	    TVector3 recP(event.p[recParticleIndex] * event.cx[recParticleIndex],
			  event.p[recParticleIndex] * event.cy[recParticleIndex],
			  event.p[recParticleIndex] * event.cz[recParticleIndex]
			  ); 

	    TVector3 genP(event.mcp[genParticleIndex] * cos(event.mctheta[genParticleIndex] * TMath::Pi() / 180.0) * cos(event.mcphi[genParticleIndex] * TMath::Pi() / 180.0), 
			  event.mcp[genParticleIndex] * cos(event.mctheta[genParticleIndex] * TMath::Pi() / 180.0) * sin(event.mcphi[genParticleIndex] * TMath::Pi() / 180.0),
			  event.mcp[genParticleIndex] * sin(event.mctheta[genParticleIndex]));

	    TVector3 diff = (recP - genP); 
	    diff *=  1 / genP.Mag(); 

	    if (diff.Mag2() < 0.05){
	      //	      particleMapping[genParticleIndex] = recParticleIndex; 
	      //	      std::cout << diff.X() << " ," << diff.Y() << " ," << diff.Z() << std::endl; 

	      tupleWriter.setInt("q", event.q[recParticleIndex]); 
	      tupleWriter.setInt("pid", event.mcpid[genParticleIndex]); 
	      tupleWriter.setFloat("p", event.mcp[genParticleIndex]); 	      
	      tupleWriter.setFloat("dp", diff.Mag2()); 
	      tupleWriter.writeEvent(); 
	    }
	  }
	}	
      } 
      
      // Process a data event. 
      else {

	std::vector<int> electronIndices = event.GetVectorOfUserAnaParticleIndices(11); 
	if(!electronIndices.empty()){
	  
	  int electronIndex = electronIndices[0];
	  event.SetElectronIndex(electronIndex);
	  
	  TLorentzVector electron = event.GetTLorentzVector(electronIndex, 11); 
	  PhysicsEvent ev = builder.getPhysicsEvent(electron);
	}
      }
      if (ientry%10000 == 0){
	stat.PrintStatus(ientry, eventsToProcess); 
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
  analysis.Loop(opts.args["N"].arg);
  analysis.Save(opts.args["OUT"].args);
  
  } else {
    std::cerr << "No files found." << std::endl; 
  }

  return 0; 
}
