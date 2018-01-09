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
#include "ParameterSet.h"
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

class Analysis {

public:  
  Analysis() {
    sliceFitter = new SliceFitter(40, 0.5, 3.5); 
  }

  ~Analysis(){
    if(fInputFile){
      if(fInputFile->IsOpen()){
	fInputFile->Close(); 
      }
    }
  }

  void Load(std::string filename){
    fInputFile = TFile::Open(filename.c_str());

    mesonHistos[211][cutTypes::physicsEnhanced]  = new MesonHistograms("211_physics_enhanced",  211);  
    mesonHistos[321][cutTypes::physicsEnhanced]  = new MesonHistograms("321_physics_enhanced",  321);  
    mesonHistos[-211][cutTypes::physicsEnhanced]  = new MesonHistograms("-211_physics_enhanced",  -211);  
    mesonHistos[-321][cutTypes::physicsEnhanced]  = new MesonHistograms("-321_physics_enhanced",  -321);  
    mesonHistos[2212][cutTypes::physicsEnhanced] = new MesonHistograms("2212_physics_enhanced", 2212);  

    mesonHistos[211][cutTypes::physicsEnhanced] ->Load(filename); 
    mesonHistos[321][cutTypes::physicsEnhanced] ->Load(filename); 
    mesonHistos[-211][cutTypes::physicsEnhanced] ->Load(filename); 
    mesonHistos[-321][cutTypes::physicsEnhanced] ->Load(filename); 
    mesonHistos[2212][cutTypes::physicsEnhanced]->Load(filename); 
  }

  void Calibrate(int pid){
    
    sliceFitter->SetExpectedMean("0");

    if (pid == 2212){
      sliceFitter->SetUpperTolerance(1.15);
      sliceFitter->SetLowerTolerance(0.85); 
      sliceFitter->SetNumberSlices(40); 
      sliceFitter->SetXMin(0.2);
      sliceFitter->SetXMax(3.25);
    } else {
      sliceFitter->SetUpperTolerance(1.15);
      sliceFitter->SetLowerTolerance(0.85); 
      sliceFitter->SetNumberSlices(40); 
      sliceFitter->SetXMin(0.2);
      sliceFitter->SetXMax(3.25);
    }
    
    for(int sector=1; sector<7; sector++){
      sliceFitter->Fit(mesonHistos[pid][cutTypes::physicsEnhanced]->h2_p_dbeta[sector], Form("p_dbeta_%d_sector%d", pid, sector)); 

      fits[pid].push_back(sliceFitter->GetFits());
      slices[pid].push_back(sliceFitter->GetSlices()); 
      mesonMus[pid].push_back(sliceFitter->GetFitToMu("pol2", Form("p_dbeta_mu_%d_sector%d", pid, sector))); 
      mesonSigmas[pid].push_back(sliceFitter->GetFitToSigma("pol2", Form("p_dbeta_sigma_%d_sector%d", pid, sector))); 
    }
  }


