{
  // ----------------------------------------
  //  User Parameters 
  // ----------------------------------------
  TFile *inputFile = TFile::Open("../out/withW.root");
  string imagePath = "/volatile/clas12/dmriser/plots/elastic/";

  const int thetaBins = 10;  
  const int phiBins   = 1; 

  // ----------------------------------------
  //  Other Things
  // ----------------------------------------
  THnSparseI *events = (THnSparseI*) inputFile->Get("dataEvents");
  TH2D     *thetaPhi = events->Projection(2,3); 
  TH2D     *thetaW[7];

  thetaW[0] = events->Projection(2,4); 

  TCanvas *c1 = new TCanvas("c1","",800,500);
  thetaPhi->Draw("colz");
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();


  int fontIndex = 102;
  TLatex tit, xtit, ytit;
  tit.SetNDC();
  tit.SetTextFont(fontIndex);
  tit.SetTextSize(0.05);

  xtit.SetNDC();
  xtit.SetTextFont(fontIndex);
  xtit.SetTextSize(0.05);

  ytit.SetNDC();
  ytit.SetTextFont(fontIndex);
  ytit.SetTextSize(0.05);
  ytit.SetTextAngle(90.0);

  tit.DrawLatex(0.37, 0.9, "E1-F Elastic Sample");
  xtit.DrawLatex(0.48, 0.07, "#phi_{Rel}");
  ytit.DrawLatex(0.07, 0.48, "#theta_{e'}");

  c1->Print(Form("%sthetaPhiSector0.png",imagePath.c_str()));
  c1->Clear(); 

  // ----------------------------------------------------
  // ----------------------------------------------------
  // ----------------------------------------------------  
  thetaW[0]->Draw("colz");
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  tit.DrawLatex(0.37, 0.9, "E1-F Elastic Sample");
  xtit.DrawLatex(0.48, 0.07, "W (GeV/c^{2})");
  ytit.DrawLatex(0.07, 0.48, "#theta_{e}");

  TLine *protLine = new TLine(0.938, 17.0, 0.938, 45.0); 
  protLine->SetLineStyle(8); 
  protLine->SetLineWidth(2); 
  protLine->Draw(); 
  c1->Print(Form("%sthetaWSector0.png",imagePath.c_str()));

  // ----------------------------------------------------
  // ----------------------------------------------------
  // ----------------------------------------------------  

  TCanvas *c2 = new TCanvas("c2","",800,500);
  thetaPhi->Draw("colz");
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();

  TLine *xLines[phiBins+1], *yLines[thetaBins+1];
  double xMin   = events->GetAxis(2)->GetBinLowEdge(1);
  double xMax   = events->GetAxis(2)->GetBinUpEdge( events->GetAxis(2)->GetNbins() );
  double yMin   = events->GetAxis(3)->GetBinLowEdge(1);
  double yMax   = events->GetAxis(3)->GetBinUpEdge( events->GetAxis(3)->GetNbins() );
  double xWidth = (xMax-xMin)/phiBins; 
  double yWidth = (yMax-yMin)/thetaBins; 

  cout << "xMin = " << xMin << endl; 
  cout << "xMax = " << xMax << endl; 
  cout << "yMin = " << yMin << endl; 
  cout << "yMax = " << yMax << endl; 
  cout << "xWidth = " << xWidth << endl; 
  cout << "yWidth = " << yWidth << endl; 

  for(int xbin=0; xbin<phiBins; xbin++){
    xLines[xbin] = new TLine(xbin*xWidth+xMin, yMin, xbin*xWidth+xMin, yMax);
    xLines[xbin]->SetLineWidth(2);
    xLines[xbin]->SetLineColor(kBlack);
    xLines[xbin]->Draw();
  }

  for(int ybin=0; ybin<thetaBins; ybin++){
    yLines[ybin] = new TLine(xMin, yMin+ybin*yWidth, xMax, yMin+ybin*yWidth);
    yLines[ybin]->Draw();
  }

  tit.DrawLatex(0.37, 0.9, "E1-F Elastic Sample");
  xtit.DrawLatex(0.48, 0.07, "#phi_{Rel}");
  ytit.DrawLatex(0.07, 0.48, "#theta_{e'}");

  c2->Print(Form("%sthetaPhiSector0Binned.png",imagePath.c_str()));

  // ----------------------------------------------------
  // ----------------------------------------------------
  // ----------------------------------------------------  
  c1->Clear(); 
  TH1D *wSlices[6][thetaBins]; 

  int origThetaBins = thetaW[0]->GetYaxis()->GetNbins();
  double thetaMin   = thetaW[0]->GetYaxis()->GetBinLowEdge(1); 
  double thetaMax   = thetaW[0]->GetYaxis()->GetBinUpEdge(origThetaBins); 
  double thetaStep  = (thetaMax-thetaMin)/thetaBins;

  TF1 *fit = new TF1("fit","gaus",0.87,1.0); 
  fit->SetLineColor(kBlack); 

  TF1 *fitFull = new TF1("fitFull","gaus",thetaMin,thetaMax); 
  fitFull->SetLineColor(kBlack); 


  gPad->SetGridx(0); 
  gPad->SetGridy(0); 

  for(int sector=0; sector<6; sector++){
    int startBinS = events->GetAxis(0)->FindBin((double)sector+1.0); 
    int stopBinS  = events->GetAxis(0)->FindBin((double)sector+2.0); 
    events->GetAxis(0)->SetRange(startBinS, stopBinS);     
    thetaW[sector+1] = events->Projection(2,4); 
    thetaW[sector+1]->SetName(Form("thetaW_sect%d",sector)); 

    for(int theta=0; theta<thetaBins; theta++){
      double thetaPoint = (0.5+theta)*thetaStep + thetaMin;
      int startBinT     = events->GetAxis(2)->FindBin(thetaPoint); 
      int stopBinT      = events->GetAxis(2)->FindBin(thetaPoint+thetaStep); 
      events->GetAxis(2)->SetRange(startBinT,stopBinT);     
      wSlices[sector][theta] = events->Projection(4); 
      wSlices[sector][theta]->SetName(Form("wSlice_sect%d_bin%d",sector,theta)); 
      wSlices[sector][theta]->SetTitle(Form("#theta_{e} = %.3f",thetaPoint)); 
      wSlices[sector][theta]->SetFillColorAlpha(62,0.2);
      wSlices[sector][theta]->SetLineColor(62);
      wSlices[sector][theta]->Fit("fit","RQ");
      wSlices[sector][theta]->Draw("h");

      fitFull->SetParameter(0,fit->GetParameter(0));
      fitFull->SetParameter(1,fit->GetParameter(1));
      fitFull->SetParameter(2,fit->GetParameter(2));
      fitFull->Draw("same");      

      tit.DrawLatex(0.24, 0.9, Form("E1-F Elastic Sample %s",wSlices[sector][theta]->GetTitle()));
      xtit.DrawLatex(0.48, 0.07, "W (GeV/c^{2})");
      ytit.DrawLatex(0.07, 0.48, "Counts");
      xtit.DrawLatex(0.64, 0.77, Form("#splitline{#mu = %.3f}{#sigma = %.3f}",fitFull->GetParameter(1),fitFull->GetParameter(2)));
      c1->Print(Form("%swByTheta_sect%d_bin%d.png",imagePath.c_str(),sector,theta));
    }
  }

  TCanvas *c2 = new TCanvas("c2","",1100,800); 

  for(int sector=0; sector<6; sector++){
    c2->Clear(); 
    c2->Divide(3,4); 

    for(int theta=0; theta<thetaBins; theta++){
      c2->cd(theta+1); 
     wSlices[sector][theta]->Draw("same");
    }
    
    for(int pad=0; pad<3*4; pad++){
      c2->cd(pad+1); 
      gPad->SetMargin(0.05, 0.05, 0.05, 0.05); 
      if( padIsLeft(theta+1, 3, 4) )       { gPad->SetLeftMargin(0.2); }
      else if( padIsRight(theta+1, 3, 4) ) { gPad->SetRightMargin(0.2);}
      if( padIsTop(theta+1, 3, 4) )        { gPad->SetTopMargin(0.2); }
      else if( padIsBottom(theta+1, 3, 4) ){ gPad->SetBottomMargin(0.2);}
     }

    c2->Print(Form("%swByTheta_sect%d.png",imagePath.c_str(),sector));
  }

}
