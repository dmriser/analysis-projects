#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"

void quickPlotter(){

  std::string inputFilename("test.root"); 
  TFile *inputFile = TFile::Open(inputFilename.c_str()); 
  TTree *events    = (TTree*) inputFile->Get("events"); 

  // Define histograms 
  TH2F *hitmapPositives    = new TH2F(   "hitmapPositives",    "hitmapPositives", 400, 50, 550, 400, -280, 280); 
  TH2F *hitmapZeros        = new TH2F(       "hitmapZeros",        "hitmapZeros", 400, 50, 550, 400, -280, 280); 
  TH2F *hitmapPositivesCut = new TH2F("hitmapPositivesCut", "hitmapPositivesCut", 400, 50, 550, 400, -280, 280); 
  TH2F *hitmapZerosCut     = new TH2F(    "hitmapZerosCut",     "hitmapZerosCut", 400, 50, 550, 400, -280, 280); 

  // Fill 
  events->Draw("tl3_y:tl3_x >> hitmapPositives", "", ""); 
  events->Draw("tl3_y_0:tl3_x_0 >> hitmapZeros", "", ""); 
  events->Draw("tl3_y:tl3_x >> hitmapPositivesCut", "tl3_x > 365", ""); 
  events->Draw("tl3_y_0:tl3_x_0 >> hitmapZerosCut", "tl3_x > 365", ""); 

  // Setup drawings 
  TCanvas *can = new TCanvas("can", "", 1200, 1200); 
  can->Divide(2,2);

  can->cd(1); 
  hitmapPositives->Draw("colz"); 
  gPad->SetLogz(); 
  can->cd(2); 
  hitmapZeros->Draw("colz"); 
  gPad->SetLogz(); 
  can->cd(3); 
  hitmapPositivesCut->Draw("colz"); 
  gPad->SetLogz(); 
  can->cd(4); 
  hitmapZerosCut->Draw("colz"); 
  gPad->SetLogz(); 
 
}
