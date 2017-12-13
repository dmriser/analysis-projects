#ifndef signal_h
#define signal_h

#include "Bins.h"
#include "Constants.h"
#include "PIDHistograms.h"
#include "Fits.h"
#include "Histograms.h"
#include "IntegratedHistograms.h"

#include "TF1.h"
#include "TFitResult.h"
#include "TH1.h"
#include "TMatrixD.h"

class SignalBackgroundFitter { 

 public:
 SignalBackgroundFitter(PidHistos *h, std::string name, int index) : fHistos(h), fName(name), fMesonIndex(index) {
    fBins = Bins::GetInstance(fMesonIndex);
    Init(); 
    fInputFile = new TFile(); 
  }

 SignalBackgroundFitter(std::string name, int index) : fName(name), fMesonIndex(index) {
    fBins = Bins::GetInstance(fMesonIndex);
    Init(); 
    fInputFile = new TFile(); 
  }

  ~SignalBackgroundFitter(){
    if(fInputFile){
      if (fInputFile->IsOpen()){
	fInputFile->Close(); 
      }
    }
  }
  
  double signal_p[constants::MAX_BINS_X][constants::MAX_BINS_Q2][constants::MAX_BINS_Z][constants::MAX_BINS_PT];
  double signal_k[constants::MAX_BINS_X][constants::MAX_BINS_Q2][constants::MAX_BINS_Z][constants::MAX_BINS_PT];
  double err_p[constants::MAX_BINS_X][constants::MAX_BINS_Q2][constants::MAX_BINS_Z][constants::MAX_BINS_PT];
  double err_k[constants::MAX_BINS_X][constants::MAX_BINS_Q2][constants::MAX_BINS_Z][constants::MAX_BINS_PT];
  double background_p[constants::MAX_BINS_X][constants::MAX_BINS_Q2][constants::MAX_BINS_Z][constants::MAX_BINS_PT];
  double background_k[constants::MAX_BINS_X][constants::MAX_BINS_Q2][constants::MAX_BINS_Z][constants::MAX_BINS_PT];

  TF1 *fit_p[constants::MAX_BINS_X][constants::MAX_BINS_Q2][constants::MAX_BINS_Z][constants::MAX_BINS_PT];
  TF1 *fit_k[constants::MAX_BINS_X][constants::MAX_BINS_Q2][constants::MAX_BINS_Z][constants::MAX_BINS_PT];
  TF1 *fit_bg[constants::MAX_BINS_X][constants::MAX_BINS_Q2][constants::MAX_BINS_Z][constants::MAX_BINS_PT];
  TF1 *fit_tot[constants::MAX_BINS_X][constants::MAX_BINS_Q2][constants::MAX_BINS_Z][constants::MAX_BINS_PT];

  // pion signal 
  TH1D *h1_x_signal_p[constants::MAX_BINS_Q2][constants::MAX_BINS_Z][constants::MAX_BINS_PT];
  TH1D *h1_q2_signal_p[constants::MAX_BINS_Z][constants::MAX_BINS_PT][constants::MAX_BINS_X];
  TH1D *h1_z_signal_p[constants::MAX_BINS_PT][constants::MAX_BINS_X][constants::MAX_BINS_Q2];
  TH1D *h1_pt_signal_p[constants::MAX_BINS_X][constants::MAX_BINS_Q2][constants::MAX_BINS_Z];

  // kaon signal 
  TH1D *h1_x_signal_k[constants::MAX_BINS_Q2][constants::MAX_BINS_Z][constants::MAX_BINS_PT];
  TH1D *h1_q2_signal_k[constants::MAX_BINS_Z][constants::MAX_BINS_PT][constants::MAX_BINS_X];
  TH1D *h1_z_signal_k[constants::MAX_BINS_PT][constants::MAX_BINS_X][constants::MAX_BINS_Q2];
  TH1D *h1_pt_signal_k[constants::MAX_BINS_X][constants::MAX_BINS_Q2][constants::MAX_BINS_Z];

  double GetPionSignal(int x, int q2, int z, int pt) const {
    return signal_p[x][q2][z][pt];
  }

  double GetKaonSignal(int x, int q2, int z, int pt) const {
    return signal_k[x][q2][z][pt];
  }

