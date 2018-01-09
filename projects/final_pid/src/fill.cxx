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
#include "SliceFitter.h"
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

enum cutTypes {
  none,
  physicsEnhanced,
  dvz, 
  fid,
  all
};

class Analysis : public h22Reader {
public:  
  Analysis() {

    std::string path        = Global::Environment::GetAnalysisPath(); 
    std::string momCorrPath = Form("%s/momCorr/",path.c_str());
    momCorr                 = new MomCorr_e1f(momCorrPath);


    std::string nathanPath = Global::Environment::GetNathanPath();

    // needs parameters 
    params = new Parameters(); 
    params->loadParameters(Form("%s/lists/parameters/data/data_tofmass.pars", path.c_str())); 
    filter      = new ParticleFilter(params);

    counts[211]  = 0; 
    counts[-211] = 0; 
    counts[321]  = 0; 
    counts[-321] = 0; 

    // nothing 
    mesonHistos[211][cutTypes::none]  = new MesonHistograms("211_no_cut",   211); 
    
    // I am trying to use 'standard' reactions to get really nice clean 
    // particle traces without applying cuts.  
    mesonHistos[211][cutTypes::physicsEnhanced]  = new MesonHistograms("211_physics_enhanced",   211); 
    mesonHistos[321][cutTypes::physicsEnhanced]  = new MesonHistograms("321_physics_enhanced",   321);     
    mesonHistos[-211][cutTypes::physicsEnhanced]  = new MesonHistograms("-211_physics_enhanced",  -211); 
    mesonHistos[-321][cutTypes::physicsEnhanced]  = new MesonHistograms("-321_physics_enhanced",  -321);     
    mesonHistos[2212][cutTypes::physicsEnhanced]  = new MesonHistograms("2212_physics_enhanced",   2212);     

    // this will be the same for each particle type let's just save once
    mesonHistos[211][cutTypes::dvz]  = new MesonHistograms("211_dvz_cut",   211); 
    mesonHistos[211][cutTypes::fid]  = new MesonHistograms("211_fid_cut",   211); 

    // wahtever passes the current pid 
    mesonHistos[211][cutTypes::all]  = new MesonHistograms("211_all_cut",   211); 
    mesonHistos[321][cutTypes::all]  = new MesonHistograms("321_all_cut",   321);     

    // slice fitter
    sliceFitter = new SliceFitter(40, 0.2, 4.0); 
    sliceFitter->SetUpperTolerance(1.10);
    sliceFitter->SetLowerTolerance(0.90); 
  }

  ~Analysis(){
    // total destruction 
  }

