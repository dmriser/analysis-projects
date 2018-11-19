#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1.h"
#include "TLatex.h"

void plot_w_res(){

  const std::string input_filename("/volatile/clas12/dmriser/farm_out/momentum_corrections_pass0/backup_momentum_corrections.root");
  const std::string image_directory("/volatile/clas12/dmriser/plots/momentum_corrections");

  const static int display_sector = 2; 

  const static float fit_left = 0.86; 
  const static float fit_right = 1.0; 

  TH1F *w, *w_corr;
  TF1 *fit[2], *fit_w[2], *fit_background[2];

  TFile *input_file = TFile::Open(input_filename.c_str());

  w = (TH1F*) input_file->Get(Form("w_pid_sect%d", display_sector + 1)); 
  w_corr = (TH1F*) input_file->Get(Form("w_corr_pid_sect%d", display_sector + 1)); 

  w->SetLineColor(kBlack); 
  w->SetLineWidth(2); 
  w->SetMarkerColor(kBlack);
  w->SetMarkerStyle(8);     
  w->SetMarkerSize(1); 
  w_corr->SetLineColor(kBlack); 
  w_corr->SetLineWidth(2); 
  w_corr->SetMarkerColor(kBlack);
  w_corr->SetMarkerStyle(8); 
  w_corr->SetMarkerSize(1); 

  for (int i = 0; i < 2; i++){
    fit[i] = new TF1(Form("fit_%d", i), "[0] * exp(-1 * ((x - [1])/[2])^2 ) + [3] * exp(-1 * ((x - [4])/[5])^2 )");
    fit[i]->SetParameter(0, 30000); 
    fit[i]->SetParameter(1, 0.938); 
    fit[i]->SetParameter(2, 0.03); 
    fit[i]->SetParameter(3, 0); 
    fit[i]->SetParameter(4, 0.0); 
    fit[i]->SetParameter(5, 0.0);

    if (i == 0 ){
      w->Fit(fit[i], "r", "", fit_left, fit_right); 
    } else {
      w_corr->Fit(fit[i], "r", "", fit_left, fit_right); 
    }
    
    fit_w[i] = new TF1(Form("fit_sigma_%d", i), "[0] * exp(-1 * ((x - [1])/[2])^2 )"); 
    fit_w[i]->SetParameter(0, fit[i]->GetParameter(0)); 
    fit_w[i]->SetParameter(1, fit[i]->GetParameter(1)); 
    fit_w[i]->SetParameter(2, fit[i]->GetParameter(2)); 
    fit_w[i]->SetLineColor(99);
    fit_w[i]->SetLineWidth(2);
    fit_w[i]->SetRange(fit_left, fit_right);
      
    fit_background[i] = new TF1(Form("fit_background_%d", i), "[0] * exp(-1 * ((x - [1])/[2])^2 )"); 
    fit_background[i]->SetParameter(0, fit[i]->GetParameter(3)); 
    fit_background[i]->SetParameter(1, fit[i]->GetParameter(4)); 
    fit_background[i]->SetParameter(2, fit[i]->GetParameter(5)); 
    fit_background[i]->SetLineColor(71);
    fit_background[i]->SetLineWidth(2);    
    fit_background[i]->SetRange(fit_left, fit_right);
  }

  TCanvas *canvas = new TCanvas("canvas", "", 1600, 800);
  canvas->Divide(2,1);
    
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  TLatex label; 
  label.SetNDC(); 
  label.SetTextFont(42);
  label.SetTextSize(0.03); 

  canvas->cd(1);
  w->Draw("pe");
  fit[0]->Draw("same"); 
  fit_w[0]->Draw("same"); 
  fit_background[0]->Draw("same"); 

  label.DrawLatex(0.35, 0.92, "Before Correction of P_{e}"); 
  label.DrawLatex(0.45, 0.03, "W (GeV/c^2)"); 
  //  label.DrawLatex(0.55, 0.82, Form("#splitline{#Sigma = (%0.3f #pm %0.3f) GeV}{#Lambda = (%0.3f #pm %0.3f) GeV}", fit_sigma[0]->GetParameter(1), fit_sigma[0]->GetParameter(2), 

  canvas->cd(2);
  w_corr->Draw("pe");
  fit[1]->Draw("same"); 
  fit_w[1]->Draw("same");   
  fit_background[1]->Draw("same"); 

  label.DrawLatex(0.35, 0.92, "After Correction of P_{e}"); 
  label.DrawLatex(0.45, 0.03, "W (GeV/c^2)"); 
  //  label.DrawLatex(0.55, 0.82, Form("#splitline{#Sigma = (%0.3f #pm %0.3f) GeV}{#Lambda = (%0.3f #pm %0.3f) GeV}", fit_sigma[1]->GetParameter(1), fit_sigma[1]->GetParameter(2), 

  canvas->Print(Form("%s/w_fit.pdf", image_directory.c_str())); 

}


