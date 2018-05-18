#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "TCanvas.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLine.h"

#include "utils.C"

void thesisPlots(){

  TFile * file = TFile::Open("/volatile/clas12/dmriser/farm_out/corrections_pass0/corrections.root");
  //  TFile * file = TFile::Open("PID.root");
  TCanvas * c1 = new TCanvas("c1", "", 1000, 900);

  std::string imagePath("/volatile/clas12/dmriser/plots/corrections/"); 
  //  std::string parFile("/u/home/dmriser/clas/analysis-main/lists/parameters/data/final.pars");

  // --------------------------------------------
  //    Styling options 
  //  -------------------------------------------- 
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetTextFont(42);
  gStyle->SetTextSize(0.05);
  gStyle->SetLabelSize(0.05);
  gStyle->SetPalette(kViridis);

  const static int NSECT =  7;

  string xAxisTitle = "x";
  string yAxisTitle = "y";

  float xPosFrac = 0.585; 
  float yPosFrac = 0.95; 

  float xPosCut = 0.2; 
  float yPosCut = 0.87; 
 
  TLatex lab, xlabel, ylabel; 
  lab.SetNDC();
  lab.SetTextFont(42);
  lab.SetTextSize(0.08);

  xlabel.SetNDC();
  xlabel.SetTextFont(42);
  xlabel.SetTextSize(0.03);

  ylabel.SetNDC();
  ylabel.SetTextFont(42);
  ylabel.SetTextSize(0.03);
  ylabel.SetTextAngle(90.0);

  // 1-D

  // 2-D
  TH2D * h2_vz_phi[2][NSECT];
  TH2D * h2_theta_dw[2][NSECT];
  TH2D * h2_phi_dw[2][NSECT];
  TH2D * h2_p_dbeta[2];
      
  string printer = "";
                        
  // --------------------------------------------
  //    Load the histograms from the file 
  //  -------------------------------------------- 
  for(int isect = 0; isect < 7; isect++){
    h2_vz_phi[0][isect] = (TH2D*) file->Get(Form("CorrectionHistograms/h2_vz_phi_uncorr_%d_test", isect));
    h2_vz_phi[1][isect] = (TH2D*) file->Get(Form("CorrectionHistograms/h2_vz_phi_corr_%d_test", isect));

    h2_theta_dw[0][isect] = (TH2D*) file->Get(Form("CorrectionHistograms/h2_theta_dw_uncorr_%d_test", isect));
    h2_theta_dw[1][isect] = (TH2D*) file->Get(Form("CorrectionHistograms/h2_theta_dw_corr_%d_test", isect));

    h2_phi_dw[0][isect] = (TH2D*) file->Get(Form("CorrectionHistograms/h2_phi_dw_uncorr_%d_test", isect));
    h2_phi_dw[1][isect] = (TH2D*) file->Get(Form("CorrectionHistograms/h2_phi_dw_corr_%d_test", isect));
  }

  h2_p_dbeta[0] = (TH2D*) file->Get("CorrectionHistograms/h2_p_dbeta_uncorr_24_1_test");
  h2_p_dbeta[1] = (TH2D*) file->Get("CorrectionHistograms/h2_p_dbeta_corr_24_1_test");

  // --------------------------------------------
  //    Draw the momentum corrections for phi 
  //  -------------------------------------------- 
  c1->Divide(2,3);
  TLegend *legend[6];
 
  TLine *zeroDwLine = new TLine(-30, 0, 30, 0);
  zeroDwLine->SetLineStyle(8);

  for (int isect=1; isect<7; isect++){
    c1->cd(isect);
    gPad->SetMargin(0.15,0.15,0.15,0.15);

    h2_phi_dw[0][isect]->Draw("colz");
    gPad->SetLogz(); 
    lab.DrawLatex(0.33, 0.03, "#phi_{Rel} [Degrees]");
    lab.SetTextAngle(90.0); 
    lab.DrawLatex(0.05, 0.20, "#Delta W [GeV/c^{2}]");
    lab.SetTextAngle(0.0); 
    zeroDwLine->Draw();
  }
  c1->Print(Form("%sphi-dw-uncorr.pdf", imagePath.c_str()));
  c1->Clear(); 

  // ---------------------------------
  c1->Divide(2,3);
  for (int isect=1; isect<7; isect++){
    c1->cd(isect);
    gPad->SetMargin(0.15,0.15,0.15,0.15);

    h2_phi_dw[1][isect]->Draw("colz");
    gPad->SetLogz(); 
    lab.DrawLatex(0.33, 0.03, "#phi_{Rel} [Degrees]");
    lab.SetTextAngle(90.0); 
    lab.DrawLatex(0.05, 0.20, "#Delta W [GeV/c^{2}]");
    lab.SetTextAngle(0.0); 
    zeroDwLine->Draw();
  }
  c1->Print(Form("%sphi-dw-corr.pdf", imagePath.c_str()));
  c1->Clear(); 
 
  // --------------------------------------------
  //    Draw the momentum corrections for theta  
  //  -------------------------------------------- 
  c1->Divide(2,3);
 
  zeroDwLine = new TLine(5, 0, 30, 0);
  zeroDwLine->SetLineStyle(8);

  for (int isect=1; isect<7; isect++){
    c1->cd(isect);
    gPad->SetMargin(0.15,0.15,0.15,0.15);

    h2_theta_dw[0][isect]->Draw("colz");
    gPad->SetLogz(); 
    lab.DrawLatex(0.33, 0.03, "#theta [Degrees]");
    lab.SetTextAngle(90.0); 
    lab.DrawLatex(0.05, 0.20, "#Delta W [GeV/c^{2}]");
    lab.SetTextAngle(0.0); 
    zeroDwLine->Draw();
  }
  c1->Print(Form("%stheta-dw-uncorr.pdf", imagePath.c_str()));
  c1->Clear(); 

  c1->Divide(2,3);
 
  zeroDwLine = new TLine(5, 0, 30, 0);
  zeroDwLine->SetLineStyle(8);

  for (int isect=1; isect<7; isect++){
    c1->cd(isect);
    gPad->SetMargin(0.15,0.15,0.15,0.15);

    h2_theta_dw[1][isect]->Draw("colz");
    gPad->SetLogz(); 
    lab.DrawLatex(0.33, 0.03, "#theta [Degrees]");
    lab.SetTextAngle(90.0); 
    lab.DrawLatex(0.05, 0.20, "#Delta W [GeV/c^{2}]");
    lab.SetTextAngle(0.0); 
    zeroDwLine->Draw();
  }
  c1->Print(Form("%stheta-dw-corr.pdf", imagePath.c_str()));
  c1->Clear(); 

  
  // --------------------------------------------
  //    Draw the timing correction 
  //  -------------------------------------------- 
  TCanvas *c2 = new TCanvas("c2", "c2", 1000, 350);
  c2->Divide(2,1);

  TLine *zeroDBetaLine = new TLine(0.0, 0.0, 4.5, 0.0);
  zeroDBetaLine->SetLineStyle(8);

  c2->cd(1);
  gPad->SetMargin(0.15,0.15,0.15,0.15);
  h2_p_dbeta[0]->Draw("colz");
  gPad->SetLogz();
  zeroDBetaLine->Draw();
  lab.DrawLatex(0.33, 0.03, "P [GeV/c]");
  lab.SetTextAngle(90.0); 
  lab.DrawLatex(0.05, 0.20, "#Delta #beta = #beta_{obs} - #beta_{pred}");
  lab.SetTextAngle(0.0); 
  
  c2->cd(2);
  gPad->SetMargin(0.15,0.15,0.15,0.15);
  h2_p_dbeta[1]->Draw("colz");
  gPad->SetLogz();
  zeroDBetaLine->Draw();
  lab.DrawLatex(0.33, 0.03, "P [GeV/c]");
  lab.SetTextAngle(90.0); 
  lab.DrawLatex(0.05, 0.20, "#Delta #beta = #beta_{obs} - #beta_{pred}");
  lab.SetTextAngle(0.0); 

  c2->Print(Form("%stiming.pdf", imagePath.c_str()));
  c2->Clear(); 

  // --------------------------------------------
  //    Draw the vertex correction 
  //  -------------------------------------------- 
  c2->Divide(2,1);

  TLine *leftVertex  = new TLine(-27.73, -30, -27.73, 330);
  TLine *rightVertex = new TLine(-22.68, -30, -22.68, 330);

  leftVertex->SetLineStyle(8);
  leftVertex->SetLineColor(99);
  leftVertex->SetLineWidth(2);
  rightVertex->SetLineStyle(8);
  rightVertex->SetLineColor(99);
  rightVertex->SetLineWidth(2);

  c2->cd(1);
  gPad->SetMargin(0.15,0.15,0.15,0.15);
  h2_vz_phi[0][0]->Draw("colz");
  gPad->SetLogz();

  lab.DrawLatex(0.45, 0.03, "v_{z}");
  lab.SetTextAngle(90.0); 
  lab.DrawLatex(0.05, 0.45, "#phi");
  lab.SetTextAngle(0.0); 
  
  leftVertex->Draw();
  rightVertex->Draw();

  c2->cd(2);
  gPad->SetMargin(0.15,0.15,0.15,0.15);
  h2_vz_phi[1][0]->Draw("colz");
  gPad->SetLogz();

  lab.DrawLatex(0.45, 0.03, "v_{z}");
  lab.SetTextAngle(90.0); 
  lab.DrawLatex(0.05, 0.45, "#phi");
  lab.SetTextAngle(0.0); 

  leftVertex->Draw();
  rightVertex->Draw();

  c2->Print(Form("%svertex-phi.pdf", imagePath.c_str()));
  c2->Clear(); 

}
