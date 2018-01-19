#include <iostream>

// h22 libs 
#include "CommonTools.h"
#include "Corrections.h"
#include "CheckPoints.h"
#include "DBins.h"
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
    params->loadParameters(Form("%s/lists/parameters/data/final.pars", path.c_str())); 
    filter      = new ParticleFilter(params);

    mesonHistos[211][cutTypes::none]  = new MesonHistograms("211_no_cut",   211); 
    mesonHistos[321][cutTypes::none]  = new MesonHistograms("321_no_cut",   321);     
    
    // I am trying to use 'standard' reactions to get really nice clean 
    // particle traces without applying cuts.  
    mesonHistos[211][cutTypes::physicsEnhanced]  = new MesonHistograms("211_physics_enhanced",   211); 
    mesonHistos[321][cutTypes::physicsEnhanced]  = new MesonHistograms("321_physics_enhanced",   321);     
    mesonHistos[2212][cutTypes::physicsEnhanced]  = new MesonHistograms("2212_physics_enhanced",   2212);     

    // wahtever passes the current pid 
    mesonHistos[211][cutTypes::all]   = new MesonHistograms("211_all_cut",   211); 
    mesonHistos[321][cutTypes::all]   = new MesonHistograms("321_all_cut",   321);     
    mesonHistos[2212][cutTypes::all]  = new MesonHistograms("2212_all_cut",   2212);     

    betaPLikelihood[211]  = new DataEventCut_BetaPLikelihood(211); 
    betaPLikelihood[321]  = new DataEventCut_BetaPLikelihood(321); 
    betaPLikelihood[2212] = new DataEventCut_BetaPLikelihood(2212); 

    betaPLikelihood[-211]  = new DataEventCut_BetaPLikelihood(-211); 
    betaPLikelihood[-321]  = new DataEventCut_BetaPLikelihood(-321); 
    
    betaPLikelihood[211]->AddPossibleParticle(321);
    betaPLikelihood[211]->AddPossibleParticle(2212);

    betaPLikelihood[321]->AddPossibleParticle(211);
    betaPLikelihood[321]->AddPossibleParticle(2212);

    betaPLikelihood[2212]->AddPossibleParticle(211);
    betaPLikelihood[2212]->AddPossibleParticle(321);

    betaPLikelihood[-211]->AddPossibleParticle(-321);
    betaPLikelihood[-321]->AddPossibleParticle(-211);

    Parameters *resoPars = new Parameters(); 
    resoPars->loadParameters("resolution_parameters.dat");

    betaPLikelihood[211]  ->Configure(resoPars); 
    betaPLikelihood[321]  ->Configure(resoPars); 
    betaPLikelihood[-211] ->Configure(resoPars); 
    betaPLikelihood[-321] ->Configure(resoPars); 
    betaPLikelihood[2212] ->Configure(resoPars); 

    writer.addFloat("beta"); 
    writer.addFloat("p"); 
    writer.addFloat("cl"); 
    writer.addInt("pid");
  }

  ~Analysis(){
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
	    
	    if (event.q[ipart] > 0){
	      mesonHistos[211][cutTypes::none]->Fill(event, ipart); 
	      mesonHistos[321][cutTypes::none]->Fill(event, ipart); 

	  
	      if (betaPLikelihood[211] ->CanBeApplied(event, ipart) && betaPLikelihood[211] ->IsPassed(event, ipart)){ 
		mesonHistos[211][cutTypes::all] ->Fill(event,ipart); 
		
		writer.setFloat("cl",   betaPLikelihood[211]->GetConfidence());
		writer.setFloat("beta", event.corr_b[ipart]);
		writer.setFloat("p",    event.p[ipart]); 
		writer.setInt("pid",    211);
		writer.writeEvent(); 
	      }
	      if (betaPLikelihood[321] ->CanBeApplied(event, ipart) && betaPLikelihood[321] ->IsPassed(event, ipart)){ 
		mesonHistos[321][cutTypes::all] ->Fill(event,ipart); 
		writer.setFloat("cl",   betaPLikelihood[321]->GetConfidence());
		writer.setFloat("beta", event.corr_b[ipart]);
		writer.setFloat("p",    event.p[ipart]); 
		writer.setInt("pid",    321);
		writer.writeEvent(); 
		
	  }
	      //  why does this break?
	      if (betaPLikelihood[2212]->CanBeApplied(event, ipart) && betaPLikelihood[2212]->IsPassed(event, ipart)){ 
		mesonHistos[2212][cutTypes::all]->Fill(event,ipart); 
		writer.setFloat("cl",   betaPLikelihood[2212]->GetConfidence());
		writer.setFloat("beta", event.corr_b[ipart]);
		writer.setFloat("p",    event.p[ipart]); 
		writer.setInt("pid",    2212);
		writer.writeEvent(); 
	      }
	    }

	    else if (event.q[ipart] < 0){	  

	      if (betaPLikelihood[-211] ->CanBeApplied(event, ipart) && betaPLikelihood[-211] ->IsPassed(event, ipart)){ 
		//		mesonHistos[-211][cutTypes::all] ->Fill(event,ipart); 
		
		writer.setFloat("cl",   betaPLikelihood[-211]->GetConfidence());
		writer.setFloat("beta", event.corr_b[ipart]);
		writer.setFloat("p",    event.p[ipart]); 
		writer.setInt("pid",    -211);
		writer.writeEvent(); 
	      }

	      if (betaPLikelihood[-321] ->CanBeApplied(event, ipart) && betaPLikelihood[-321] ->IsPassed(event, ipart)){ 
		//		mesonHistos[321][cutTypes::all] ->Fill(event,ipart); 
		writer.setFloat("cl",   betaPLikelihood[-321]->GetConfidence());
		writer.setFloat("beta", event.corr_b[ipart]);
		writer.setFloat("p",    event.p[ipart]); 
		writer.setInt("pid",    -321);
		writer.writeEvent(); 		
	      }

	    }
	  }
	}
      }
	
      // at the end of the loop 
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
    
    writer.save(out); 

    mesonHistos[211][cutTypes::none]->Save(out); 
    mesonHistos[321][cutTypes::none]->Save(out); 

    mesonHistos[211][cutTypes::physicsEnhanced] ->Save(out); 
    mesonHistos[321][cutTypes::physicsEnhanced] ->Save(out); 
    mesonHistos[2212][cutTypes::physicsEnhanced] ->Save(out); 

    mesonHistos[211][cutTypes::all] ->Save(out); 
    mesonHistos[321][cutTypes::all] ->Save(out); 
    mesonHistos[2212][cutTypes::all]->Save(out); 

    out->Close();
  }


protected:

  MomCorr_e1f             *momCorr; 
  PhysicsEventBuilder      builder; 
  ParticleFilter          *filter;
  Parameters              *params;
  SimpleNTupleWriter       writer; 

  std::map<int, DataEventCut_BetaPLikelihood*>    betaPLikelihood; 
  std::map<int, std::map<int, MesonHistograms*> > mesonHistos; 
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
