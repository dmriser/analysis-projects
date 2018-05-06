#include <iostream>

// h22 libs 
#include "CommonTools.h"
#include "Corrections.h"
#include "h22Event.h"
#include "h22Reader.h"
#include "MomCorr.h"
#include "Parameters.h"
#include "ParticleFilter.h"
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
    std::string path = Global::Environment::GetAnalysisPath(); 
    fMomCorr = new MomCorr_e1f(Form("%s/momCorr/", path.c_str()));


    // needs parameters 
    params = new Parameters(); 
    params->loadParameters(Form("%s/lists/parameters/data/final.pars", path.c_str())); 

    // particle filtering 
    fFilter = new ParticleFilter(params); 

    // for holding output shit 
    fTree = new TTree("events", "events");
    branchPid        = fTree->Branch("pid",            pid); 
    branchConfidence = fTree->Branch("confidence",     confidence); 
    branchParticles  = fTree->Branch("particle",       particles); 
    branchNParticles = fTree->Branch("num_particles", &nparticles);

    // so something is there 
    for (int ipart = 0; ipart < MAX_PARTICLES; ipart++){
      particles[ipart] = TLorentzVector(); 
    }

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

      nparticles = 0;

      std::vector<int> electronIndices = fFilter->getVectorOfParticleIndices(event, 11); 
      if(!electronIndices.empty()){
	
	int electronIndex = electronIndices[0];
	event.SetElectronIndex(electronIndex);

	particles[0] = event.GetTLorentzVector(electronIndex, 11); 
	pid[0] = 11;
	nparticles++;

	// Look for the other particles that we 
	// are asked to find. 
	std::map<int, int> particleCounts; 
	for(int currentPid : targetParticles){
	  particleCounts[currentPid] = 0; 
	}

	// start getting them 
	for(int currentPid : targetParticles){
	  std::vector<int> particleIndices = fFilter->getVectorOfParticleIndices(event, currentPid);
	  
	  if(particleIndices.size() > particleCounts[currentPid]){
	    particles[nparticles] = event.GetTLorentzVector(particleIndices[particleCounts[currentPid]], 
							    currentPid);

	    pid[nparticles] = currentPid; 
	    particleCounts[currentPid]++; 
	    nparticles++; 
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
    fTree->Write(); 
    out->Close();
  }

  void addTargetParticle(int p){
    targetParticles.push_back(p);
  }

protected:

  PhysicsEventBuilder      builder; 
  MomCorr_e1f             *fMomCorr; 
  ParticleFilter          *fFilter; 
  Parameters              *params; 

  TTree *fTree; 
  
  const static int MAX_PARTICLES = 6;
  
  // tree variables 
  int nparticles; 
  int pid[MAX_PARTICLES];
  float confidence[MAX_PARTICLES];
  TLorentzVector particles[MAX_PARTICLES]; 

  // for the output file 
  TBranch *branchNParticles; 
  TBranch *branchPid;
  TBranch *branchConfidence;
  TBranch *branchParticles;

  std::vector<int> targetParticles; 


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
