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

namespace constants {
  const static int n_sectors = 6; 
  const static int n_w_bins = 100; 
  const static float w_min = 0.6; 
  const static float w_max = 1.3; 
};

struct Histograms {

  // Electron Only 
  TH1F *w[constants::n_sectors];
  TH1F *w_corr[constants::n_sectors];
  TH2F *w_vz[constants::n_sectors];
  TH2F *w_px[constants::n_sectors];
  TH2F *w_py[constants::n_sectors];
  TH2F *w_pz[constants::n_sectors];
  TH2F *w_p_perp[constants::n_sectors];

  // Positive Kaon 
  TH1F *mm_sigma[constants::n_sectors];
  TH1F *mm_sigma_corr[constants::n_sectors];
};

Histograms *buildHistograms(std::string baseName){
  Histograms *histos = new Histograms(); 
  
  // Setup Histos 
  for (int isect = 0; isect < constants::n_sectors; isect++){
    std::string title = Form("w_%s_sect%d", baseName.c_str(), isect+1); 

    histos->w[isect] = new TH1F(title.c_str(), title.c_str(), 
			       constants::n_w_bins, 
			       constants::w_min, 
			       constants::w_max); 

    title = Form("w_corr_%s_sect%d", baseName.c_str(), isect+1); 
    histos->w_corr[isect] = new TH1F(title.c_str(), title.c_str(), 
				     constants::n_w_bins, 
				     constants::w_min, 
				     constants::w_max); 

    title = Form("mm_sigma_%s_sect%d", baseName.c_str(), isect+1); 
    histos->mm_sigma[isect] = new TH1F(title.c_str(), title.c_str(), 
				       200, 1.0, 1.3);

    title = Form("mm_sigma_corr_%s_sect%d", baseName.c_str(), isect+1); 
    histos->mm_sigma_corr[isect] = new TH1F(title.c_str(), title.c_str(), 
				       200, 1.0, 1.3);

    title = Form("w_vz_%s_sect%d", baseName.c_str(), isect+1); 
    histos->w_vz[isect] = new TH2F(title.c_str(), title.c_str(), 
				   constants::n_w_bins, 
				   constants::w_min, 
				   constants::w_max,
				   100, -30, -18); 

    title = Form("w_px_%s_sect%d", baseName.c_str(), isect+1); 
    histos->w_px[isect] = new TH2F(title.c_str(), title.c_str(), 
				   constants::n_w_bins, 
				   constants::w_min, 
				   constants::w_max,
				   100, -2, 2); 

    title = Form("w_py_%s_sect%d", baseName.c_str(), isect+1); 
    histos->w_py[isect] = new TH2F(title.c_str(), title.c_str(), 
				   constants::n_w_bins, 
				   constants::w_min, 
				   constants::w_max,
				   100, -2, 2); 

    title = Form("w_p_perp_%s_sect%d", baseName.c_str(), isect+1); 
    histos->w_p_perp[isect] = new TH2F(title.c_str(), title.c_str(), 
				       constants::n_w_bins, 
				       constants::w_min, 
				       constants::w_max,
				       100, 0, 2); 
 
    title = Form("w_pz_%s_sect%d", baseName.c_str(), isect+1); 
    histos->w_pz[isect] = new TH2F(title.c_str(), title.c_str(), 
				   constants::n_w_bins, 
				   constants::w_min, 
				   constants::w_max,
				   100, 0, 5);     
  }

  return histos; 
}

class Analysis : public h22Reader {
public:  
  Analysis() {

    std::string path        = Global::Environment::GetAnalysisPath(); 
    std::string momCorrPath = Form("%s/momCorr/",path.c_str());
    momCorr                 = new MomCorr_e1f(momCorrPath);

    // needs parameters 
    params = new Parameters(); 
    params->loadParameters(Form("%s/lists/parameters/data/final.pars", path.c_str())); 

    // particle filter for doing pid
    filter      = new ParticleFilter(params);

    // setup historams 
    histosUA  = buildHistograms("userana");
    histosPID = buildHistograms("pid");


    builder = new PhysicsEventBuilder(); 
  }

  ~Analysis(){
    filter->electronSelector->PrintSummary(); 
  }

