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
    tupleWriter.addInt("helicity");
    tupleWriter.addInt("meson_id");
    tupleWriter.addFloat("missing_mass"); 
    tupleWriter.addFloat("x"); 
    tupleWriter.addFloat("q2"); 
    tupleWriter.addFloat("z"); 
    tupleWriter.addFloat("pt"); 
    tupleWriter.addFloat("w"); 
    tupleWriter.addFloat("eta"); 
    tupleWriter.addFloat("phi_h"); 
    tupleWriter.addFloat("theta_h"); 
    tupleWriter.addFloat("p_ele"); 
    tupleWriter.addFloat("p_mes"); 
    tupleWriter.addFloat("phi_ele"); 
    tupleWriter.addFloat("phi_mes");
    tupleWriter.addFloat("theta_ele"); 
    tupleWriter.addFloat("theta_mes");
    tupleWriter.addFloat("dvz");
    tupleWriter.addFloat("alpha");
    tupleWriter.addFloat("dist_ecsf");
    tupleWriter.addFloat("dist_ec_edep");
    tupleWriter.addFloat("dist_vz");
    tupleWriter.addFloat("dist_cc_theta");
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
    //    hid->SetRunNumber( GetRunNumber() );
    //    hid->SetGSIM(GSIM); 

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

	std::map<std::string, float> distances = filter->eid_distance_map(event, electronIndices[0]); 
        tupleWriter.setFloat("dist_ecsf",    distances["EC_SAMPLING"]); 
        tupleWriter.setFloat("dist_ec_edep", distances["EC_IN_OUT"]); 
        tupleWriter.setFloat("dist_vz",      distances["Z_VERTEX"]); 
        tupleWriter.setFloat("dist_cc_theta",distances["CC_THETA"]); 
	  
	std::vector<int> kpIndices = filter->getVectorOfParticleIndices(event,  321); 
	std::vector<int> kmIndices = filter->getVectorOfParticleIndices(event, -321); 
	//	Nathan::HadronIDResult result = hid->getIDResult(event);

	if(!kpIndices.empty()){
		  //	if(result.hasPip()){

	  int mesonIndex       = kpIndices[0];
	  TLorentzVector meson = event.GetTLorentzVector(mesonIndex, 321); 

	  // momentum correction done here 
	  // because if we dont find proton it's useless 
	  // to waste cpu doing it above 
	  TLorentzVector electron = event.GetTLorentzVector(electronIndex, 11); 
	  electron                = momCorr->PcorN(electron, -1, 11);
	  
	  // Dan Carman method for rejecting some pi+ events in the
	  // kaon sample.
	  //
	  // Classify an event and if it's a kaon you pretend
	  // it's a pion.  Then check the missing mass spectrum
	  // and if the event is near the neutron peak it must
	  // really be a pion, so reassign it to be.
	  //
	  // Currently all it does it throw out the pion,
	  // if doesn't actually add it to the pion sample
	  // because the code never makes it that far in the
	  // pion case it would just return 0 pions and not enter.
	    TLorentzVector pion = event.GetTLorentzVector(mesonIndex, 211);
	    PhysicsEvent pionEvent = builder.getPhysicsEvent(electron,pion);

	    // reject and dont process event
	    if (sqrt(pionEvent.mm2) < 1.07){
	      continue;
	    }

	  // build event 
	  PhysicsEvent ev = builder.getPhysicsEvent(electron, meson);

	  if (ev.w > 2.0 && ev.qq > 1.0) {	
	    DataEventCut_BetaPLikelihood *bpCut = (DataEventCut_BetaPLikelihood*) filter->GetSelector(211)->GetCut("Beta P Likelihood Cut 211");

	    // this needs to be called to get the confidence correct 
	    bool garbageCan = bpCut->IsPassed(event, kpIndices[0]); 

	    tupleWriter.setInt("helicity",       event.corr_hel);
	    tupleWriter.setInt("meson_id",       321);
	    tupleWriter.setFloat("missing_mass", sqrt(ev.mm2));
	    tupleWriter.setFloat("x",            ev.x);
	    tupleWriter.setFloat("q2",           ev.qq);
	    tupleWriter.setFloat("z",            ev.z);
	    tupleWriter.setFloat("pt",           ev.pT);
	    tupleWriter.setFloat("w",            ev.w);
	    tupleWriter.setFloat("eta",          ev.eta);
	    tupleWriter.setFloat("theta_h",      ev.thetaHadron);
	    tupleWriter.setFloat("phi_h",        ev.phiHadron);
	    tupleWriter.setFloat("p_ele",        electron.P()); 
	    tupleWriter.setFloat("p_mes",        meson.P()); 
	    tupleWriter.setFloat("theta_ele",    to_degrees*electron.Theta()); 
	    tupleWriter.setFloat("theta_mes",    to_degrees*meson.Theta()); 
	    tupleWriter.setFloat("phi_ele",      to_degrees*electron.Phi()); 
	    tupleWriter.setFloat("phi_mes",      to_degrees*meson.Phi()); 
	    tupleWriter.setFloat("dvz",          event.vz[electronIndex]-event.vz[mesonIndex]); 
	    tupleWriter.setFloat("alpha",        bpCut->GetConfidence()); 
	    tupleWriter.writeEvent();
	  }

	}

	if(!kmIndices.empty()){

	  int mesonIndex          = kmIndices[0];
	  TLorentzVector meson    = event.GetTLorentzVector(mesonIndex, -321); 
	  TLorentzVector electron = event.GetTLorentzVector(electronIndex, 11); 
	  electron                = momCorr->PcorN(electron, -1, 11);
	  
	  // build event 
	  PhysicsEvent ev = builder.getPhysicsEvent(electron, meson);

	  if (ev.w > 2.0 && ev.qq > 1.0) {	    
	    DataEventCut_BetaPLikelihood *bpCut = (DataEventCut_BetaPLikelihood*) filter->GetSelector(211)->GetCut("Beta P Likelihood Cut 211");

	    // this needs to be called to get the confidence correct 
	    bool garbageCan = bpCut->IsPassed(event, kmIndices[0]); 

	    tupleWriter.setInt("helicity",       event.corr_hel);
	    tupleWriter.setInt("meson_id",       -321);
	    tupleWriter.setFloat("missing_mass", sqrt(ev.mm2));
	    tupleWriter.setFloat("x",            ev.x);
	    tupleWriter.setFloat("q2",           ev.qq);
	    tupleWriter.setFloat("z",            ev.z);
	    tupleWriter.setFloat("pt",           ev.pT);
	    tupleWriter.setFloat("w",            ev.w);
	    tupleWriter.setFloat("eta",          ev.eta);
	    tupleWriter.setFloat("theta_h",      ev.thetaHadron);
	    tupleWriter.setFloat("phi_h",        ev.phiHadron);
	    tupleWriter.setFloat("p_ele",        electron.P()); 
	    tupleWriter.setFloat("p_mes",        meson.P()); 
	    tupleWriter.setFloat("theta_ele",    to_degrees*electron.Theta()); 
	    tupleWriter.setFloat("theta_mes",    to_degrees*meson.Theta()); 
	    tupleWriter.setFloat("phi_ele",      to_degrees*electron.Phi()); 
	    tupleWriter.setFloat("phi_mes",      to_degrees*meson.Phi()); 
	    tupleWriter.setFloat("dvz",          event.vz[electronIndex]-event.vz[mesonIndex]); 
	    tupleWriter.setFloat("alpha",        bpCut->GetConfidence()); 
	    tupleWriter.writeEvent();
	  }
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
