{

  gROOT->LoadMacro("utils.C");

  TFile * inputFile = TFile::Open("/volatile/clas12/dmriser/rootFiles/inclusive/xs/Phi4to12.root");
  string imagePath  = "/volatile/clas12/dmriser/plots/inclusive/"; 
  const int numberSector = 7;
  const int numberSlices = 10;

  TH1D *sim[numberSector][numberSlices];
  TH1D *data[numberSector][numberSlices];

  readHisto(data,inputFile,numberSector,numberSlices,"dataEvents_wByQQ");
  readHisto(sim,inputFile,numberSector,numberSlices,"recEventsRad_wByQQ");
 
  plot2Histos(data,sim,numberSector,numberSlices,1);

}
