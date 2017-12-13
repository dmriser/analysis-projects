#include <iostream>

// h22 libs 
#include "CommonTools.h"
#include "Corrections.h"
#include "CheckPoints.h"
#include "DataEventCut.h"
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
    std::string momCorrPath = Form("%s/momCorr/",path.c_str());
    momCorr                 = new MomCorr_e1f(momCorrPath);


    std::string nathanPath = Global::Environment::GetNathanPath();

    // setup reader options 
    GSIM = false; 
    Init();

    // needs parameters 
    params = new Parameters(); 
    params->loadParameters(Form("%s/lists/final.pars", path.c_str())); 

    paramsLoose = new Parameters(); 
    paramsLoose->loadParameters(Form("%s/lists/dataLoose.pars", path.c_str())); 
    //    paramsLoose->loadParameters(Form("%s/lists/data.pars", path.c_str())); 

    paramsTight = new Parameters(); 
    paramsTight->loadParameters(Form("%s/lists/dataTight.pars", path.c_str())); 
    //    paramsTight->loadParameters(Form("%s/lists/data.pars", path.c_str())); 

    filter      = new ParticleFilter(params);
    filterLoose = new ParticleFilter(paramsLoose);
    filterTight = new ParticleFilter(paramsTight);

    //    filter->GetSelector(211)->DisableByName("Delta Z-Vertex Cut");
    //    filter->GetSelector(211)->DisableByRegex("Fid");

    // setup the hadron filter for nathan 
    pipTable.setPath(nathanPath);
    pipTable.loadValues();

    pimTable.setPath(nathanPath);
    pimTable.loadValues();

    hid = new Nathan::HadronID(pipTable, pimTable);

    // setup structure of ntuple 
    tupleWriter.addInt("helicity");
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
    tupleWriter.addFloat("p_pip"); 
    tupleWriter.addFloat("phi_ele"); 
    tupleWriter.addFloat("phi_pip");
    tupleWriter.addFloat("theta_ele"); 
    tupleWriter.addFloat("theta_pip");
    tupleWriter.addFloat("dvz");
    tupleWriter.addFloat("alpha");
    tupleWriter.addFloat("dist_ecsf");
    tupleWriter.addFloat("dist_ec_edep");
    tupleWriter.addFloat("dist_vz");
    tupleWriter.addFloat("dist_cc_theta");

    // for varying cut values
    tupleWriter.addInt("strict_ele_r1fid");
    tupleWriter.addInt("strict_ele_r3fid");
    tupleWriter.addInt("strict_ele_ecsf");
    tupleWriter.addInt("strict_ele_vz");
    tupleWriter.addInt("strict_ele_ec");
  }

  ~Analysis(){
    // total destruction 
  }

  void Loop(){
    
    // setup particle filter 
    filter->set_info(GSIM, GetRunNumber());
    hid->SetRunNumber( GetRunNumber() );
    hid->SetGSIM(GSIM); 

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
	  
	std::vector<int> pipIndices = filter->getVectorOfParticleIndices(event, 211); 
	//	Nathan::HadronIDResult result = hid->getIDResult(event);

	if(!pipIndices.empty()){
		  //	if(result.hasPip()){

	  int pipIndex = pipIndices[0];
		  //	  int pipIndex = result.pipIndex; 
	  TLorentzVector pip = event.GetTLorentzVector(pipIndex, 211); 

	  // momentum correction done here 
	  // because if we dont find proton it's useless 
	  // to waste cpu doing it above 
	  TLorentzVector electron = event.GetTLorentzVector(electronIndex, 11); 
	  electron = momCorr->PcorN(electron, -1, 11);
	  
	  // build event 
	  PhysicsEvent ev = builder.getPhysicsEvent(electron, pip);

	  if ( sqrt(ev.mm2) > 0.8 && sqrt(ev.mm2) < 1.08 
	       && ev.w > 2.0 && ev.qq > 1.0) {
	    
	    std::map<std::string, bool> results_nom   = filter     ->eid_map(event, electronIndices[0]);
	    std::map<std::string, bool> results_tight = filterTight->eid_map(event, electronIndices[0]);

	    std::map<std::string, float> distances = filter->eid_distance_map(event, electronIndices[0]); 
	    tupleWriter.setFloat("dist_ecsf",    distances["EC_SAMPLING"]); 
	    tupleWriter.setFloat("dist_ec_edep", distances["EC_IN_OUT"]); 
	    tupleWriter.setFloat("dist_vz",      distances["Z_VERTEX"]); 
	    tupleWriter.setFloat("dist_cc_theta",distances["CC_THETA"]); 

	    tupleWriter.setInt("strict_ele_r1fid", -1);
	    tupleWriter.setInt("strict_ele_r3fid", -1);
	    tupleWriter.setInt("strict_ele_ecsf",  -1);
	    tupleWriter.setInt("strict_ele_vz",    -1);
	    tupleWriter.setInt("strict_ele_ec",    -1);
	    
	    if (results_nom["EC_SAMPLING"])  { tupleWriter.setInt("strict_ele_ecsf", 0); }
	    if (results_tight["EC_SAMPLING"]){ tupleWriter.setInt("strict_ele_ecsf", 1); }

	    if (results_nom["DC_R1_FID"])    { tupleWriter.setInt("strict_ele_r1fid", 0); }
	    if (results_tight["DC_R1_FID"])  { tupleWriter.setInt("strict_ele_r1fid", 1); }

	    if (results_nom["DC_R3_FID"])    { tupleWriter.setInt("strict_ele_r3fid", 0); }
	    if (results_tight["DC_R3_FID"])  { tupleWriter.setInt("strict_ele_r3fid", 1); }

	    if (results_nom["EC_FID"])       { tupleWriter.setInt("strict_ele_ec", 0); }
	    if (results_tight["EC_FID"])     { tupleWriter.setInt("strict_ele_ec", 1); }

	    if (results_nom["Z_VERTEX"])     { tupleWriter.setInt("strict_ele_vz", 0); }
	    if (results_tight["Z_VERTEX"])   { tupleWriter.setInt("strict_ele_vz", 1); }
	
	    DataEventCut_BetaPLikelihood *bpCut = (DataEventCut_BetaPLikelihood*) filter->GetSelector(211)->GetCut("Beta P Likelihood Cut 211"); 
	    

	    tupleWriter.setInt("helicity",       event.corr_hel);
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
	    tupleWriter.setFloat("p_pip",        pip.P()); 
	    tupleWriter.setFloat("theta_ele",    to_degrees*electron.Theta()); 
	    tupleWriter.setFloat("theta_pip",    to_degrees*pip.Theta()); 
	    tupleWriter.setFloat("phi_ele",      to_degrees*electron.Phi()); 
	    tupleWriter.setFloat("phi_pip",      to_degrees*pip.Phi()); 
	    tupleWriter.setFloat("dvz",          event.vz[electronIndex]-event.vz[pipIndex]); 
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
  opts.args["PARS"].args = "/u/home/dmriser/Analysis_v2/lists/parameters/data/data.pars";
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
