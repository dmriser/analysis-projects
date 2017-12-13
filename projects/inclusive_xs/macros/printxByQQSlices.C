{

  gROOT->LoadMacro("utils.C");

  const int numberSector = 7;
  const int numberSlices = 40;

  TH1D *data[numberSector][numberSlices];
  TH1D *rec[numberSector][numberSlices];
  TH1D *gen[numberSector][numberSlices];
  TH1D *acceptance[numberSector][numberSlices];
  TH1D *crossSection[numberSector][numberSlices];
  TH1D *crossSectionRatio[numberSector][numberSlices];
  TH1D *radCorr[numberSector][numberSlices];


  TFile *inputFile = TFile::Open("../out/crossSectionElastSubPass1.root");
  //  TFile *inputFile = TFile::Open("../out/crossSectionNoFidPass1.root");

  readHisto(data, inputFile, numberSector, numberSlices, "dataEvents_xByQQ");
  readHisto(rec, inputFile, numberSector, numberSlices, "recEventsRad_xByQQ");
  readHisto(gen, inputFile, numberSector, numberSlices, "genEventsRad_xByQQ");
  readHisto(acceptance, inputFile, numberSector, numberSlices, "acceptance_xByQQ");
  readHisto(crossSection, inputFile, numberSector, numberSlices, "radCorrCrossSection_xByQQ");
  readHisto(crossSectionRatio, inputFile, numberSector, numberSlices, "radCorrCrossSectionRatio_xByQQ");
  readHisto(radCorr, inputFile, numberSector, numberSlices, "radCorr_xByQQ");
  
  printSlices(data, numberSector, numberSlices, 1, 1);
  printSlices(rec, numberSector, numberSlices, 1, 2);
  printSlices(gen, numberSector, numberSlices, 1, 3);
  printSlices(acceptance, numberSector, numberSlices, 1, 4);
  printSlices(crossSection, numberSector, numberSlices, 1, 5);
  printSlices(crossSectionRatio, numberSector, numberSlices, 1, 6);
  printSlices(radCorr, numberSector, numberSlices, 1, 8);

}
