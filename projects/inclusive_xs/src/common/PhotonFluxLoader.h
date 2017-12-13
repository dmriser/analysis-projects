#ifndef photon_flux_loader_h
#define photon_flux_loader_h

#include <iostream>
using std::cout; 
using std::endl; 
using std::string; 
using std::vector; 

#include "CommonTools.h"
#include "DIS1DHistograms.h"

class PhotonFluxLoader{
 public:
  PhotonFluxLoader();
  ~PhotonFluxLoader();

  DIS1DHistograms *photonFlux; 

  void provideBinningTemplate(DIS1DHistograms * templateHistograms);
  void loadPhotonFlux(double beamEnergy);
  void Save(string outputFilenameWithExtension, string saveOption); 
  DIS1DHistograms *getPhotonFlux(){ return photonFlux; } 
};
 
PhotonFluxLoader::PhotonFluxLoader(){
  photonFlux = new DIS1DHistograms(); 
}

PhotonFluxLoader::~PhotonFluxLoader(){
}

void PhotonFluxLoader::provideBinningTemplate(DIS1DHistograms * templateHistograms){
  photonFlux->CreateFromExisting(templateHistograms,"photonFlux","photonFlux");
}

void PhotonFluxLoader::loadPhotonFlux(double beamEnergy){

    for (int sector=0; sector<7; sector++){
      for (int slice=0; slice<photonFlux->wByQQ[sector].size(); slice++){
	for (int wBin=1; wBin<=photonFlux->numberOfWBins; wBin++){
	  double wMid  = photonFlux->wByQQ[sector][slice]->GetBinCenter(wBin);
	  double qqMid = photonFlux->qqMin + (0.5 + slice)*photonFlux->qqWidth; 
	  photonFlux->wByQQ[sector][slice]->SetBinContent(wBin,calculatePhotonFlux(beamEnergy,wMid,qqMid)); 
	  photonFlux->wByQQ[sector][slice]->SetBinError(wBin,0.0); 
      }
    }
  }
}

void PhotonFluxLoader::Save(string outputFilenameWithExtension, string saveOption){
    photonFlux->Save(outputFilenameWithExtension,saveOption);
}

#endif
