{

  gROOT->LoadMacro("utils.C");

  const int numberSector = 7;
  const int numberSlices = 10;

  string imagePath = "/volatile/clas12/dmriser/plots/inclusive/";

  TH1D *data[numberSector][numberSlices];
  TH1D *rec[numberSector][numberSlices];
  TH1D *gen[numberSector][numberSlices];
  TH1D *acceptance[numberSector][numberSlices];
  TH1D *crossSection[numberSector][numberSlices];
  TH1D *crossSectionRatio[numberSector][numberSlices];
  TH1D *radCorr[numberSector][numberSlices];
  TH1D *binCorr[numberSector][numberSlices];


  TFile *inputFile = TFile::Open("/volatile/clas12/dmriser/rootFiles/inclusive/xs/xsBugFix.root");
  //  TFile *inputFile = TFile::Open("../out/crossSectionNoFidPass1.root");

  readHisto(data, inputFile, numberSector, numberSlices, "dataEvents_wByQQ");
  readHisto(rec, inputFile, numberSector, numberSlices, "recEventsRad_wByQQ");
  readHisto(gen, inputFile, numberSector, numberSlices, "genEventsRad_wByQQ");
  readHisto(acceptance, inputFile, numberSector, numberSlices, "acceptance_wByQQ");
  //  readHisto(crossSection, inputFile, numberSector, numberSlices,      "radCorrCrossSection_wByQQ");
  //  readHisto(crossSectionRatio, inputFile, numberSector, numberSlices, "radCorrCrossSectionRatio_wByQQ");
  readHisto(crossSection, inputFile, numberSector, numberSlices,      "binCorrectedCrossSection_wByQQ");
  readHisto(crossSectionRatio, inputFile, numberSector, numberSlices, "binCorrectedCrossSectionRatio_wByQQ");
  readHisto(radCorr, inputFile, numberSector, numberSlices, "radCorr_wByQQ");
  readHisto(binCorr, inputFile, numberSector, numberSlices, "binCenterCorrection_wByQQ");

  printSlices(data, numberSector, numberSlices, 2, 1);
  printSlices(rec, numberSector, numberSlices, 2, 2);
  printSlices(gen, numberSector, numberSlices, 2, 3);
  printSlices(acceptance, numberSector, numberSlices, 2, 4);
  printSlices(crossSection, numberSector, numberSlices, 2, 5);
  printSlices(crossSectionRatio, numberSector, numberSlices, 2, 6);
  printSlices(radCorr, numberSector, numberSlices, 2, 8);
  printSlices(binCorr, numberSector, numberSlices, 2, 10);

}
