#ifndef model_cs_loader_h
#define model_cs_loader_h

#include <iostream>
using std::cout; 
using std::endl; 
using std::string; 
using std::vector; 

#include "CommonTools.h"
#include "DIS1DHistograms.h"
//#include "BostedInclusiveWrapper.h"
#include "KeppelInclusiveWrapper.h"

class ModelCrossSectionLoader{
 public:
  ModelCrossSectionLoader();
  ~ModelCrossSectionLoader();

//  F1F209Wrapper *bostedModel; 
//  DIS1DHistograms *bostedCrossSection; 
//  DIS1DHistograms *bostedCrossSectionAverage; 

  KeppelInclusiveWrapper *keppelModel; 
  DIS1DHistograms *crossSection; 
  DIS1DHistograms *crossSectionAverage; 

  void provideBinningTemplate(DIS1DHistograms * templateHistograms);
  void loadCrossSection(double A, double Z, double beamEnergy);
  void Save(string outputFilenameWithExtension, string saveOption); 
  DIS1DHistograms *getCrossSection(){ return crossSection; } 
  DIS1DHistograms *getCrossSectionAverage(){ return crossSectionAverage; }
 
};

ModelCrossSectionLoader::ModelCrossSectionLoader(){
//  bostedModel = new F1F209Wrapper();
  keppelModel = new KeppelInclusiveWrapper();
  crossSection = new DIS1DHistograms(); 
  crossSectionAverage = new DIS1DHistograms(); 
}

ModelCrossSectionLoader::~ModelCrossSectionLoader(){

}

void ModelCrossSectionLoader::provideBinningTemplate(DIS1DHistograms * templateHistograms){
  crossSection->CreateFromExisting(templateHistograms,"modelCrossSection","crossSection");
  crossSectionAverage->CreateFromExisting(templateHistograms,"modelCrossSectionAverage","crossSectionAverage");
}

void ModelCrossSectionLoader::loadCrossSection(double A, double Z, double beamEnergy){

  double CS_MIN_VALUE = 1e-10; 
  double CS_MAX_VALUE = 1e10;
  
  for (int sector=0; sector<7; sector++){
    for (int wBin=1; wBin<=crossSection->numberOfWBins; wBin++){
      double wLow = crossSection->allwByQQ[sector]->GetBinLowEdge(wBin);
      double wUp = wLow + crossSection->allwByQQ[sector]->GetBinWidth(wBin);
      double wMid = crossSection->allwByQQ[sector]->GetBinCenter(wBin);
      
      double qqMid = crossSection->qqWidth/2 + crossSection->qqMin; 
      
      double csLow = keppelModel->GetXS(beamEnergy, qqMid, wLow);// * calculatePhotonFlux(beamEnergy, wLow, qqMid);
      double csUp = keppelModel->GetXS(beamEnergy, qqMid, wUp);// * calculatePhotonFlux(beamEnergy, wUp, qqMid);
      double csMid = keppelModel->GetXS(beamEnergy, qqMid, wMid);// * calculatePhotonFlux(beamEnergy, wMid, qqMid);
      double csAverage = (csUp+csLow)/2; 
      
	if (csMid > CS_MIN_VALUE && csMid < CS_MAX_VALUE) {
	  crossSection->allwByQQ[sector]->SetBinContent(wBin,csMid);
	  crossSection->allwByQQ[sector]->SetBinError(wBin,0.0);
	}
	if (csAverage > CS_MIN_VALUE && csAverage < CS_MAX_VALUE) {
	  crossSectionAverage->allwByQQ[sector]->SetBinContent(wBin,csAverage);
	  crossSectionAverage->allwByQQ[sector]->SetBinError(wBin,0.0);
	}      

      }
 
    for (int slice=0; slice<crossSection->wByQQ[sector].size(); slice++){
      for (int wBin=1; wBin<=crossSection->numberOfWBins; wBin++){
	double wLow = crossSection->wByQQ[sector][slice]->GetBinLowEdge(wBin);
	double wUp = wLow + crossSection->wByQQ[sector][slice]->GetBinWidth(wBin);
	double wMid = crossSection->wByQQ[sector][slice]->GetBinCenter(wBin);

	double qqLow = crossSection->qqMin + slice*crossSection->qqWidth; 
	double qqUp = crossSection->qqMin + (1+slice)*crossSection->qqWidth;
	double qqMid = crossSection->qqWidth/2 + qqLow; 
	
	double csLow = keppelModel->GetXS(beamEnergy, qqLow, wLow);// * calculatePhotonFlux(beamEnergy, wLow, qqMid);
	double csUp = keppelModel->GetXS(beamEnergy, qqUp, wUp);// * calculatePhotonFlux(beamEnergy, wUp, qqMid);
	double csMid = keppelModel->GetXS(beamEnergy, qqMid, wMid);// * calculatePhotonFlux(beamEnergy, wMid, qqMid);
	double csAverage = (csUp+csLow)/2; 
  
	if (csMid > CS_MIN_VALUE && csMid < CS_MAX_VALUE) {
	  crossSection->wByQQ[sector][slice]->SetBinContent(wBin,csMid);
	  crossSection->wByQQ[sector][slice]->SetBinError(wBin,0.0);
	}
	if (csAverage > CS_MIN_VALUE && csAverage < CS_MAX_VALUE) {
	  crossSectionAverage->wByQQ[sector][slice]->SetBinContent(wBin,csAverage);
	  crossSectionAverage->wByQQ[sector][slice]->SetBinError(wBin,0.0);
	}      

      }
    }
  }
}

void ModelCrossSectionLoader::Save(string outputFilenameWithExtension, string saveOption){

  // Don't do recreate twice it kills the first file.
  if (saveOption == "recreate" || saveOption == "RECREATE"){
    crossSection->Save(outputFilenameWithExtension,saveOption);
    crossSectionAverage->Save(outputFilenameWithExtension,"update");
  }
  
  else {
    crossSection->Save(outputFilenameWithExtension,saveOption);
    crossSectionAverage->Save(outputFilenameWithExtension,saveOption);
  }


}

#endif
