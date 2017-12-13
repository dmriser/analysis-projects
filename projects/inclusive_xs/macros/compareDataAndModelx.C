{

  gROOT->LoadMacro("utils.C");

  TFile * inputFile = TFile::Open("../out/crossSectionElastSubPass1.root");
  //  TFile * inputFile = TFile::Open("../out/crossSectionNoFidPass1.root");

  const int numberSector = 7;
  const int numberSlices = 40;

  TH1D *data[numberSector][numberSlices];
  TH1D *model[numberSector][numberSlices];

  readHisto(data,inputFile,numberSector,numberSlices,"binCorrectedCrossSection_xByQQ");
  readHisto(model,inputFile,numberSector,numberSlices,"modelCrossSection_xByQQ");
 
  plot2Histos(data,model,numberSector,numberSlices,2);

}
