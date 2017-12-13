{

  gROOT->LoadMacro("utils.C");
  
  const int numberQQBins = 40; 
  const int numberWBins = 40; 
  const int numberSlices = 40; 
  const int numberSector = 7; 

  TFile *inputFile = TFile::Open("../out/crossSectionWithResonancePass1.root");

  TH2D *model = new TH2D("model","",numberQQBins,1.0,4.8,numberWBins,1.1,2.7);
  TH1D *modelQQSlices[numberSlices]; 
  TH1D *modelWSlices[numberSlices]; 

  TH1D *crossSection_wByQQ[numberSector][numberSlices];
  
  readHisto(crossSection_wByQQ, inputFile, numberSector, numberSlices, "crossSection_wByQQ");
  loadModel(model,numberQQBins,numberWBins,"KeppelTheoryWithResonance.dat");
  makeSlicesX(model, modelQQSlices, numberSlices, 1);
  makeSlicesY(model, modelWSlices, numberSlices, 1);

  TCanvas *modelCanvas = new TCanvas("modelCanvas","",800,800);
  gPad->SetLogz();
  model->Draw("colz");

  TCanvas *xSliceCanvas = new TCanvas("xSliceCanvas","",800,800);

  /*
  xSliceCanvas->cd();
  gPad->SetLogy();
  modelWSlices[2]->Draw("l");
  crossSection_wByQQ[1][2]->Draw("pesame");
  xSliceCanvas->Print("keppelModelQQSlice.png");
  */

  
  xSliceCanvas->Print("keppelQQSlices.pdf[");
  for (int slice=0; slice< numberSlices; slice++){
    gPad->SetLogy();
    modelQQSlices[slice]->Draw("l");
    xSliceCanvas->Print("keppelQQSlices.pdf");
  }
  xSliceCanvas->Print("keppelQQSlices.pdf]");

  TCanvas *ySliceCanvas = new TCanvas("ySliceCanvas","",800,800);  
  ySliceCanvas->Print("keppelWSlices.pdf[");
  for (int slice=0; slice< numberSlices; slice++){
    gPad->SetLogy();
    modelWSlices[slice]->Draw("l");
    //    crossSection_wByQQ[0][slice]->Scale(1/1000.0);
    crossSection_wByQQ[0][slice]->Draw("pesame");
    ySliceCanvas->Print("keppelWSlices.pdf");
  }
  ySliceCanvas->Print("keppelWSlices.pdf]");
  
}
