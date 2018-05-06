#include <iostream>

// h22 libs 
#include "CommonTools.h"
#include "Corrections.h"
#include "CorrectionHistograms.h"
#include "h22Event.h"
#include "h22Reader.h"
#include "MomCorr.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"
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
    fCorrHistos = new CorrectionHistograms("test");

    std::string path = Global::Environment::GetAnalysisPath(); 
    fMomCorr = new MomCorr_e1f(Form("%s/momCorr/", path.c_str()));
  }

  ~Analysis(){
  }

  void Loop(){

    // setup reader options 
    GSIM = false; 
    Init();
    
    StatusBar  stat; 
    TStopwatch timer; 
    timer.Reset();
    timer.Start();

    // for every event
    for(int ientry=0; ientry<GetEntries(); ientry++){
      GetEntry(ientry);

      std::vector<int> electronIndices = event.GetVectorOfUserAnaParticleIndices(11); 
      if(!electronIndices.empty()){
	
	int electronIndex = electronIndices[0];
	event.SetElectronIndex(electronIndex);

	int sector = event.GetSector(electronIndex) + 1;

	// vertex corrections 
	fCorrHistos->h1_vz[0][0]->Fill(event.vz[electronIndex]);
	fCorrHistos->h1_vz[1][0]->Fill(event.corr_vz[electronIndex]);
	fCorrHistos->h1_vz[0][sector]->Fill(event.vz[electronIndex]);
	fCorrHistos->h1_vz[1][sector]->Fill(event.corr_vz[electronIndex]);

	fCorrHistos->h2_vz_phi[0][0]->Fill(event.vz[electronIndex], event.GetPhi(electronIndex));
	fCorrHistos->h2_vz_phi[1][0]->Fill(event.corr_vz[electronIndex], event.GetPhi(electronIndex));
	fCorrHistos->h2_vz_phi[0][sector]->Fill(event.vz[electronIndex], event.GetPhi(electronIndex));
	fCorrHistos->h2_vz_phi[1][sector]->Fill(event.corr_vz[electronIndex], event.GetPhi(electronIndex));

	// timing corrections 
	for(int ipart=1; ipart<event.gpart; ipart++){
	  if (event.q[ipart] > 0){
	    int hadronSector = event.GetSector(ipart) + 1;
	    int hadronPaddle = event.sc_pd[ipart];

	    // correct that sucker ! 
	    Corrections::correctEvent(&event, GetRunNumber(), GSIM);

	    float betaPred = 1/sqrt(1+pow(pi_mass/event.p[ipart],2));
	    fCorrHistos->h2_p_dbeta[0][hadronPaddle][0]->Fill(event.p[ipart], event.b[ipart]-betaPred);
	    fCorrHistos->h2_p_dbeta[0][hadronPaddle][hadronSector]->Fill(event.p[ipart], event.b[ipart]-betaPred);
	    fCorrHistos->h2_p_dbeta[1][hadronPaddle][0]->Fill(event.p[ipart], event.corr_b[ipart]-betaPred);
	    fCorrHistos->h2_p_dbeta[1][hadronPaddle][hadronSector]->Fill(event.p[ipart], event.corr_b[ipart]-betaPred);
	  }
	}


	
	std::vector<int> protonIndices = event.GetVectorOfUserAnaParticleIndices(2212); 
	if (protonIndices.size()){
	  // momentum corrections 
	  TLorentzVector electron = event.GetTLorentzVector(electronIndex, 11); 
	  TLorentzVector electronCorr = fMomCorr->PcorN(electron, -1, 11);
	  
	  PhysicsEvent physicsEvent = builder.getPhysicsEvent(electron);
	  PhysicsEvent physicsEventCorr = builder.getPhysicsEvent(electronCorr);
	  

	  fCorrHistos->h2_theta_dw[0][0]->Fill(event.GetTheta(electronIndex), physicsEvent.w - proton_mass);
	  fCorrHistos->h2_phi_dw[0][0]->Fill(event.GetRelativePhi(electronIndex), physicsEvent.w - proton_mass);
	  fCorrHistos->h2_theta_dw[0][sector]->Fill(event.GetTheta(electronIndex), physicsEvent.w - proton_mass);
	  fCorrHistos->h2_phi_dw[0][sector]->Fill(event.GetRelativePhi(electronIndex), physicsEvent.w - proton_mass);

	  fCorrHistos->h2_theta_dw[1][0]->Fill(event.GetTheta(electronIndex), physicsEventCorr.w - proton_mass);
	  fCorrHistos->h2_phi_dw[1][0]->Fill(event.GetRelativePhi(electronIndex), physicsEventCorr.w - proton_mass);
	  fCorrHistos->h2_theta_dw[1][sector]->Fill(event.GetTheta(electronIndex), physicsEventCorr.w - proton_mass);
	  fCorrHistos->h2_phi_dw[1][sector]->Fill(event.GetRelativePhi(electronIndex), physicsEventCorr.w - proton_mass);
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
    fCorrHistos->Save(out);
    out->Close();
  }


protected:

  PhysicsEventBuilder      builder; 
  CorrectionHistograms    *fCorrHistos; 
  MomCorr_e1f             *fMomCorr; 
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
 
  if (opts.args["N"].arg == 0){
    std::cerr << "Please pass in a number of files using -N option." << std::endl; 
    exit(0); 
  }

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
