{
  // Input file
  string inputFile = "out/data.root";

  // Bounds on W for slice 
  double wMin = 2.35; 
  double wMax = 2.40;

  int color = 64;

  // Getting file and events 
  TFile      *file   = TFile::Open(inputFile.c_str());
  THnSparseI *events = (THnSparseI*) file->Get("events");

  int lowBin  = events->GetAxis(3)->FindBin(wMin); 
  int highBin = events->GetAxis(3)->FindBin(wMax);

  // Grab histograms after restricting W  
  events->GetAxis(3)->SetRange(lowBin, highBin);
  TH1D *qq  = (TH1D*) events->Projection(4);   qq ->SetName("qq");

  TCanvas *sliceCan = new TCanvas("sliceCan","",800,800);  

  qq->SetLineColor(color);
  qq->SetFillColorAlpha(color, 0.5);
  qq->Draw();

  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gPad  ->SetMargin(0.15, 0.15, 0.15, 0.15);
  gPad  ->SetGridx();
  gPad  ->SetGridy();

  TLatex lab; 
  lab.SetNDC();
  lab.SetTextSize(0.04);
  lab.DrawLatex(0.72,0.06,"Q^{2} [GeV^{2}/c^{2}]");
  lab.DrawLatex(0.15,0.90,Form("N_{events} = %.2e",qq->GetEntries()));
  lab.DrawLatex(0.65,0.90,Form("%.2f #leq W #leq %.2f",wMin,wMax));

  sliceCan->SaveAs("img/QQSlice.png");
}
