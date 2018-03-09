#include <iostream>

// h22 libs 
#include "CommonTools.h"
#include "Corrections.h"
#include "CheckPoints.h"
#include "DataEventCut.h"
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
    std::string momCorrPath = Form("%s/momCorr/",path.c_str());
    momCorr                 = new MomCorr_e1f(momCorrPath);


    std::string nathanPath = Global::Environment::GetNathanPath();


    // needs parameters 
    params = new Parameters(); 
    params->loadParameters(Form("%s/lists/parameters/data/final.pars", path.c_str())); 

    paramsLoose = new Parameters(); 
    paramsLoose->loadParameters(Form("%s/lists/parameters/data/final-loose.pars", path.c_str())); 

    filter      = new ParticleFilter(params);
    filterLoose = new ParticleFilter(paramsLoose);

    //    filter->GetSelector(211)->DisableByName("Delta Z-Vertex Cut");
    //    filter->GetSelector(211)->DisableByRegex("Fid");

    // setup the hadron filter for nathan 
    /* 
    pipTable.setPath(nathanPath);
    pipTable.loadValues();

    pimTable.setPath(nathanPath);
    pimTable.loadValues();

    hid = new Nathan::HadronID(pipTable, pimTable);
    */

    // setup structure of ntuple 
    tupleWriter.addInt("sector");
    tupleWriter.addInt("run");
    tupleWriter.addFloat("p"); 
    tupleWriter.addFloat("theta"); 
    tupleWriter.addFloat("phi"); 
    tupleWriter.addFloat("w"); 
  }

  ~Analysis(){
    // total destruction 
  }

  void Loop(){
    
    // setup reader options 
    GSIM = false; 
    Init();

    // setup particle filter 
    filter->set_info(GSIM, GetRunNumber());

    StatusBar  stat; 
    TStopwatch timer; 
    timer.Reset();
    timer.Start();

    // for every event
    for(int ientry=0; ientry<GetEntries(); ientry++){
      GetEntry(ientry);

      std::vector<int> electronIndices = filterLoose->getVectorOfParticleIndices(event, 11);
      if(!electronIndices.empty()){
	
	int electronIndex = electronIndices[0];
	event.SetElectronIndex(electronIndex);
	Corrections::correctEvent(&event, GetRunNumber(), GSIM);
	
	TLorentzVector electron = event.GetTLorentzVector(electronIndex, 11); 
	//electron                = momCorr->PcorN(electron, -1, 11);

	PhysicsEvent physicsEvent = builder.getPhysicsEvent(electron);

	if(physicsEvent.w < 1.15){
	  tupleWriter.setInt("sector", event.GetSector(electronIndex));
	  tupleWriter.setInt("run", GetRunNumber());
	  tupleWriter.setFloat("p", event.p[electronIndex]);
	  tupleWriter.setFloat("w", physicsEvent.w);
	  tupleWriter.setFloat("theta", event.GetTheta(electronIndex)); 
	  tupleWriter.setFloat("phi", event.GetRelativePhi(electronIndex)); 

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

  MomCorr_e1f             *momCorr; 
  PhysicsEventBuilder      builder; 
  ParticleFilter          *filter, *filterLoose, *filterTight; 
  Parameters              *params, *paramsLoose, *paramsTight; 

  SimpleNTupleWriter      tupleWriter; 

  // Nathan's hadron identification 
  Nathan::PipBetaPTable pipTable; 
  Nathan::PimBetaPTable pimTable; 
  Nathan::HadronID      *hid; 

};

int main(int argc, char *argv[]){

  if (argc > 1){
  // setup analysis 
  Analysis analysis; 

  // Setup Options
  h22Options opts;
  opts.args["PARS"].args = "/u/home/dmriser/analysis-main/lists/data.pars";
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
