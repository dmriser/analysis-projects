#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TLatex.h"

void testY(){

  TFile *file[5];
  file[0] = TFile::Open("0.7.root");
  file[1] = TFile::Open("0.75.root");
  file[2] = TFile::Open("0.8.root");
  file[3] = TFile::Open("0.85.root");
  file[4] = TFile::Open("0.9.root");

  TFile *modelFile = TFile::Open("../out/crossSectionPass1WithResonance.root");

  TH2I *wByQQ[5];
  TH1D *projection[5]; 
  TH1D *model[3];
  TH1D *dQhist;

  dQhist = (TH1D*) file[0]->Get("totalCharge");
  model[0] = (TH1D*) modelFile->Get("modelCrossSection_wByQQ_s1_slice15");
  model[1] = (TH1D*) modelFile->Get("modelCrossSection_wByQQ_s1_slice22");
  model[2] = (TH1D*) modelFile->Get("modelCrossSection_wByQQ_s1_slice30");
  
  double avogadro = 6.022e23; 
  double molarMass = 1.0;
  double density = 0.0708;  
  double cm2ToMicroBarns = 1e30;
  double electronCharge = 1.602e-13;
  double lumi = (avogadro*density*dQhist->GetBinContent(1))/(electronCharge*molarMass);
  lumi /= cm2ToMicroBarns;

  model[0]->Scale(lumi);
  model[1]->Scale(lumi);
  model[2]->Scale(lumi);

  cout << " >>> Integrated luminosity =" << lumi << "s^-1 uB^-1" << endl;

  const int numberFiles = 5;
  Double_t max = 0.0;

  for (int f=0; f<numberFiles; f++){
    wByQQ[f] = (TH2I*) file[f]->Get("dataEvents_wByQQ_s0");
    projection[f] = wByQQ[f]->ProjectionX();
    projection[f]->SetName(Form("projection%d",f));
    projection[f]->SetTitle(Form("projection%d",f));

    if (projection[f]->GetMaximum() > max){ max = projection[f]->GetMaximum(); }
  }


  TCanvas *c1 = new TCanvas("c1","",1000,500);
  TLatex lab, caption;
  lab.SetNDC();
  lab.SetTextSize(0.025);

  caption.SetNDC();
  caption.SetTextSize(0.045);
  caption.SetTextColor(kBlack);

  Int_t lineBase = 50;
  Int_t lineStep = 3; 
  for (int f=0; f<numberFiles; f++){
    projection[f]->SetMaximum(max); 
    projection[f]->SetLineColor(lineBase+f*lineStep);
  }

  projection[0]->Draw();
  projection[1]->Draw("same");
  projection[2]->Draw("same");
  projection[3]->Draw("same");
  projection[4]->Draw("same");

  for (int f=0; f<numberFiles; f++){
    lab.SetTextColor(lineBase+f*lineStep);
    lab.DrawLatex(0.18, 0.675+0.03*f,Form("y < %.3f",0.05*f+0.7));
  }
  
  caption.DrawLatex(0.78,0.88,"y = 1 - #frac{E'}{E}");
  caption.DrawLatex(0.78,0.07,"W [GeV/c^{2}]");
  caption.SetTextAngle(90.0);
  caption.DrawLatex(0.10,0.7,"Counts");

  c1->Print("yCuts.png");

  TCanvas *modelCanvas = new TCanvas("modelCanvas","",1000,500);

  double modelMax = 0.0;
  for (int f=0; f<3; f++){
    if (model[f]->GetMaximum() > modelMax){ modelMax = model[f]->GetMaximum(); }
  }
  
  for (int f=0; f<3; f++){
    model[f]->SetLineColor(lineBase+f*lineStep);
    model[f]->SetMaximum(modelMax); 
  }

  model[0]->Draw();
  model[1]->Draw("same");
  model[2]->Draw("same");

  for (int f=0; f<3; f++){
    lab.SetTextColor(lineBase+f*lineStep);
    lab.DrawLatex(0.18, 0.675+0.03*f,Form("Q^{2} bin %d",f));
  }

}