  void Fit(double min, double cut, double max){

    for(int i=0; i<fBins->GetXBins()->GetNumber()+1; i++){
      for(int j=0; j<fBins->GetQ2Bins()->GetNumber()+1; j++){
	for(int k=0; k<fBins->GetZBins()->GetNumber()+1; k++){
	  for(int l=0; l<fBins->GetPtBins()->GetNumber()+1; l++){
	    fHistos->h1_tof_mass[i][j][k][l]->Fit(Form("fit_p_%s_x%d_q2%d_z%d_pt%d_%s", 
						       constants::Names::mesons[fMesonIndex].c_str(),
						       i, j, k, l, fName.c_str()), "RQ");
	    
	    fHistos->h1_tof_mass[i][j][k][l]->Fit(Form("fit_k_%s_x%d_q2%d_z%d_pt%d_%s", 
						       constants::Names::mesons[fMesonIndex].c_str(),
						       i, j, k, l, fName.c_str()), "RQ");
	    
	    fit_tot[i][j][k][l]->SetParameter(0, fit_p[i][j][k][l]->GetParameter(0)); 
	    fit_tot[i][j][k][l]->SetParameter(1, fit_p[i][j][k][l]->GetParameter(1)); 
	    fit_tot[i][j][k][l]->SetParameter(2, fit_p[i][j][k][l]->GetParameter(2)); 
	    fit_tot[i][j][k][l]->SetParameter(3, fit_k[i][j][k][l]->GetParameter(0)); 
	    fit_tot[i][j][k][l]->SetParameter(4, fit_k[i][j][k][l]->GetParameter(1)); 
	    fit_tot[i][j][k][l]->SetParameter(5, fit_k[i][j][k][l]->GetParameter(2)); 

	    TFitResultPtr resultOfFit = fHistos->h1_tof_mass[i][j][k][l]->Fit(Form("fit_tot_%s_x%d_q2%d_z%d_pt%d_%s", 
										   constants::Names::mesons[fMesonIndex].c_str(),
										   i, j, k, l, fName.c_str()), "RQS");
	    /*	  
	  TMatrixD covMatrix = resultOfFit->GetCovarianceMatrix(); 	  
	  TMatrixD covMatrixPion(3,3); 
	  TMatrixD covMatrixKaon(3,3); 
	 
	  // the first one is easy 
	  for (int row=0; row<3; row++){
	    for (int col=0; col<3; col++){
	      covMatrixPion[row][col] = covMatrix[row][col];
	    }
	  }

	  // the next one is also easy, but different
	  for (int row=0; row<3; row++){
	    for (int col=0; col<3; col++){
	      covMatrixKaon[row][col] = covMatrix[row+3][col+3];
	    }
	  }
	    */
	  fit_p[i][j][k][l] ->SetRange(0.0, 1.0); 
	  fit_k[i][j][k][l] ->SetRange(0.0, 1.0); 
	  fit_bg[i][j][k][l]->SetRange(0.0, 1.0); 
	  
	  fit_p[i][j][k][l] ->SetParameter(0, fit_tot[i][j][k][l]->GetParameter(0));
	  fit_p[i][j][k][l] ->SetParameter(1, fit_tot[i][j][k][l]->GetParameter(1));
	  fit_p[i][j][k][l] ->SetParameter(2, fit_tot[i][j][k][l]->GetParameter(2));
	  fit_k[i][j][k][l] ->SetParameter(0, fit_tot[i][j][k][l]->GetParameter(3));
	  fit_k[i][j][k][l] ->SetParameter(1, fit_tot[i][j][k][l]->GetParameter(4));
	  fit_k[i][j][k][l] ->SetParameter(2, fit_tot[i][j][k][l]->GetParameter(5));
	  fit_bg[i][j][k][l]->SetParameter(0, fit_tot[i][j][k][l]->GetParameter(6));
	  fit_bg[i][j][k][l]->SetParameter(1, fit_tot[i][j][k][l]->GetParameter(7));
	  fit_bg[i][j][k][l]->SetParameter(2, fit_tot[i][j][k][l]->GetParameter(8));
	  fit_bg[i][j][k][l]->SetParameter(3, fit_tot[i][j][k][l]->GetParameter(9));

	  // initialize the stuff to zero 
	  double p_below = fit_p[i][j][k][l]->Integral(min, cut); 
	  double p_above = fit_p[i][j][k][l]->Integral(cut, max); 
	  double k_below = fit_k[i][j][k][l]->Integral(min, cut); 
	  double k_above = fit_k[i][j][k][l]->Integral(cut, max); 
	  /*
	  double p_below_err = fit_p[i][j][k][l]->IntegralError(min, cut, fit_p[i][j][k][l]->GetParameters(), covMatrixPion.GetMatrixArray()); 
	  double p_above_err = fit_p[i][j][k][l]->IntegralError(cut, max, fit_p[i][j][k][l]->GetParameters(), covMatrixPion.GetMatrixArray()); 
	  double k_below_err = fit_k[i][j][k][l]->IntegralError(min, cut, fit_k[i][j][k][l]->GetParameters(), covMatrixKaon.GetMatrixArray()); 
	  double k_above_err = fit_k[i][j][k][l]->IntegralError(cut, max, fit_k[i][j][k][l]->GetParameters(), covMatrixKaon.GetMatrixArray()); 
	  */
	  signal_p[i][j][k][l]  = p_below/(p_below+k_below); 
	  signal_k[i][j][k][l]  = k_above/(p_above+k_above);
	  /*
	  err_p[i][j][k][l]        = sqrt(pow(p_below_err/(p_below+k_below),2) + pow(p_below*(p_below_err+k_below_err),2)/pow(p_below+k_below,4));
	  err_k[i][j][k][l]        = sqrt(pow(k_above_err/(p_above+k_above),2) + pow(k_above*(p_above_err+k_above_err),2)/pow(p_above+k_above,4));
	  */
	  background_p[i][j][k][l] = 1.0 - signal_k[i][j][k][l]; 
	  background_k[i][j][k][l] = 1.0 - signal_p[i][j][k][l]; 

	  }
	}
      }
    }

    for (int i=0; i<fBins->GetQ2Bins()->GetNumber()+1; i++){
      for (int j=0; j<fBins->GetZBins()->GetNumber()+1; j++){
	for(int k=0; k<fBins->GetPtBins()->GetNumber()+1; k++){

	  // iterate on x 
	  for(int b=1; b<fBins->GetXBins()->GetNumber()+1; b++){
	    h1_x_signal_p[i][j][k]->SetBinContent(b, signal_p[b][i][j][k]); 
	    //	    h1_x_signal_p[i][j][k]->SetBinError(b,   err_p[b][i][j][k]); 
	    h1_x_signal_k[i][j][k]->SetBinContent(b, signal_k[b][i][j][k]); 
	    //	    h1_x_signal_k[i][j][k]->SetBinError(b,   err_k[b][i][j][k]); 
	  }
	}
      }
    }

    for (int i=0; i<fBins->GetZBins()->GetNumber()+1; i++){
      for (int j=0; j<fBins->GetPtBins()->GetNumber()+1; j++){
	for(int k=0; k<fBins->GetXBins()->GetNumber()+1; k++){

	  // iterate on q2 
	  for(int b=1; b<fBins->GetQ2Bins()->GetNumber()+1; b++){
	    h1_q2_signal_p[i][j][k]->SetBinContent(b, signal_p[k][b][i][j]); 
	    //	    h1_q2_signal_p[i][j][k]->SetBinError(b,   err_p   [k][b][i][j]); 
	    h1_q2_signal_k[i][j][k]->SetBinContent(b, signal_k[k][b][i][j]); 
	    //	    h1_q2_signal_k[i][j][k]->SetBinError(b,   err_k   [k][b][i][j]); 
	  }
	}
      }
    }

    // pay close attention to the mapping of the indices 
    // here i -> pt, j -> x, k -> q2, b -> z 
    for (int i=0; i<fBins->GetPtBins()->GetNumber()+1; i++){
      for(int j=0; j<fBins->GetXBins()->GetNumber()+1; j++){
	for(int k=0; k<fBins->GetQ2Bins()->GetNumber()+1; k++){
	  
	  // iterate on z 
	  for(int b=1; b<fBins->GetZBins()->GetNumber()+1; b++){
	    h1_z_signal_p[i][j][k]->SetBinContent(b, signal_p[j][k][b][i]); 
	    //	    h1_z_signal_p[i][j][k]->SetBinError(b, err_p     [j][k][b][i]); 
	    h1_z_signal_k[i][j][k]->SetBinContent(b, signal_k[j][k][b][i]); 
	    //	    h1_z_signal_k[i][j][k]->SetBinError(b, err_k     [j][k][b][i]); 
	  }
	}
      }
    }
    
    for (int i=0; i<fBins->GetXBins()->GetNumber()+1; i++){
      for(int j=0; j<fBins->GetQ2Bins()->GetNumber()+1; j++){
	for(int k=0; k<fBins->GetZBins()->GetNumber()+1; k++){
	  
	  // iterate on pt 
	  for(int b=1; b<fBins->GetPtBins()->GetNumber()+1; b++){
	    h1_pt_signal_p[i][j][k]->SetBinContent(b, signal_p[i][j][k][b]); 
	    //	    h1_pt_signal_p[i][j][k]->SetBinError(b, err_p     [i][j][k][b]); 
	    h1_pt_signal_k[i][j][k]->SetBinContent(b, signal_k[i][j][k][b]); 
	    //	    h1_pt_signal_k[i][j][k]->SetBinError(b, err_k     [i][j][k][b]); 
	  }
	}
      }
    }
  }

