{

  gROOT->LoadMacro("utils.C");

  //  TFile * inputFile = TFile::Open("../out/crossSectionWithResonancePass1.root");
  TFile * inputFile = TFile::Open("/volatile/clas12/dmriser/rootFiles/inclusive/xs/xsBugFixNoES.root");

  string imagePath       = "/volatile/clas12/dmriser/plots/inclusive/"; 
  const int numberSector = 7;
  const int numberSlices = 10;

  TH1D *data[numberSector][numberSlices];
  TH1D *model[numberSector][numberSlices];

  readHisto(data,inputFile,numberSector,numberSlices, "radCorrCrossSection_wByQQ");
  readHisto(model,inputFile,numberSector,numberSlices,"modelCrossSection_wByQQ");
 
  plot2Histos(data,model,numberSector,numberSlices,2);

}
