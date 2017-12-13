{

  // -------------------------------------------------------
  //  User Parameters
  // -------------------------------------------------------
  //  TFile *inputFile = TFile::Open("/volatile/clas12/dmriser/analysis/e1f_analysis/fCupRoot/fCupV2.root");
  TFile *inputFile = TFile::Open("../fCupV2.root");
  string imagePath = "/volatile/clas12/dmriser/plots/faradayCup/"; 


  // -------------------------------------------------------
  //  The Rest
  // -------------------------------------------------------
  TTree *fCupTree  = (TTree*) inputFile->Get("fCup");  
  TTreeReader reader("fCup",inputFile);
  
  TTreeReaderValue<Double_t> charge(reader, "dQ");
  TTreeReaderValue<Int_t>    events(reader, "dN");
  TTreeReaderValue<Int_t>    runno(reader, "runno");


  TH1D *ratioHist      = new TH1D("ratioHist","",100,-1000,8000); 
  TH1D *ratioByRun     = new TH1D("ratioByRun","",1500,37400,38900); 
  TH1D *goodRatioByRun = new TH1D("goodRatioByRun","",1500,37400,38900); 

  vector<int> acceptedRuns; 

  while( reader.Next() ){
    double ratio = (double) *events / *charge; 

    if (ratio > 4000){
      cout.width(12); cout << *runno; 
      cout.width(12); cout << *events; 
      cout.width(12); cout << *charge; 
      cout.width(12); cout << ratio << endl; 
    
      acceptedRuns.push_back(*runno); 
    }

    ratioHist ->Fill(ratio);
    if (ratio < 1e9 && ratio > 1e-9) { 
      ratioByRun->SetBinContent(ratioByRun->FindBin(*runno),ratio); 
      if (ratio > 4000){
	goodRatioByRun->SetBinContent(ratioByRun->FindBin(*runno),ratio); 
      }
    }
  }


  //  for (int run=0; run<acceptedRuns.size(); run++){
  //    cout << acceptedRuns[run] << endl; 
  //  }

  TCanvas *c1 = new TCanvas("c1","",800,500); 

  TLatex xTitle, yTitle, title, passTit, failTit; 

  title.SetNDC(); 
  title.SetTextFont(42);
  title.SetTextSize(0.05); 

  passTit.SetNDC(); 
  passTit.SetTextFont(42);
  passTit.SetTextSize(0.04); 
  passTit.SetTextColor(77); 

  failTit.SetNDC(); 
  failTit.SetTextFont(42);
  failTit.SetTextSize(0.04); 
  failTit.SetTextColor(98); 

  xTitle.SetNDC(); 
  xTitle.SetTextFont(42);
  xTitle.SetTextSize(0.04); 

  yTitle.SetNDC(); 
  yTitle.SetTextFont(42);
  yTitle.SetTextSize(0.04); 
  yTitle.SetTextAngle(90.0); 

  gPad->SetGridx();
  gPad->SetGridy();
  ratioHist->SetFillColorAlpha(72,0.4); 
  ratioHist->Draw();  

  title.DrawLatex(0.27, 0.9, "Inclusive Events Normalized by Charge"); 
  xTitle.DrawLatex(0.45, 0.05, "Ratio N/Q"); 
  yTitle.DrawLatex(0.08, 0.42, "Counts (Number of Runs) "); 

  c1->Print(Form("%sinclusiveRatesNormByCharge.png",imagePath.c_str())); 

  TCanvas *c2 = new TCanvas("c2","",800,500);
  //  gPad->SetGridx();
  //  gPad->SetGridy();
  gPad->SetLogy();
  ratioByRun->SetMarkerStyle(33);
  ratioByRun->SetMarkerColor(98);
  ratioByRun->Draw("p");  
  goodRatioByRun->SetMarkerStyle(33);
  goodRatioByRun->SetMarkerColor(77);
  goodRatioByRun->Draw("psame");  

  title .DrawLatex(0.27, 0.9, "Inclusive Events Normalized by Charge"); 
  xTitle.DrawLatex(0.45, 0.05, "Run Number"); 
  yTitle.DrawLatex(0.08, 0.42, "Ratio N/Q"); 
  passTit.DrawLatex(0.87, 0.8, "Pass"); 
  failTit.DrawLatex(0.87, 0.75, "Fail"); 

  c2->Print(Form("%sinclusiveRatesByRun.png",imagePath.c_str())); 

}