  void Load(std::string inputFilename){
    TFile *inputFile = TFile::Open(inputFilename.c_str()); 
    
    if (inputFile->IsOpen()){
      for(int i=0; i<fBins->GetXBins()->GetNumber()+1; i++){
	for(int j=0; j<fBins->GetQ2Bins()->GetNumber()+1; j++){
	  for(int k=0; k<fBins->GetZBins()->GetNumber()+1; k++){
	    for(int l=0; l<fBins->GetPtBins()->GetNumber()+1; l++){
	      fit_p[i][j][k][l] = (TF1*) inputFile->Get(Form("signal_fits/fit_p_%s_x%d_q2%d_z%d_pt%d_%s", 
							     constants::Names::mesons[fMesonIndex].c_str(),
							     i, j, k, l, fName.c_str()));
	      fit_k[i][j][k][l] = (TF1*) inputFile->Get(Form("signal_fits/fit_k_%s_x%d_q2%d_z%d_pt%d_%s", 
							     constants::Names::mesons[fMesonIndex].c_str(),
							     i, j, k, l, fName.c_str()));
	      fit_bg[i][j][k][l] = (TF1*) inputFile->Get(Form("signal_fits/fit_bg_%s_x%d_q2%d_z%d_pt%d_%s", 
							      constants::Names::mesons[fMesonIndex].c_str(),
							      i, j, k, l, fName.c_str()));
	      fit_tot[i][j][k][l] = (TF1*) inputFile->Get(Form("signal_fits/fit_tot_%s_x%d_q2%d_z%d_pt%d_%s", 
							       constants::Names::mesons[fMesonIndex].c_str(),
							       i, j, k, l, fName.c_str()));
	      
	    }
	  }
	}
      }

    

      // get x 
      for (int i=0; i<fBins->GetQ2Bins()->GetNumber()+1; i++){
	for(int j=0; j<fBins->GetZBins()->GetNumber()+1; j++){
	  for(int k=0; k<fBins->GetPtBins()->GetNumber()+1; k++){
	    h1_x_signal_p[i][j][k] = (TH1D*) inputFile->Get(Form("h1_x_signal_p_q2%d_z%d_pt%d_%s", 
								 i, j, k, fName.c_str()));
	    h1_x_signal_k[i][j][k] = (TH1D*) inputFile->Get(Form("h1_x_signal_k_q2%d_z%d_pt%d_%s", 
								 i, j, k, fName.c_str()));	    
	  }
	}
      }

      // get q2 
      for (int i=0; i<fBins->GetZBins()->GetNumber()+1; i++){
	for(int j=0; j<fBins->GetPtBins()->GetNumber()+1; j++){
	  for(int k=0; k<fBins->GetXBins()->GetNumber()+1; k++){
	    h1_q2_signal_p[i][j][k] = (TH1D*) inputFile->Get(Form("h1_q2_signal_p_z%d_pt%d_x%d_%s", 
								 i, j, k, fName.c_str()));
	    h1_q2_signal_k[i][j][k] = (TH1D*) inputFile->Get(Form("h1_q2_signal_k_z%d_pt%d_x%d_%s", 
								 i, j, k, fName.c_str()));	    
	  }
	}
      }

      // get z 
      for (int i=0; i<fBins->GetPtBins()->GetNumber()+1; i++){
	for(int j=0; j<fBins->GetXBins()->GetNumber()+1; j++){
	  for(int k=0; k<fBins->GetQ2Bins()->GetNumber()+1; k++){
	    h1_z_signal_p[i][j][k] = (TH1D*) inputFile->Get(Form("h1_z_signal_p_pt%d_x%d_q2%d_%s", 
								 i, j, k, fName.c_str()));
	    h1_z_signal_k[i][j][k] = (TH1D*) inputFile->Get(Form("h1_z_signal_k_pt%d_x%d_q2%d_%s", 
								 i, j, k, fName.c_str()));	    
	  }
	}
      }

      // get pt 
      for (int i=0; i<fBins->GetXBins()->GetNumber()+1; i++){
	for(int j=0; j<fBins->GetQ2Bins()->GetNumber()+1; j++){
	  for(int k=0; k<fBins->GetZBins()->GetNumber()+1; k++){
	    h1_pt_signal_p[i][j][k] = (TH1D*) inputFile->Get(Form("h1_pt_signal_p_x%d_q2%d_z%d_%s", 
								  i, j, k, fName.c_str()));
	    h1_pt_signal_k[i][j][k] = (TH1D*) inputFile->Get(Form("h1_pt_signal_k_x%d_q2%d_z%d_%s", 
								  i, j, k, fName.c_str()));	    
	  }
	}
      }

    } else {
      std::cerr << "[SignalBackgroundFitter::Load] Failed to open input file" << std::endl; 
    }
  }

  
  void Save(std::string outputFilename, std::string writeMode){
    TFile *outputFile = new TFile(outputFilename.c_str(), writeMode.c_str()); 
    
    if (outputFile->IsOpen()){
      
      outputFile->mkdir("signal/"); 
      outputFile->cd("signal/"); 
      for (int i=0; i<fBins->GetQ2Bins()->GetNumber()+1; i++){
	for(int j=0; j<fBins->GetZBins()->GetNumber()+1; j++){
	  for(int k=0; k<fBins->GetPtBins()->GetNumber()+1; k++){
	    h1_x_signal_p[i][j][k]->Write(); 
	    h1_x_signal_k[i][j][k]->Write(); 
	  }
	}
      }
      
      for (int i=0; i<fBins->GetZBins()->GetNumber()+1; i++){
	for(int j=0; j<fBins->GetPtBins()->GetNumber()+1; j++){
	  for(int k=0; k<fBins->GetXBins()->GetNumber()+1; k++){
	    h1_q2_signal_p[i][j][k]->Write(); 
	    h1_q2_signal_k[i][j][k]->Write(); 
	  }
	}
      }
      
      for (int i=0; i<fBins->GetPtBins()->GetNumber()+1; i++){
	for(int j=0; j<fBins->GetXBins()->GetNumber()+1; j++){
	  for(int k=0; k<fBins->GetQ2Bins()->GetNumber()+1; k++){
	    h1_z_signal_p[i][j][k]->Write(); 
	    h1_z_signal_k[i][j][k]->Write();
	  }
	}
      }
      
      for (int i=0; i<fBins->GetXBins()->GetNumber()+1; i++){
	for(int j=0; j<fBins->GetQ2Bins()->GetNumber()+1; j++){
	  for(int k=0; k<fBins->GetZBins()->GetNumber()+1; k++){
	    h1_pt_signal_p[i][j][k]->Write(); 
	    h1_pt_signal_k[i][j][k]->Write();
	  }
	}
      }

      outputFile->cd("/");       
      outputFile->mkdir("signal_fits/");      
      outputFile->cd("signal_fits/");
      for(int i=0; i<fBins->GetXBins()->GetNumber()+1; i++){
	for(int j=0; j<fBins->GetQ2Bins()->GetNumber()+1; j++){
	  for(int k=0; k<fBins->GetZBins()->GetNumber()+1; k++){
	    for(int l=0; l<fBins->GetPtBins()->GetNumber()+1; l++){
	      fit_p[i][j][k][l]  ->Write();
	      fit_k[i][j][k][l]  ->Write(); 
	      fit_bg[i][j][k][l] ->Write(); 
	      fit_tot[i][j][k][l]->Write(); 
	    }
	  }
	}
      }
      
      outputFile->cd("/");       
      outputFile->Write(); 
      outputFile->Close(); 
    }
    else {
      std::cerr << "[SignalBackgroundFitter::Save] File could not be opened for writing! " << outputFilename << std::endl; 
    }
  }
    
