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
#include "Skimmer.h"
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
    params->loadParameters(Form("%s/lists/parameters/data/final-loose.pars", path.c_str())); 
 
    filter      = new ParticleFilter(params);


  }

  ~Analysis(){
    // total destruction 
  }

  void Loop(){
    
    // setup reader options 
    GSIM = false; 
    Init();

    // doing this here so the chain is built correctly 
    fSkimmer = new Skimmer(fchain);

    // setup particle filter 
    filter->set_info(GSIM, GetRunNumber());

    StatusBar  stat; 
    TStopwatch timer; 
    timer.Reset();
    timer.Start();

    // for every event
    for(int ientry=0; ientry<GetEntries(); ientry++){
      GetEntry(ientry);

      std::vector<int> electronIndices = filter->getVectorOfParticleIndices(event, 11);
      if(!electronIndices.empty()){
	
	int electronIndex = electronIndices[0];
	event.SetElectronIndex(electronIndex);
	Corrections::correctEvent(&event, GetRunNumber(), GSIM);

	std::vector<int> kpIndices = filter->getVectorOfParticleIndices(event,  321); 
	//	std::vector<int> kmIndices = filter->getVectorOfParticleIndices(event, -321); 

	//	if(!kpIndices.empty() || !kmIndices.empty()){
	if(!kpIndices.empty()){
	  DataEventCut_BetaPLikelihood *bpCut = (DataEventCut_BetaPLikelihood*) filter->GetSelector(321)->GetCut("Beta P Likelihood Cut 321");
	  bool garbageCan = bpCut->IsPassed(event, kpIndices[0]);

	  if (bpCut->GetConfidence() > 0.01){
	    fSkimmer->Fill(); 
	  }
	}
      }

      if(ientry%10000 == 0){
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
    fSkimmer->Save(outFile);
  }

protected:

  MomCorr_e1f             *momCorr; 
  PhysicsEventBuilder      builder; 
  ParticleFilter          *filter, *filterLoose, *filterTight; 
  Parameters              *params, *paramsLoose, *paramsTight; 
  Skimmer                 *fSkimmer; 

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
  opts.args["PARS"].args = "/u/home/dmriser/clas/analysis-main/lists/final-loose.pars";
  opts.args["PARS"].type = 1;
  opts.args["PARS"].name = "Parameter file";
  opts.args["LIST"].args = "UNSET";
  opts.args["LIST"].type = 1;
  opts.args["LIST"].name = "List of files";
  opts.set(argc,argv);
 
  std::vector<std::string> files; 
  if (opts.args["LIST"].args != "UNSET"){
    files = loadFilesFromList(opts.args["LIST"].args, opts.args["N"].arg);
  } else {
    files = loadFilesFromCommandLine(&opts, opts.args["N"].arg);
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
