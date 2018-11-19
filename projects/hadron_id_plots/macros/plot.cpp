#include "TCanvas.h"
#include "TFile.h"
#include "TH2.h"
#include "TTree.h"

void plot(){

  const std::string input_filename("../this.root");
  const std::string output_directory("./"); 
  const std::string dvz_cut("dvz > -4 && dvz < 4");

  TFile *input_file = TFile::Open(input_filename.c_str()); 
  TTree *tree = (TTree*) input_file->Get("events"); 

  // These histograms 
  TH2F *beta_p_before, *beta_p_after; 
  TH2F *diff; 
  beta_p_before = new TH2F("beta_p_before", "beta_p_before", 
			   200, 0, 6, 200, 0.2, 1.2);
  beta_p_after = new TH2F("beta_p_after", "beta_p_after", 
			   200, 0, 6, 200, 0.2, 1.2);
  diff = new TH2F("diff", "diff", 
		   200, 0, 6, 200, 0.2, 1.2);  

  TH1F *w_before, *w_after; 
  w_before = new TH1F("w_before", "w_before", 200, 0.5, 1.5); 
  w_after = new TH1F("w_after", "w_after", 200, 0.5, 1.5); 
  w_after->SetFillColorAlpha(99, 0.8); 
  w_after->SetLineColor(99); 

  tree->Draw("beta:p >> beta_p_before", "", "colz"); 
  tree->Draw("beta:p >> beta_p_after", dvz_cut.c_str(), "colz"); 

  tree->Draw("w >> w_before", "", ""); 
  tree->Draw("w >> w_after", dvz_cut.c_str(), ""); 

  diff = (TH2F*) beta_p_before->Clone(); 
  diff->Add(beta_p_after, -1); 
  //  diff->Divide(beta_p_before); 
  diff->SetName("diff"); 

  TCanvas *canvas = new TCanvas("canvas", "", 1600, 1600); 
  canvas->Divide(2,2);

  gStyle->SetOptStat(0); 
  gStyle->SetOptTitle(0); 

  canvas->cd(1); 
  beta_p_before->Draw("colz");
  gPad->SetLogz(); 

  canvas->cd(2); 
  beta_p_after->Draw("colz");
  gPad->SetLogz(); 

  canvas->cd(3);
  diff->Draw("colz");
  gPad->SetLogz(); 

  TCanvas *canvas2 = new TCanvas("canvas2", "", 800, 600); 

  w_before->Draw(); 
  w_after->Draw("same"); 
}
