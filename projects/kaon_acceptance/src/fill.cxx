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
#include "TMath.h"
#include "TStopwatch.h"
#include "TVector3.h"

enum mc_status {
  not_generated,
  not_reconstructed, 
  reconstructed
};

struct MonteCarloParticle {
public:
  int rec_index, gen_index; 
  int status, pid; 

  MonteCarloParticle () {
    rec_index = -1;
    gen_index = -1; 
    pid = -1;
    status = mc_status::not_generated; 
  }
};

MonteCarloParticle * getParticle(h22Event & event, int pid, float tolerance){
  MonteCarloParticle *particle = new MonteCarloParticle(); 

  particle->pid = pid; 

  for (int igen = 0; igen < event.mcnentr; igen++){
    if (event.mcid[igen] == pid){
      particle->status = mc_status::not_reconstructed; 
      particle->gen_index = igen; 
      break;
    }
  }

  if (particle->status > mc_status::not_generated){
    TVector3 genParticle; 
    genParticle.SetX(event.mcp[particle->gen_index] * sin(event.mctheta[particle->gen_index] * TMath::Pi() / 180.0) * cos(event.mcphi[particle->gen_index] * TMath::Pi() / 180.0));
    genParticle.SetY(event.mcp[particle->gen_index] * sin(event.mctheta[particle->gen_index] * TMath::Pi() / 180.0) * sin(event.mcphi[particle->gen_index] * TMath::Pi() / 180.0));
    genParticle.SetZ(event.mcp[particle->gen_index] * cos(event.mctheta[particle->gen_index] * TMath::Pi() / 180.0));

    for (int irec = 0; irec < event.gpart; irec++){
      TVector3 recParticle; 
      recParticle.SetX(event.p[irec] * event.cx[irec]);
      recParticle.SetY(event.p[irec] * event.cy[irec]);
      recParticle.SetZ(event.p[irec] * event.cz[irec]);

      TVector3 difference = (genParticle - recParticle); 
      if ( (difference.Mag() / genParticle.Mag()) < tolerance){
	particle->status = mc_status::reconstructed; 
	particle->rec_index = irec; 
	break; 
      }
    }
  }

  return particle; 
}

class Analysis : public h22Reader {
public:  
  Analysis() {

    std::string path        = Global::Environment::GetAnalysisPath(); 
    std::string momCorrPath = Form("%s/momCorr/",path.c_str());

    // needs parameters 
    params = new Parameters(); 
    params->loadParameters(Form("%s/lists/parameters/data/final.pars", path.c_str())); 

    // particle filter for doing pid
    filter      = new ParticleFilter(params);

    // setup structure of ntuple 
    tupleWriter.addInt("hadron_pid");
    tupleWriter.addFloat("hadron_momentum"); 
    tupleWriter.addFloat("hadron_beta"); 

    pionLikelihood = new DataEventCut_BetaPLikelihood(211);
    kaonLikelihood = new DataEventCut_BetaPLikelihood(321);
    protonLikelihood = new DataEventCut_BetaPLikelihood(2212);
 
    pionLikelihood->AddPossibleParticle(321); 
    pionLikelihood->AddPossibleParticle(2212); 
    kaonLikelihood->AddPossibleParticle(211); 
    kaonLikelihood->AddPossibleParticle(2212); 
    protonLikelihood->AddPossibleParticle(211); 
    protonLikelihood->AddPossibleParticle(321); 

    pionLikelihood->Configure(params);
    kaonLikelihood->Configure(params);
    protonLikelihood->Configure(params);
  }

  ~Analysis(){
    std::cout << "[Analysis::~Analysis] Destruction complete." << std::endl; 
  }

  void Loop(){

    // setup reader options 
    GSIM = true; 
    Init();
    
    // setup particle filter with run number = 0  
    filter->set_info(GSIM, 0);

    StatusBar  stat; 
    TStopwatch timer; 
    timer.Reset();
    timer.Start();

    // for every event
    for(int ientry=0; ientry<GetEntries(); ientry++){
      GetEntry(ientry);
      
      MonteCarloParticle *mc_electron = getParticle(event, 11, 0.1); 

      if (mc_electron->status == mc_status::reconstructed){
	// This is weird, but it's done to change the beta values 
	// so that the likelihood methods can be used. 
	event.SetElectronIndex(mc_electron->rec_index); 
	Corrections::correctEvent(&event, 0, 0); 

	std::vector<int> pionIdx = filter->getVectorOfParticleIndices(event, 211); 
	std::vector<int> kaonIdx = filter->getVectorOfParticleIndices(event, 321); 
	std::vector<int> protonIdx = filter->getVectorOfParticleIndices(event, 2212); 

	MonteCarloParticle *mc_pion   = getParticle(event,  211, 0.05); 
	MonteCarloParticle *mc_kaon   = getParticle(event,  321, 0.05); 
	MonteCarloParticle *mc_proton = getParticle(event, 2212, 0.05); 

	if (mc_pion->status == mc_status::reconstructed){
	  tupleWriter.setInt("hadron_pid", 211);
	  tupleWriter.setFloat("hadron_momentum", event.p[mc_pion->rec_index]);
	  tupleWriter.setFloat("hadron_beta", event.corr_b[mc_pion->rec_index]);
	  tupleWriter.writeEvent();
	}
	if (mc_kaon->status == mc_status::reconstructed){
	  tupleWriter.setInt("hadron_pid", 321);
	  tupleWriter.setFloat("hadron_momentum", event.p[mc_kaon->rec_index]);
	  tupleWriter.setFloat("hadron_beta", event.corr_b[mc_kaon->rec_index]);
	  tupleWriter.writeEvent();
	}
	if (mc_proton->status == mc_status::reconstructed){
	  tupleWriter.setInt("hadron_pid", 2212);
	  tupleWriter.setFloat("hadron_momentum", event.p[mc_proton->rec_index]);
	  tupleWriter.setFloat("hadron_beta", event.corr_b[mc_proton->rec_index]);
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
  ParticleFilter          *filter;
  Parameters              *params;
  SimpleNTupleWriter       tupleWriter; 

  DataEventCut_BetaPLikelihood *pionLikelihood, *kaonLikelihood, *protonLikelihood; 

};

int main(int argc, char *argv[]){

  if (argc > 1){
  // setup analysis 
  Analysis analysis; 

  // Setup Options
  h22Options opts;
  opts.args["PARS"].args = "/u/home/dmriser/analysis-main/lists/parameters/data/final.pars";
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
