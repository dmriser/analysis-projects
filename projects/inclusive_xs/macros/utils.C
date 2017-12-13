void readIntegratedHisto(TH1D *histo[numberSector], TFile *file, const int numberSector, string title){
  for (int s=0; s< numberSector; s++){
    string fullTitle = Form("%s_s%d",title.c_str(),s);
    histo[s] = (TH1D*) file->Get(fullTitle.c_str());
    cout << " >>> Getting "  << fullTitle << " with " << histo[s]->GetEntries() << " entries. " << endl; 
  }
}

void readHisto(TH1D *histo[numberSector][numberSlices], TFile *file, const int numberSector, const int numberSlices, string title){
  for (int s=0; s< numberSector; s++){
    for (int b=0; b< numberSlices; b++) {
      string fullTitle = Form("%s_s%d_slice%d",title.c_str(),s,b);
      histo[s][b] = (TH1D*) file->Get(fullTitle.c_str());
      cout << " >>> Getting "  << fullTitle << " with " << histo[s][b]->GetEntries() << " entries. " << endl; 
    }
  }
}

void readEvents(TH2D *histo[numberSector], TFile *file, const int numberSector, string title){
  for (int s=0; s< numberSector; s++){
    string fullTitle = Form("%s_s%d",title.c_str(),s);
    histo[s] = (TH2D*) file->Get(fullTitle.c_str());
    cout << " >>> Getting "  << fullTitle << " with " << histo[s]->GetEntries() << " entries. " << endl; 
  }
}

void showIntegratedHisto(TH1D *histo[numberSector], const int numberSector){
  TCanvas * can = new TCanvas("can","",1200,800);
  can->Divide(3,2);
  
  TLatex sectorCaption;
  sectorCaption.SetNDC();
  sectorCaption.SetTextFont(12);

  for (int s=1; s<numberSector; s++){
    can->cd(s);
    histo[s]->SetMarkerColor(kBlack);
    histo[s]->SetMarkerStyle(7);
    histo[s]->Draw();
    sectorCaption.DrawLatex(0.4, 0.95, Form("Sector %d",s));
  }

}

void showEvents(TH2D *histo[numberSector], const int numberSector){
  TCanvas * can = new TCanvas("can","",1200,800);
  can->Divide(3,2);
  
  TLatex sectorCaption;
  sectorCaption.SetNDC();
  sectorCaption.SetTextFont(12);

  for (int s=1; s<numberSector; s++){
    can->cd(s);
    histo[s]->Draw("colz");
    sectorCaption.DrawLatex(0.4, 0.925, Form("Sector %d",s));
  }

}

std::string getBinString(TH1D *h, int index){
  std::string binString(Form("%d,%.4e,%.4e,%.4e", index, h->GetBinCenter(index), h->GetBinContent(index), h->GetBinError(index))); 
  return binString; 
}

void makeSlicesX(TH2D *histo, TH1D *slices[numberSlices], const int numberSlices, int histoType){
  
  string sliceTitle;
  for (int slice=0; slice< numberSlices; slice++){
    if (histoType == 1){ sliceTitle = Form("keppelModel_wSlice%d",slice); }
    slices[slice] = histo->ProjectionX(sliceTitle.c_str(),slice+1,slice+2);
  }
  
}

void makeSlicesY(TH2D *histo, TH1D *slices[numberSlices], const int numberSlices, int histoType){
  
  string sliceTitle;
  for (int slice=0; slice< numberSlices; slice++){
    if (histoType == 1){ sliceTitle = Form("keppelModel_qqSlice%d",slice); }
    slices[slice] = histo->ProjectionY(sliceTitle.c_str(),slice+1,slice+2);
  }
  
}

void showHisto(TH1D *histo[numberSector][numberSlices], const int numberSector, const int numberSlices){
  int canHeight=0;
  while(canHeight*(canHeight+1) < numberSlices){canHeight++;}

  TCanvas * can = new TCanvas("can","",1200,800);
  can->Divide(canHeight+1,canHeight);
  
  TLatex sectorCaption;
  sectorCaption.SetNDC();
  sectorCaption.SetTextFont(12);

  for (int s=1; s<numberSlices; s++){
    can->cd(s);
    histo[0][s]->SetLineColor(kRed);
    histo[0][s]->Draw("pe");
    sectorCaption.DrawLatex(0.4, 0.95, Form("Slice %d",s));
  }

}


