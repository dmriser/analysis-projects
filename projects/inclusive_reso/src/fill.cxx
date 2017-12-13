#include <iostream>

// h22 libs 
#include "CommonTools.h"
#include "Corrections.h"
#include "CheckPoints.h"
#include "DBins.h"
#include "h22Event.h"
#include "h22Reader.h"
#include "HadronID.h"
#include "MesonHistograms.h"
#include "MomCorr.h"
#include "ParticleFilter.h"
#include "Parameters.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"
#include "SimpleNTupleWriter.h"
#include "StatusBar.h"
#include "StandardHistograms.h"

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

    std::string path        = Global::Environment::GetAnalysisPath(); 

    // needs parameters 
    params = new Parameters(); 
    params->loadParameters(Form("%s/lists/parameters/mc/keppelRad.pars", path.c_str())); 

    // particle filter for doing pid
    filter = new ParticleFilter(params);

    // setup structure of ntuple 
    tupleWriter.addFloat("x"); 
    tupleWriter.addFloat("q2"); 
    tupleWriter.addFloat("w"); 
    tupleWriter.addFloat("x_gen"); 
    tupleWriter.addFloat("q2_gen"); 
    tupleWriter.addFloat("w_gen"); 
  }

  ~Analysis(){
    filter->electronSelector->PrintSummary(); 
  }

  void Loop(){

    // setup reader options 
    GSIM = true; 
    Init();
    
    // setup particle filter 
    filter->set_info(GSIM, 0);

    StatusBar  stat; 
    TStopwatch timer; 
    timer.Reset();
    timer.Start();

    // for every event
    for(int ientry=0; ientry<GetEntries(); ientry++){
      GetEntry(ientry);

      if(EventHasGeneratedElectron()){

	// true information 
	TLorentzVector genElectron   = event.GetGeneratedParticle(11);
	PhysicsEvent genPhysicsEvent = builder.getPhysicsEvent(genElectron);

	tupleWriter.setFloat("x_gen",  genPhysicsEvent.x);
	tupleWriter.setFloat("q2_gen", genPhysicsEvent.qq);
	tupleWriter.setFloat("w_gen",  genPhysicsEvent.w);	
	tupleWriter.setFloat("x",            -999.99);
	tupleWriter.setFloat("q2",           -999.99);
	tupleWriter.setFloat("w",            -999.99);

	std::vector<int> electronIndices = filter->getVectorOfParticleIndices(event, 11);
	if(!electronIndices.empty()){
	  
	  std::cout << " electron found " << std::endl; 

	  int electronIndex = electronIndices[0];
	  event.SetElectronIndex(electronIndex);
	  
	  TLorentzVector electron = event.GetTLorentzVector(electronIndex, 11); 
	  PhysicsEvent ev = builder.getPhysicsEvent(electron);
	  
	  tupleWriter.setFloat("x",            ev.x);
	  tupleWriter.setFloat("q2",           ev.qq);
	  tupleWriter.setFloat("w",            ev.w);
	}	
      }

      tupleWriter.writeEvent();
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

  bool EventHasGeneratedElectron(){
    for(int ipart=0; ipart<event.mcnentr; ipart++){
      if (event.mcid[ipart] == 11){
	return true;
      }
    }
    return false;
  }

  void Save(std::string outFile){
    TFile *out = new TFile(outFile.c_str(), "recreate");
    tupleWriter.save(out); 
    out->Close();
  }


protected:
  PhysicsEventBuilder      builder; 
  ParticleFilter          *filter;
  Parameters              *params;
  SimpleNTupleWriter       tupleWriter; 
};

int main(int argc, char *argv[]){

  if (argc > 1){
  // setup analysis 
  Analysis analysis; 

  // Setup Options
  h22Options opts;
  opts.args["PARS"].args = "/u/home/dmriser/analysis-main/lists/parameters/mc/keppel_rad.pars";
  opts.args["PARS"].type = 1;
  opts.args["PARS"].name = "Parameter file";

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
