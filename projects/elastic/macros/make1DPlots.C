{
  // --------------------------------------
  //   User Parameters 
  // --------------------------------------
  TFile *inputFile = TFile::Open("../out/testWithoutMC.root"); 
  string imagePath = "/volatile/clas12/dmriser/plots/elastic/";
  const int numberSectors = 6;  
  const int numberPhiBins = 1;  

  // --------------------------------------
  //   Other Things 
  // --------------------------------------
  TH1D *data[numberSectors][numberPhiBins];
  TH1D *cs[numberSectors][numberPhiBins];
  TH1D *ratio[numberSectors][numberPhiBins];
  TH1D *w[numberSectors][numberPhiBins];
  TH1D *model    = (TH1D*) inputFile->Get("model_theta"); 
  TH1D *modelRad = (TH1D*) inputFile->Get("modelRad_theta"); 

  for(int s=0; s<numberSectors; s++){
    for(int b=0; b<numberPhiBins; b++){
      data[s][b]  = (TH1D*) inputFile->Get( Form("dataEvents_thetaByPhi_sect%d_bin%d",s,b) );
      cs[s][b]    = (TH1D*) inputFile->Get( Form("crossSection_thetaByPhi_sect%d_bin%d",s,b) );
      ratio[s][b] = (TH1D*) inputFile->Get( Form("ratio_thetaByPhi_sect%d_bin%d",s,b) );
      w[s][b]     = (TH1D*) inputFile->Get( Form("w_thetaByPhi_sect%d_bin%d",s,b) );
    }
  }

  TCanvas *c1 = new TCanvas("c1","",800,500); 

  model    ->SetLineColor(kBlack); 
  modelRad ->SetLineColor(99); 

  int fontIndex      = 102;
  int fontIndexStats =  82;
  TLatex tit, xtit, ytit, stats;
  tit.SetNDC();
  tit.SetTextFont(fontIndex);
  tit.SetTextSize(0.05);

  stats.SetNDC();
  stats.SetTextFont(fontIndexStats);
  stats.SetTextSize(0.03);
 
  xtit.SetNDC();
  xtit.SetTextFont(fontIndex);
  xtit.SetTextSize(0.05);

  ytit.SetNDC();
  ytit.SetTextFont(fontIndex);
  ytit.SetTextSize(0.05);
  ytit.SetTextAngle(90.0);
  
  for(int s=0; s<numberSectors; s++){
    for(int b=0; b<numberPhiBins; b++){
      data[s][b]->Draw();
      data[s][b]->SetFillColorAlpha(77,0.3);
      gPad->SetGridx();
      gPad->SetGridy();

      tit.DrawLatex(0.37, 0.9, "E1-F Elastic Sample");
      xtit.DrawLatex(0.48, 0.07, "#theta_{e}");
      ytit.DrawLatex(0.07, 0.46, "Counts");
      stats.DrawLatex(0.75, 0.9, Form("#splitline{Sector %d}{N_{Events} = %.4E}",s+1,data[s][b]->Integral())); 

      c1->Print(Form("%sdata_sect%d_bin%d.png",imagePath.c_str(),s,b));
      c1->Clear();
    }
  }
  
  for(int s=0; s<numberSectors; s++){
    for(int b=0; b<numberPhiBins; b++){
      cs[s][b]->Draw("pe");
      cs[s][b]->SetMarkerStyle(8);
      cs[s][b]->SetMarkerColor(kBlack);
      model->Draw("lsame");
      modelRad->Draw("lsame");
      gPad->SetGridx();
      gPad->SetGridy();
      //      gPad->SetLogy(1);

      tit.DrawLatex(0.37, 0.9, "E1-F Elastic Sample");
      xtit.DrawLatex(0.48, 0.07, "#theta_{e}");
      ytit.DrawLatex(0.07, 0.48, "#muB/sr");
      stats.DrawLatex(0.75, 0.87, Form("Sector %d",s+1)); 

      c1->Print(Form("%scs_sect%d_bin%d.png",imagePath.c_str(),s,b));
      c1->Clear();
    }
  }

  gPad->SetLogy(0);
  for(int s=0; s<numberSectors; s++){
    for(int b=0; b<numberPhiBins; b++){
      ratio[s][b]->Draw();
      ratio[s][b]->SetMarkerStyle(8);
      ratio[s][b]->SetMarkerColor(kBlack);
      gPad->SetGridx();
      gPad->SetGridy();

      tit.DrawLatex(0.37, 0.9, "E1-F Elastic Sample");
      xtit.DrawLatex(0.48, 0.07, "#theta_{e}");
      ytit.DrawLatex(0.07, 0.48, "Ratio");
      stats.DrawLatex(0.75, 0.87, Form("Sector %d",s+1)); 

      c1->Print(Form("%sratio_sect%d_bin%d.png",imagePath.c_str(),s,b));
      c1->Clear();
    }
  }

  gPad->SetLogy(0);
  for(int s=0; s<numberSectors; s++){
    for(int b=0; b<numberPhiBins; b++){
      w[s][b]->SetFillColorAlpha(94,0.2);
      w[s][b]->SetLineColor(94);
      w[s][b]->Draw();
      gPad->SetGridx();
      gPad->SetGridy();

      tit.DrawLatex(0.37, 0.9, "E1-F Elastic Sample");
      xtit.DrawLatex(0.48, 0.07, "W (GeV/c^{2})");
      ytit.DrawLatex(0.07, 0.48, "Counts");
      stats.DrawLatex(0.75, 0.9, Form("#splitline{Sector %d}{N_{Events} = %.4E}",s+1,w[s][b]->Integral())); 

      c1->Print(Form("%sw_sect%d_bin%d.png",imagePath.c_str(),s,b));
      c1->Clear();
    }
  }
  // -----------------------------------------------  
  //      Print all together 
  // -----------------------------------------------  

  for(int b=0; b<numberPhiBins; b++){
    TCanvas *c2 = new TCanvas("c2","",800,500); 
    for(int s=0; s<numberSectors; s++){
      ratio[s][b]->Draw("same");
      ratio[s][b]->SetMarkerStyle(22+s);
      ratio[s][b]->SetMarkerColor(50+8*s);
    }
    gPad->SetGridy();
    tit.DrawLatex(0.37, 0.9, "E1-F Elastic Sample");
    xtit.DrawLatex(0.48, 0.07, "#theta_{e}");
    ytit.DrawLatex(0.07, 0.48, "Ratio");
    c2->Print(Form("%sratio_bin%d.png",imagePath.c_str(),b));
    //    c2->Clear();
  }



}
