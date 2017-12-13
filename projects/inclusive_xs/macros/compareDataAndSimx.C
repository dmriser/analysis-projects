{

  gROOT->LoadMacro("utils.C");

  TFile * inputFile = TFile::Open("../out/crossSectionElastSubPass1.root");
  //  TFile * inputFile = TFile::Open("../out/crossSectionNoFidPass1.root");

  const int numberSector = 7;
  const int numberSlices = 40;

  TH1D *sim[numberSector][numberSlices];
  TH1D *data[numberSector][numberSlices];

  readHisto(data,inputFile,numberSector,numberSlices,"dataEvents_xByQQ");
  readHisto(sim,inputFile,numberSector,numberSlices,"recEventsRad_xByQQ");
 
  plot2Histos(data,sim,numberSector,numberSlices,1);

}
