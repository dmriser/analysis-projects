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
};

struct Histograms {
  TH2F *dcr3[constants::n_sectors];
};

Histograms *buildHistograms(std::string baseName){
  Histograms *histos = new Histograms(); 
  
  // Setup Histos 
  for (int isect = 0; isect < constants::n_sectors; isect++){
    std::string title = Form("dcr3_%s_sect%d", baseName.c_str(), isect+1); 

    histos->dcr3[isect] = new TH2F(title.c_str(), title.c_str(), 
				   100, 60, 520,
				   100, -450, 450); 
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
    filter->positivePionSelector->DisableByName("Delta Z-Vertex Cut"); 

    // setup historams 
    histosBefore  = buildHistograms("before");
    histosAfter = buildHistograms("after");


    builder = new PhysicsEventBuilder(); 
  }

  ~Analysis(){
    filter->electronSelector->PrintSummary(); 
    filter->positivePionSelector->PrintSummary();
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

      std::vector<int> electronIndices = filter->getVectorOfParticleIndices(event, 11);
      std::vector<int> electronIndicesUA = event.GetVectorOfUserAnaParticleIndices(11);
      std::vector<int> kpIndicesUA   = event.GetVectorOfUserAnaParticleIndices(211);
      std::vector<int> kpIndices   = filter->getVectorOfParticleIndices(event, 211);

      if(!electronIndices.empty() && 
	 ( !kpIndices.empty() || !kpIndicesUA.empty() )){
	
	int electronIndex = electronIndices[0];
	event.SetElectronIndex(electronIndex);
	Corrections::correctEvent(&event, GetRunNumber(), GSIM);
	  
	TLorentzVector electron = event.GetTLorentzVector(electronIndex, 11); 

	if (!kpIndices.empty()){
	  int index = kpIndices[0]; 
	  int sect = event.dc_sect[index] - 1; 
	  if (sect > -1){
	    histosAfter->dcr3[sect]->Fill(event.tl3_x[index], event.tl3_y[index]);
	  }
	}

	if (!kpIndicesUA.empty()){
	  int index = kpIndicesUA[0]; 
	  int sect = event.dc_sect[index] - 1; 
	  if (sect > -1){
	    histosBefore->dcr3[sect]->Fill(event.tl3_x[index], event.tl3_y[index]);
	  }
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
      histosBefore->dcr3[isect]->Write();
      histosAfter ->dcr3[isect]->Write();
    }

    out->Close();
  }


protected:

  MomCorr_e1f             *momCorr; 
  PhysicsEventBuilder     *builder; 
  ParticleFilter          *filter;
  Parameters              *params;
  Histograms              *histosBefore, *histosAfter; 
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
