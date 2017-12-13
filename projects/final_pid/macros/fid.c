#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TLatex.h"

void fid(){

  std::string imagePath("/volatile/clas12/dmriser/plots/pid/hid/");

  TFile *histFile = TFile::Open("/volatile/clas12/dmriser/farm_out/final_pid_pass11/pid.root"); 
  const int NSECT  = 7; 

  double height = 10.0; 
  double angle  = 60.0; 
  double slope  = 1/tan(0.5*(3.14159/180.0)*angle);

  TF1 upper("upper","(x-[0])/[1]",0,60);
  upper.SetParameter(0, height);
  upper.SetParameter(1, slope);
  upper.SetLineWidth(2);
  upper.SetLineColor(99);

  TF1 lower("lower","-1*(x-[0])/[1]",0,60);
  lower.SetParameter(0, height);
  lower.SetParameter(1, slope);
  lower.SetLineWidth(2);
  lower.SetLineColor(99);

  TH2F *xy[NSECT];
  for(int isect=0; isect<NSECT; isect++){
    xy[isect] = (TH2F*) histFile->Get(Form("MesonHistograms/h2_dcx_dcy_%d_211_no_cut", isect));
    printf("loading (%d) \n", isect); 
  }

  TLatex tit, xtit, ytit;
  tit.SetNDC();
  tit.SetTextFont(102);
  tit.SetTextSize(0.03);
  xtit.SetNDC();
  xtit.SetTextFont(102);
  xtit.SetTextSize(0.05);
  ytit.SetNDC();
  ytit.SetTextFont(102);
  ytit.SetTextSize(0.05);
  ytit.SetTextAngle(90.0);  

  TCanvas *c1 = new TCanvas("c1","",800,1100);
  c1->Divide(2,3);

  double xtitx = 0.0;
  double xtity = 0.0;

  for(int s=0; s<6; s++){
    c1->cd(s+1);

    xy[s+1]   ->Draw("colz"); 
    gPad->SetLogz();
    upper.Draw("same"); 
    lower.Draw("same");

    if(s==0 || s==2 || s==4){
      gPad->SetLeftMargin(0.2);
      gPad->SetRightMargin(0.05);
      ytit.DrawLatex(0.1,0.48,"y (cm)");
      xtitx = 0.66;
    }

    else {
      gPad->SetRightMargin(0.2);
      gPad->SetLeftMargin(0.05);

      xtitx = 0.52;
    }

    if(s==0 || s==1){
      gPad->SetTopMargin(0.2);
      gPad->SetBottomMargin(0.05);
      xtity = 0.68;
    }
    else if (s==4 || s==5){
      gPad->SetTopMargin(0.05);
      gPad->SetBottomMargin(0.2);
      xtit.DrawLatex(0.46,0.1,"x (cm)");
      xtity = 0.81;
    }
    else{
      gPad->SetTopMargin(0.05);
      gPad->SetBottomMargin(0.05);
      xtity = 0.81;
    }
  }

  c1->cd();
  tit.DrawLatex(0.1, 0.95, "DC Region 1 Fiducial Cuts for Postives");
  c1->Print(Form("%sfid.pdf",imagePath.c_str()));

}
