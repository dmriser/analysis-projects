{

  TFile *histFile = TFile::Open("/volatile/clas12/dmriser/farm_out/final_pid_pass11/pid.root"); 
  TFile *fitFile  = TFile::Open("../out/fits.root"); 

  const int NSECT  = 7; 
  const int NSLICE = 40; 
  const int NPART  = 3; 

  const int part[NPART]             = {211, 321, 2212};
  const std::string partName[NPART] = {"pi+", "k+", "proton"}; 

  float mass[NPART] = {0.139, 0.498, 0.938};

  //  TH1D *slices[NSECT][NSLICE]; 
  TH2D *p_beta[NPART][NSECT];
  TF1  *fit_mu[NPART][NSECT];
  TF1  *fit_sigma[NPART][NSECT];
  TF1  *up[NPART][NSECT];
  TF1  *low[NPART][NSECT];

  for(int ipart=0; ipart<NPART; ipart++){
    for(int isect=1; isect<NSECT; isect++){
      p_beta[ipart][isect] = (TH2D*) histFile->Get(Form("MesonHistograms/h2_p_beta_%d_%d_physics_enhanced", isect, part[ipart]));

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
  //  latex->SetTextFont(42); 

  TCanvas *can = new TCanvas("can", "", 600, 1800);
  can->Divide(3,6); 

  can->cd(1);
  latex->DrawLatex(0.5, 0.99, "#pi^+"); 

  can->cd(2);
  latex->DrawLatex(0.5, 0.99, "#K^+"); 

  can->cd(3);
  latex->DrawLatex(0.5, 0.99, "#p^+"); 


  for(int s=1; s<7; s++){
    can->cd(1+3*(s-1)); 
    p_beta[0][s]->Draw("colz"); 
    gPad->SetLogz(); 
    //    fit_mu[0][s]->Draw("same"); 
    //    fit_sigma[0][s]->Draw("same"); 
    up[0][s]->Draw("same"); 
    low[0][s]->Draw("same"); 

    can->cd(2+3*(s-1)); 
    p_beta[1][s]->Draw("colz"); 
    gPad->SetLogz(); 
    //    fit_mu[1][s]->Draw("same"); 
    //    fit_sigma[1][s]->Draw("same"); 
    up[1][s]->Draw("same"); 
    low[1][s]->Draw("same"); 
    
    can->cd(3+3*(s-1)); 
    p_beta[0][s]->Draw("colz"); 
    gPad->SetLogz(); 
    //    fit_mu[2][s]->Draw("same"); 
    //    fit_sigma[2][s]->Draw("same"); 
    up[2][s]->Draw("same"); 
    low[2][s]->Draw("same"); 

  }

  can->Print("beautiful_pbeta.pdf");
}