// Don't forget to change the binning limits for Q^2 here if you change them in the main code. There are 
// other ways to get around doing it manually but I choose this for now.
void printSlices(TH1D *histo[numberSector][numberSlices], const int numberSector, const int numberSlices, int distType, int histoType){

  // distType: 
  //           1 = x-dist 
  //           2 = w-dist 
  //
  // histoType:  
  //           1 = data events
  //           2 = rec sim events  
  //           3 = gen sim events    
  //           4 = acceptance 
  //           5 = cross section 
  //           6 = cross section ratio 
  //           7 = purity 
  //           8 = rad corr 
  //           9 = inelastic fraction 
  //           10 = bin centering corrections 

  double qqMin = 1.7; double qqMax = 4.2;
  double qqWidth = (qqMax-qqMin)/numberSlices; // slices are always Q^2 bins 

  TCanvas *can = new TCanvas("can","",800,500);
  
  TLatex sectorCaption, xCaption, yCaption, toleranceCaption, prelimCaption;
  sectorCaption.SetNDC();
  sectorCaption.SetTextFont(102); 
  sectorCaption.SetTextSize(0.045); 

  xCaption.SetNDC(); 
  xCaption.SetTextSize(0.045);
  xCaption.SetTextFont(102);

  prelimCaption.SetNDC(); 
  prelimCaption.SetTextSize(0.1);
  prelimCaption.SetTextFont(102);
  prelimCaption.SetTextColor(99); 

  yCaption.SetNDC();
  yCaption.SetTextSize(0.045);
  yCaption.SetTextAngle(90.0);
  yCaption.SetTextFont(102);

  toleranceCaption.SetNDC();
  toleranceCaption.SetTextSize(0.045);
  toleranceCaption.SetTextAngle(90.0);
  toleranceCaption.SetTextFont(102);

  TLine ratioLower(histo[0][0]->GetXaxis()->GetXmin(), 0.95, histo[0][0]->GetXaxis()->GetXmax(), 0.95);
  TLine ratioUpper(histo[0][0]->GetXaxis()->GetXmin(), 1.05, histo[0][0]->GetXaxis()->GetXmax(), 1.05);
  ratioLower.SetLineStyle(8);
  ratioUpper.SetLineStyle(8);

  int numberOfXBins = histo[0][0]->GetXaxis()->GetNbins();

  for (int sect=0; sect<numberSector; sect++){
    for (int slice=0; slice<numberSlices; slice++){
      double qqValue = qqMin + slice*qqWidth;

      if (histoType < 4){ 
	histo[sect][slice]->SetFillStyle(3003); 
	histo[sect][slice]->SetFillColorAlpha(kBlack,1.0); 
	histo[sect][slice]->SetLineColor(kBlack); 
	histo[sect][slice]->Draw("hist");
      }
      
      if (histoType == 4){ 
	histo[sect][slice]->SetMinimum(0.0); 
	histo[sect][slice]->SetMaximum(1.0); 
	histo[sect][slice]->SetMarkerStyle(8); 
	histo[sect][slice]->Draw("pe");
      }

      if (histoType == 5){ 
	histo[sect][slice]->SetMarkerStyle(7); 
	histo[sect][slice]->Draw("pe");
      }

      if (histoType == 6){ 
	histo[sect][slice]->SetMarkerStyle(8); 
	histo[sect][slice]->SetMinimum(0.2); 
	histo[sect][slice]->SetMaximum(1.8); 
	histo[sect][slice]->Draw("pe");
	ratioLower.Draw();
	ratioUpper.Draw();
	
	prelimCaption.DrawLatex(0.25, 0.25, "Preliminary"); 
      }

      if (histoType == 7){
	histo[sect][slice]->SetFillStyle(3004); 
	histo[sect][slice]->SetFillColorAlpha(kRed,1.0); 
	histo[sect][slice]->SetLineColor(kRed);
 	histo[sect][slice]->SetMinimum(0.0); 
 	histo[sect][slice]->SetMaximum(1.0); 
	histo[sect][slice]->Draw("hist");
      }

      if (histoType == 8) {
	histo[sect][slice]->SetMarkerStyle(8); 
	histo[sect][slice]->SetMinimum(0.5); 
	histo[sect][slice]->SetMaximum(2.0); 
	histo[sect][slice]->Draw("pe");
      }

      if (histoType == 9) {
	histo[sect][slice]->SetFillColorAlpha(72,0.5); 
	histo[sect][slice]->SetMinimum(0.8); 
	histo[sect][slice]->SetMaximum(1.1); 
	histo[sect][slice]->Draw("hist");
      }
      
      if (histoType == 10){ 
	histo[sect][slice]->SetMinimum(0.9); 
	histo[sect][slice]->SetMaximum(1.1); 
	histo[sect][slice]->SetMarkerStyle(8); 
	histo[sect][slice]->Draw("pe");
      }

      if (distType == 1){ xCaption.DrawLatex(0.65,0.06,"X_{Bjorken}"); }
      if (distType == 2){ xCaption.DrawLatex(0.47,0.06,"W (GeV/c^{2})"); }

      if (histoType < 4){ yCaption.DrawLatex(0.05,0.48,"Counts"); }
      if (histoType == 4){ yCaption.DrawLatex(0.05,0.48,"Ratio"); }
      if (histoType == 5){ yCaption.DrawLatex(0.05,0.48,"#muBarn"); }
      if (histoType == 6){ yCaption.DrawLatex(0.05,0.48,"Ratio"); toleranceCaption.DrawLatex(0.91,0.44,"#pm 5%"); }
      if (histoType == 7){ yCaption.DrawLatex(0.05,0.48,"Purity"); }
      if (histoType == 8){ yCaption.DrawLatex(0.05,0.48,"Ratio rad/born"); }
      if (histoType == 9){ yCaption.DrawLatex(0.05,0.62,"Inelastic Fraction"); }
      if (histoType == 10){ yCaption.DrawLatex(0.05,0.48,"Ratio"); }

      sectorCaption.DrawLatex(0.68, 0.89, Form("Sector %d Q^{2}=%.3f",sect,qqValue));

      if (histoType == 1){      can->Print(Form("%sdata/dataSector%dSlice%d.png",imagePath.c_str(),sect,slice)); }
      else if (histoType == 2){ can->Print(Form("%srec/recSector%dSlice%d.png",imagePath.c_str(),sect,slice)); }
      else if (histoType == 3){ can->Print(Form("%sgen/genSector%dSlice%d.png",imagePath.c_str(),sect,slice)); }
      else if (histoType == 4){ can->Print(Form("%sacceptance/acceptanceSector%dSlice%d.png",imagePath.c_str(),sect,slice)); }
      else if (histoType == 5){ can->Print(Form("%scrossSection/crossSectionSector%dSlice%d.png",imagePath.c_str(),sect,slice)); }
      else if (histoType == 6){ can->Print(Form("%scrossSectionRatio/crossSectionRatioSector%dSlice%d.png",imagePath.c_str(),sect,slice)); }
      else if (histoType == 7){ can->Print(Form("%spurity/purityBins%dSector%dSlice%d.png",imagePath.c_str(),numberOfXBins,sect,slice)); }
      else if (histoType == 8){ can->Print(Form("%sradCorr/radCorrSector%dSlice%d.png",imagePath.c_str(),sect,slice)); }
      else if (histoType == 9){ can->Print(Form("%selasticSubtraction/inelasticFractionSector%dSlice%d.png",imagePath.c_str(),sect,slice)); }
      else if (histoType == 10){ can->Print(Form("%sbinCorr/binCorrSector%dSlice%d.png",imagePath.c_str(),sect,slice)); }
    }
  }
  
}