  bool isResonance(float mass, float pole, float resolution){
    return fabs(mass-pole) < resolution; 
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

	event.SetElectronIndex(electronIndices[0]); 

	// critical 
	Corrections::correctEvent(&event, GetRunNumber(), GSIM); 

	TLorentzVector electron = event.GetTLorentzVector(electronIndices[0], 11); 
	electron = momCorr->PcorN(electron, -1, 11); 

	for(int ipart=0; ipart<event.gpart; ipart++){
	  if (event.dc_sect[ipart]>0){
	    
	    if(event.id[ipart] == -211){
	      mesonHistos[-211][cutTypes::physicsEnhanced]->Fill(event, ipart); 
	    } else if (event.id[ipart] == -321 && event.HasParticle(321)){
	      mesonHistos[-321][cutTypes::physicsEnhanced]->Fill(event, ipart); 
	    }
	    

	    if (event.q[ipart] > 0){
	      mesonHistos[211][cutTypes::none]->Fill(event, ipart); 

	      // ------------------------------------------
	      // check for passing the basic cleaning cuts 
	      // ------------------------------------------
	      if (filter->positiveMesonCandidateSelector->GetCut("Delta Z-Vertex Cut")->IsPassed(event, ipart)){
		mesonHistos[211][cutTypes::dvz]->Fill(event, ipart); 
	      }
	      if (filter->positiveMesonCandidateSelector->GetCut("DC Region 1 Fid Cut")->IsPassed(event, ipart)){
		mesonHistos[211][cutTypes::fid]->Fill(event, ipart); 
	      }
	      


	      // ------------------------------------------
	      // start checking for some standard reactions 
	      // ------------------------------------------

	      // build 4-vectors 
	      TLorentzVector partPion = event.GetTLorentzVector(ipart, 211); 
	      TLorentzVector partProt = event.GetTLorentzVector(ipart, 2212); 
	      TLorentzVector partKaon = event.GetTLorentzVector(ipart, 321); 

	      // build kinematics of event for each assumption 
	      PhysicsEvent eventPion = builder.getPhysicsEvent(electron, partPion); 
	      PhysicsEvent eventProt = builder.getPhysicsEvent(electron, partProt); 
	      PhysicsEvent eventKaon = builder.getPhysicsEvent(electron, partKaon); 

	      // check pion channels 
	      // e p -> e pi X
	      // (1) x = Neutron
	      // 
	      // could also be e p -> e p pi0
	      // so from this we get pi+, proton
	      if ( isResonance(sqrt(eventPion.mm2), pid_to_mass(2212), 0.025) ){
		mesonHistos[211][cutTypes::physicsEnhanced]->Fill(event, ipart); 
		//		mesonHistos[2212][cutTypes::physicsEnhanced]->Fill(event, ipart); 
	      }

	      if ( isResonance(sqrt(eventPion.mm2), 0.00, 0.2) || event.HasParticle(2212) ){
		mesonHistos[2212][cutTypes::physicsEnhanced]->Fill(event, ipart); 
	      }
 	      
	      // check kaon channels 
	      // e p -> e K X
	      // (1) x = lambda (1115 MeV) -> pi-, p+ 
	      if ( isResonance(sqrt(eventKaon.mm2), 1.115, 0.03) && event.HasParticle(2212) && event.HasParticle(321)) {
		mesonHistos[321][cutTypes::physicsEnhanced]->Fill(event, ipart); 
	      }
	    }	    
	  }
	}

	std::map<int, std::vector<int> > particleIndices; 
	particleIndices[211]  = filter->getVectorOfParticleIndices(event,  211); 
	particleIndices[321]  = filter->getVectorOfParticleIndices(event,  321); 

	for(std::pair<int, std::vector<int> > indices : particleIndices){
	  counts[indices.first] += indices.second.size(); 

	  for(int index : indices.second){
	    float tofmass = pow(event.p[index], 2) * (1-pow(event.corr_b[index], 2))/pow(event.corr_b[index], 2);

	    /* 
	       This could be misleading.  The output file says "all cuts", but this 
	       is simply a time of flight mass cut and doesn't reflect the result
	       of the full particle identification.  
	       
	       Those results can be produced by 
	       running the codes found in /analysis-projects/final_pid/src/test.cxx.
	    */

	    if ( fabs(tofmass - pid_to_mass(indices.first)) < 0.125)
	      mesonHistos[indices.first][cutTypes::all]->Fill(event, index); 
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


    std::cout << "Priting counts." << std::endl; 
    for(std::pair<int,int> c : counts){
      std::cout << c.first << ", " << c.second << std::endl; 
    }

  }

  void Calibrate(int pid){
    
    sliceFitter->SetExpectedMean(Form("x / sqrt(x^2 + %f^2)", pid_to_mass(pid)));
    
    for(int sector=1; sector<7; sector++){
      sliceFitter->Fit(mesonHistos[pid][cutTypes::all]->h2_p_beta[sector], Form("p_beta_%d", pid)); 
      mesonMus[pid].push_back(sliceFitter->GetFitToMu("pol3", Form("p_beta_mu_%d_sector%d", pid, sector))); 
      mesonSigmas[pid].push_back(sliceFitter->GetFitToSigma("pol3", Form("p_beta_sigma_%d_sector%d", pid, sector))); 
    }

  }

  void Save(std::string outFile){
    TFile *out = new TFile(outFile.c_str(), "recreate");

    mesonHistos[211][cutTypes::none]->Save(out); 

    mesonHistos[211][cutTypes::physicsEnhanced] ->Save(out); 
    mesonHistos[321][cutTypes::physicsEnhanced] ->Save(out); 
    mesonHistos[-211][cutTypes::physicsEnhanced] ->Save(out); 
    mesonHistos[-321][cutTypes::physicsEnhanced] ->Save(out); 
    mesonHistos[2212][cutTypes::physicsEnhanced] ->Save(out); 

    mesonHistos[211][cutTypes::dvz]->Save(out); 
    mesonHistos[211][cutTypes::fid]->Save(out); 

    mesonHistos[211][cutTypes::all]->Save(out); 
    mesonHistos[321][cutTypes::all]->Save(out); 

    for(int i=0; i<mesonMus[211].size(); i++){
      mesonMus[211][i].Write(); 
      mesonSigmas[211][i].Write(); 
    }
    for(int i=0; i<mesonMus[321].size(); i++){
      mesonMus[321][i].Write(); 
      mesonSigmas[321][i].Write(); 
    }

    out->Close();
  }


protected:

  MomCorr_e1f             *momCorr; 
  PhysicsEventBuilder      builder; 
  ParticleFilter          *filter;
  Parameters              *params;
  SliceFitter             *sliceFitter; 

  std::map<int, int> counts; 

  std::map<int, std::map<int, MesonHistograms*> > mesonHistos; 
  std::map<int, std::vector<TF1> > mesonMus; 
  std::map<int, std::vector<TF1> > mesonSigmas; 
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
  //  analysis.Calibrate( 211); 
  //  analysis.Calibrate(-211); 
  //  analysis.Calibrate( 321); 
  //  analysis.Calibrate(-321); 
  analysis.Save(opts.args["OUT"].args);
  
  } else {
    std::cerr << "No files found." << std::endl; 
  }

  return 0; 
}
