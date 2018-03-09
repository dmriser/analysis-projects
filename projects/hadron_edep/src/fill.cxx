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
 
    filter      = new ParticleFilter(params);

    // setup structure of ntuple 
    tupleWriter.addInt("id");
    tupleWriter.addFloat("edep_inner"); 
    tupleWriter.addFloat("edep_outer"); 
    tupleWriter.addFloat("beta");
    tupleWriter.addFloat("p");
    tupleWriter.addFloat("alpha");
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

      std::vector<int> electronIndices = filter->getVectorOfParticleIndices(event, 11);
      if(!electronIndices.empty()){
	
	int electronIndex = electronIndices[0];
	event.SetElectronIndex(electronIndex);
	Corrections::correctEvent(&event, GetRunNumber(), GSIM);

	std::vector<int> pipIndices  = filter->getVectorOfParticleIndices(event,  211); 
	std::vector<int> protIndices = filter->getVectorOfParticleIndices(event, 2212); 

	if(!pipIndices.empty()){

	  int mesonIndex       = pipIndices[0];
	  TLorentzVector meson = event.GetTLorentzVector(mesonIndex, 211); 

	  // momentum correction done here 
	  // because if we dont find proton it's useless 
	  // to waste cpu doing it above 
	  TLorentzVector electron = event.GetTLorentzVector(electronIndex, 11); 
	  electron                = momCorr->PcorN(electron, -1, 11);

	  // build event 
	  PhysicsEvent ev = builder.getPhysicsEvent(electron, meson);

	  DataEventCut_BetaPLikelihood *bpCut = (DataEventCut_BetaPLikelihood*) filter->GetSelector(211)->GetCut("Beta P Likelihood Cut 211");

	    // this needs to be called to get the confidence correct 
 	    bool garbageCan = bpCut->IsPassed(event, pipIndices[0]); 
	    tupleWriter.setInt("id", 211);
	    tupleWriter.setFloat("edep_inner", event.ec_ei[mesonIndex]); 
	    tupleWriter.setFloat("edep_outer", event.ec_eo[mesonIndex]); 
	    tupleWriter.setFloat("p",          event.p[mesonIndex]); 
	    tupleWriter.setFloat("alpha",      bpCut->GetConfidence()); 
	    tupleWriter.writeEvent();
	  }


	if(!protIndices.empty()){

	  int mesonIndex       = protIndices[0];
	  TLorentzVector meson = event.GetTLorentzVector(mesonIndex, 211); 

	  // momentum correction done here 
	  // because if we dont find proton it's useless 
	  // to waste cpu doing it above 
	  TLorentzVector electron = event.GetTLorentzVector(electronIndex, 11); 
	  electron                = momCorr->PcorN(electron, -1, 11);

	  // build event 
	  PhysicsEvent ev = builder.getPhysicsEvent(electron, meson);

	  DataEventCut_BetaPLikelihood *bpCut = (DataEventCut_BetaPLikelihood*) filter->protonSelector->GetCut("Beta P Likelihood Cut 2212");

	    // this needs to be called to get the confidence correct 
 	    bool garbageCan = bpCut->IsPassed(event, protIndices[0]); 
	    tupleWriter.setInt("id", 2212);
	    tupleWriter.setFloat("edep_inner", event.ec_ei[mesonIndex]); 
	    tupleWriter.setFloat("edep_outer", event.ec_eo[mesonIndex]); 
	    tupleWriter.setFloat("p",          event.p[mesonIndex]); 
	    tupleWriter.setFloat("alpha",      bpCut->GetConfidence()); 
	    tupleWriter.writeEvent();
	  }
      }

      if(ientry%100000 == 0){
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
