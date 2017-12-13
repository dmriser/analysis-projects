/* ########################################################################
   #                                                                      #
   #               David Riser, University of Connecticut                 #
   #                         October 6, 2016                              #
   #                                                                      #
   ######################################################################## */
#include <cmath>
#include <iostream>
#include <fstream>
using namespace std;

#include "CommonTools.h"
#include "h22Option.h"

#include "common/BaseDISHistograms.h"
#include "common/DIS1DHistograms.h"
#include "common/ModelCrossSectionLoader.h"
#include "common/PhotonFluxLoader.h"
#include "common/FaradayCupLoader.h"

int PrintUsage();
void configureCommandLineOptions(h22Options * theseOpts); 

int main(int argc, char * argv[]){

  // ------------- Physics Options -------------
  int numberOfXBins = 35;     //     W
  int numberOfYBins = 10;     //    Q^2

  bool phiIsRestricted = false; 
  double relPhiRange   = 2*(12.0 - 4.0); 

  double normalizationScale = cm_to_outhouse*(hydrogen_molar_weight*electron_c*1e6)/(5.00*avogadro*hydrogen_density);
  // -------------------------------------------

  // Check for improper usage of this code. 
  if (argc < 3){
    return PrintUsage();
  }

  // Collect command line options, configure ours and compare.
  h22Options * options = new h22Options(); 
  configureCommandLineOptions(options); 
  options->set(argc, argv);

  string inputFilename      = options->args["INPUT"].args; 
  string elastSubFile       = options->args["ELASTIC_SUB_FILE"].args; 
  string outputFilename     = options->args["OUT"].args; 
  string binCenteringStatus = options->args["BINCORR"].args; 
  string radCorrStatus      = options->args["RADCORR"].args; 
  string radCorrFile        = options->args["RADCORR_FILE"].args; 

  if (inputFilename != "UNSET"){
    FaradayCupQuickLoader fcup; 
    normalizationScale /= fcup.openFileGetCharge(inputFilename); 
    cout << "[doCrossSection] Normalization Scale =" << normalizationScale << endl;

    BaseDISHistograms * dataEvents2D = new BaseDISHistograms();
    dataEvents2D->Load(inputFilename.c_str(),"dataEvents");
    
    int xRebinFactor = floor(dataEvents2D->numberOfXBins/numberOfXBins); 
    int yRebinFactor = floor(dataEvents2D->numberOfQQBins/numberOfYBins); 
    dataEvents2D->Rebin2D(xRebinFactor, yRebinFactor); 

    BaseDISHistograms *recEventsRad2D = new BaseDISHistograms();
    recEventsRad2D->Load(inputFilename.c_str(),"recEventsRad");
    recEventsRad2D->Rebin2D(xRebinFactor, yRebinFactor); 

    BaseDISHistograms *genEventsRad2D = new BaseDISHistograms();
    genEventsRad2D->Load(inputFilename.c_str(),"genEventsRad");
    genEventsRad2D->Rebin2D(xRebinFactor, yRebinFactor); 

    BaseDISHistograms *recAndGenEventsRad2D = new BaseDISHistograms();
    recAndGenEventsRad2D->Load(inputFilename.c_str(),"recAndGenEventsRad");

    // Create output file 
    DIS1DHistograms *dataEvents = new DIS1DHistograms();
    dataEvents->Create(dataEvents2D);
    dataEvents->SetErrors();
    dataEvents->Save(outputFilename.c_str(),"recreate");

    PhotonFluxLoader photonFluxLoader;
    photonFluxLoader.provideBinningTemplate(dataEvents); 
    photonFluxLoader.loadPhotonFlux(5.498); 
    DIS1DHistograms *photonFlux = photonFluxLoader.getPhotonFlux();
    photonFlux->Save(outputFilename.c_str(),"update"); 

    if (elastSubFile != "UNSET"){
      BaseDISHistograms *recElasticEvents = new BaseDISHistograms();
      recElasticEvents->Load(elastSubFile.c_str(),"recEventsElastic");
      recElasticEvents->Rebin2D(xRebinFactor, yRebinFactor); 

      BaseDISHistograms *recInelasticEvents = new BaseDISHistograms();
      recInelasticEvents->Load(elastSubFile.c_str(),"recEventsInelastic");
      recInelasticEvents->Rebin2D(xRebinFactor, yRebinFactor);  

      DIS1DHistograms *elasticEvents = new DIS1DHistograms();
      elasticEvents->Create(recElasticEvents);
      elasticEvents->SetErrors();
      elasticEvents->Save(outputFilename.c_str(),"update");
      
      DIS1DHistograms *inelasticEvents = new DIS1DHistograms();
      inelasticEvents->Create(recInelasticEvents);
      inelasticEvents->SetErrors();
      inelasticEvents->Save(outputFilename.c_str(),"update");

      DIS1DHistograms *elasticAndInelasticEvents = new DIS1DHistograms();
      elasticAndInelasticEvents->CreateFromExisting(elasticEvents,"elasticAndInelasticEvents","elasticAndInelasticEvents");
      elasticAndInelasticEvents->Add(inelasticEvents);
      elasticAndInelasticEvents->Save(outputFilename.c_str(),"update");

      DIS1DHistograms *inelasticFraction = new DIS1DHistograms(); 
      inelasticFraction->CreateFromExisting(inelasticEvents,"inelasticFraction","inelasticFraction");
      inelasticFraction->Divide(elasticAndInelasticEvents);
      inelasticFraction->Save(outputFilename.c_str(),"update");

      // Apply the correction to data events 
      dataEvents->MultiplyByZero( inelasticFraction );
    }

    DIS1DHistograms *recEventsRad = new DIS1DHistograms();
    recEventsRad->Create(recEventsRad2D);
    recEventsRad->SetErrors();
    recEventsRad->Save(outputFilename.c_str(),"update");

    DIS1DHistograms *recEventsRadNorm = new DIS1DHistograms();
    recEventsRadNorm->Create(recEventsRad2D);
    recEventsRadNorm->SetErrors();
    recEventsRadNorm->SetNumberOfEventsSame(dataEvents);

    DIS1DHistograms *dataSimRatio = new DIS1DHistograms();
    dataSimRatio->CreateByDivision(dataEvents, recEventsRadNorm,"dataSimRatio", "dataSimRatio"); 
    dataSimRatio->Save(outputFilename.c_str(),"update"); 

    DIS1DHistograms * genEventsRad = new DIS1DHistograms();
    genEventsRad->Create(genEventsRad2D);
    genEventsRad->SetErrors();
    genEventsRad->Save(outputFilename.c_str(),"update");

    DIS1DHistograms * acceptance = new DIS1DHistograms();
    acceptance->CreateByDivision(recEventsRad,genEventsRad,"acceptance","Acceptance");
    acceptance->SetBinsOutsideRangeToValue(0.1, 1.01, 1.0);
    acceptance->Save(outputFilename.c_str(),"update");

    DIS1DHistograms * crossSection = new DIS1DHistograms();
    crossSection->CreateByDivision(dataEvents,acceptance,"crossSection","Cross Section W/ Acceptance");
    crossSection->Scale(normalizationScale);
    crossSection->ScaleByBinWidth();
    crossSection->ScaleByNumberSectors();
    crossSection->ScaleAllByNumberBins();
    
    if(phiIsRestricted){ crossSection->Scale(60.0/relPhiRange); }

    crossSection->Save(outputFilename.c_str(),"update");

    ModelCrossSectionLoader *modelLoader = new ModelCrossSectionLoader();
    modelLoader->provideBinningTemplate(dataEvents);
    modelLoader->loadCrossSection(1,1,5.498);
    modelLoader->Save(outputFilename.c_str(),"update");

    DIS1DHistograms * modelCrossSection = modelLoader->getCrossSection();
    DIS1DHistograms * modelCrossSectionAverage = modelLoader->getCrossSectionAverage();
  
    DIS1DHistograms * crossSectionRatio = new DIS1DHistograms();
    crossSectionRatio->CreateByDivision(crossSection,modelCrossSection,"crossSectionRatio","Cross Section Ratio W/ Acceptance");
    crossSectionRatio->Save(outputFilename.c_str(),"update");
    

    if (binCenteringStatus == "true" && radCorrStatus == "false"){
      DIS1DHistograms * binCenterCorrection = new DIS1DHistograms();
      binCenterCorrection->CreateByDivision(modelCrossSection, modelCrossSectionAverage, "binCenterCorrection", "Bin Centering Correction");
      binCenterCorrection->Save(outputFilename.c_str(),"update");

      DIS1DHistograms * binCorrCS = new DIS1DHistograms();
      binCorrCS->CreateByDivision(crossSection,binCenterCorrection,"binCorrectedCrossSection","Cross Section W/ Bin Centering Corr.");
      binCorrCS->Save(outputFilename.c_str(),"update");
      
      DIS1DHistograms * binCorrCrossSectionRatio = new DIS1DHistograms();
      binCorrCrossSectionRatio->CreateByDivision(binCorrCS,modelCrossSection,"binCorrectedCrossSectionRatio","Cross Section Ratio W/ Bin Centering Correction");
      binCorrCrossSectionRatio->Save(outputFilename.c_str(),"update");
    }
 
    if (radCorrFile != "UNSET" && binCenteringStatus == "false"){
      BaseDISHistograms *radEvents2D = new BaseDISHistograms();
      radEvents2D->Load(radCorrFile.c_str(),"radEvents");
      radEvents2D->Rebin2D(xRebinFactor, yRebinFactor);

      BaseDISHistograms *noRadEvents2D = new BaseDISHistograms();
      noRadEvents2D->Load(radCorrFile.c_str(),"noRadEvents");
      noRadEvents2D->Rebin2D(xRebinFactor, yRebinFactor);

      DIS1DHistograms *rad = new DIS1DHistograms();
      rad->Create(radEvents2D);
      rad->SetErrors();

      DIS1DHistograms *noRad = new DIS1DHistograms();
      noRad->Create(noRadEvents2D);
      noRad->SetErrors();

      DIS1DHistograms *radCorr = new DIS1DHistograms();
      radCorr->CreateByDivision(rad, noRad,"radCorr","Radiative Correction");
      radCorr->Save(outputFilename.c_str(),"update");

      DIS1DHistograms *radCorrCrossSection = new DIS1DHistograms();
      radCorrCrossSection->CreateFromExisting(crossSection,"radCorrCrossSection","Cross Section w/ Rad Corr."); 
      radCorrCrossSection->DivideByZero(radCorr); // This just uses all -> [0]
      radCorrCrossSection->Save(outputFilename.c_str(),"update");

      DIS1DHistograms *radCorrCrossSectionRatio = new DIS1DHistograms(); 
      radCorrCrossSectionRatio->CreateByDivision(radCorrCrossSection, modelCrossSection, "radCorrCrossSectionRatio", "Cross Section Ratio w/ Rad. Correction");
      radCorrCrossSectionRatio->Save(outputFilename.c_str(), "update");
    }
    
    if (binCenteringStatus == "true" && radCorrFile != "UNSET") {
      DIS1DHistograms * binCenterCorrection = new DIS1DHistograms();
      binCenterCorrection->CreateByDivision(modelCrossSection, modelCrossSectionAverage, "binCenterCorrection", "Bin Centering Correction");
      binCenterCorrection->Save(outputFilename.c_str(),"update");

      DIS1DHistograms * binCorrCS = new DIS1DHistograms();
      binCorrCS->CreateByDivision(crossSection,binCenterCorrection,"binCorrectedCrossSection","Cross Section W/ Bin Centering Corr.");
      binCorrCS->Save(outputFilename.c_str(),"update");
      
      DIS1DHistograms * binCorrCrossSectionRatio = new DIS1DHistograms();
      binCorrCrossSectionRatio->CreateByDivision(binCorrCS,modelCrossSection,"binCorrectedCrossSectionRatio","Cross Section Ratio W/ Bin Centering Correction");
      binCorrCrossSectionRatio->Save(outputFilename.c_str(),"update");

      BaseDISHistograms *radEvents2D = new BaseDISHistograms();
      radEvents2D->Load(radCorrFile.c_str(),"radEvents");
      radEvents2D->Rebin2D(xRebinFactor, yRebinFactor);

      BaseDISHistograms *noRadEvents2D = new BaseDISHistograms();
      noRadEvents2D->Load(radCorrFile.c_str(),"noRadEvents");
      noRadEvents2D->Rebin2D(xRebinFactor, yRebinFactor);

      DIS1DHistograms *rad = new DIS1DHistograms();
      rad->Create(radEvents2D);
      rad->SetErrors();

      DIS1DHistograms *noRad = new DIS1DHistograms();
      noRad->Create(noRadEvents2D);
      noRad->SetErrors();

      DIS1DHistograms *radCorr = new DIS1DHistograms();
      radCorr->CreateByDivision(rad, noRad,"radCorr","Radiative Correction");
      radCorr->SetBinsOutsideRangeToValue(0.1, 2.0, 1.0);
      radCorr->Save(outputFilename.c_str(),"update");

      DIS1DHistograms *radCorrCrossSection = new DIS1DHistograms();
      radCorrCrossSection->CreateFromExisting(binCorrCS,"radCorrCrossSection","Cross Section w/ Rad Corr."); 
      radCorrCrossSection->DivideByZero(radCorr); // Divides all by [0] instead of [1-6]
      radCorrCrossSection->Save(outputFilename.c_str(),"update");

      DIS1DHistograms *radCorrCrossSectionRatio = new DIS1DHistograms(); 
      radCorrCrossSectionRatio->CreateByDivision(radCorrCrossSection, modelCrossSection, "radCorrCrossSectionRatio", "Cross Section Ratio w/ Rad. Correction");
      radCorrCrossSectionRatio->Save(outputFilename.c_str(), "update");
    }


  } else {
    return PrintUsage();
  }

  return 0;
}

