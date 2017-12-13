{

  TFile *f   = TFile::Open("keppelRad.root");
  TH1D *pEle = (TH1D*) f->Get("h1_ele_p_sect0_generatedEvents");
  TH1D *pGam = (TH1D*) f->Get("h1_part1_p_sect0_generatedEvents");

  pEle->SetFillColorAlpha(99,0.2);
  pGam->SetFillColorAlpha(65,0.2);

  TCanvas *c1 = new TCanvas("c1","",800,500);
  pEle->Draw();
  pGam->Draw("same");
  c1->Print("momentum.png");

}
