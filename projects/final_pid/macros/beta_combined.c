{

  TFile *histFile = TFile::Open("/volatile/clas12/dmriser/farm_out/final_pid_pass15/pid.root"); 
  TFile *fitFile  = TFile::Open("../out/fit_neg.root"); 

  const int NSECT  = 7; 
  const int NSLICE = 40; 
  const int NPART  = 5; 

  const int part[NPART]             = {211, 321, 2212, -211, -321};
  const std::string partName[NPART] = {"pi+", "k+", "proton", "pi-", "k-"}; 

  float mass[NPART] = {0.139, 0.498, 0.938, 0.139, 0.498};

  //  TH1D *slices[NSECT][NSLICE]; 
  TH2D *p_beta[NPART][NSECT];
  TF1  *fit_mu[NPART][NSECT];
  TF1  *fit_sigma[NPART][NSECT];
  TF1  *up[NPART][NSECT];
  TF1  *low[NPART][NSECT];

  for(int ipart=0; ipart<NPART; ipart++){
    for(int isect=1; isect<NSECT; isect++){
      p_beta[ipart][isect] = (TH2D*) histFile->Get(Form("MesonHistograms/h2_p_beta_%d_%d_no_cut", isect, part[ipart]));

      fit_mu[ipart][isect] = (TF1*) fitFile->Get(Form("p_dbeta_mu_%d_sector%d_slicesMuFit", part[ipart], isect));
      fit_sigma[ipart][isect] = (TF1*) fitFile->Get(Form("p_dbeta_sigma_%d_sector%d_slicesSigmaFit", part[ipart], isect));

      up[ipart][isect]  = new TF1(Form("up_%d_%d", ipart, isect), Form("1/sqrt(1 + (%f/x)^2) + pol2", mass[ipart]), 0.2, 4.0); 
      low[ipart][isect] = new TF1(Form("low_%d_%d", ipart, isect), Form("1/sqrt(1 + (%f/x)^2) + pol2", mass[ipart]), 0.2, 4.0); 

      up[ipart][isect]->SetParameter(0, fit_mu[ipart][isect]->GetParameter(0) + fit_sigma[ipart][isect]->GetParameter(0)); 
      up[ipart][isect]->SetParameter(1, fit_mu[ipart][isect]->GetParameter(1) + fit_sigma[ipart][isect]->GetParameter(1)); 
      up[ipart][isect]->SetParameter(2, fit_mu[ipart][isect]->GetParameter(2) + fit_sigma[ipart][isect]->GetParameter(2)); 
      //      up[ipart][isect]->SetParameter(3, fit_mu[ipart][isect]->GetParameter(3) + fit_sigma[ipart][isect]->GetParameter(3)); 

      low[ipart][isect]->SetParameter(0, fit_mu[ipart][isect]->GetParameter(0) - fit_sigma[ipart][isect]->GetParameter(0)); 
      low[ipart][isect]->SetParameter(1, fit_mu[ipart][isect]->GetParameter(1) - fit_sigma[ipart][isect]->GetParameter(1)); 
      low[ipart][isect]->SetParameter(2, fit_mu[ipart][isect]->GetParameter(2) - fit_sigma[ipart][isect]->GetParameter(2)); 
      //      low[ipart][isect]->SetParameter(3, fit_mu[ipart][isect]->GetParameter(3) - fit_sigma[ipart][isect]->GetParameter(3)); 

      up[ipart][isect]->SetLineColor(kBlack);
      up[ipart][isect]->SetLineWidth(1);
      up[ipart][isect]->SetLineStyle(2);
      low[ipart][isect]->SetLineColor(kBlack);
      low[ipart][isect]->SetLineWidth(1);
      low[ipart][isect]->SetLineStyle(2);
      

      printf("loading (%d,%d) \n", part[ipart], isect); 
    }
  }


  TLatex *latex = new TLatex();
  latex->SetNDC();
  latex->SetTextSize(0.04);
  latex->SetTextFont(42); 

  TCanvas *can = new TCanvas("can", "", 1200, 1200);

  can->cd(); 
  p_beta[0][1]->Draw("colz"); 
  gPad->SetLogz(); 
  up[0][1]->Draw("same"); 
  low[0][1]->Draw("same"); 

  latex->DrawLatex(0.75, 0.90, "#pi^{+}"); 
  latex->DrawLatex(0.45, 0.05, "p (GeV/c)"); 
  latex->SetTextAngle(90.0); 
  latex->DrawLatex(0.05, 0.45, "#beta"); 
  latex->SetTextAngle(0.0); 
  can->Print("beautiful_pbeta_pip.pdf");
  
  can->Clear(); 
  p_beta[0][1]->Draw("colz"); 
  gPad->SetLogz(); 
  up[1][1]->Draw("same"); 
  low[1][1]->Draw("same"); 
  latex->DrawLatex(0.45, 0.95, "K^{+}"); 
  latex->DrawLatex(0.45, 0.05, "p (GeV/c)"); 
  latex->SetTextAngle(90.0); 
  latex->DrawLatex(0.05, 0.45, "#beta"); 
  latex->SetTextAngle(0.0); 
  can->Print("beautiful_pbeta_kp.pdf");

  can->Clear(); 
  p_beta[0][1]->Draw("colz"); 
  gPad->SetLogz(); 
  up[2][1]->Draw("same"); 
  low[2][1]->Draw("same"); 
  latex->DrawLatex(0.45, 0.95, "p^{+}"); 
  latex->DrawLatex(0.45, 0.05, "p (GeV/c)"); 
  latex->SetTextAngle(90.0); 
  latex->DrawLatex(0.05, 0.45, "#beta"); 
  latex->SetTextAngle(0.0); 
  can->Print("beautiful_pbeta_prot.pdf");

  can->Clear(); 
  p_beta[3][1]->Draw("colz"); 
  gPad->SetLogz(); 
  up[3][1]->Draw("same"); 
  low[3][1]->Draw("same"); 
  latex->DrawLatex(0.45, 0.95, "p^{-}"); 
  latex->DrawLatex(0.45, 0.05, "p (GeV/c)"); 
  latex->SetTextAngle(90.0); 
  latex->DrawLatex(0.05, 0.45, "#beta"); 
  latex->SetTextAngle(0.0); 
  can->Print("beautiful_pbeta_pim.pdf");

  can->Clear(); 
  p_beta[3][1]->Draw("colz"); 
  gPad->SetLogz(); 
  up[4][1]->Draw("same"); 
  low[4][1]->Draw("same"); 
  latex->DrawLatex(0.45, 0.95, "K^{-}"); 
  latex->DrawLatex(0.45, 0.05, "p (GeV/c)"); 
  latex->SetTextAngle(90.0); 
  latex->DrawLatex(0.05, 0.45, "#beta"); 
  latex->SetTextAngle(0.0); 
  can->Print("beautiful_pbeta_km.pdf");


}
