{

  gROOT->LoadMacro("utils.C");

  const int numberSector = 7;
  const int numberSlices = 100;

  TH1D *purity[numberSector][numberSlices];

  TFile *inputFile = TFile::Open("../out/purityResultPass1.1.root");
  
  readHisto(purity, inputFile, numberSector, numberSlices, Form("purity%dBins_wByQQ",numberSlices));
  printSlices(purity, numberSector, numberSlices, 2, 7);

}
