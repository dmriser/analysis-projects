#include <iostream>

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

class Analysis : public h22Reader {
public:  
  Analysis() {

    std::string path        = Global::Environment::GetAnalysisPath(); 
    std::string momCorrPath = Form("%s/momCorr/",path.c_str());
    momCorr                 = new MomCorr_e1f(momCorrPath);

    // needs parameters 
    params = new Parameters(); 
    params->loadParameters(Form("%s/lists/parameters/data/final.pars", path.c_str())); 

    paramsLoose = new Parameters(); 
    paramsLoose->loadParameters(Form("%s/lists/parameters/data/final-loose.pars", path.c_str())); 

    filter = new ParticleFilter(params);
    filterLoose = new ParticleFilter(paramsLoose);

    // ntuple 
    tupleWriter.addInt("helicity");
    tupleWriter.addInt("topology");
    tupleWriter.addTLorentzVector("electron"); 
    tupleWriter.addTLorentzVector("proton"); 
    tupleWriter.addTLorentzVector("kaon_pos"); 
    tupleWriter.addTLorentzVector("kaon_neg"); 
    tupleWriter.addTLorentzVector("pion_pos"); 
    tupleWriter.addTLorentzVector("pion_neg"); 
    
    // pid variables for hadrons 
    tupleWriter.addFloat("alpha_pion_pos");
    tupleWriter.addFloat("alpha_pion_neg");
    tupleWriter.addFloat("alpha_kaon_pos");
    tupleWriter.addFloat("alpha_kaon_neg");
    tupleWriter.addFloat("alpha_proton");

    // distances for systematics 
    tupleWriter.addFloat("dist_ecsf");
    tupleWriter.addFloat("dist_ec_edep");
    tupleWriter.addFloat("dist_vz");
    tupleWriter.addFloat("dist_cc_theta");
    tupleWriter.addFloat("dist_dcr1");
    tupleWriter.addFloat("dist_dcr3");
    tupleWriter.addFloat("dist_ecu");
    tupleWriter.addFloat("dist_ecv");
    tupleWriter.addFloat("dist_ecw");
    tupleWriter.addFloat("dist_cc");


    fMinConfidence = 1e-3; 
  }

   ~Analysis(){
    // total destruction 
  }

