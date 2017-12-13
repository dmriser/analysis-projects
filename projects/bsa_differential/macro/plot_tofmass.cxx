void plot_tofmass(std::string file){
  // --------------------------------------------
  //    user parameters 
  // --------------------------------------------
  TFile *inputFile      = TFile::Open(file.c_str()); 
  std::string imagePath = "/volatile/clas12/dmriser/plots/asymmetry";

  const int numberXBins  = 6; 
  const int numberZBins  = 6; 
  const int numberPtBins = 6; 

  // --------------------------------------------
  //    styling 
  // --------------------------------------------
  TStyle *style = new TStyle("style", "style"); 

  style->Reset("Modern"); 
  style->SetOptStat(0);
  //  style->SetOptTitle(0);
  //  style->SetOptDate(0);
  style->SetLabelSize(0.1,"xyz");
  style->SetTitleSize(0.1,"xyz");
  style->SetTitleFont(112,"xyz");
  style->SetLabelFont(112,"xyz");
  style->SetLabelSize(0.1);
  style->SetTitleOffset(1.2,"y");
  style->SetCanvasColor(0);
  style->SetCanvasBorderMode(0);
  style->SetCanvasBorderSize(0);
  style->SetPadBottomMargin(0.15);
  style->SetPadTopMargin(0.15);
  style->SetPadLeftMargin(0.15);
  style->SetPadRightMargin(0.15);
  style->SetPadGridX(0);
  style->SetPadGridY(0);
  style->SetPadTickX(1);
  style->SetPadTickY(1);
  style->SetFrameBorderMode(0);
  gROOT->SetStyle("style"); 
  
  // --------------------------------------------
  //     plotting 
  // --------------------------------------------
  TCanvas *can = new TCanvas("can","",800,500); 

  TLatex title, xtit, ytit; 
  title.SetNDC(); 
  title.SetTextSize(0.065); 

  xtit.SetNDC(); 
  xtit.SetTextSize(0.04); 

  ytit.SetNDC(); 
  ytit.SetTextSize(0.04); 
  ytit.SetTextAngle(90.0); 

  TH1D *tofmass[numberXBins+1][numberZBins+1][numberPtBins+1];
  TF1  *totalFit[numberXBins+1][numberZBins+1][numberPtBins+1];
  TF1  *pionFit[numberXBins+1][numberZBins+1][numberPtBins+1];
  TF1  *kaonFit[numberXBins+1][numberZBins+1][numberPtBins+1];
  TF1  *backgroundFit[numberXBins+1][numberZBins+1][numberPtBins+1];


  for(int i=0; i<numberXBins+1; i++){
    for(int j=0; j<numberZBins+1; j++){
      for(int k=0; k<numberPtBins+1; k++){
	tofmass[i][j][k] = (TH1D*) inputFile->Get(Form("pid/kp/h1_tofmass_kp_x%d_q20_z%d_pt%d_test", i, j, k));
	tofmass[i][j][k]->SetLineColor(kBlack); 
	tofmass[i][j][k]->SetLineWidth(0.8); 
	tofmass[i][j][k]->Draw(); 

	totalFit[i][j][k] = (TF1*) inputFile->Get(Form("signal_fits/fit_tot_pp_x%d_q20_z%d_pt%d_test", i, j, k)); 
	totalFit[i][j][k]->SetLineColor(99); 
	totalFit[i][j][k]->SetLineWidth(0.5); 
	totalFit[i][j][k]->Draw("same"); 

	pionFit[i][j][k] = (TF1*) inputFile->Get(Form("signal_fits/fit_p_pp_x%d_q20_z%d_pt%d_test", i, j, k)); 
	pionFit[i][j][k]->SetLineColor(6); 
	pionFit[i][j][k]->SetLineWidth(0.5); 
	pionFit[i][j][k]->Draw("same"); 

	kaonFit[i][j][k] = (TF1*) inputFile->Get(Form("signal_fits/fit_k_pp_x%d_q20_z%d_pt%d_test", i, j, k)); 
	kaonFit[i][j][k]->SetLineColor(66); 
	kaonFit[i][j][k]->SetLineWidth(0.5); 
	kaonFit[i][j][k]->Draw("fsame"); 

	backgroundFit[i][j][k] = (TF1*) inputFile->Get(Form("signal_fits/fit_bg_pp_x%d_q20_z%d_pt%d_test", i, j, k)); 
	backgroundFit[i][j][k]->SetLineColor(kBlack); 
	backgroundFit[i][j][k]->SetLineStyle(9); 
	backgroundFit[i][j][k]->SetLineWidth(0.5); 
	backgroundFit[i][j][k]->Draw("same"); 

	title.DrawLatex(0.3, 0.88, Form("M_{ToF} for bin (%d, %d, %d)", i, j, k)); 
	xtit.DrawLatex(0.65, 0.07, "M_{ToF} (GeV/c^{2})"); 
	ytit.DrawLatex(0.08, 0.45, "Events"); 


	can->Print(Form("%s/tofmass_x%d_z%d_pt%d_pos.png", imagePath.c_str(), i, j, k)); 
	can->Clear(); 
      }
    }
  }


}
