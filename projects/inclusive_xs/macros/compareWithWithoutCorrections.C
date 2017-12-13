{

  gROOT->LoadMacro("utils.C"); 

  // -----------------------------------------------
  TFile *file = TFile::Open("/volatile/clas12/dmriser/rootFiles/inclusive/xs/xsBugFixNoES.root"); 
  string imagePath = "/volatile/clas12/dmriser/plots/inclusive/"; 

  const int numberSector = 7; 
  const int numberSlices = 10; 
  // -----------------------------------------------

  TH1D *xs[numberSector][numberSlices];
  TH1D *xsRad[numberSector][numberSlices];
  TH1D *xsBin[numberSector][numberSlices];
  TH1D *xsModel[numberSector][numberSlices];

  readHisto(xs,     file,numberSector,numberSlices,"crossSection_wByQQ");
  readHisto(xsRad,  file,numberSector,numberSlices,"radCorrCrossSection_wByQQ");
  readHisto(xsBin,  file,numberSector,numberSlices,"binCorrectedCrossSection_wByQQ");
  readHisto(xsModel,file,numberSector,numberSlices,"modelCrossSection_wByQQ");

  TCanvas *can = new TCanvas("can","",800,500); 
  xs     [0][2]->SetMarkerStyle(8);
  xs     [0][2]->SetMarkerColor(kBlack);
  xsRad  [0][2]->SetMarkerStyle(8);
  xsRad  [0][2]->SetMarkerColor(99);
  xsBin  [0][2]->SetMarkerStyle(8);
  xsBin  [0][2]->SetMarkerColor(77);
  xsModel[0][2]->SetLineColor(55);
  xsModel[0][2]->SetLineWidth(2);
  xs     [0][2]->Draw("pe");
  xsRad  [0][2]->Draw("pesame");
  xsBin  [0][2]->Draw("pesame");
  xsModel[0][2]->Draw("lsame");

}
