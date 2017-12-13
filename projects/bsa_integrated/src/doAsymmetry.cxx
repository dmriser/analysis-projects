#include <iostream>
#include <vector>

#include "common/Config.h"
#include "common/ConfigLoader.h"
#include "common/ConfigLoader.cxx"
#include "common/PhiHistograms.h"
#include "common/PhiFits.h"
#include "common/PtAverager.h"
#include "common/Fitter.h"
#include "common/KinematicHistograms.h"

// temp 
#include "TH1.h"

int main(int argc, char *argv[]){

  if (argc < 2){
    std::cerr << "[doAsymmetry] Please pass in a list of configuration files. " << std::endl; 
    exit(0);
  }

  std::vector<Config> configs; 
  for (int iconf=1; iconf<argc; iconf++){
    std::string configFile(argv[iconf]);
    configs.push_back( ConfigLoader::getConfiguration(configFile) );
  }

  for(Config config : configs){

    PhiHistos *plus  = new PhiHistos(); 
    PhiHistos *minus = new PhiHistos(); 
    
    std::string file(Form("%s/%s/histos.root", config.analysisHome.c_str(), config.name.c_str()));
    plus ->Load(config.axes, file, "countsPlus");
    minus->Load(config.axes, file, "countsMinus");

    // here you would do some background subtraction or 
    // other general tampering with counts 
    
    PhiHistos *asym = new PhiHistos();
    asym->CreateAsymmetry(plus, minus);
    
    Fitter *fitter = new Fitter(); 
    std::pair<PhiFits, PhiHistos> fitPair = fitter->Fit(asym,
							"fit",
							config.fitter["function"],
							config.fitter["options"]);
    
    KinematicHistos *kin = new KinematicHistos(); 
    kin->Init(config.axes, "integratedAsymmetry"); 
    kin->BuildFromFits(&fitPair.first);

    KinematicHistos *kinPt = new KinematicHistos(); 
    kinPt->Init(config.axes, "integratedAsymmetryPt"); 
    kinPt->BuildFromFits(&fitPair.first);

    PtAverager *ptAverager = new PtAverager(config);
    ptAverager->Load(file); 
    ptAverager->CalculateAverage();
    ptAverager->ScaleKinematicHistograms(kinPt);

    TFile *outFile = new TFile(Form("%s/%s/fits.root",config.analysisHome.c_str(),config.name.c_str()),"recreate");
    asym->Save(outFile);
    fitPair.first.Save(outFile);
    fitPair.second.Save(outFile);
    kin->Save(outFile);
    kinPt->Save(outFile);
    outFile->Close();    
  }

  return 0; 
}
