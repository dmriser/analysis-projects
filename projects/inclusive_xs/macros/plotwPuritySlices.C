{

  gROOT->LoadMacro("utils.C");

  const int numberSector = 7;
  const int numberSlices = 40;
  const int sectorToPrint = 3;

  TFile * inputFile = TFile::Open("../out/purityResultPass1.1.root");

  TH1D *purity[numberSector][numberSlices];
  TH1D *rec[numberSector][numberSlices];
  TH1D *gen[numberSector][numberSlices];
  TH1D *recAndGen[numberSector][numberSlices];

  string purityTitle = Form("purity%dBins_wByQQ",numberSlices);
  string recTitle = Form("recEvents%dBins_wByQQ",numberSlices);
  string genTitle = Form("genEvents%dBins_wByQQ",numberSlices);
  string recAndGenTitle = Form("recAndGenEvents%dBins_wByQQ",numberSlices);

  readHisto(purity, inputFile, numberSector, numberSlices, purityTitle);
  readHisto(gen, inputFile, numberSector, numberSlices, genTitle);
  readHisto(rec, inputFile, numberSector, numberSlices, recTitle);
  readHisto(recAndGen, inputFile, numberSector, numberSlices, recAndGenTitle);
  
  string pdfTitle = Form("purity/purity%dBinsSector%d.pdf",numberSlices,sectorToPrint);
  printPurityStudy(purity, rec, gen, recAndGen, numberSector, numberSlices, sectorToPrint, pdfTitle);

}
