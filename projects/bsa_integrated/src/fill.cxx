#include <iostream>

#include "common/Config.h"
#include "common/ConfigLoader.h"
#include "common/ConfigLoader.cxx"
#include "common/MissingMassHistograms.h"
#include "common/PhiHistograms.h"
#include "common/PtAverager.h"
#include "common/TreeOutput.h"
#include "common/Types.h"

// h22 libs 
#include "CommonTools.h"
#include "Corrections.h"
#include "CheckPoints.h"
#include "DBins.h"
#include "h22Event.h"
#include "h22Reader.h"
#include "MesonHistograms.h"
#include "MomCorr.h"
#include "ParticleFilter.h"
#include "Parameters.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"
#include "StatusBar.h"
#include "StandardHistograms.h"

// root 
#include "TFile.h"
#include "TLorentzVector.h"
#include "TStopwatch.h"

struct configPack{
  Config              conf;
  Parameters         *pars; 
  ParticleFilter     *filter;
  PhiHistos          *counts[2]; 
  CheckPoints        *checkPoints; 
  PtAverager         *ptAverager; 

  TreeOutput *tree; 
  std::map<int, StandardHistograms*> standardHistos; 
  std::map<int, MesonHistograms*>    mesonHistos; 
  std::map<int, MissingMassHistos*>  missingMassHistos; 
};

class Analysis : public h22Reader {
public:  
  Analysis(std::vector<Config> confs) {

    std::string path        = Global::Environment::GetAnalysisPath(); 
    std::string momCorrPath = Form("%s/momCorr/",path.c_str());
    momCorr                 = new MomCorr_e1f(momCorrPath);

    for(int iconf=0; iconf<confs.size(); iconf++){
      configPack currentPack; 
      currentPack.conf = confs[iconf]; 
      currentPack.pars = new Parameters(); 
      currentPack.pars->loadParameters(currentPack.conf.parameterFile);      
      currentPack.filter = new ParticleFilter(currentPack.pars);

      // setting up histograms for different cut levels 
      // these types are defined in types.h
      currentPack.standardHistos[pass::mesonID] = new StandardHistograms("PassMesonID",0);
      currentPack.standardHistos[pass::kin]     = new StandardHistograms("PassKinematic"  ,0);
      currentPack.standardHistos[pass::all]     = new StandardHistograms("PassAll"    ,0);

      // setup meson histograms 
      currentPack.mesonHistos[pass::mesonID] = new MesonHistograms("PassMesonID", currentPack.conf.mesonIndex); 
      currentPack.mesonHistos[pass::kin]     = new MesonHistograms("PassKinematic", currentPack.conf.mesonIndex); 
      currentPack.mesonHistos[pass::all]     = new MesonHistograms("PassAll", currentPack.conf.mesonIndex); 

      // setup missing mass histos 
      currentPack.missingMassHistos[pass::mesonID] = new MissingMassHistos(); 
      currentPack.missingMassHistos[pass::kin]     = new MissingMassHistos(); 

      currentPack.missingMassHistos[pass::mesonID]->Init(currentPack.conf, "PassMesonID");
      currentPack.missingMassHistos[pass::kin]    ->Init(currentPack.conf, "PassKinematic");

      // counts in phi for different helicties 
      currentPack.counts[helicity::minus] = new PhiHistos(); 
      currentPack.counts[helicity::minus]->Init(currentPack.conf.axes, "countsMinus");

      currentPack.counts[helicity::plus] = new PhiHistos(); 
      currentPack.counts[helicity::plus]->Init(currentPack.conf.axes, "countsPlus");

      // output tree 
      currentPack.tree = new TreeOutput(); 

      // pt averaging 
      currentPack.ptAverager = new PtAverager(currentPack.conf); 

      // checkpoints 
      currentPack.checkPoints = new CheckPoints(); 
      currentPack.checkPoints->AddCheckPoint("Passing Electron ID"); 
      currentPack.checkPoints->AddCheckPoint("Passing Meson ID"); 
      currentPack.checkPoints->AddCheckPoint("Passing Kinematic Cuts");
      currentPack.checkPoints->AddCheckPoint("Passing Everything"); 

      packs.push_back(currentPack);
    }

    GSIM = false; 
    Init();
  }