void showIntegratedCombinedHisto(TH1D *histo[numberSector], const int numberSector){
  TCanvas * can = new TCanvas("can","",800,800);
  histo[0]->SetMarkerColor(kBlack);
  histo[0]->SetMarkerStyle(7);
  histo[0]->Draw("pe");
}

void showRatio0To1(TH1D *histo[numberSector], const int numberSector){
  TCanvas *can = new TCanvas("can","",800,800);
  histo[0]->SetMinimum(0.0);
  histo[0]->SetMaximum(1.1);
  histo[0]->SetLineColor(kRed);
  histo[0]->Draw("hist");
}

void printPurityStudy(TH1D *purity[numberSector][numberSlices], TH1D* rec[numberSector][numberSlices],
		      TH1D* gen[numberSector][numberSlices],TH1D* recAndGen[numberSector][numberSlices], const int numberSector, const int numberSlices, const int sectorToPrint, string pdfTitle){


  TCanvas *purityCanvas = new TCanvas("purityCanvas","",1600,800);
  purityCanvas->Divide(4,3);

  string openPdf = Form("%s[",pdfTitle.c_str());
  string closePdf = Form("%s]",pdfTitle.c_str());
  purityCanvas->Print(openPdf.c_str());
  purityCanvas->SetGridx();
  purityCanvas->SetGridy();

  TLatex xCaption, yCaption, sectorCaption;
  xCaption.SetNDC();
  xCaption.SetTextFont(22);
  yCaption.SetNDC();
  yCaption.SetTextAngle(90.0);
  yCaption.SetTextFont(22);
  sectorCaption.SetNDC();
  sectorCaption.SetTextFont(22);

  // For calculating which QQ bins we're in
  double qqMin = 1.7; double qqMax = 4.2; 
  double qqWidth = (qqMax-qqMin)/numberSlices; 

  int currentPad = 1; 
  for (int slice=0; slice<numberSlices; slice++){

    if (currentPad > 12){ 
      currentPad = 1; 
      purityCanvas->Print(pdfTitle.c_str());
    }

    double qq = qqMin + slice*qqWidth/2; 
    purityCanvas->cd(currentPad);

    gen[sectorToPrint][slice]->SetLineColor(kRed);
    rec[sectorToPrint][slice]->SetLineColor(kBlue);
    recAndGen[sectorToPrint][slice]->SetLineColor(kBlack);

    purityCanvas->SetLogy(1);
    gen[sectorToPrint][slice]->Draw("hist");
    rec[sectorToPrint][slice]->Draw("histsame");
    recAndGen[sectorToPrint][slice]->Draw("histsame");
    xCaption.DrawLatex(0.65, 0.05, "W (GeV/c^{2})");
    yCaption.DrawLatex(0.05, 0.65, "Events");
    purityCanvas->SetLogy(0);

    sectorCaption.SetTextColor(kBlue);
    sectorCaption.DrawLatex(0.6, 0.8, "#rightarrow rec");
    sectorCaption.SetTextColor(kRed);
    sectorCaption.DrawLatex(0.6, 0.75, "#rightarrow gen");
    sectorCaption.SetTextColor(kBlack);
    sectorCaption.DrawLatex(0.6, 0.7, "#rightarrow rec+gen");
    sectorCaption.DrawLatex(0.4, 0.95,Form("Q^{2} = %.3f (GeV^{2}/c^{2})",qq));

    purityCanvas->cd(currentPad+1);
    purity[sectorToPrint][slice]->SetLineColor(kBlack);
    purity[sectorToPrint][slice]->SetMinimum(0.0);
    purity[sectorToPrint][slice]->SetMaximum(1.0);
    purity[sectorToPrint][slice]->Draw("hist");
    xCaption.DrawLatex(0.65, 0.05, "W (GeV/c)");
    yCaption.DrawLatex(0.05, 0.65, "Purity");
    sectorCaption.DrawLatex(0.4, 0.95,Form("Q^{2} = %.3f (GeV^{2}/c^{2})",qq));

    currentPad += 2;
  }

  purityCanvas->Print(closePdf.c_str());

} 