void configureCommandLineOptions(h22Options * theseOpts){
  
  theseOpts->args["INPUT"].args = "UNSET";
  theseOpts->args["INPUT"].type = 1;
  theseOpts->args["INPUT"].name = "Processed file from fillHistograms";
  
  theseOpts->args["RADCORR"].args = "false";
  theseOpts->args["RADCORR"].type = 1;
  theseOpts->args["RADCORR"].name = "Radiative Corrections status";

  theseOpts->args["RADCORR_FILE"].args = "UNSET";
  theseOpts->args["RADCORR_FILE"].type = 1;
  theseOpts->args["RADCORR_FILE"].name = "Radiative Corrections file";

  theseOpts->args["BINCORR"].args = "false";
  theseOpts->args["BINCORR"].type = 1;
  theseOpts->args["BINCORR"].name = "Bin Centering Corrections status";
  
  theseOpts->args["ELASTIC_SUB_FILE"].args = "UNSET";
  theseOpts->args["ELASTIC_SUB_FILE"].type = 1;
  theseOpts->args["ELASTIC_SUB_FILE"].name = "Elastic subtraction file ";
  
}

int PrintUsage(){

  cout << endl;
  cout << endl; 
  cout << "######################################################" << endl;
  cout << "#                                                    #" << endl;
  cout << "#    doCrossSection -> Used to fill CS histograms    #" << endl;
  cout << "#     after fillHistograms has been run.             #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#   This code requires an input/output flagged       #" << endl;
  cout << "#   -INPUT=filename.root    -OUT=outputFile.root     #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#   You may also choose to specify -RADCORR=true     #" << endl;
  cout << "#   if your file contains radiative corrections.     #" << endl;
  cout << "#                                                    #" << endl;
  cout << "#   You may also choose to specify -BINCORR=true     #" << endl;
  cout << "#   if your file contains bin centering corrections. #" << endl;
  cout << "#                                                    #" << endl;
  cout << "######################################################" << endl;
  cout << endl;
  cout << endl;
  
  return 0;
} 