  ~Analysis(){
    for(configPack p : packs){
      std::cout << p.conf.name << std::endl; 
      p.filter->GetSelector(11)->PrintSummary();
      p.filter->GetSelector(p.conf.mesonIndex)->PrintSummary();
    }
    for(configPack p : packs){
      std::cout << p.conf.name << std::endl; 
      p.checkPoints->PrintSummary(); 
    }
  }

  void Loop(){

    // setup filters with current info after 
    // files ahve been added 
    for (configPack p : packs){
      p.filter->set_info(GSIM, GetRunNumber());
    }

    StatusBar stat; 

    TStopwatch timer; 
    timer.Reset();
    timer.Start();

    // for every event
    for(int ientry=0; ientry<GetEntries(); ientry++){
      GetEntry(ientry);
      
      // process each configuration 
      for (configPack pack : packs){

	std::vector<int> electronIndices = pack.filter->getVectorOfParticleIndices(event, 11);
	if(!electronIndices.empty()){

	  pack.checkPoints->CheckIn(); 
	  
	  int electronIndex = electronIndices[0];
	  event.SetElectronIndex(electronIndex);
	  Corrections::correctEvent(&event, GetRunNumber(), GSIM);

	  std::vector<int> mesonIndices = pack.filter->getVectorOfParticleIndices(event, pack.conf.mesonIndex);
	  if (!mesonIndices.empty()){

	    pack.checkPoints->CheckIn(); 
	  
	    // build an event 
	    int mesonIndex = mesonIndices[0];
	    TLorentzVector electron = event.GetTLorentzVector(electronIndex, 11);  
	    TLorentzVector meson = event.GetTLorentzVector(mesonIndex, pack.conf.mesonIndex);

	    electron = momCorr->PcorN(electron, -1, 11);
	    PhysicsEvent ev = builder.getPhysicsEvent(electron,meson); 


	    // Dan Carman method for rejecting some pi+ events in the 
	    // kaon sample. 
	    //
	    // Classify an event and if it's a kaon you pretend 
	    // it's a pion.  Then check the missing mass spectrum 
	    // and if the event is near the neutron peak it must 
	    // really be a pion, so reassign it to be.  
	    //
	    // Currently all it does it throw out the pion, 
	    // if doesn't actually add it to the pion sample 
	    // because the code never makes it that far in the 
	    // pion case it would just return 0 pions and not enter.
	    if (pack.conf.mesonIndex == 321){
	      
	      TLorentzVector pion = event.GetTLorentzVector(mesonIndex, 211); 
	      PhysicsEvent pionEvent = builder.getPhysicsEvent(electron,pion); 
	      
	      // reject and dont process event 
	      if (sqrt(pionEvent.mm2) < 1.07){
		continue; 
	      }
	    }
	    
	    // if a proton is required to continue 
	    if (int(pack.conf.cuts["require_proton"]["value"]) == 1){
	      std::vector<int> protonIndices = pack.filter->getVectorOfParticleIndices(event, 2212);

	      // If there is no proton, we're breaking out of the
	      // current loop iteration and discarding the event.
	      if(protonIndices.empty()){
		continue; 
	      }
	    }

	    pack.standardHistos[pass::mesonID]->Fill(event, electronIndex, mesonIndex, ev);
	    pack.mesonHistos[pass::mesonID]->Fill(event, ev, mesonIndex);
	    pack.missingMassHistos[pass::mesonID]->Fill(ev);

	    // check that event is kinematically 
	    // desirable
	    if (ev.w  > pack.conf.cuts["w"]["min"]                     && 
		ev.w  < pack.conf.cuts["w"]["max"]                     && 
		ev.qq > pack.conf.cuts["q2"]["min"]                    && 
		ev.qq < pack.conf.cuts["q2"]["max"]                    && 
		event.p[mesonIndex] > pack.conf.cuts["meson_p"]["min"] && 
		event.p[mesonIndex] < pack.conf.cuts["meson_p"]["max"]){ 
	      
	    
	      pack.checkPoints->CheckIn(); 

	      // do something 
	      pack.standardHistos[pass::kin]->Fill(event, electronIndex, mesonIndex, ev);
	      pack.mesonHistos[pass::kin]->Fill(event, ev, mesonIndex);
	      pack.missingMassHistos[pass::kin]->Fill(ev);

	      if (sqrt(ev.mm2) > pack.conf.cuts["missing_mass"]["min"] && 
		  sqrt(ev.mm2) < pack.conf.cuts["missing_mass"]["max"] ){

		// do something more 
		// these are the real events 
		// determine helicity state 
		int hel = helicity::unknown; 
		if (event.corr_hel > 0){ hel = helicity::plus;  }
		if (event.corr_hel < 0){ hel = helicity::minus; }
		
		if(hel != helicity::unknown){
		  pack.checkPoints->CheckIn(); 
	  
		  pack.standardHistos[pass::all]->Fill(event, electronIndex, mesonIndex, ev);
		  pack.mesonHistos[pass::all]->Fill(event, ev, mesonIndex);
		  pack.counts[hel]->Fill(ev);

		  // fill averge pt 
		  pack.ptAverager->Fill(ev);

		  // fill output tree 
		  pack.tree->hel      = hel; 
		  pack.tree->id       = pack.conf.mesonIndex; 
		  pack.tree->electron = electron; 
		  pack.tree->meson    = meson; 
		  pack.tree->tree->Fill();

		}
	      }
	    }
	  }
	}

      
	pack.checkPoints->Reset(); 
      }
      

      if (ientry%10000 == 0){
	stat.PrintStatus(ientry, GetEntries()); 
      }
    }

    std::cout << std::endl;
    double loopTime  = timer.RealTime(); 
    double eventRate = GetEntries()/loopTime; 
    std::cout << "[GenericAnalysis::Loop] Total time: " 
	      << loopTime << " seconds, Event rate: " 
	      << eventRate << " events/sec. " << std::endl;

  }

