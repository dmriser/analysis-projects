#include <iostream>

#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"

void plot_ratio(){

  const std::string input_filename("testing_this_dirty_dingus.root");

  TFile *input_file = TFile::Open(input_filename.c_str());
  TTree *events = (TTree*) input_file->Get("events");
  std::cout << "Loaded " << events->GetEntries() << " events" << std::endl; 

  const static int n_p_bins = 100; 
  const float p_min = 0.3; 
  const float p_max = 4.5; 

  TH1F *p_pion = new TH1F("p_pion", "p_pion", n_p_bins, p_min, p_max);
  TH1F *p_kaon = new TH1F("p_kaon", "p_kaon", n_p_bins, p_min, p_max);
  TH1F *p_proton = new TH1F("p_proton", "p_proton", n_p_bins, p_min, p_max);
  TH1F *frac_pion = new TH1F("frac_pion", "frac_pion", n_p_bins, p_min, p_max);
  TH1F *frac_kaon = new TH1F("frac_kaon", "frac_kaon", n_p_bins, p_min, p_max);
  TH1F *frac_proton = new TH1F("frac_proton", "frac_proton", n_p_bins, p_min, p_max);
  TH1F *p_total = new TH1F("p_total", "p_total", n_p_bins, p_min, p_max);

  events->Draw("hadron_momentum >> p_pion", "hadron_pid == 211", "");
  events->Draw("hadron_momentum >> p_kaon", "hadron_pid == 321", "");
  events->Draw("hadron_momentum >> p_proton", "hadron_pid == 2212", "");

  p_total->Add(p_pion);
  p_total->Add(p_kaon);  
  p_total->Add(p_proton);
  
  frac_pion->Divide(p_pion, p_total); 
  frac_kaon->Divide(p_kaon, p_total); 
  frac_proton->Divide(p_proton, p_total); 

  TCanvas *canvas = new TCanvas("canvas", "canvas", 1600, 1000);
  canvas->Divide(4,2);

  canvas->cd(1);
  p_pion->Draw(); 

  canvas->cd(2);
  p_kaon->Draw(); 

  canvas->cd(3);
  p_proton->Draw(); 

  canvas->cd(4);
  p_total->Draw(); 

  canvas->cd(5);
  frac_pion->Draw(); 

  canvas->cd(6);
  frac_kaon->Draw(); 

  canvas->cd(7);
  frac_proton->Draw(); 
}
