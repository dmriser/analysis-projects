{
  
  std::string outputPath("/volatile/clas12/dmriser/plots/asymmetry");

  TFile *inputFile = TFile::Open("/volatile/clas12/dmriser/rootFiles/asymmetry/return_pass7.root");

  const int NMESON    = 4; 
  const int MAX_WIDTH = 6; 

  std::string name[NMESON] = {"pm","pp","km","kp"};
  std::string latex[NMESON] = {"#pi^{-}","#pi^{+}","K^{-}","K^{+}"};

  const int nx[NMESON]     = {3, 6, 3, 6}; 
  const int nq2[NMESON]    = {2, 6, 2, 6};
  const int nz[NMESON]     = {6, 6, 6, 6};  
  const int npt[NMESON]    = {3, 6, 3, 6};  
  
  // styling 
  int markerColor = kBlack;
  int markerStyle = 8;
  int lineColor   = kBlack; 
  int lineWidth   = 1; 
  int fillColor   = 99;
  float fillAlpha = 0.4; 

  float lowerLimit = -0.07; 
  float upperLimit =  0.07; 

  TH1F *x[NMESON][nx],    *q2[NMESON][nq2],    *z[NMESON][nz],    *pt[NMESON][npt];
  TF1  *fitX[NMESON][nx], *fitQ2[NMESON][nq2], *fitZ[NMESON][nz], *fitPt[NMESON][npt];

  // loop over particles to load histograms and
  // fits. 
  for (int meson=0; meson<NMESON; meson++){
    
    // x 
    for (int xbin=0; xbin<nx[meson]; xbin++){
      x[meson][xbin] = (TH1F*) inputFile->Get(Form("asym/%s/h1_asym_%s_x%d_q20_z0_pt0_test", 
					     name[meson].c_str(), name[meson].c_str(), xbin+1));
      fitX[meson][xbin] = (TF1*) inputFile->Get(Form("fit/%s/fit_asym_%s_x%d_q20_z0_pt0_test", 
					       name[meson].c_str(), name[meson].c_str(), xbin+1));

      // some styling options 
      x[meson][xbin]->SetMarkerStyle(markerStyle);
      x[meson][xbin]->SetMarkerColor(markerColor);
      x[meson][xbin]->SetFillColorAlpha(fillColor, fillAlpha);
      x[meson][xbin]->SetLineColor(lineColor);
      x[meson][xbin]->SetLineWidth(lineWidth);
      x[meson][xbin]->SetMinimum(lowerLimit);
      x[meson][xbin]->SetMaximum(upperLimit);

    }

    // q2
    for (int bin=0; bin<nq2[meson]; bin++){
      q2[meson][bin] = (TH1F*) inputFile->Get(Form("asym/%s/h1_asym_%s_x0_q2%d_z0_pt0_test", 
						   name[meson].c_str(), name[meson].c_str(), bin+1));
      fitQ2[meson][bin] = (TF1*) inputFile->Get(Form("fit/%s/fit_asym_%s_x0_q2%d_z0_pt0_test", 
						     name[meson].c_str(), name[meson].c_str(), bin+1));

      // some styling options 
      q2[meson][bin]->SetMarkerStyle(markerStyle);
      q2[meson][bin]->SetMarkerColor(markerColor);
      q2[meson][bin]->SetFillColorAlpha(fillColor, fillAlpha);
      q2[meson][bin]->SetLineColor(lineColor);
      q2[meson][bin]->SetLineWidth(lineWidth);
      q2[meson][bin]->SetMinimum(lowerLimit);
      q2[meson][bin]->SetMaximum(upperLimit);
    }

    // z
    for (int bin=0; bin<nz[meson]; bin++){
      z[meson][bin] = (TH1F*) inputFile->Get(Form("asym/%s/h1_asym_%s_x0_q20_z%d_pt0_test", 
						   name[meson].c_str(), name[meson].c_str(), bin+1));
      fitZ[meson][bin] = (TF1*) inputFile->Get(Form("fit/%s/fit_asym_%s_x0_q20_z%d_pt0_test", 
						    name[meson].c_str(), name[meson].c_str(), bin+1));

      // some styling options 
      z[meson][bin]->SetMarkerStyle(markerStyle);
      z[meson][bin]->SetMarkerColor(markerColor);
      z[meson][bin]->SetFillColorAlpha(fillColor, fillAlpha);
      z[meson][bin]->SetLineColor(lineColor);
      z[meson][bin]->SetLineWidth(lineWidth);
      z[meson][bin]->SetMinimum(lowerLimit);
      z[meson][bin]->SetMaximum(upperLimit);
    }

    // pt
    for (int bin=0; bin<npt[meson]; bin++){
      pt[meson][bin] = (TH1F*) inputFile->Get(Form("asym/%s/h1_asym_%s_x0_q20_z0_pt%d_test", 
						   name[meson].c_str(), name[meson].c_str(), bin+1));
      fitPt[meson][bin] = (TF1*) inputFile->Get(Form("fit/%s/fit_asym_%s_x0_q20_z0_pt%d_test", 
						     name[meson].c_str(), name[meson].c_str(), bin+1));

      // some styling options 
      pt[meson][bin]->SetMarkerStyle(markerStyle);
      pt[meson][bin]->SetMarkerColor(markerColor);
      pt[meson][bin]->SetFillColorAlpha(fillColor, fillAlpha);
      pt[meson][bin]->SetLineColor(lineColor);
      pt[meson][bin]->SetLineWidth(lineWidth);
      pt[meson][bin]->SetMinimum(lowerLimit);
      pt[meson][bin]->SetMaximum(upperLimit);
    }

  }

  TCanvas *can = new TCanvas("can","",1600,1600);
  can->Print("/volatile/clas12/dmriser/plots/asymmetry/asymmetry_fits.pdf[");

  TLatex *lab = new TLatex(); 
  lab->SetNDC(); 
  lab->SetTextSize(0.02); 
  

  for (int meson=0; meson < NMESON; meson++){
    can->cd();
    can->Divide(MAX_WIDTH, 4);
  
    lab->DrawLatex(0.03, 0.975, Form("particle - %s", latex[meson].c_str())); 
  
    for (int bin = 0; bin < nx[meson]; bin++){
      can->cd(1 + bin);
      x[meson][bin]   ->Draw("pe");
      fitX[meson][bin]->Draw("lsame");
    }
    
    for (int bin = 0; bin < nq2[meson]; bin++){
      can->cd(1 + nz[meson] + bin);
      q2[meson][bin]   ->Draw("pe");
      fitQ2[meson][bin]->Draw("lsame");
    }

    for (int bin = 0; bin < nz[meson]; bin++){
      can->cd(1 + 2*nz[meson] + bin);
      z[meson][bin]   ->Draw("pe");
      fitZ[meson][bin]->Draw("lsame");
    }
    
    for (int bin = 0; bin < npt[meson]; bin++){
      can->cd(1 + 3*nz[meson] + bin);
      pt[meson][bin]   ->Draw("pe");
      fitPt[meson][bin]->Draw("lsame");
    }
   
    can->Print("/volatile/clas12/dmriser/plots/asymmetry/asymmetry_fits.pdf");
    can->Clear();
  }

  can->Print("/volatile/clas12/dmriser/plots/asymmetry/asymmetry_fits.pdf]");

}