    void Save(std::string outFile){
      TFile *out = new TFile(outFile.c_str(), "recreate");
      
      mesonHistos[211][cutTypes::physicsEnhanced] ->Save(out);
      mesonHistos[321][cutTypes::physicsEnhanced] ->Save(out);
      mesonHistos[-211][cutTypes::physicsEnhanced] ->Save(out);
      mesonHistos[-321][cutTypes::physicsEnhanced] ->Save(out);
      mesonHistos[2212][cutTypes::physicsEnhanced] ->Save(out);

      for(int i=0; i<mesonMus[211].size(); i++){
	mesonMus[211][i].Write();
	mesonSigmas[211][i].Write();
      }
      for(int i=0; i<mesonMus[-211].size(); i++){
	mesonMus[-211][i].Write();
	mesonSigmas[-211][i].Write();
      }
      for(int i=0; i<mesonMus[321].size(); i++){
	mesonMus[321][i].Write();
	mesonSigmas[321][i].Write();
      }
      for(int i=0; i<mesonMus[-321].size(); i++){
	mesonMus[-321][i].Write();
	mesonSigmas[-321][i].Write();
      }
      for(int i=0; i<mesonMus[2212].size(); i++){
	mesonMus[2212][i].Write();
	mesonSigmas[2212][i].Write();
      }

      for(std::vector<TH1D> s : slices[211]){
	for(TH1D h : s){
	  h.Write(); 
	}
      }
      for(std::vector<TF1> s : fits[211]){
	for(TF1 f : s){
	  f.Write(); 
	}
      }
      for(std::vector<TH1D> s : slices[321]){
	for(TH1D h : s){
	  h.Write(); 
	}
      }
      for(std::vector<TF1> s : fits[321]){
	for(TF1 f : s){
	  f.Write(); 
	}
      }

      for(std::vector<TH1D> s : slices[-211]){
	for(TH1D h : s){
	  h.Write(); 
	}
      }
      for(std::vector<TF1> s : fits[-211]){
	for(TF1 f : s){
	  f.Write(); 
	}
      }
      for(std::vector<TH1D> s : slices[-321]){
	for(TH1D h : s){
	  h.Write(); 
	}
      }
      for(std::vector<TF1> s : fits[-321]){
	for(TF1 f : s){
	  f.Write(); 
	}
      }
      for(std::vector<TH1D> s : slices[2212]){
	for(TH1D h : s){
	  h.Write(); 
	}
      }
      for(std::vector<TF1> s : fits[2212]){
	for(TF1 f : s){
	  f.Write(); 
	}
      }

      out->Close();

      fParameterSets[211]["PIP_DBETA_MU_A"] = ParameterSet(); 
      fParameterSets[211]["PIP_DBETA_MU_A"].setName("PIP_DBETA_MU_A"); 
      for(TF1 f : mesonMus[211]){ 
	fParameterSets[211]["PIP_DBETA_MU_A"].addValueAndError(f.GetParameter(2), 0.0); 
      }

      fParameterSets[211]["PIP_DBETA_MU_B"] = ParameterSet(); 
      fParameterSets[211]["PIP_DBETA_MU_B"].setName("PIP_DBETA_MU_B"); 
      for(TF1 f : mesonMus[211]){ 
	fParameterSets[211]["PIP_DBETA_MU_B"].addValueAndError(f.GetParameter(1), 0.0); 
      }

      fParameterSets[211]["PIP_DBETA_MU_C"] = ParameterSet(); 
      fParameterSets[211]["PIP_DBETA_MU_C"].setName("PIP_DBETA_MU_C"); 
      for(TF1 f : mesonMus[211]){ 
	fParameterSets[211]["PIP_DBETA_MU_C"].addValueAndError(f.GetParameter(0), 0.0); 
      }

      // --------------------------------------------------------------------------------
      fParameterSets[321]["KP_DBETA_MU_A"] = ParameterSet(); 
      fParameterSets[321]["KP_DBETA_MU_A"].setName("KP_DBETA_MU_A"); 
      for(TF1 f : mesonMus[321]){ 
	fParameterSets[321]["KP_DBETA_MU_A"].addValueAndError(f.GetParameter(2), 0.0); 
      }

      fParameterSets[321]["KP_DBETA_MU_B"] = ParameterSet(); 
      fParameterSets[321]["KP_DBETA_MU_B"].setName("KP_DBETA_MU_B"); 
      for(TF1 f : mesonMus[321]){ 
	fParameterSets[321]["KP_DBETA_MU_B"].addValueAndError(f.GetParameter(1), 0.0); 
      }

      fParameterSets[321]["KP_DBETA_MU_C"] = ParameterSet(); 
      fParameterSets[321]["KP_DBETA_MU_C"].setName("KP_DBETA_MU_C"); 
      for(TF1 f : mesonMus[321]){ 
	fParameterSets[321]["KP_DBETA_MU_C"].addValueAndError(f.GetParameter(0), 0.0); 
      }


      fParameterSets[-211]["PIM_DBETA_MU_A"] = ParameterSet(); 
      fParameterSets[-211]["PIM_DBETA_MU_A"].setName("PIM_DBETA_MU_A"); 
      for(TF1 f : mesonMus[-211]){ 
	fParameterSets[-211]["PIM_DBETA_MU_A"].addValueAndError(f.GetParameter(2), 0.0); 
      }

      fParameterSets[-211]["PIM_DBETA_MU_B"] = ParameterSet(); 
      fParameterSets[-211]["PIM_DBETA_MU_B"].setName("PIM_DBETA_MU_B"); 
      for(TF1 f : mesonMus[-211]){ 
	fParameterSets[211]["PIM_DBETA_MU_B"].addValueAndError(f.GetParameter(1), 0.0); 
      }

      fParameterSets[-211]["PIM_DBETA_MU_C"] = ParameterSet(); 
      fParameterSets[-211]["PIM_DBETA_MU_C"].setName("PIM_DBETA_MU_C"); 
      for(TF1 f : mesonMus[-211]){ 
	fParameterSets[-211]["PIM_DBETA_MU_C"].addValueAndError(f.GetParameter(0), 0.0); 
      }

      // --------------------------------------------------------------------------------
      fParameterSets[-321]["KM_DBETA_MU_A"] = ParameterSet(); 
      fParameterSets[-321]["KM_DBETA_MU_A"].setName("KM_DBETA_MU_A"); 
      for(TF1 f : mesonMus[-321]){ 
	fParameterSets[321]["KM_DBETA_MU_A"].addValueAndError(f.GetParameter(2), 0.0); 
      }

      fParameterSets[-321]["KM_DBETA_MU_B"] = ParameterSet(); 
      fParameterSets[-321]["KM_DBETA_MU_B"].setName("KM_DBETA_MU_B"); 
      for(TF1 f : mesonMus[-321]){ 
	fParameterSets[321]["KM_DBETA_MU_B"].addValueAndError(f.GetParameter(1), 0.0); 
      }

      fParameterSets[-321]["KM_DBETA_MU_C"] = ParameterSet(); 
      fParameterSets[-321]["KM_DBETA_MU_C"].setName("KM_DBETA_MU_C"); 
      for(TF1 f : mesonMus[-321]){ 
	fParameterSets[-321]["KM_DBETA_MU_C"].addValueAndError(f.GetParameter(0), 0.0); 
      }

      // --------------------------------------------------------------------------------
      fParameterSets[2212]["PROT_DBETA_MU_A"] = ParameterSet(); 
      fParameterSets[2212]["PROT_DBETA_MU_A"].setName("PROT_DBETA_MU_A"); 
      for(TF1 f : mesonMus[2212]){ 
	fParameterSets[2212]["PROT_DBETA_MU_A"].addValueAndError(f.GetParameter(2), 0.0); 
      }

      fParameterSets[2212]["PROT_DBETA_MU_B"] = ParameterSet(); 
      fParameterSets[2212]["PROT_DBETA_MU_B"].setName("PROT_DBETA_MU_B"); 
      for(TF1 f : mesonMus[2212]){ 
	fParameterSets[2212]["PROT_DBETA_MU_B"].addValueAndError(f.GetParameter(1), 0.0); 
      }

      fParameterSets[2212]["PROT_DBETA_MU_C"] = ParameterSet(); 
      fParameterSets[2212]["PROT_DBETA_MU_C"].setName("PROT_DBETA_MU_C"); 
      for(TF1 f : mesonMus[2212]){ 
	fParameterSets[2212]["PROT_DBETA_MU_C"].addValueAndError(f.GetParameter(0), 0.0); 
      }

      
      // --------------------------------------------------------------------------------
      //      now for the sigma 
      // --------------------------------------------------------------------------------

      fParameterSets[211]["PIP_DBETA_SIGMA_A"] = ParameterSet(); 
      fParameterSets[211]["PIP_DBETA_SIGMA_A"].setName("PIP_DBETA_SIGMA_A"); 
      for(TF1 f : mesonSigmas[211]){ 
	fParameterSets[211]["PIP_DBETA_SIGMA_A"].addValueAndError(f.GetParameter(2), 0.0); 
      }

      fParameterSets[211]["PIP_DBETA_SIGMA_B"] = ParameterSet(); 
      fParameterSets[211]["PIP_DBETA_SIGMA_B"].setName("PIP_DBETA_SIGMA_B"); 
      for(TF1 f : mesonSigmas[211]){ 
	fParameterSets[211]["PIP_DBETA_SIGMA_B"].addValueAndError(f.GetParameter(1), 0.0); 
      }

      fParameterSets[211]["PIP_DBETA_SIGMA_C"] = ParameterSet(); 
      fParameterSets[211]["PIP_DBETA_SIGMA_C"].setName("PIP_DBETA_SIGMA_C"); 
      for(TF1 f : mesonSigmas[211]){ 
	fParameterSets[211]["PIP_DBETA_SIGMA_C"].addValueAndError(f.GetParameter(0), 0.0); 
      }

      // --------------------------------------------------------------------------------
      fParameterSets[321]["KP_DBETA_SIGMA_A"] = ParameterSet(); 
      fParameterSets[321]["KP_DBETA_SIGMA_A"].setName("KP_DBETA_SIGMA_A"); 
      for(TF1 f : mesonSigmas[321]){ 
	fParameterSets[321]["KP_DBETA_SIGMA_A"].addValueAndError(f.GetParameter(2), 0.0); 
      }

      fParameterSets[321]["KP_DBETA_SIGMA_B"] = ParameterSet(); 
      fParameterSets[321]["KP_DBETA_SIGMA_B"].setName("KP_DBETA_SIGMA_B"); 
      for(TF1 f : mesonSigmas[321]){ 
	fParameterSets[321]["KP_DBETA_SIGMA_B"].addValueAndError(f.GetParameter(1), 0.0); 
      }

      fParameterSets[321]["KP_DBETA_SIGMA_C"] = ParameterSet(); 
      fParameterSets[321]["KP_DBETA_SIGMA_C"].setName("KP_DBETA_SIGMA_C"); 
      for(TF1 f : mesonSigmas[321]){ 
	fParameterSets[321]["KP_DBETA_SIGMA_C"].addValueAndError(f.GetParameter(0), 0.0); 
      }

      // --------------------------------------------------------------------------------
      fParameterSets[2212]["PROT_DBETA_SIGMA_A"] = ParameterSet(); 
      fParameterSets[2212]["PROT_DBETA_SIGMA_A"].setName("PROT_DBETA_SIGMA_A"); 
      for(TF1 f : mesonSigmas[2212]){ 
	fParameterSets[2212]["PROT_DBETA_SIGMA_A"].addValueAndError(f.GetParameter(2), 0.0); 
      }

      fParameterSets[2212]["PROT_DBETA_SIGMA_B"] = ParameterSet(); 
      fParameterSets[2212]["PROT_DBETA_SIGMA_B"].setName("PROT_DBETA_SIGMA_B"); 
      for(TF1 f : mesonSigmas[2212]){ 
	fParameterSets[2212]["PROT_DBETA_SIGMA_B"].addValueAndError(f.GetParameter(1), 0.0); 
      }

      fParameterSets[2212]["PROT_DBETA_SIGMA_C"] = ParameterSet(); 
      fParameterSets[2212]["PROT_DBETA_SIGMA_C"].setName("PROT_DBETA_SIGMA_C"); 
      for(TF1 f : mesonSigmas[2212]){ 
	fParameterSets[2212]["PROT_DBETA_SIGMA_C"].addValueAndError(f.GetParameter(0), 0.0); 
      }


      // write parameters 
      for(std::pair<int, std::map<std::string, ParameterSet> > part : fParameterSets){
	for(std::pair<std::string, ParameterSet> par : part.second){
	  fParameters.addParameterSet(par.second); 
	}
      }

      fParameters.saveParameters("resolution_parameters.dat"); 

    }


protected:
  
  TFile                   *fInputFile; 
  SliceFitter             *sliceFitter; 

  Parameters               fParameters; 
  
  std::map<int, std::map<int, MesonHistograms*> >    mesonHistos; 
  std::map<int, std::vector<TF1> >                   mesonMus; 
  std::map<int, std::vector<TF1> >                   mesonSigmas; 
  std::map<int, std::vector< std::vector<TH1D> > >   slices; 
  std::map<int, std::vector< std::vector<TF1> > >    fits; 
  std::map<int, std::map<std::string, ParameterSet>> fParameterSets; 
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
    opts.args["INPUT"].args = "UNSET";
    opts.args["INPUT"].type = 1;
    opts.args["INPUT"].name = "input root file";
    opts.set(argc,argv);

    if(opts.args["INPUT"].args != "UNSET"){
      analysis.Load(opts.args["INPUT"].args); 
      
      // run analysis loop
      analysis.Calibrate( 211  ); 
      analysis.Calibrate( 321  ); 
      analysis.Calibrate(-211  ); 
      analysis.Calibrate(-321  ); 
      analysis.Calibrate( 2212 ); 


      analysis.Save(opts.args["OUT"].args); 
    }
  }


  return 0; 
}
