void plot_integrated_asymmetries(std::string file){
  // --------------------------------------------
  //    user parameters 
  // --------------------------------------------
  TFile *inputFile      = TFile::Open(file.c_str()); 
  std::string imagePath = "/volatile/clas12/dmriser/plots/asymmetry";

  const int numberXBins   = 6; 
  const int numberQ2Bins  = 2; 
  const int numberZBins   = 6; 
  const int numberPtBins  = 6; 

  // --------------------------------------------
  //    styling 
  // --------------------------------------------
  TStyle *style = new TStyle("style", "style"); 

  style->Reset("Modern"); 
  style->SetOptStat(0);
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

  TH1D *pion_x[numberQ2Bins+1][numberZBins+1][numberPtBins+1];
  TH1D *kaon_x[numberQ2Bins+1][numberZBins+1][numberPtBins+1];

  TH1D *pion_q2[numberZBins+1][numberPtBins+1][numberXBins+1];
  TH1D *kaon_q2[numberZBins+1][numberPtBins+1][numberXBins+1];

  TH1D *pion_z[numberPtBins+1][numberXBins+1][numberQ2Bins+1];
  TH1D *kaon_z[numberPtBins+1][numberXBins+1][numberQ2Bins+1];

  TH1D *pion_pt[numberXBins+1][numberQ2Bins+1][numberZBins+1];
  TH1D *kaon_pt[numberXBins+1][numberQ2Bins+1][numberZBins+1];

  for(int i=0; i<numberQ2Bins+1; i++){
    for(int j=0; j<numberZBins+1; j++){
      for(int k=0; k<numberPtBins+1; k++){
	pion_x[i][j][k] = (TH1D*) inputFile->Get(Form("integrated/pp/h1_x_q2%d_z%d_pt%d_base", i, j, k));
	pion_x[i][j][k]->SetLineColor(99); 
	pion_x[i][j][k]->SetFillColor(99); 
	pion_x[i][j][k]->SetMarkerColor(99); 
	pion_x[i][j][k]->SetMarkerStyle(8); 
	pion_x[i][j][k]->SetMarkerSize(0.7); 
	pion_x[i][j][k]->SetLineWidth(1.5); 
	pion_x[i][j][k]->SetMaximum(0.08); 
	pion_x[i][j][k]->SetMinimum(-0.08); 
	pion_x[i][j][k]->Draw("pe"); 

	kaon_x[i][j][k] = (TH1D*) inputFile->Get(Form("integrated/kp/h1_x_q2%d_z%d_pt%d_base", i, j, k));
	kaon_x[i][j][k]->SetLineColor(55); 
	kaon_x[i][j][k]->SetFillColor(55); 
	kaon_x[i][j][k]->SetMarkerColor(55); 
	kaon_x[i][j][k]->SetMarkerStyle(8); 
	kaon_x[i][j][k]->SetMarkerSize(0.7); 
	kaon_x[i][j][k]->SetLineWidth(1.5); 
	kaon_x[i][j][k]->SetMaximum(0.06); 
	kaon_x[i][j][k]->SetMinimum(-0.02); 
	kaon_x[i][j][k]->Draw("pesame"); 

	title.DrawLatex(0.15, 0.88, Form("Integrated Asymmetry for bin (%d, %d, %d) #color[99]{#pi^{+}} and #color[55]{K^{+}}", i, j, k)); 
	xtit.DrawLatex(0.65, 0.07, "x_{Bjorken}"); 
	ytit.DrawLatex(0.08, 0.45, "A_{LU}^{sin#phi}"); 


	can->Print(Form("%s/asymmetry_q2%d_z%d_pt%d_pos.png", imagePath.c_str(), i, j, k)); 
	can->Clear(); 
      }
    }
  }

  for(int i=0; i<numberZBins; i++){
    for(int j=0; j<numberPtBins+1; j++){
      for(int k=0; k<numberXBins+1; k++){
	pion_q2[i][j][k] = (TH1D*) inputFile->Get(Form("integrated/pp/h1_q2_z%d_pt%d_x%d_base", i, j, k));
	pion_q2[i][j][k]->SetLineColor(99); 
	pion_q2[i][j][k]->SetFillColor(99); 
	pion_q2[i][j][k]->SetMarkerColor(99); 
	pion_q2[i][j][k]->SetMarkerStyle(8); 
	pion_q2[i][j][k]->SetMarkerSize(0.7); 
	pion_q2[i][j][k]->SetLineWidth(1.5); 
	pion_q2[i][j][k]->SetMaximum(0.06); 
	pion_q2[i][j][k]->SetMinimum(-0.02); 
	pion_q2[i][j][k]->Draw("pe"); 

	kaon_q2[i][j][k] = (TH1D*) inputFile->Get(Form("integrated/kp/h1_q2_z%d_pt%d_x%d_base", i, j, k));
	kaon_q2[i][j][k]->SetLineColor(55); 
	kaon_q2[i][j][k]->SetFillColor(55); 
	kaon_q2[i][j][k]->SetMarkerColor(55); 
	kaon_q2[i][j][k]->SetMarkerStyle(8); 
	kaon_q2[i][j][k]->SetMarkerSize(0.7); 
	kaon_q2[i][j][k]->SetLineWidth(1.5); 
	kaon_q2[i][j][k]->SetMaximum(0.06); 
	kaon_q2[i][j][k]->SetMinimum(-0.02); 
	kaon_q2[i][j][k]->Draw("pesame"); 

	title.DrawLatex(0.3, 0.88, Form("Asymmetry for bin (%d, %d, %d) #color[99]{#pi^{+}} and #color[55]{K^{+}}", i, j, k)); 
	xtit.DrawLatex(0.65, 0.07, "Q^{2}"); 
	ytit.DrawLatex(0.08, 0.45, "A_{LU}^{sin#phi}"); 

	can->Print(Form("%s/asymmetry_z%d_pt%d_x%d_pos.png", imagePath.c_str(), i, j, k)); 
	can->Clear(); 
      }
    }
  }


  for(int i=0; i<numberPtBins; i++){
    for(int j=0; j<numberXBins+1; j++){
      for(int k=0; k<numberQ2Bins+1; k++){
	pion_z[i][j][k] = (TH1D*) inputFile->Get(Form("integrated/pp/h1_z_pt%d_x%d_q2%d_base", i, j, k));
	pion_z[i][j][k]->SetLineColor(99); 
	pion_z[i][j][k]->SetFillColor(99); 
	pion_z[i][j][k]->SetMarkerColor(99); 
	pion_z[i][j][k]->SetMarkerStyle(8); 
	pion_z[i][j][k]->SetMarkerSize(0.7); 
	pion_z[i][j][k]->SetLineWidth(1.5); 
	pion_z[i][j][k]->SetMaximum(0.06); 
	pion_z[i][j][k]->SetMinimum(-0.02); 
	pion_z[i][j][k]->Draw("pe"); 
 
	kaon_z[i][j][k] = (TH1D*) inputFile->Get(Form("integrated/kp/h1_z_pt%d_x%d_q2%d_base", i, j, k));
	kaon_z[i][j][k]->SetLineColor(55); 
	kaon_z[i][j][k]->SetFillColor(55); 
	kaon_z[i][j][k]->SetMarkerColor(55); 
	kaon_z[i][j][k]->SetMarkerStyle(8); 
	kaon_z[i][j][k]->SetMarkerSize(0.7); 
	kaon_z[i][j][k]->SetLineWidth(1.5); 
	kaon_z[i][j][k]->SetMaximum(0.06); 
	kaon_z[i][j][k]->SetMinimum(-0.02); 
	kaon_z[i][j][k]->Draw("pesame"); 

	title.DrawLatex(0.3, 0.88, Form("Asymmetry for bin (%d, %d, %d) #color[99]{#pi^{+}} and #color[55]{K^{+}}", i, j, k)); 
	xtit.DrawLatex(0.65, 0.07, "z"); 
	ytit.DrawLatex(0.08, 0.45, "A_{LU}^{sin#phi}"); 

	can->Print(Form("%s/asymmetry_pt%d_x%d_q2%d_pos.png", imagePath.c_str(), i, j, k)); 
	can->Clear(); 
      }
    }
  }


  for(int i=0; i<numberXBins+1; i++){
    for(int j=0; j<numberQ2Bins+1; j++){
      for(int k=0; k<numberZBins+1; k++){
	pion_pt[i][j][k] = (TH1D*) inputFile->Get(Form("integrated/pp/h1_pt_x%d_q2%d_z%d_base", i, j, k));
	pion_pt[i][j][k]->SetLineColor(99); 
	pion_pt[i][j][k]->SetFillColor(99); 
	pion_pt[i][j][k]->SetMarkerColor(99); 
	pion_pt[i][j][k]->SetMarkerStyle(8); 
	pion_pt[i][j][k]->SetMarkerSize(0.7); 
	pion_pt[i][j][k]->SetLineWidth(1.5); 
	pion_pt[i][j][k]->SetMaximum(0.06); 
	pion_pt[i][j][k]->SetMinimum(-0.02); 
	pion_pt[i][j][k]->Draw("pe"); 

	kaon_pt[i][j][k] = (TH1D*) inputFile->Get(Form("integrated/kp/h1_pt_x%d_q2%d_z%d_base", i, j, k));
	kaon_pt[i][j][k]->SetLineColor(55); 
	kaon_pt[i][j][k]->SetFillColor(55); 
	kaon_pt[i][j][k]->SetMarkerColor(55); 
	kaon_pt[i][j][k]->SetMarkerStyle(8); 
	kaon_pt[i][j][k]->SetMarkerSize(0.7); 
	kaon_pt[i][j][k]->SetLineWidth(1.5); 
	kaon_pt[i][j][k]->SetMaximum(0.06); 
	kaon_pt[i][j][k]->SetMinimum(-0.02); 
	kaon_pt[i][j][k]->Draw("pesame"); 

	title.DrawLatex(0.3, 0.88, Form("Asymmetry for bin (%d, %d, %d) #color[99]{#pi^{+}} and #color[55]{K^{+}}", i, j, k)); 
	xtit.DrawLatex(0.65, 0.07, "P_{T} (GeV/c)"); 
	ytit.DrawLatex(0.08, 0.45, "A_{LU}^{sin#phi}"); 

	can->Print(Form("%s/asymmetry_x%d_q2%d_z%d_pos.png", imagePath.c_str(), i, j, k)); 
	can->Clear(); 
      }
    }
  }


  /* 
    TCanvas *can2 = new TCanvas("can2", "", 1200, 600); 

    can2->Divide(numberXBins,2);     
    for(int b=1; b<numberXBins+1; b++){
      can2->cd(b); 
      gPad->SetMargin(0.1, 0.1, 0.1, 0.1); 
      pion_z[0][b]     ->Draw("pe");
      pion_z_corr[0][b]->Draw("pesame");
      kaon_z[0][b]     ->Draw("pesame");
      kaon_z_corr[0][b]->Draw("pesame");

      title.DrawLatex(0.67, 0.14, Form("x-bin %d", b)); 
      title.DrawLatex(0.8, 0.02, "z"); 
    }

    for(int b=1; b<numberXBins+1; b++){
      can2->cd(b +numberXBins); 
      gPad->SetMargin(0.1, 0.1, 0.1, 0.1); 
      pion_pt[b][0]     ->Draw("pe");
      pion_pt_corr[b][0]->Draw("pesame");
      kaon_pt[b][0]     ->Draw("pesame");
      kaon_pt_corr[b][0]->Draw("pesame");

      title.DrawLatex(0.67, 0.14, Form("x-bin %d", b)); 
      title.DrawLatex(0.8, 0.02, "P_{T}"); 
    }
    
    can2->cd();
    
    xtit.SetTextSize(0.03); 
    xtit.DrawLatex(0.35, 0.965, "A_{LU}^{sin#phi} for #color[99]{#pi^{+}}, #color[55]{K^{+}}, open circles corrected"); 

    can2->Print(Form("%s/asymmetry_x_pos.png", imagePath.c_str())); 

    // -------------
    can2->Clear(); 
    can2->Divide(numberZBins,2);     
    for(int b=1; b<numberZBins+1; b++){
      can2->cd(b); 
      gPad->SetMargin(0.1, 0.1, 0.1, 0.1); 
      pion_pt[0][b]     ->Draw("pe");
      pion_pt_corr[0][b]->Draw("pesame");
      kaon_pt[0][b]     ->Draw("pesame");
      kaon_pt_corr[0][b]->Draw("pesame");

      title.DrawLatex(0.67, 0.14, Form("z-bin %d", b)); 
      title.DrawLatex(0.8, 0.02, "P_{T}"); 
    }

    for(int b=1; b<numberZBins+1; b++){
      can2->cd(b +numberZBins); 
      gPad->SetMargin(0.1, 0.1, 0.1, 0.1); 
      pion_x[b][0]     ->Draw("pe");
      pion_x_corr[b][0]->Draw("pesame");
      kaon_x[b][0]     ->Draw("pesame");
      kaon_x_corr[b][0]->Draw("pesame");

      title.DrawLatex(0.67, 0.14, Form("z-bin %d", b)); 
      title.DrawLatex(0.8, 0.02, "x"); 
    }
    
    can2->cd();
    xtit.SetTextSize(0.03); 
    xtit.DrawLatex(0.35, 0.965, "A_{LU}^{sin#phi} for #color[99]{#pi^{+}}, #color[55]{K^{+}}, open circles corrected"); 
    can2->Print(Form("%s/asymmetry_z_pos.png", imagePath.c_str())); 

    // ---------
    can2->Clear(); 
    can2->Divide(numberPtBins,2);     
    for(int b=1; b<numberPtBins+1; b++){
      can2->cd(b); 
      gPad->SetMargin(0.1, 0.1, 0.1, 0.1); 
      pion_x[0][b]     ->Draw("pe");
      pion_x_corr[0][b]->Draw("pesame");
      kaon_x[0][b]     ->Draw("pesame");
      kaon_x_corr[0][b]->Draw("pesame");

      title.DrawLatex(0.67, 0.14, Form("P_{T}-bin %d", b)); 
      title.DrawLatex(0.8, 0.02, "x"); 
    }

    for(int b=1; b<numberPtBins+1; b++){
      can2->cd(b +numberPtBins); 
      gPad->SetMargin(0.1, 0.1, 0.1, 0.1); 
      pion_z[b][0]     ->Draw("pe");
      pion_z_corr[b][0]->Draw("pesame");
      kaon_z[b][0]     ->Draw("pesame");
      kaon_z_corr[b][0]->Draw("pesame");

      title.DrawLatex(0.67, 0.14, Form("P_{T}-bin %d", b)); 
      title.DrawLatex(0.8, 0.02, "z"); 
    }
    
    can2->cd();
    xtit.SetTextSize(0.03); 
    xtit.DrawLatex(0.35, 0.965, "A_{LU}^{sin#phi} for #color[99]{#pi^{+}}, #color[55]{K^{+}}, open circles corrected"); 
    can2->Print(Form("%s/asymmetry_pt_pos.png", imagePath.c_str())); 
  */

}
