#include <iostream>

#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TTree.h"

namespace constants {
  const std::string input_filename("../reset_pid.root");
  const static int n_bins = 80;
  const float p_min = 0.0; 
  const float p_max = 5.5; 
}

void write_hadron_fraction_parameters () {

  TFile *input_file = TFile::Open( constants::input_filename.c_str() );
  TTree *events = (TTree*) input_file->Get("events");
  std::cout << "Opened tree with " << events->GetEntries() << " entries." << std::endl; 

  TH1F *pion, *kaon, *proton, *total; 
  pion = new TH1F("pion", "pion", constants::n_bins, constants::p_min, constants::p_max);
  kaon = new TH1F("kaon", "kaon", constants::n_bins, constants::p_min, constants::p_max);
  proton = new TH1F("proton", "proton", constants::n_bins, constants::p_min, constants::p_max);
  total = new TH1F("total", "total", constants::n_bins, constants::p_min, constants::p_max);
  
  // Fill histograms using the draw trick. 
  events->Draw("hadron_momentum >> pion", "hadron_true == 211", "");
  events->Draw("hadron_momentum >> kaon", "hadron_true == 321", "");
  events->Draw("hadron_momentum >> proton", "hadron_true == 2212", "");
  events->Draw("hadron_momentum >> total", "hadron_true != 0", "");

  pion->Divide(total);
  kaon->Divide(total);  
  proton->Divide(total);

  std::cout << "PION_FRACTION " << constants::n_bins << " V ";
  for (int ibin = 1; ibin < pion->GetNbinsX(); ibin++){
    std::cout << pion->GetBinContent(ibin) << "    "; 
  }
  std::cout << std::endl; 

  std::cout << "PION_FRACTION " << constants::n_bins << " E ";
  for (int ibin = 1; ibin < pion->GetNbinsX(); ibin++){
    std::cout << 0.0 << "    "; 
  }
  std::cout << std::endl; 

  std::cout << "KAON_FRACTION " << constants::n_bins << " V ";
  for (int ibin = 1; ibin < kaon->GetNbinsX(); ibin++){
    std::cout << kaon->GetBinContent(ibin) << "    "; 
  }
  std::cout << std::endl; 

  std::cout << "KAON_FRACTION " << constants::n_bins << " E ";
  for (int ibin = 1; ibin < kaon->GetNbinsX(); ibin++){
    std::cout << 0.0 << "    "; 
  }
  std::cout << std::endl; 

  std::cout << "PROTON_FRACTION " << constants::n_bins << " V ";
  for (int ibin = 1; ibin < proton->GetNbinsX(); ibin++){
    std::cout << proton->GetBinContent(ibin) << "    "; 
  }
  std::cout << std::endl; 

  std::cout << "PROTON_FRACTION " << constants::n_bins << " E ";
  for (int ibin = 1; ibin < proton->GetNbinsX(); ibin++){
    std::cout << 0.0 << "    "; 
  }
  std::cout << std::endl; 

  TCanvas *canvas = new TCanvas("canvas", "canvas", 1600, 500);
  canvas->Divide(3,1);
  
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  canvas->cd(1);
  pion->Draw();

  canvas->cd(2);
  kaon->Draw();

  canvas->cd(3);
  proton->Draw();

  TFile *output_file = new TFile("hadron_fraction_parameters.root", "recreate");
  pion->Write();
  kaon->Write();
  proton->Write();
  output_file->Close();
}
