{

  // ---------------------------------------------------------
  //    User Parameters
  // ---------------------------------------------------------
  TFile *inputFile = TFile::Open("/volatile/clas12/dmriser/rootFiles/inclusive/both/goodRuns.root");


  // ---------------------------------------------------------
  //    The Rest of the Program
  // ---------------------------------------------------------
  const int numberSectors = 7;

  TH1I *numberEvents = new TH1I("numberEvents","",numberSectors,0,numberSectors-1); 
  TH2I *dataEvents[numberSectors];

  for (int sector=0; sector<numberSectors; sector++){
    dataEvents[sector] = (TH2I*) inputFile->Get(Form("dataEvents_wByQQ_s%d",sector));

    numberEvents->SetBinContent(sector+1,dataEvents[sector]->Integral());
  }

  TCanvas *c1 = new TCanvas("c1","",800,500); 
  gPad->SetLogy();
  gPad->SetGridx();
  gPad->SetGridy();


  numberEvents->SetFillColorAlpha(66,0.4);
  numberEvents->SetLineColor(kBlack);
  numberEvents->Draw();
  c1->Print("/volatile/clas12/dmriser/plots/inclusive/numberOfEvents.png");
}