  void Loop(){
    
    // setup reader options 
    GSIM = false; 
    Init();

    // setup particle filter 
    filter->set_info(GSIM, GetRunNumber());

    StatusBar stat; 

    TStopwatch timer; 
    timer.Reset();
    timer.Start();

    // for every event
    for(int ientry=0; ientry<GetEntries(); ientry++){
      GetEntry(ientry);
      
      std::vector<int> electronIndices = filterLoose->getVectorOfParticleIndices(event, 11);
      if(!electronIndices.empty()){
	
	int electronIndex = electronIndices[0];
	event.SetElectronIndex(electronIndex);
	Corrections::correctEvent(&event, GetRunNumber(), GSIM);

	// fill distances 
	std::map<std::string, float> distances = filter->eid_distance_map(event, electronIndex);
        tupleWriter.setFloat("dist_ecsf",    distances["EC_SAMPLING"]);
        tupleWriter.setFloat("dist_ec_edep", distances["EC_IN_OUT"]);
        tupleWriter.setFloat("dist_vz",      distances["Z_VERTEX"]);
        tupleWriter.setFloat("dist_cc_theta",distances["CC_THETA"]);
        tupleWriter.setFloat("dist_dcr1",    distances["DC_R1_FID"]);
        tupleWriter.setFloat("dist_dcr3",    distances["DC_R3_FID"]);
        tupleWriter.setFloat("dist_ecu",     distances["EC_FID_U"]);
        tupleWriter.setFloat("dist_ecv",     distances["EC_FID_V"]);
        tupleWriter.setFloat("dist_ecw",     distances["EC_FID_W"]);
        tupleWriter.setFloat("dist_cc",      distances["CC_FID"]);
	
	// search mofo 
	std::vector<int> kpIndices     = filterLoose->getVectorOfParticleIndices(event,  321);
	std::vector<int> kmIndices     = filterLoose->getVectorOfParticleIndices(event, -321);
	std::vector<int> ppIndices     = filterLoose->getVectorOfParticleIndices(event,  211);
	std::vector<int> pmIndices     = filterLoose->getVectorOfParticleIndices(event, -211);
	std::vector<int> protonIndices = filterLoose->getVectorOfParticleIndices(event, 2212);


	TLorentzVector electron = event.GetTLorentzVector(electronIndex, 11); 
	electron = momCorr->PcorN(electron, -1, 11);
	PhysicsEvent electronEvent = builder.getPhysicsEvent(electron); 

	if(electronEvent.w > 2.0 && electronEvent.qq > 1.0){
	  // something will be written 
	  if ( (kpIndices.size() > 0)&&(kmIndices.size() > 0) || (kpIndices.size() > 0) && (protonIndices.size() > 0) || (protonIndices.size() > 0) && (kmIndices.size() > 0) ) {
	    int top = -1; 
	    
	    
	    tupleWriter.setInt("helicity", event.corr_hel); 
	    
	       if ( (kpIndices.size() > 0)&&(kmIndices.size() > 0) ){
		 top = 2;
		 tupleWriter.setInt("topology", top); 
		 
		 // getting likelihood 
		 DataEventCut_BetaPLikelihood *bpCutKP = (DataEventCut_BetaPLikelihood*) filter->GetSelector(321) ->GetCut("Beta P Likelihood Cut 321");
		 DataEventCut_BetaPLikelihood *bpCutKM = (DataEventCut_BetaPLikelihood*) filter->GetSelector(-321)->GetCut("Beta P Likelihood Cut -321");
		 
		 // this needs to be called to get the confidence correct
		 bool garbageCan = bpCutKP->IsPassed(event, kpIndices[0]);
		 tupleWriter.setFloat("alpha_kaon_pos", bpCutKP->GetConfidence());
		 
		 garbageCan      = bpCutKM->IsPassed(event, kmIndices[0]);
		 tupleWriter.setFloat("alpha_kaon_neg", bpCutKM->GetConfidence());
		 
		 TLorentzVector kp = event.GetTLorentzVector(kpIndices[0], 321); 
		 TLorentzVector km = event.GetTLorentzVector(kmIndices[0], -321); 
		 
		 PhysicsEvent physicsEvent = builder.getPhysicsEvent(electron, kp, km); 
		 TLorentzVector proton = physicsEvent.finalState; 
		 tupleWriter.setTLorentzVector("electron", electron); 
		 tupleWriter.setTLorentzVector("proton", proton); 
		 tupleWriter.setTLorentzVector("kaon_pos", kp); 
		 tupleWriter.setTLorentzVector("kaon_neg", km); 
		 
		 
		 
		 tupleWriter.setFloat("alpha_proton",   0.0);
		 
		 tupleWriter.writeEvent(); 
	       }
	       
	       
	       if ( (kmIndices.size() > 0)&&(protonIndices.size() > 0) ){
		 top = 1;
		 tupleWriter.setInt("topology", top); 
		 
		 // getting likelihood
		 DataEventCut_BetaPLikelihood *bpCutProton = (DataEventCut_BetaPLikelihood*) filter->GetSelector(2212) ->GetCut("Beta P Likelihood Cut 2212");
		 DataEventCut_BetaPLikelihood *bpCutKM     = (DataEventCut_BetaPLikelihood*) filter->GetSelector(-321)->GetCut("Beta P Likelihood Cut -321");
		 
		 // this needs to be called to get the confidence correct
		 bool garbageCan = bpCutProton->IsPassed(event, protonIndices[0]);
		 tupleWriter.setFloat("alpha_proton",   bpCutProton->GetConfidence());
		 
		 garbageCan      = bpCutKM->IsPassed(event, kmIndices[0]);
		 tupleWriter.setFloat("alpha_kaon_neg", bpCutKM->GetConfidence());
		 
		 TLorentzVector proton = event.GetTLorentzVector(protonIndices[0], 2212); 
		 TLorentzVector km = event.GetTLorentzVector(kmIndices[0], -321); 
		 
		 PhysicsEvent physicsEvent = builder.getPhysicsEvent(electron, proton, km); 
		 TLorentzVector kp = physicsEvent.finalState; 
		 tupleWriter.setTLorentzVector("electron", electron); 
		 tupleWriter.setTLorentzVector("proton", proton); 
		 tupleWriter.setTLorentzVector("kaon_pos", kp); 
		 tupleWriter.setTLorentzVector("kaon_neg", km); 
		 
		 tupleWriter.setFloat("alpha_kaon_pos", 0.0);
		 
		 
		 tupleWriter.writeEvent(); 
	       }
	       
	       if ( (kpIndices.size() > 0)&&(protonIndices.size() > 0) ){
		 top = 0;
		 tupleWriter.setInt("topology", top); 
		 
		 // getting likelihood
		 DataEventCut_BetaPLikelihood *bpCutProton = (DataEventCut_BetaPLikelihood*) filter->GetSelector(2212) ->GetCut("Beta P Likelihood Cut 2212");
		 DataEventCut_BetaPLikelihood *bpCutKP     = (DataEventCut_BetaPLikelihood*) filter->GetSelector(321)->GetCut("Beta P Likelihood Cut 321");
		 
		 // this needs to be called to get the confidence correct
		 bool garbageCan = bpCutProton->IsPassed(event, protonIndices[0]);
		 float protonConf = bpCutProton->GetConfidence();
		 tupleWriter.setFloat("alpha_proton", protonConf);
		 
		 garbageCan      = bpCutKP->IsPassed(event, kpIndices[0]);
		 float kaonConf = bpCutKP->GetConfidence(); 
		 tupleWriter.setFloat("alpha_kaon_pos", kaonConf);
		 
		 
		 if( (protonConf > fMinConfidence) && (kaonConf > fMinConfidence) ){
		   TLorentzVector kp = event.GetTLorentzVector(kpIndices[0], 321); 
		   TLorentzVector proton = event.GetTLorentzVector(protonIndices[0], 2212); 
		   
		   PhysicsEvent physicsEvent = builder.getPhysicsEvent(electron, kp, proton); 
		   
		   TLorentzVector km = physicsEvent.finalState; 
		   tupleWriter.setTLorentzVector("electron", electron); 
		   tupleWriter.setTLorentzVector("proton",   proton); 
		   tupleWriter.setTLorentzVector("kaon_pos", kp); 
		   tupleWriter.setTLorentzVector("kaon_neg", km); 
		   tupleWriter.setFloat("alpha_kaon_neg", 0.0);
		   tupleWriter.writeEvent(); 
		 }
	       }
	       
	       if ( (kpIndices.size() > 0)&&(kmIndices.size() > 0)&&(protonIndices.size() > 0) ){
		 top = 3;
		 tupleWriter.setInt("topology", top); 
		 
		 DataEventCut_BetaPLikelihood *bpCutProton = (DataEventCut_BetaPLikelihood*) filter->GetSelector(2212)->GetCut("Beta P Likelihood Cut 2212");
		 DataEventCut_BetaPLikelihood *bpCutKP     = (DataEventCut_BetaPLikelihood*) filter->GetSelector( 321)->GetCut("Beta P Likelihood Cut 321");
		 DataEventCut_BetaPLikelihood *bpCutKM     = (DataEventCut_BetaPLikelihood*) filter->GetSelector(-321)->GetCut("Beta P Likelihood Cut -321");
		 
		 // this needs to be called to get the confidence correct
		 bool garbageCan = bpCutProton->IsPassed(event, protonIndices[0]);
		 float protonConf = bpCutProton->GetConfidence();	    
		 tupleWriter.setFloat("alpha_proton",   protonConf);
		 
		 garbageCan      = bpCutKP->IsPassed(event, kpIndices[0]);
		 float kpConf = bpCutKP->GetConfidence(); 
		 tupleWriter.setFloat("alpha_kaon_pos", kpConf);
		 
		 garbageCan      = bpCutKM->IsPassed(event, kmIndices[0]);
		 float kmConf = bpCutKM->GetConfidence(); 
		 tupleWriter.setFloat("alpha_kaon_neg", kmConf);
		 
		 if ( (kmConf > fMinConfidence) && (kpConf > fMinConfidence) && (protonConf > fMinConfidence) ){
		   TLorentzVector kp     = event.GetTLorentzVector(kpIndices[0], 321); 
		   TLorentzVector km     = event.GetTLorentzVector(kmIndices[0], -321); 
		   TLorentzVector proton = event.GetTLorentzVector(protonIndices[0], 2212); 
		   
		   PhysicsEvent physicsEvent = builder.getPhysicsEvent(electron, kp, km, proton); 
		   
		   tupleWriter.setTLorentzVector("electron", electron); 
		   tupleWriter.setTLorentzVector("proton", proton); 
		   tupleWriter.setTLorentzVector("kaon_pos", kp); 
		   tupleWriter.setTLorentzVector("kaon_neg", km); 
		   tupleWriter.writeEvent(); 
		 }
	       }

	       if ( (kpIndices.size() > 0) && 
		    (kmIndices.size() > 0) &&
		    (protonIndices.size() > 0) && 
		    (ppIndices.size() > 0)){
		 top = 4;
		 tupleWriter.setInt("topology", top); 
		 
		 DataEventCut_BetaPLikelihood *bpCutProton = (DataEventCut_BetaPLikelihood*) filter->GetSelector(2212)->GetCut("Beta P Likelihood Cut 2212");
		 DataEventCut_BetaPLikelihood *bpCutKP     = (DataEventCut_BetaPLikelihood*) filter->GetSelector( 321)->GetCut("Beta P Likelihood Cut 321");
		 DataEventCut_BetaPLikelihood *bpCutPP     = (DataEventCut_BetaPLikelihood*) filter->GetSelector( 211)->GetCut("Beta P Likelihood Cut 211");
		 DataEventCut_BetaPLikelihood *bpCutKM     = (DataEventCut_BetaPLikelihood*) filter->GetSelector(-321)->GetCut("Beta P Likelihood Cut -321");
		 
		 // this needs to be called to get the confidence correct
		 bool garbageCan = bpCutProton->IsPassed(event, protonIndices[0]);
		 float protonConf = bpCutProton->GetConfidence();	    
		 tupleWriter.setFloat("alpha_proton",   protonConf);
		 
		 garbageCan      = bpCutKP->IsPassed(event, kpIndices[0]);
		 float kpConf = bpCutKP->GetConfidence(); 
		 tupleWriter.setFloat("alpha_kaon_pos", kpConf);

		 garbageCan      = bpCutPP->IsPassed(event, ppIndices[0]);
		 float ppConf = bpCutPP->GetConfidence(); 
		 tupleWriter.setFloat("alpha_pion_pos", ppConf);
		 
		 garbageCan      = bpCutKM->IsPassed(event, kmIndices[0]);
		 float kmConf = bpCutKM->GetConfidence(); 
		 tupleWriter.setFloat("alpha_kaon_neg", kmConf);
		 
		 if ( (kmConf > fMinConfidence) && (kpConf > fMinConfidence) && 
		      (protonConf > fMinConfidence) && (ppConf > fMinConfidence)){
		   TLorentzVector kp     = event.GetTLorentzVector(kpIndices[0], 321); 
		   TLorentzVector pp     = event.GetTLorentzVector(ppIndices[0], 211); 
		   TLorentzVector km     = event.GetTLorentzVector(kmIndices[0], -321); 
		   TLorentzVector proton = event.GetTLorentzVector(protonIndices[0], 2212); 
		   
		   PhysicsEvent physicsEvent = builder.getPhysicsEvent(electron, kp, km, proton); 

		   tupleWriter.setTLorentzVector("electron", electron); 
		   tupleWriter.setTLorentzVector("proton",   proton); 
		   tupleWriter.setTLorentzVector("kaon_pos", kp); 
		   tupleWriter.setTLorentzVector("pion_pos", pp); 
		   tupleWriter.setTLorentzVector("kaon_neg", km); 
		   tupleWriter.writeEvent(); 
		 }
	       }


	       if ( (kpIndices.size() > 0) && 
		    (kmIndices.size() > 0) &&
		    (protonIndices.size() > 0) && 
		    (pmIndices.size() > 0)){
		 top = 5;
		 tupleWriter.setInt("topology", top); 
		 
		 DataEventCut_BetaPLikelihood *bpCutProton = (DataEventCut_BetaPLikelihood*) filter->GetSelector(2212)->GetCut("Beta P Likelihood Cut 2212");
		 DataEventCut_BetaPLikelihood *bpCutKP     = (DataEventCut_BetaPLikelihood*) filter->GetSelector( 321)->GetCut("Beta P Likelihood Cut 321");
		 DataEventCut_BetaPLikelihood *bpCutPM     = (DataEventCut_BetaPLikelihood*) filter->GetSelector( -211)->GetCut("Beta P Likelihood Cut -211");
		 DataEventCut_BetaPLikelihood *bpCutKM     = (DataEventCut_BetaPLikelihood*) filter->GetSelector(-321)->GetCut("Beta P Likelihood Cut -321");
		 
		 // this needs to be called to get the confidence correct
		 bool garbageCan = bpCutProton->IsPassed(event, protonIndices[0]);
		 float protonConf = bpCutProton->GetConfidence();	    
		 tupleWriter.setFloat("alpha_proton",   protonConf);
		 
		 garbageCan      = bpCutKP->IsPassed(event, kpIndices[0]);
		 float kpConf = bpCutKP->GetConfidence(); 
		 tupleWriter.setFloat("alpha_kaon_pos", kpConf);

		 garbageCan      = bpCutPM->IsPassed(event, pmIndices[0]);
		 float pmConf = bpCutPM->GetConfidence(); 
		 tupleWriter.setFloat("alpha_pion_pos", pmConf);
		 
		 garbageCan      = bpCutKM->IsPassed(event, kmIndices[0]);
		 float kmConf = bpCutKM->GetConfidence(); 
		 tupleWriter.setFloat("alpha_kaon_neg", kmConf);
		 
		 if ( (kmConf > fMinConfidence) && (kpConf > fMinConfidence) && 
		      (protonConf > fMinConfidence) && (pmConf > fMinConfidence)){
		   TLorentzVector kp     = event.GetTLorentzVector(kpIndices[0],      321); 
		   TLorentzVector pm     = event.GetTLorentzVector(pmIndices[0],     -211); 
		   TLorentzVector km     = event.GetTLorentzVector(kmIndices[0],     -321); 
		   TLorentzVector proton = event.GetTLorentzVector(protonIndices[0], 2212); 
		   
		   tupleWriter.setTLorentzVector("electron", electron); 
		   tupleWriter.setTLorentzVector("proton",   proton); 
		   tupleWriter.setTLorentzVector("kaon_pos", kp); 
		   tupleWriter.setTLorentzVector("pion_neg", pm); 
		   tupleWriter.setTLorentzVector("kaon_neg", km); 
		   tupleWriter.writeEvent(); 
		 }
	       }
	  }	    
	}
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

  bool IsMisidentifiedPion(TLorentzVector & electron, int mesonIndex){

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
    
    TLorentzVector pion = event.GetTLorentzVector(mesonIndex, 211); 
    PhysicsEvent pionEvent = builder.getPhysicsEvent(electron,pion); 
    
    // reject and dont process event 
    if (sqrt(pionEvent.mm2) < 1.07){
      return true; 
    }
    
    return false; 
  }

  void Save(std::string outFile){
    TFile *out = new TFile(outFile.c_str(), "recreate");

    tupleWriter.save(out); 

    out->Close();
  }


protected:
  MomCorr_e1f             *momCorr; 
  PhysicsEventBuilder      builder; 
  ParticleFilter          *filter, *filterLoose; 
  Parameters              *params, *paramsLoose; 

  SimpleNTupleWriter      tupleWriter; 

  float fMinConfidence; 

};

int main(int argc, char *argv[]){

  if (argc > 1){
    // setup analysis
    Analysis analysis;

    // Setup Options
    h22Options opts;
    opts.args["PARS"].args = "/u/home/dmriser/Analysis_v2/lists/data.pars";
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
