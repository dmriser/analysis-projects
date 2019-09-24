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
#include "TRandom3.h"

enum mc_status {
  not_generated,
  not_reconstructed,
  reconstructed
};

struct MonteCarloParticle {
public:
  int rec_index, gen_index;
  int status, pid;
  float tolerance; 

  MonteCarloParticle () {
    rec_index = -1;
    gen_index = -1;
    pid = -1;
    status = mc_status::not_generated;
    tolerance = 1.0; 
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
	particle->tolerance = difference.Mag() / genParticle.Mag(); 
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
    momCorr                 = new MomCorr_e1f(momCorrPath);


    std::string nathanPath = Global::Environment::GetNathanPath();


    // needs parameters 
    params = new Parameters(); 
    params->loadParameters(Form("%s/lists/parameters/data/final.pars", path.c_str())); 

    filter      = new ParticleFilter(params);

    // setup structure of ntuple 
    tupleWriter.addInt("helicity");
    tupleWriter.addInt("meson_id");
    tupleWriter.addInt("sector");
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
    tupleWriter.addFloat("beta");
    tupleWriter.addFloat("tolerance_mes");
    tupleWriter.addFloat("tolerance_ele");
  }

  ~Analysis(){
    // total destruction 
  }

  void Loop(){
    
    // setup reader options 
    GSIM = true; 
    Init();

    // setup particle filter 
    filter->set_info(GSIM, GetRunNumber());
    //    hid->SetRunNumber( GetRunNumber() );
    //    hid->SetGSIM(GSIM); 

    StatusBar  stat; 
    TStopwatch timer; 
    timer.Reset();
    timer.Start();

    TRandom3 random = TRandom3();
      
    // for every event
    for(int ientry=0; ientry<GetEntries(); ientry++){
      GetEntry(ientry);

      MonteCarloParticle *mc_electron = getParticle(event, 11, 0.1); 
      MonteCarloParticle *mc_kaon = getParticle(event, 321, 0.1);
      if (mc_electron->status == mc_status::reconstructed && mc_kaon->status == mc_status::reconstructed){
	
	// Build generated electron and kaon, then build a generated event. 
	TLorentzVector genElectron =TLorentzVector();
	genElectron.SetE(event.mcp[mc_electron->gen_index]);
	genElectron.SetX(event.mcp[mc_electron->gen_index] * sin(event.mctheta[mc_electron->gen_index] * TMath::Pi() / 180.0) * cos(event.mcphi[mc_electron->gen_index] * TMath::Pi() / 180.0));
	genElectron.SetY(event.mcp[mc_electron->gen_index] * sin(event.mctheta[mc_electron->gen_index] * TMath::Pi() / 180.0) * sin(event.mcphi[mc_electron->gen_index] * TMath::Pi() / 180.0));
	genElectron.SetZ(event.mcp[mc_electron->gen_index] * cos(event.mctheta[mc_electron->gen_index] * TMath::Pi() / 180.0));

	TLorentzVector genKaon = TLorentzVector();
	genKaon.SetE(sqrt(pow(event.mcp[mc_kaon->gen_index],2) + pow(0.493,2)));
	genKaon.SetX(event.mcp[mc_kaon->gen_index] * sin(event.mctheta[mc_kaon->gen_index] * TMath::Pi() / 180.0) * cos(event.mcphi[mc_kaon->gen_index] * TMath::Pi() / 180.0));
	genKaon.SetY(event.mcp[mc_kaon->gen_index] * sin(event.mctheta[mc_kaon->gen_index] * TMath::Pi() / 180.0) * sin(event.mcphi[mc_kaon->gen_index] * TMath::Pi() / 180.0));
	genKaon.SetZ(event.mcp[mc_kaon->gen_index] * cos(event.mctheta[mc_kaon->gen_index] * TMath::Pi() / 180.0));

	PhysicsEvent genEvent = builder.getPhysicsEvent(genElectron, genKaon); 

	TLorentzVector electron = event.GetTLorentzVector(mc_electron->rec_index, 11); 
	TLorentzVector meson = event.GetTLorentzVector(mc_kaon->rec_index, 321); 
	PhysicsEvent ev = builder.getPhysicsEvent(electron, meson); 

	float norm = 0.72; 
	float genAsym = norm * genEvent.x * (1-genEvent.x) * genEvent.z * genEvent.pT * sin(genEvent.phiHadron * TMath::Pi() / 180.0);
	float p = 0.5 * (genAsym + 1); 

	int helicity = (random.Rndm() < p) ? 1 : -1; 

	int electronIndex = mc_electron->rec_index; 
	int mesonIndex = mc_kaon->rec_index; 
	tupleWriter.setInt("helicity",       helicity);
	tupleWriter.setInt("meson_id",       321);
	tupleWriter.setInt("sector",       event.dc_sect[electronIndex]);
	tupleWriter.setFloat("missing_mass", sqrt(ev.mm2));
	tupleWriter.setFloat("x",            ev.x);
	tupleWriter.setFloat("q2",           ev.qq);
	tupleWriter.setFloat("z",            ev.z);
	tupleWriter.setFloat("pt",           ev.pT);
	tupleWriter.setFloat("w",            ev.w);
	tupleWriter.setFloat("eta",          ev.eta);
	tupleWriter.setFloat("theta_h",      ev.thetaHadron);
	tupleWriter.setFloat("phi_h",        ev.phiHadron);
	tupleWriter.setFloat("p_ele",        event.p[electronIndex]); 
	tupleWriter.setFloat("p_mes",        event.p[mesonIndex]); 
	tupleWriter.setFloat("theta_ele",    to_degrees*electron.Theta()); 
	tupleWriter.setFloat("theta_mes",    to_degrees*meson.Theta()); 
	tupleWriter.setFloat("phi_ele",      to_degrees*electron.Phi()); 
	tupleWriter.setFloat("phi_mes",      to_degrees*meson.Phi()); 
	tupleWriter.setFloat("dvz",          event.vz[electronIndex]-event.vz[mesonIndex]); 
	tupleWriter.setFloat("alpha",        1.0); 
	tupleWriter.setFloat("beta",         event.b[mesonIndex]); 
	tupleWriter.setFloat("tolerance_mes",    mc_kaon->tolerance); 
	tupleWriter.setFloat("tolerance_ele",    mc_electron->tolerance); 
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

  MomCorr_e1f             *momCorr; 
  PhysicsEventBuilder      builder; 
  ParticleFilter          *filter; 
  Parameters              *params; 
  SimpleNTupleWriter      tupleWriter; 

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