  void Loop(long int eventsToProcess){

    // setup reader options 
    GSIM = false; 
    Init();
    
    // setup particle filter 
    filter->set_info(GSIM, GetRunNumber());

    StatusBar  stat; 
    TStopwatch timer; 
    timer.Reset();
    timer.Start();

    eventsToProcess = (GetEntries() < eventsToProcess ? GetEntries() : eventsToProcess); 

    // for every event
    for(int ientry=0; ientry < eventsToProcess; ientry++){
      GetEntry(ientry);

      std::vector<int> electronIndicesUA = event.GetVectorOfUserAnaParticleIndices(11); 
      std::vector<int> electronIndices   = filter->getVectorOfParticleIndices(event, 11);
      std::vector<int> kpIndicesUA = event.GetVectorOfUserAnaParticleIndices(321); 
      std::vector<int> kpIndices   = filter->getVectorOfParticleIndices(event, 321);

      if(!electronIndices.empty()){
	
	int electronIndex = electronIndices[0];
	event.SetElectronIndex(electronIndex);
	Corrections::correctEvent(&event, GetRunNumber(), GSIM);
	  
	TLorentzVector electron = event.GetTLorentzVector(electronIndex, 11); 
	    
	// build event 
	PhysicsEvent ev = builder->getPhysicsEvent(electron);

	int sect = event.dc_sect[electronIndex] - 1; 
	if (sect > -1){
	  histosPID->w[sect]->Fill(ev.w);
	}
 
	if ( !kpIndices.empty() && sect > -1){
	  TLorentzVector kpVector = event.GetTLorentzVector(kpIndices[0], 321); 	  
	  PhysicsEvent kaonEvent = builder->getPhysicsEvent(electron, kpVector); 
	  histosPID->mm_sigma[sect]->Fill(sqrt(kaonEvent.mm2));
	}

	electron = momCorr->PcorN(electron, -1, 11);
	ev = builder->getPhysicsEvent(electron);

	if (sect > -1){
	  histosPID->w_corr[sect]->Fill(ev.w);
	  histosPID->w_px[sect]->Fill(ev.w, event.p[electronIndex] * event.cx[electronIndex]);
	  histosPID->w_py[sect]->Fill(ev.w, event.p[electronIndex] * event.cy[electronIndex]);
	  histosPID->w_pz[sect]->Fill(ev.w, event.p[electronIndex] * event.cz[electronIndex]);
	  histosPID->w_p_perp[sect]->Fill(ev.w, event.p[electronIndex] * sqrt(pow(event.cx[electronIndex],2) + 
									      pow(event.cy[electronIndex],2)));
	  histosPID->w_vz[sect]->Fill(ev.w, event.vz[electronIndex]);


	  if ( !kpIndices.empty() ){
	    TLorentzVector kpVector = event.GetTLorentzVector(kpIndices[0], 321); 	  
	    PhysicsEvent kaonEvent = builder->getPhysicsEvent(electron, kpVector); 
	    histosPID->mm_sigma_corr[sect]->Fill(sqrt(kaonEvent.mm2));
	  }
	}

      }
      
      if(!electronIndicesUA.empty()){
	
	int electronIndex = electronIndicesUA[0];
	event.SetElectronIndex(electronIndex);
	Corrections::correctEvent(&event, GetRunNumber(), GSIM);
	  
	TLorentzVector electron = event.GetTLorentzVector(electronIndex, 11); 

	    
	// build event 
	PhysicsEvent ev = builder->getPhysicsEvent(electron);

	int sect = event.dc_sect[electronIndex] - 1; 
	if (sect > -1){
	  histosUA->w[sect]->Fill(ev.w);
	}

	electron = momCorr->PcorN(electron, -1, 11);
	ev = builder->getPhysicsEvent(electron);
	
	if (sect > -1){
	  histosUA->w_corr[sect]->Fill(ev.w);
	  histosUA->w_px[sect]->Fill(ev.w, event.p[electronIndex] * event.cx[electronIndex]);
	  histosUA->w_py[sect]->Fill(ev.w, event.p[electronIndex] * event.cy[electronIndex]);
	  histosUA->w_pz[sect]->Fill(ev.w, event.p[electronIndex] * event.cz[electronIndex]);
	  histosUA->w_p_perp[sect]->Fill(ev.w, event.p[electronIndex] * sqrt(pow(event.cx[electronIndex],2) + 
									      pow(event.cy[electronIndex],2)));
	  histosUA->w_vz[sect]->Fill(ev.w, event.vz[electronIndex]);
	}
	
      }
      
      
      if (ientry%10000 == 0){
	stat.PrintStatus(ientry, eventsToProcess); 
      }
    }

    // summarize results 
    std::cout << std::endl;
    double loopTime  = timer.RealTime(); 
    double eventRate = eventsToProcess/loopTime; 
    std::cout << "[GenericAnalysis::Loop] Total time: " 
	      << loopTime << " seconds, Event rate: " 
	      << eventRate << " events/sec. " << std::endl;

  }

  void Save(std::string outFile){
    TFile *out = new TFile(outFile.c_str(), "recreate");
    
    for (int isect = 0; isect < constants::n_sectors; isect++){
      histosPID->w[isect]->Write();
      histosPID->w_corr[isect]->Write();
      histosPID->w_vz[isect]->Write();
      histosPID->w_px[isect]->Write();
      histosPID->w_py[isect]->Write();
      histosPID->w_pz[isect]->Write();
      histosPID->w_p_perp[isect]->Write();
      histosPID->mm_sigma[isect]->Write();
      histosPID->mm_sigma_corr[isect]->Write();

      histosUA->w[isect]->Write(); 
      histosUA->w_corr[isect]->Write(); 
      histosUA->w_vz[isect]->Write();
      histosUA->w_px[isect]->Write();
      histosUA->w_py[isect]->Write();
      histosUA->w_pz[isect]->Write();
      histosUA->w_p_perp[isect]->Write();
      histosUA->mm_sigma[isect]->Write();
      histosUA->mm_sigma_corr[isect]->Write();
    }

    out->Close();
  }


protected:

  MomCorr_e1f             *momCorr; 
  PhysicsEventBuilder     *builder; 
  ParticleFilter          *filter;
  Parameters              *params;
  Histograms              *histosUA, *histosPID; 
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
  analysis.Loop(opts.args["N"].arg);
  analysis.Save(opts.args["OUT"].args);
  
  } else {
    std::cerr << "No files found." << std::endl; 
  }

  return 0; 
}
