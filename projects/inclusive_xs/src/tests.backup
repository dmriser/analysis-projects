#include "DInformation.h"
#include "MomCorr.h"
#include "PhysicsEventCut.h"
#include "PhysicsEventSelector.h"
#include "TLorentzVector.h"

#include "common/BaseDISHistograms.h"
#include "common/DataLoader.h"
#include "common/FaradayCupLoader.h"
#include "common/HistogramLoader.h"
#include "common/MCLoader.h"
#include "common/DIS1DHistograms.h"
//#include "common/ModelCrossSectionLoader.h"

#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::string;
using std::vector;

void testBaseHistograms();
void testHistogramBuilder();
void testDataHistogramBuilder();
void testMCHistogramBuilder();
void testFaradayCupLoader();
void test1DHistograms();

int main(){

  //  testBaseHistograms();
  //  testHistogramBuilder();
  //  testDataHistogramBuilder();
  //  testMCHistogramBuilder(); 
  //  testFaradayCupLoader();
  test1DHistograms();  


  return 0; 
}

void testBaseHistograms(){
  BaseDISHistograms dataEvents;
  dataEvents.Init("dataEvents","Data Events ");
  dataEvents.Save("../out/test.root","RECREATE");
}

void testHistogramBuilder(){

  PhysicsEventSelector *sel = new PhysicsEventSelector();
  PhysicsEventCut_w * w_cut = new PhysicsEventCut_w();
  w_cut->set_min(2.05);
  w_cut->set_max(4.00);
  sel->add_cut(w_cut); 
  sel->enable_all();
  
  HistogramLoader testLoader(sel, "../out/testLoader.root", "RECREATE");  
  testLoader.Execute();
  
}

void testDataHistogramBuilder(){

  PhysicsEventSelector *sel = new PhysicsEventSelector();
  PhysicsEventCut_w * w_cut = new PhysicsEventCut_w();
  w_cut->set_min(2.05);
  w_cut->set_max(400.00);
  sel->add_cut(w_cut); 
  sel->enable_all();

  MomCorr_e1f * momCorr = new MomCorr_e1f("/Users/dmriser/Work/analysis/momCorr/");
  
  DataLoader testLoader(sel, momCorr, "../out/testBothLoader.root", "RECREATE");  
  testLoader.AddFile("/Users/dmriser/Data/clas_038534.A01.root");
  testLoader.Execute();
  
}

void testMCHistogramBuilder(){

  PhysicsEventSelector *sel = new PhysicsEventSelector();
  PhysicsEventCut_w * w_cut = new PhysicsEventCut_w();
  w_cut->set_min(2.05);
  w_cut->set_max(400.00);
  sel->add_cut(w_cut); 
  sel->enable_all();
  
  MCLoader testLoader(sel, "out/testBothLoader.root", "UPDATE", "NoRad");  
  testLoader.AddFile("/Users/dmriser/Data/mc/dis/ftp/dis_pass0.2510.root");
  testLoader.Execute();
  
}

void testFaradayCupLoader(){
  DInformation * thisInformation = new DInformation();
  thisInformation->load("/u/home/dmriser/mydoc/analysis/root_scripts/Analysis_v2/lists/runs.info"); 

  vector<string> files; 
  files.push_back("/Users/dmriser/Data/clas_038534.A01.root");
  files.push_back("/Users/dmriser/Data/clas_038534.A02.root");
  files.push_back("/Users/dmriser/Data/clas_038534.A03.root"); 

  FaradayCupAppender testAppender(thisInformation);
  testAppender.AddFiles(files);
  testAppender.AppendToFile("out/testfile.root"); 
}

void test1DHistograms(){
  BaseDISHistograms * dataEvents = new BaseDISHistograms();
  dataEvents->Load("out/testFile.root","dataEvents"); 
  dataEvents->Rebin2D(2,2);

  BaseDISHistograms * recEvents = new BaseDISHistograms();
  recEvents->Load("out/testFile.root","recEventsRad"); 
  recEvents->Rebin2D(2,2);

  BaseDISHistograms * genEvents = new BaseDISHistograms();
  genEvents->Load("out/testFile.root","genEventsRad"); 
  genEvents->Rebin2D(2,2);

  BaseDISHistograms * recAndGenEvents = new BaseDISHistograms();
  recAndGenEvents->Load("out/testFile.root","recAndGenEventsRad"); 

  DIS1DHistograms * dataHistos = new DIS1DHistograms(); 
  dataHistos->Create(dataEvents);
  dataHistos->SetErrors();
  dataHistos->Save("out/testSlicing.root","recreate");

  DIS1DHistograms * recHistos = new DIS1DHistograms(); 
  recHistos->Create(recEvents);
  recHistos->SetErrors();
  recHistos->Save("out/testSlicing.root","update");

  DIS1DHistograms * genHistos = new DIS1DHistograms(); 
  genHistos->Create(genEvents);
  genHistos->SetErrors();
  genHistos->Save("out/testSlicing.root","update");

  DIS1DHistograms * recAndGenHistos = new DIS1DHistograms(); 
  recAndGenHistos->Create(recAndGenEvents);
  recAndGenHistos->SetErrors();
  recAndGenHistos->Save("out/testSlicing.root","update");

  DIS1DHistograms * accHistos = new DIS1DHistograms(); 
  accHistos->CreateFromExisting(recHistos,"acc","Acceptance");
  accHistos->Divide(genHistos);
  accHistos->Save("out/testSlicing.root","update");
  
  DIS1DHistograms * purity = new DIS1DHistograms();
  purity->CreateByDivision(recAndGenHistos, recHistos, "purity", "Purity");
  purity->Scale(1.0);
  purity->ScaleByBinWidth();
  purity->Save("out/testSlicing.root","update");
  /*
  ModelCrossSectionLoader modelLoader;
  modelLoader.provideBinningTemplate(purity); 
  modelLoader.loadCrossSection(1,1,5.498); 
  //  modelLoader.Save("out/modelTest.root","recreate"); 
  
  DIS1DHistograms * crossSection = modelLoader.getCrossSection(); 
  DIS1DHistograms * crossSectionAverage = modelLoader.getCrossSectionAverage(); 
  DIS1DHistograms * binCenterCorrection = new DIS1DHistograms(); 
  binCenterCorrection->CreateByDivision(crossSection, crossSectionAverage, "binCenterCorrection", "Bin Centering Correction"); 
  binCenterCorrection->Save("out/testSlicing.root","update"); 

  DIS1DHistograms * loadingTest = new DIS1DHistograms(); 
  loadingTest->Load("out/crossSectionTest.root","crossSection"); 
  loadingTest->PrintPDF("thisTestPDF.pdf");
  */
}
