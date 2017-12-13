#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"

void confidence_levels(){

  // ---------------------------------------------------
  //    user input 
  // ---------------------------------------------------
  std::string imagePath("/volatile/clas12/dmriser/plots/pid/hid/"); 
  std::string inputFile("../out/test_dingus.root"); 
  //  std::string inputFile("/volatile/clas12/dmriser/farm_out/final_pid_pass12/pid.root"); 

  // ---------------------------------------------------
  //    
  // ---------------------------------------------------
  const int NPART         = 3; 
  const int pid[NPART]    = {211, 321, 2212};
  std::string name[NPART] = {"pi+", "k+", "proton"};

  TFile *rootFile = TFile::Open(inputFile.c_str());
  TTree *events   = (TTree*) rootFile->Get("events");

  TH1F *conf_level[NPART];
  for(int p=0; p<NPART; p++){
    conf_level[p] = new TH1F(Form("conf_level_%d", pid[p]), "", 100, -0.05, 1.05);
  }

  // ---------------------------------------------------
  //    
  // ---------------------------------------------------
  for(int p=0; p<NPART; p++){
    events->Draw(Form("cl >> conf_level_%d", pid[p]), Form("pid == %d", pid[p]), "");
  }
  
  TLatex *latex = new TLatex(); 
  latex->SetNDC();
  latex->SetTextFont(102);
  latex->SetTextSize(0.04); 

  TCanvas *can = new TCanvas("can", "", 1200, 500); 
  can->Divide(3,1); 
  for(int p=0; p<NPART; p++){
    can->cd(p+1); 

    conf_level[p]->SetLineColor(kBlack);
    conf_level[p]->SetFillColor(kBlack);
    conf_level[p]->SetFillStyle(3004); 
    conf_level[p]->Draw(); 
    gPad->SetLogy();
    gPad->SetMargin(0.1, 0.12, 0.1, 0.12); 

    latex->DrawLatex(0.48, 0.02, "#alpha"); 
    latex->DrawLatex(0.46, 0.92, name[p].c_str()); 
  }
  
  can->Print(Form("%sconfidence_level.pdf", imagePath.c_str()));
}



