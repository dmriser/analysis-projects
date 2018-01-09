{
  
  TFile *inputFile = TFile::Open("/volatile/clas12/dmriser/rootFiles/inclusive/mc/bugFix.root"); 
  TH2F *wByQQ      = (TH2F*) inputFile->Get("genEventsRad_wByQQ_s0");  


  const int nq = 10; 
  const int nw = 35; 
  float wMin, wMax; 
  float qMin, qMax; 
  wMin = 1.1; wMax = 2.1; 
  qMin = 1.7; qMax = 4.2; 

  float qStep = (qMax-qMin)/nq; 
  float wStep = (wMax-wMin)/nw; 

  TCanvas *can = new TCanvas("can", "", 800, 450); 
  can->cd();

  wByQQ->Draw("colz"); 
  gPad->SetLogz(); 

  for (int qbin = 0; qbin <= nq; qbin++){
    float q = qbin*qStep + qMin; 
    TLine *line = new TLine(wMin,q,wMax,q);
    line->SetLineColor(99);
    line->SetLineWidth(2);
    line->Draw("same"); 
  }

  for (int wbin = 0; wbin <= nw; wbin++){
    float w= wbin*wStep + wMin; 
    TLine *line = new TLine(w,qMin,w,qMax);
    line->SetLineColor(99);
    line->SetLineWidth(2);
    line->Draw("same"); 
  }

  TLatex tit; 
  tit.SetNDC(); 
  tit.SetTextFont(42); 
  tit.SetTextSize(0.05); 
  tit.DrawLatex(0.45, 0.05, "W (GeV/c^{2})"); 
  tit.DrawLatex(0.275, 0.925, "Generated Inclusive (radiated) Events w/ Binning");
  tit.SetTextAngle(90.0); 
  tit.DrawLatex(0.1, 0.4, "Q^{2} (GeV/c)^{2}"); 


  can->Print("/volatile/clas12/dmriser/plots/inclusive/generatedWithBinning.pdf");
}
