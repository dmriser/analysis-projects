#include <iostream>

// h22 libs 
#include "CommonTools.h"
#include "Corrections.h"
#include "h22Event.h"
#include "h22Reader.h"
#include "Parameters.h"
#include "ParticleFilter.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"
#include "SimpleNTupleWriter.h"
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

    // setup structure of ntuple 
    tupleWriter.addFloat("dvz");
    tupleWriter.addFloat("beta");
    tupleWriter.addFloat("p");
    tupleWriter.addFloat("w");

    InitializeParticleIdentification(); 
  }

  ~Analysis(){
  }

  void InitializeParticleIdentification ( ) {
    std::string base_path = Global::Environment::GetAnalysisPath();
    std::string parameters_path(Form("%s/lists/parameters/data/final.pars", base_path.c_str()));

    parameters = new Parameters(); 
    parameters->loadParameters(parameters_path.c_str()); 
    particleFilter = new ParticleFilter(parameters); 
    
    builder = new PhysicsEventBuilder(); 
  }

  void Loop(){

    // setup reader options 
    GSIM = false; 
    Init();

    // Now let the machine know what kind 
    // of data we're using.  
    particleFilter->set_info(GSIM, GetRunNumber());
    
    StatusBar  stat; 
    TStopwatch timer; 
    timer.Reset();
    timer.Start();

    // for every event
    for(int ientry=0; ientry<GetEntries(); ientry++){
      GetEntry(ientry);

      //      std::vector<int> electronIndices = event.GetVectorOfUserAnaParticleIndices(11); 
      std::vector<int> electronIndices = particleFilter->getVectorOfParticleIndices(event, 11); 
      if(!electronIndices.empty()){
	
	int electronIndex = electronIndices[0];
	event.SetElectronIndex(electronIndex);

	// Let's apply some corretions to the 
	// parts of this event that need it! 
	Corrections::correctEvent(&event, GetRunNumber(), GSIM);

	// Keep this to calculate kinematics. 
	TLorentzVector electron = event.GetTLorentzVector(electronIndex, 11);

	// If there are positives, let's fill some histograms. 
	for (int ipart = 1; ipart < event.gpart; ipart++){
	  if (ipart != electronIndex && event.q[ipart] > 0 && 
	      event.dc_sect[ipart] == 3){

	    TLorentzVector positiveTrack = event.GetTLorentzVector(ipart, 211);
	    PhysicsEvent physicsEvent = builder->getPhysicsEvent(electron, positiveTrack); 

	    tupleWriter.setFloat("beta", event.corr_b[ipart]);
	    tupleWriter.setFloat("p", event.p[ipart]);
	    tupleWriter.setFloat("dvz", event.corr_vz[electronIndex] - event.corr_vz[ipart]);
	    tupleWriter.setFloat("w", physicsEvent.w); 
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

  PhysicsEventBuilder      *builder; 
  SimpleNTupleWriter       tupleWriter; 

  ParticleFilter *particleFilter; 
  Parameters *parameters; 
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