void loadModel(TH2D *model, const int numberQQBins, const int numberWBins, string modelFile){
  ifstream inputDataFile(modelFile.c_str());


  int xBin, yBin; 
  double val;
  for (int qqBin=1; qqBin<= numberQQBins; qqBin++){
    for (int wBin=1; wBin<= numberWBins; wBin++){
      inputDataFile >> xBin; 
      inputDataFile >> yBin; 
      inputDataFile >> val; 
      cout << Form("Filling (%d, %d) = %f",xBin, yBin, val) << endl; 
      model->SetBinContent(xBin, yBin, val);
      model->SetBinError(xBin, yBin, 0.00);
    }
  }

  inputDataFile.close();
}

int getNumberOfCanvasDivisions(const int numberOfHistograms){
  int divs = 1;
  while( divs*(divs+1) < numberOfHistograms){ divs++; }
  return divs; 
}

// If you change the binning you must change this routine, there are better ways to do it but 
// this is how i'm doing it for now. 
void plot2Histos(TH1D *histo1[numberSector][numberSlices], TH1D *histo2[numberSector][numberSlices], const int numberSector, const int numberSlices, int histoType){
 
  // histoType 
  // 1 - Data & Simulation 
  // 2 - Data & Model 
  // 3 - Elastic & Inelastic Simulated Events 

  // For calculating which QQ bins we're in
  double qqMin = 1.7; double qqMax = 4.2; 
  double qqWidth = (qqMax-qqMin)/numberSlices; 

  TCanvas *singleCanvas = new TCanvas("singleCanvas","",800,500);
  TLatex xCaption, yCaption, sectorCaption, histo1Caption, histo2Caption; 

  xCaption.SetNDC();
  yCaption.SetNDC();
  sectorCaption.SetNDC();
  histo1Caption.SetNDC();
  histo2Caption.SetNDC();

  xCaption.SetTextSize(0.04);
  yCaption.SetTextSize(0.04);
  sectorCaption.SetTextSize(0.04); 
  histo1Caption.SetTextSize(0.04);
  histo2Caption.SetTextSize(0.04);
  histo2Caption.SetTextColor(kRed);

  xCaption.SetTextFont(102); 
  yCaption.SetTextFont(102); 
  sectorCaption.SetTextFont(102); 
  histo1Caption.SetTextFont(102); 
  histo2Caption.SetTextFont(102); 

  yCaption.SetTextAngle(90.0); 

  for (int s=0; s< numberSector; s++){
    double nEvents1 = 0; double nEvents2 = 0;


    /* 
    for (int sl=0; sl< numberSlices; sl++){
      // The model doesn't really work well above W=1.9 
      //      int stopIntegrating = histo1[s][sl]->GetBin(1.9);
      
      //      nEvents1 += histo1[s][sl]->Integral(1,stopIntegrating);
      //      nEvents2 += histo2[s][sl]->Integral(1,stopIntegrating);
      nEvents1 += histo1[s][sl]->Integral();
      nEvents2 += histo2[s][sl]->Integral();
    }
    */

    for (int sl=0; sl< numberSlices; sl++){
      double qqValue = qqMin + sl*qqWidth;

      if (histoType == 1) {
	//	histo1[s][sl]->Scale(1.0/nEvents1);
	//	histo2[s][sl]->Scale(1.0/nEvents2);
	//	histo1[s][sl]->Scale(1.0/histo1[s][sl]->Integral());
	histo2[s][sl]->Scale(histo1[s][sl]->Integral()/histo2[s][sl]->Integral());	
	histo1[s][sl]->SetMaximum( histo2[s][sl]->GetMaximum()*1.2 );
      }   
   

      if (histoType == 1){
	histo1[s][sl]->SetLineColor(99); 	
	histo2[s][sl]->SetLineColor(55);
	histo1[s][sl]->SetLineWidth(2); 	
	histo2[s][sl]->SetLineWidth(2);

	//	histo1[s][sl]->SetFillColorAlpha(99, 0.2); 
	//	histo2[s][sl]->SetFillColorAlpha(55, 0.2); 

	histo1Caption.SetTextColor(99); 
	histo2Caption.SetTextColor(55); 

	//	histo2[s][sl]->SetFillStyle(3004);
	histo1[s][sl]->Draw("hist");
	histo2[s][sl]->Draw("histsame");
      }

      else if (histoType == 2) { 
      histo1[s][sl]->SetMarkerStyle(8);
      histo1[s][sl]->Draw("pe");

	//	histo2[s][sl]->SetLineStyle(8);
	histo2[s][sl]->SetLineColor(55);
	histo2[s][sl]->SetLineWidth(2);
	histo2[s][sl]->Draw("csame"); 
      }

      else if (histoType == 3) {
	gPad->SetLogy(); 
	histo1[s][sl]->SetFillColorAlpha(99,0.4);
	histo1[s][sl]->SetLineColor(99);
	histo1[s][sl]->SetMaximum( histo2[s][sl]->GetMaximum()*1.2 );
	histo1Caption.SetTextColor(99);
	histo1[s][sl]->Draw("hist");


	//	histo2[s][sl]->SetFillColorAlpha(58,0.4);
	histo2[s][sl]->SetLineColor(58);
	histo2Caption.SetTextColor(58);
	histo2[s][sl]->Draw("histsame");
      }

      sectorCaption.DrawLatex(0.61,0.89,Form("Sector %d Q^{2}=%.3f",s,qqValue));
      xCaption.DrawLatex(0.46, 0.04, "W (GeV/c^{2})");


      if (histoType == 1){
	yCaption.DrawLatex(0.05, 0.45, "Counts"); 
	histo1Caption.DrawLatex(0.05, 0.96, Form("Data Events = %.2E",histo1[s][sl]->GetEntries()));
	histo2Caption.DrawLatex(0.05, 0.92, Form("Sim Events = %.2E",histo2[s][sl]->GetEntries()));
      }

      else if (histoType == 2){
	yCaption.DrawLatex(0.05, 0.45, "#muB"); 
	histo2Caption.SetTextColor(55);
	histo1Caption.DrawLatex(0.05, 0.96, "E1-F Data");
	histo2Caption.DrawLatex(0.05, 0.92, "Keppel Model");
      }

      else if (histoType == 3){
	histo1Caption.DrawLatex(0.18, 0.76, "Elastic");
	histo2Caption.DrawLatex(0.18, 0.72, "Inelastic");
      }

      if (histoType == 1)      { singleCanvas->Print(Form("%scompareDataAndSim/compareDataSimSector%dSlice%d.png",imagePath.c_str(),s,sl)); }
      else if (histoType == 2) { singleCanvas->Print(Form("%scompareDataAndModel/compareDataModelSector%dSlice%d.png",imagePath.c_str(),s,sl)); }
      else if (histoType == 3) { singleCanvas->Print(Form("%selasticSubtraction/compareElasticAndInelasticSector%dSlice%d.png",imagePath.c_str(),s,sl)); }
    }
  }
}

