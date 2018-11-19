#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1.h"
#include "TLatex.h"

void plot_kaon_res(){

  const std::string input_filename("/volatile/clas12/dmriser/farm_out/momentum_corrections_pass0/backup_momentum_corrections.root");
  const std::string image_directory("/volatile/clas12/dmriser/plots/momentum_corrections");

  const static int display_sector = 2; 
  const static float sigma_mass  = 1.192;  
  const static float lambda_mass = 1.115;  

  TH1F *mm, *mm_corr;
  TF1 *fit[2], *fit_sigma[2], 
    *fit_lambda[2], *fit_background[2];

  TFile *input_file = TFile::Open(input_filename.c_str());

  mm = (TH1F*) input_file->Get(Form("mm_sigma_pid_sect%d", display_sector + 1)); 
  mm_corr = (TH1F*) input_file->Get(Form("mm_sigma_corr_pid_sect%d", display_sector + 1)); 

  mm->SetLineColor(kBlack); 
  mm->SetLineWidth(2); 
  mm->SetMarkerColor(kBlack);
  mm->SetMarkerStyle(8);     
  mm->SetMarkerSize(1); 
  mm_corr->SetLineColor(kBlack); 
  mm_corr->SetLineWidth(2); 
  mm_corr->SetMarkerColor(kBlack);
  mm_corr->SetMarkerStyle(8); 
  mm_corr->SetMarkerSize(1); 

  for (int i = 0; i < 2; i++){
    fit[i] = new TF1(Form("fit_%d", i), "[0] * exp(-1 * ((x - [1])/[2])^2 ) + [3] * exp(-1 * ((x - [4])/[5])^2 ) + [6] * exp(-1 * ((x - [7])/[8])^2 )");
    fit[i]->SetParameter(0, 200); 
    fit[i]->SetParameter(1, sigma_mass); 
    fit[i]->SetParameter(2, 0.03); 
    fit[i]->SetParameter(3, 2000); 
    fit[i]->SetParameter(4, lambda_mass); 
    fit[i]->SetParameter(5, 0.03);
    fit[i]->SetParameter(6, 700);
    fit[i]->SetParameter(7, 1.2);    
    fit[i]->SetParameter(8, 1.0);

    if (i == 0 ){
      mm->Fit(fit[i], "r", "", 1.0, 1.3); 
    } else {
      mm_corr->Fit(fit[i], "r", "", 1.0, 1.3); 
    }
    
    fit_sigma[i] = new TF1(Form("fit_sigma_%d", i), "[0] * exp(-1 * ((x - [1])/[2])^2 )"); 
    fit_sigma[i]->SetParameter(0, fit[i]->GetParameter(0)); 
    fit_sigma[i]->SetParameter(1, fit[i]->GetParameter(1)); 
    fit_sigma[i]->SetParameter(2, fit[i]->GetParameter(2)); 
    fit_sigma[i]->SetLineColor(99);
    fit_sigma[i]->SetLineWidth(2);
    fit_sigma[i]->SetRange(1.0, 1.3);
      
    fit_lambda[i] = new TF1(Form("fit_lambda_%d", i), "[0] * exp(-1 * ((x - [1])/[2])^2 )"); 
    fit_lambda[i]->SetParameter(0, fit[i]->GetParameter(3)); 
    fit_lambda[i]->SetParameter(1, fit[i]->GetParameter(4)); 
    fit_lambda[i]->SetParameter(2, fit[i]->GetParameter(5)); 
    fit_lambda[i]->SetLineColor(55);
    fit_lambda[i]->SetLineWidth(2);
    fit_lambda[i]->SetRange(1.0, 1.3);
      
    fit_background[i] = new TF1(Form("fit_background_%d", i), "[0] * exp(-1 * ((x - [1])/[2])^2 )"); 
    fit_background[i]->SetParameter(0, fit[i]->GetParameter(6)); 
    fit_background[i]->SetParameter(1, fit[i]->GetParameter(7)); 
    fit_background[i]->SetParameter(2, fit[i]->GetParameter(8)); 
    fit_background[i]->SetLineColor(71);
    fit_background[i]->SetLineWidth(2);    
    fit_background[i]->SetRange(1.0, 1.3);
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
  mm->Draw("pe");
  fit[0]->Draw("same"); 
  fit_sigma[0]->Draw("same"); 
  fit_lambda[0]->Draw("same");   
  fit_background[0]->Draw("same"); 

  label.DrawLatex(0.35, 0.92, "Before Correction of P_{e}"); 
  label.DrawLatex(0.45, 0.03, "M_{X} (ep #rightarrow eK^{+}X)"); 
  label.DrawLatex(0.55, 0.82, Form("#splitline{#Sigma = (%0.3f #pm %0.3f) GeV}{#Lambda = (%0.3f #pm %0.3f) GeV}", fit_sigma[0]->GetParameter(1), fit_sigma[0]->GetParameter(2), 
				 fit_lambda[0]->GetParameter(1), fit_lambda[0]->GetParameter(2))); 

  canvas->cd(2);
  mm_corr->Draw("pe");
  fit[1]->Draw("same"); 
  fit_sigma[1]->Draw("same"); 
  fit_lambda[1]->Draw("same");   
  fit_background[1]->Draw("same"); 

  label.DrawLatex(0.35, 0.92, "After Correction of P_{e}"); 
  label.DrawLatex(0.45, 0.03, "M_{X} (ep #rightarrow eK^{+}X)"); 
  label.DrawLatex(0.55, 0.82, Form("#splitline{#Sigma = (%0.3f #pm %0.3f) GeV}{#Lambda = (%0.3f #pm %0.3f) GeV}", fit_sigma[1]->GetParameter(1), fit_sigma[1]->GetParameter(2), 
				 fit_lambda[1]->GetParameter(1), fit_lambda[1]->GetParameter(2))); 

  canvas->Print(Form("%s/lambda_sigma.pdf", image_directory.c_str())); 

}