  void Save(){

    for (configPack p : packs){
      TFile *out = new TFile(Form("%s%s.root",
				  p.conf.outputPath.c_str(),
				  p.conf.name.c_str()), 
				  "recreate");
      
      out->SetCompressionLevel(3); 

      p.standardHistos[pass::mesonID]->Save(out);
      p.standardHistos[pass::kin]    ->Save(out);
      p.standardHistos[pass::all]    ->Save(out);

      p.mesonHistos[pass::mesonID]->Save(out); 
      p.mesonHistos[pass::kin]    ->Save(out); 
      p.mesonHistos[pass::all]    ->Save(out); 

      p.missingMassHistos[pass::mesonID]->Save(out);
      p.missingMassHistos[pass::kin]    ->Save(out);      

      // saving counts 
      p.counts[helicity::plus] ->Save(out);      
      p.counts[helicity::minus]->Save(out);

      // save pt averager 
      p.ptAverager->Save(out);
      
      // save only if 
      // asked 
      if (p.conf.writeTree){
	p.tree->Save(out);
      }
      
      // save checkpoints 
      p.checkPoints->Save(out); 

      out->Write();
      out->Close();
    }

  }

protected:
  std::vector<configPack> packs; 
  MomCorr_e1f             *momCorr; 
  PhysicsEventBuilder     builder; 

};

int main(int argc, char *argv[]){

  if (argc > 1){

    // get configurations 
    std::vector<Config> configs; 
    for(int iarg=1; iarg<argc; iarg++){
      std::string file(argv[iarg]);
      configs.push_back(ConfigLoader::getConfiguration(file)); 
    }

    // setup analysis 
    Analysis analysis(configs); 

    // use the first configuration list of files 
    // and load the analyzer 
    std::vector<std::string> files = loadFilesFromList(configs[0].fileList, configs[0].filesToProcess);
    for(std::string f : files){
      analysis.AddFile(f); 
      std::cout << Form("Loaded file %s ",f.c_str()) << std::endl; 
    }
    std::cout << std::endl; 

    // run analysis loop
    analysis.Loop();
    analysis.Save();

  } else {
    std::cerr << "No configurations found." << std::endl; 
  }

  return 0; 
}