void plot3Histos(TH1D *histo1[numberSector][numberSlices], TH1D *histo2[numberSector][numberSlices], TH1D *histo3[numberSector][numberSlices], 
		 const int numberSector, const int numberSlices, int histoType){

   // histoType 
  // 1 - Data & Simulation 
  // 2 - Data & Model 

  // For calculating which QQ bins we're in
  double qqMin = 1.5; double qqMax = 4.8; 
  double qqWidth = (qqMax-qqMin)/numberSlices; 

  TCanvas *singleCanvas = new TCanvas("singleCanvas","",800,800);
  TLatex xCaption, sectorCaption, histo1Caption, histo2Caption; 

  xCaption.SetNDC();
  sectorCaption.SetNDC();
  histo1Caption.SetNDC();
  histo2Caption.SetNDC();

  xCaption.SetTextSize(0.03);
  sectorCaption.SetTextSize(0.03); 
  histo1Caption.SetTextSize(0.03);
  histo2Caption.SetTextSize(0.03);
  histo2Caption.SetTextColor(kRed);

  for (int s=0; s< numberSector; s++){
    double nEvents1 = 0; double nEvents2 = 0;

    for (int sl=0; sl< numberSlices; sl++){
      nEvents1 += histo1[s][sl]->Integral();
      nEvents2 += histo2[s][sl]->Integral();
    }

    for (int sl=0; sl< numberSlices; sl++){

      double qqValue = qqMin + sl*qqWidth;

      singleCanvas->cd();
      sectorCaption.DrawLatex(0.61,0.89,Form("Sector %d Q^{2}=%.3f",s,qqValue));
      xCaption.DrawLatex(0.62, 0.04, "W (GeV/c^{2})");
      //      xCaption.DrawLatex(0.62, 0.04, "X_{Bjorken}");

      TPad *upperPad = new TPad("upperPad","",0.0,0.3,1.0,1.0); 
      upperPad->SetBottomMargin(0.0); 
      upperPad->Draw(); 
      upperPad->cd(); 

      if (histoType == 1) {
	histo1[s][sl]->Scale(1.0/nEvents1);
	histo2[s][sl]->Scale(1.0/nEvents2);	
	histo1[s][sl]->SetMaximum( histo2[s][sl]->GetMaximum()*1.2 );
      }   

      histo1[s][sl]->SetMarkerStyle(8);
      histo1[s][sl]->Draw("pe");
      histo1[s][sl]->GetYaxis()->SetLabelSize(0.02);
      histo1[s][sl]->GetYaxis()->SetNdivisions(505);

      //      TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
      //      axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
      //      axis->SetLabelSize(15);
      //      axis->Draw();
   

      if (histoType == 1){
	histo2[s][sl]->SetFillColorAlpha(kRed, 1.0);
	histo2[s][sl]->SetLineColor(kRed);
	histo2[s][sl]->SetFillStyle(3004);
	histo2[s][sl]->Draw("histsame");
      }

      else if (histoType == 2) { 
	histo2[s][sl]->SetLineStyle(1);
	histo2[s][sl]->SetLineWidth(1);
	histo2[s][sl]->SetLineColor(99);
	histo2[s][sl]->Draw("lsame"); 
      }

      if (histoType == 1){
	histo1Caption.DrawLatex(0.18, 0.93, Form("Data Events = %.2E",histo1[s][sl]->GetEntries()));
	histo2Caption.DrawLatex(0.18, 0.89, Form("Sim Events = %.2E",histo2[s][sl]->GetEntries()));
      }

      else if (histoType == 2){
	histo2Caption.SetTextColor(99);
	histo1Caption.DrawLatex(0.18, 0.95, "E1-F Data");
	histo2Caption.DrawLatex(0.18, 0.90, "Keppel Model");
      }

      // Switch to lower pad for ratio plots 
      singleCanvas->cd();

      TPad *lowerPad = new TPad("lowerPad","",0.0,0.0,1.0,0.3); 
      lowerPad->SetTopMargin(0.0);
      lowerPad->SetBottomMargin(0.2);
      lowerPad->Draw();
      lowerPad->cd();

      histo3[s][sl]->SetMarkerStyle(8);
      histo3[s][sl]->GetYaxis()->SetLabelSize(0.08);
      histo3[s][sl]->GetYaxis()->SetNdivisions(303);
      histo3[s][sl]->GetXaxis()->SetLabelSize(0.08);
      histo3[s][sl]->GetXaxis()->SetNdivisions(505);
      histo3[s][sl]->SetMinimum(0.5); 
      histo3[s][sl]->SetMaximum(1.5); 
      histo3[s][sl]->Draw("pe");

      if (histoType == 1)      { singleCanvas->Print(Form("compareDataAndSim/compareDataSimRatioSector%dSlice%d.png",s,sl)); }
      else if (histoType == 2) { singleCanvas->Print(Form("compareDataAndModel/compareDataModelRatioSector%dSlice%d.png",s,sl)); }

      singleCanvas->Clear();
    }
  }
}

TH1D *linearizeTH2(TH2D *histo){
  const int nBins = histo->GetSize(); 
  TH1D *newHisto = new TH1D(Form("%sLinearized",histo->GetName()),"",nBins,1,nBins); 
  for(int b=0; b<nBins; b++){ newHisto->SetBinContent(b,histo->GetBinContent(b));}
  return newHisto;
}
