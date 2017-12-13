{

  gROOT->LoadMacro("utils.C");

  TFile *radCorrFile = TFile::Open("../out/pass5LundEvents.root");
  TFile *simFile = TFile::Open("../out/pass5PreliminaryLook.root");

  const int numberSector = 7; 
  
  TH2D *radCorrEvents[numberSector];
  TH2D *simEvents[numberSector];

  readEvents(radCorrEvents, radCorrFile, numberSector, "radEvents_wByQQ");
  readEvents(simEvents, simFile, numberSector, "genEventsRad_wByQQ");

  TCanvas *compareCanvas = new TCanvas("compareCanvas","",1200,600);   
  compareCanvas->Divide(2,1);

  // Rad Corr 
  compareCanvas->cd(1);
  //  gPad->SetLogz();
  radCorrEvents[0]->Draw("colz");

  // Gen from bos rad 
  compareCanvas->cd(2);
  //  gPad->SetLogz();
  simEvents[0]->Draw("colz");

}
