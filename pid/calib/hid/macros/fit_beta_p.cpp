#include <iostream>

#include "TStopwatch.h"

namespace constants {
  const std::string input_filename("../testing.root");
  const std::string output_directory("/volatile/clas12/dmriser/plots/pid");
  const static int n_sectors = 7;
  const static int n_slices = 20;
  const static float p_min = 0.5; 
  const static float p_max = 2.5; 
  const static float pion_mass = 0.139; 
  const static float kaon_mass = 0.493; 
  const static float proton_mass = 0.938; 
};

char * create_histogram_name(int sector){
  return Form("MesonHistograms/h2_p_beta_%d_pip", sector);
}

float beta_theory(float p, float theory_mass){
  return 1 / sqrt( 1 + pow(theory_mass / p, 2) );
}

void fit_beta_p(){

  std::cout << "Beginning fitting of beta vs. p for positive hadrons" << std::endl; 
  TStopwatch *timer = new TStopwatch();
  
  // Setup input file 
  TFile *input_file = TFile::Open(constants::input_filename.c_str());

  // Declare and load histograms from file. 
  TH2D *p_beta[constants::n_sectors];
  for (int isect = 0; isect < constants::n_sectors; isect++){
    p_beta[isect] = (TH2D*) input_file->Get( create_histogram_name(isect) );
    
    if ( !p_beta[isect] ){
      std::cerr << "Issues getting " << create_histogram_name(isect) << ", dying." << std::endl; 
      return;
    }
  }

  TH1D *slices[constants::n_sectors][constants::n_slices];
  TGraphErrors *graphs[constants::n_sectors][3];

  const static int magic_sector = 4; 

  int first_bin = p_beta[magic_sector]->GetXaxis()->FindBin(constants::p_min); 
  int last_bin = p_beta[magic_sector]->GetXaxis()->FindBin(constants::p_max); 
  int n_bins_in_range = last_bin - first_bin; 
  int n_slices = (constants::n_slices > n_bins_in_range ? n_bins_in_range : constants::n_slices);

  float slice_width = (constants::p_max - constants::p_min) / n_slices; 

  std::vector<double> par_0; 
  std::vector<double> error_0; 
  std::vector<double> par_1; 
  std::vector<double> error_1; 
  std::vector<double> par_2;
  std::vector<double> error_2; 
  std::vector<double> p_axis;
  std::vector<double> p_error;
 
  for (int islice = 0; islice < n_slices; islice++){
    std::string slice_title( Form("sect%d_slice%d", magic_sector, islice) );
    std::string fit_title( Form("sect%d_fit%d", magic_sector, islice) );

    float p_value = islice * slice_width + constants::p_min; 
    float p_mid = p_value + slice_width / 2.0; 

    float pion_peak = beta_theory(p_mid, constants::pion_mass); 
    float kaon_peak = beta_theory(p_mid, constants::kaon_mass);     
    float proton_peak = beta_theory(p_mid, constants::proton_mass); 

    // Create fitting function. 
    std::string fit_function(Form("[0] * exp(-0.5 * ((x - %.3f)/0.015)^2) + [1] * exp(-0.5 * ((x - %.3f)/0.015)^2) + [2] * exp(-0.5 * ((x - %.3f)/0.015)^2)", 
				  pion_peak, kaon_peak, proton_peak) );
    
    int start_bin = p_beta[magic_sector]->GetXaxis()->FindBin(p_value);
    int stop_bin = p_beta[magic_sector]->GetXaxis()->FindBin(p_value + slice_width);

    TH1D *h = p_beta[magic_sector]->ProjectionY( slice_title.c_str(), start_bin, stop_bin);
    h->SetTitle(slice_title.c_str());
    h->SetName(slice_title.c_str());

    // The numbers are dummy ranges. 
    TF1 f(fit_title.c_str(), fit_function.c_str(), 0, 1.2);

    // The particle widths 
    //    f.SetParameter(1, 0.02); 
    //    f.SetParameter(3, 0.02); 
    //    f.SetParameter(5, 0.02); 
    h->Fit(fit_title.c_str(), "r");

    double total = f.GetParameter(0) + f.GetParameter(1) + f.GetParameter(2); 
    p_axis.push_back(p_mid);
    p_error.push_back(0.0);
    par_0.push_back( f.GetParameter(0) / total ); 
    par_1.push_back( f.GetParameter(1) / total ); 
    par_2.push_back( f.GetParameter(2) / total ); 
    error_0.push_back( f.GetParError(0) / total );
    error_1.push_back( f.GetParError(1) / total );
    error_2.push_back( f.GetParError(2) / total );

    slices[magic_sector][islice] = h;
  }

  graphs[magic_sector][0] = new TGraphErrors(p_axis.size(), p_axis.data(), par_0.data(), p_error.data(), error_0.data()); 
  graphs[magic_sector][1] = new TGraphErrors(p_axis.size(), p_axis.data(), par_1.data(), p_error.data(), error_1.data()); 
  graphs[magic_sector][2] = new TGraphErrors(p_axis.size(), p_axis.data(), par_2.data(), p_error.data(), error_2.data()); 
  
  TCanvas *canvas = new TCanvas("canvas", "canvas", 1600, 1600);
  canvas->Divide(5,5); 

  for (int islice = 0; islice < n_slices; islice++){
    canvas->cd(islice + 1);
    slices[magic_sector][islice]->Draw(); 
  }

  TCanvas *canvas2 = new TCanvas("canvas2", "canvas2", 1600, 500);
  canvas2->Divide(3,1);

  canvas2->cd(1);
  graphs[magic_sector][0]->Draw(); 

  canvas2->cd(2);
  graphs[magic_sector][1]->Draw(); 

  canvas2->cd(3);
  graphs[magic_sector][2]->Draw(); 

  // Summarize performance of this macro. 
  float elapsed_time = timer->RealTime();
  std::cout << "Elapsed time: " << elapsed_time << " seconds. " << std::endl; 
}
