{


  TFile *f = TFile::Open("../out/asym.root");

  TH1F *data = (TH1F*) f->Get("phi/phi_x2_asym");
  TH1F *cl   = (TH1F*) f->Get("phi/phi_x2_basicFit_confidenceInterval");
  

  cl->SetFillColorAlpha(59,0.35);
  cl->Draw("e3"); 

  data->SetLineColor(kBlack);
  data->SetLineWidth(2);
  data->SetMarkerColor(kBlack);
  data->SetMarkerStyle(8);
  data->Draw("histpesame"); 

  c1->Print("checkFit.png");

}
