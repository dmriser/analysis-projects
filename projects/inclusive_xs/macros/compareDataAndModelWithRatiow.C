{

  gROOT->LoadMacro("utils.C");

  //  TFile * inputFile = TFile::Open("../out/crossSectionWithResonancePass1.root");
  TFile * inputFile = TFile::Open("/volatile/clas12/dmriser/rootFiles/inclusive/xs/superGoodRuns.root");

  const int numberSector = 1;
  const int numberSlices = 20;

  TH1D *data[numberSector][numberSlices];
  TH1D *model[numberSector][numberSlices];
  TH1D *ratio[numberSector][numberSlices];

  readHisto(data,inputFile,numberSector,numberSlices,"binCorrectedCrossSection_wByQQ");
  readHisto(model,inputFile,numberSector,numberSlices,"modelCrossSection_wByQQ");
  readHisto(ratio,inputFile,numberSector,numberSlices,"radCorrCrossSectionRatio_wByQQ");
 
  plot3Histos(data,model,ratio,numberSector,numberSlices,2);

}