    void CorrectAsymmetry(Fits *pion, Fits *kaon){

      pionFit = new Fits(pion, "corrected"); 
      kaonFit = new Fits(kaon, "corrected"); 
      
      for(int i=0; i<fBins->GetXBins()->GetNumber()+1; i++){
	for(int j=0; j<fBins->GetQ2Bins()->GetNumber()+1; j++){
	  for(int k=0; k<fBins->GetZBins()->GetNumber()+1; k++){
	    for(int l=0; l<fBins->GetPtBins()->GetNumber()+1; l++){
	      double obsP = pion->fit_asym[i][j][k][l]->GetParameter(0);  
	      double obsK = kaon->fit_asym[i][j][k][l]->GetParameter(0);  
	      
	      double trueP = (obsK*background_k[i][j][k][l]-obsP*signal_k[i][j][k][l])
		/(background_k[i][j][k][l]*background_p[i][j][k][l]
		  -signal_k[i][j][k][l]*signal_p[i][j][k][l]); 
	      
	      double trueK = (signal_p[i][j][k][l]*obsK-obsP*background_p[i][j][k][l])
		/(signal_k[i][j][k][l]*signal_p[i][j][k][l]
		  -background_k[i][j][k][l]*background_p[i][j][k][l]); 
	      
	      pionFit->fit_asym[i][j][k][l]->SetParameter(0, trueP);
	      kaonFit->fit_asym[i][j][k][l]->SetParameter(0, trueK);
	    }
	  }
	}    

	pionInt = new IntegratedHistos(pionFit, "corr", Meson::kPionPositive); 
	kaonInt = new IntegratedHistos(kaonFit, "corr", Meson::kKaonPositive); 
      }
    }

