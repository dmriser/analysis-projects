#include <iostream>

#include "TCanvas.h"
#include "TFile.h"
#include "TH2.h"
#include "TLatex.h"
#include "TTree.h"

void createPlot(){


  gStyle->SetOptStat(0); 
  gStyle->SetOptTitle(0); 

  std::cout << "Starting createPlot()..." << std::endl; 

  std::string inputFilename("testing-electron.root");

  TFile *inputFile = TFile::Open(inputFilename.c_str());
  TTree *tree = (TTree*) inputFile->Get("events"); 

  TCanvas *canvas = new TCanvas("canvas", "", 800, 600); 

  TH2F *plot = new TH2F("plot", "plot", 400, 0, 3.5, 400, 0, 7);
  tree->Draw("q2:w >> plot", "", "colz"); 

  canvas->SetLogz(); 

  TLatex *latex = new TLatex(); 
  latex->SetNDC(); 
  latex->SetTextFont(62); 
  latex->SetTextSize(0.03); 

  latex->DrawLatex(0.35, 0.925, "E1-F Inclusive Electrons"); 
  latex->DrawLatex(0.75, 0.03, "W [GeV/c^{2}]"); 
  latex->SetTextAngle(90.0); 
  latex->DrawLatex(0.04, 0.75, "Q^{2} [GeV/c]^{2}"); 


  canvas->Print("xQQ.pdf");
}