  Fits *GetPionFit() const {
    return pionFit; 
  }

  Fits *GetKaonFit() const {
    return kaonFit; 
  }

  IntegratedHistos *GetPionHistos() const {
    return pionInt; 
  }

  IntegratedHistos *GetKaonHistos() const {
    return kaonInt; 
  }
    
 protected:
  int         fMesonIndex; 
  Bins       *fBins; 
  PidHistos  *fHistos; 
  std::string fName; 
  TFile      *fInputFile; 

  Fits             *pionFit; 
  Fits             *kaonFit; 
  IntegratedHistos *pionInt; 
  IntegratedHistos *kaonInt; 

  void Init(){
    for(int i=0; i<fBins->GetXBins()->GetNumber()+1; i++){
      for(int j=0; j<fBins->GetZBins()->GetNumber()+1; j++){
	for(int k=0; k<fBins->GetPtBins()->GetNumber()+1; k++){
	  for(int l=0; l<fBins->GetPtBins()->GetNumber()+1; l++){

	    // hard coded limits, no good 
	    fit_p[i][j][k][l] = new TF1(Form("fit_p_%s_x%d_q2%d_z%d_pt%d_%s", constants::Names::mesons[fMesonIndex].c_str(),
					     i, j, k, l, fName.c_str()), "gaus", 0.05, 0.3);
	    fit_k[i][j][k][l] = new TF1(Form("fit_k_%s_x%d_q2%d_z%d_pt%d_%s", constants::Names::mesons[fMesonIndex].c_str(),
					     i, j, k, l, fName.c_str()), "gaus", 0.43, 0.60);
	    fit_bg[i][j][k][l] = new TF1(Form("fit_bg_%s_x%d_q2%d_z%d_pt%d_%s", constants::Names::mesons[fMesonIndex].c_str(),
					      i, j, k, l, fName.c_str()), "pol3");
	    fit_tot[i][j][k][l] = new TF1(Form("fit_tot_%s_x%d_q2%d_z%d_pt%d_%s", constants::Names::mesons[fMesonIndex].c_str(),
					       i, j, k, l, fName.c_str()), "gaus(0) + gaus(3) + pol3(6)", 0.0, 0.7);
	    
	    // initialize the stuff to zero 
	    signal_p[i][j][k][l]     = 0.0; 
	    signal_k[i][j][k][l]     = 0.0; 
	    err_p[i][j][k][l]        = 0.0; 
	    err_k[i][j][k][l]        = 0.0; 
	    background_p[i][j][k][l] = 0.0; 
	    background_k[i][j][k][l] = 0.0; 
	  }
	}
      }
    }

    for (int i=0; i<fBins->GetQ2Bins()->GetNumber()+1; i++){
      for(int j=0; j<fBins->GetZBins()->GetNumber()+1; j++){
	for(int k=0; k<fBins->GetPtBins()->GetNumber()+1; k++){
	  h1_x_signal_p[i][j][k] = new TH1D(Form("h1_x_signal_p_q2%d_z%d_pt%d_%s", 
						 i, j, k, fName.c_str()), "", 
					    fBins->GetXBins()->GetNumber(), 
					    fBins->GetXBins()->GetLimits().data());
	  h1_x_signal_k[i][j][k] = new TH1D(Form("h1_x_signal_k_q2%d_z%d_pt%d_%s", 
						 i, j, k, fName.c_str()), "", 
					    fBins->GetXBins()->GetNumber(), 
					    fBins->GetXBins()->GetLimits().data());
	}
      }
    }

    for (int i=0; i<fBins->GetQ2Bins()->GetNumber()+1; i++){
      for(int j=0; j<fBins->GetZBins()->GetNumber()+1; j++){
	for(int k=0; k<fBins->GetPtBins()->GetNumber()+1; k++){
	  h1_q2_signal_p[i][j][k] = new TH1D(Form("h1_q2_signal_p_z%d_pt%d_x%d_%s", 
						  i, j, k, fName.c_str()), "", 
					     fBins->GetQ2Bins()->GetNumber(), 
					     fBins->GetQ2Bins()->GetLimits().data());
	  h1_q2_signal_k[i][j][k] = new TH1D(Form("h1_q2_signal_k_z%d_pt%d_x%d_%s", 
						  i, j, k, fName.c_str()), "", 
					     fBins->GetQ2Bins()->GetNumber(), 
					     fBins->GetQ2Bins()->GetLimits().data());
	}
      }
    }

    for (int i=0; i<fBins->GetPtBins()->GetNumber()+1; i++){
      for(int j=0; j<fBins->GetXBins()->GetNumber()+1; j++){
	for(int k=0; k<fBins->GetQ2Bins()->GetNumber()+1; k++){
	  h1_z_signal_p[i][j][k] = new TH1D(Form("h1_z_signal_p_pt%d_x%d_q2%d_%s", 
						 i, j, k, fName.c_str()), "", 
					    fBins->GetZBins()->GetNumber(), 
					    fBins->GetZBins()->GetLimits().data());
	  h1_z_signal_k[i][j][k] = new TH1D(Form("h1_z_signal_k_pt%d_x%d_q2%d_%s", 
						  i, j, k, fName.c_str()), "", 
					     fBins->GetZBins()->GetNumber(), 
					     fBins->GetZBins()->GetLimits().data());
	}
      }
    }


    for (int i=0; i<fBins->GetXBins()->GetNumber()+1; i++){
      for(int j=0; j<fBins->GetQ2Bins()->GetNumber()+1; j++){
	for(int k=0; k<fBins->GetZBins()->GetNumber()+1; k++){
	  h1_pt_signal_p[i][j][k] = new TH1D(Form("h1_pt_signal_p_x%d_q2%d_z%d_%s", 
						  i, j, k, fName.c_str()), "", 
					     fBins->GetPtBins()->GetNumber(), 
					     fBins->GetPtBins()->GetLimits().data());
	  h1_pt_signal_k[i][j][k] = new TH1D(Form("h1_pt_signal_k_x%d_q2%d_z%d_%s", 
						  i, j, k, fName.c_str()), "", 
					     fBins->GetPtBins()->GetNumber(), 
					     fBins->GetPtBins()->GetLimits().data());
	}
      }
    }


  }
};

#endif
