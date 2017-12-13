{

  gROOT->LoadMacro("utils.C");

  string basePath  = "/volatile/clas12/dmriser/rootFiles/inclusive/xs";
  string imagePath = "/volatile/clas12/dmriser/plots/inclusive/compareCrossSection";

  // General Setup 
  const int NCONF          = 2; 
  //  string fileName[NCONF]   = {basePath+"/y74.root",basePath+"/y72.root",basePath+"/y70.root",basePath+"/y68.root",basePath+"/y66.root"};
  //  string configName[NCONF] = {"y < 0.74","y < 0.72","y < 0.70","y < 0.68","y < 0.66"};
  //  string fileName[NCONF]   = {basePath+"/y70LargeBins.root",basePath+"/y70Loose.root",basePath+"/y70Tight.root"};
  //  string configName[NCONF] = {"nominal","loose","tight"};
  string fileName[NCONF]   = {basePath+"/Phi4to12.root",basePath+"/Phi4to12ES.root"};
  string configName[NCONF] = {"Normal","ElastSub"};

  // Physics Specifics 
  const int numberSector = 7;
  const int numberSlices = 10;

  TFile *inputFiles[NCONF];
  TH1D  *ratio[NCONF][numberSector][numberSlices];
  TH1D  *model[NCONF][numberSector][numberSlices];
  TH1D  *crossSection[NCONF][numberSector][numberSlices];
  TH1D  *ratioSummary[NCONF][numberSector];
  TH1D  *chi2Summary[NCONF][numberSector];
  TF1 *lineFit = new TF1("lineFit","pol0",1.2,1.9);

  for(int file=0; file<NCONF; file++){
    inputFiles[file] = TFile::Open(fileName[file].c_str());
    cout << "[compareCrossSections] Opened file: " << fileName[file] << endl; 

    readHisto(ratio[file],        inputFiles[file], numberSector, numberSlices, "radCorrCrossSectionRatio_wByQQ");
    readHisto(crossSection[file], inputFiles[file], numberSector, numberSlices, "radCorrCrossSection_wByQQ");
    readHisto(model[file],        inputFiles[file], numberSector, numberSlices, "modelCrossSection_wByQQ");
    
    // Assigning uncertainty to model, need justification for 
    // what value to pick. 
    double modelError = 0.10; 

    for(int sector=0; sector<7; sector++){
      ratioSummary[file][sector] = new TH1D(Form("ratioSummarySector%d_%s",sector,configName[file].c_str()),"",numberSlices,0,numberSlices-1);
      chi2Summary[file][sector]  = new TH1D(Form("chi2SummarySector%d_%s",sector,configName[file].c_str()),"",numberSlices,0,numberSlices-1);
      
      for(int slice=0; slice<numberSlices; slice++){
	ratio[file][sector][slice]->Fit("lineFit","RQ");
	ratioSummary[file][sector]->SetBinContent(slice+1,lineFit->GetParameter(0)); 

	int numberOfXBins = model[file][sector][slice]->GetXaxis()->GetNbins();
	double chi2 = 0.0; 
	for(int bin=1; bin<=numberOfXBins; bin++){
	  double squareDiff = TMath::Power(crossSection[file][sector][slice]->GetBinContent(bin)-model[file][sector][slice]->GetBinContent(bin),2);
	  double sum        = crossSection[file][sector][slice]->GetBinContent(bin)+model[file][sector][slice]->GetBinContent(bin);
	  double var        = TMath::Power(crossSection[file][sector][slice]->GetBinError(bin),2) + TMath::Power(model[file][sector][slice]->GetBinContent(bin)*modelError,2);

	  chi2 += squareDiff/var; 
	}
	chi2 /= numberOfXBins;
	cout << Form("Calculated Chi2[%d][%d][%d]= ",file,sector,slice) << chi2 << endl; 
	chi2Summary[file][sector]->SetBinContent(slice+1,chi2); 
      }
    }
  }

  TLatex xLabel, yLabel, titleLabel, fileLabel; 
  xLabel.SetNDC();
  xLabel.SetTextFont(42);
  xLabel.SetTextSize(0.04);

  yLabel.SetNDC();
  yLabel.SetTextFont(42);
  yLabel.SetTextSize(0.04);
  yLabel.SetTextAngle(90.0);

  titleLabel.SetNDC();
  titleLabel.SetTextFont(42);
  titleLabel.SetTextSize(0.04);

  fileLabel.SetNDC();
  fileLabel.SetTextFont(42);
  fileLabel.SetTextSize(0.04);

  for(int sector=0; sector<numberSector; sector++){
  TCanvas *compareCanvas = new TCanvas("compareCanvas","",800,500); 

    for(int file=0; file<NCONF; file++){
      gPad->SetMargin(0.15, 0.25, 0.15, 0.15);
      ratioSummary[file][sector]->SetLineColor(file*5+50);
      ratioSummary[file][sector]->Draw("same");
      fileLabel.SetTextColor(file*5+50);
      fileLabel.DrawLatex(0.82, 0.86-file*0.04,Form("%s %.2f",configName[file].c_str(),ratioSummary[file][sector]->Integral()/ratioSummary[file][sector]->GetXaxis()->GetNbins())); 
    }
    
    xLabel.DrawLatex(0.72, 0.08, "Q^{2} Bin Number");
    yLabel.DrawLatex(0.10, 0.68, "Fit Param 0");
    titleLabel.DrawLatex(0.4, 0.9, Form("Sector %d",sector)); 
    compareCanvas->Print(Form("%s/ratioSector%d.png",imagePath.c_str(),sector));
  }
  
  for(int sector=0; sector<numberSector; sector++){
  TCanvas *compareCanvas = new TCanvas("compareCanvas","",800,500); 

    for(int file=0; file<NCONF; file++){
      gPad->SetMargin(0.15, 0.25, 0.15, 0.15);
      //      gPad->SetLogy();
      chi2Summary[file][sector]->SetMinimum(0.1);
      chi2Summary[file][sector]->SetMaximum(6.0);
      chi2Summary[file][sector]->SetLineColor(file*5+50);
      chi2Summary[file][sector]->Draw("same");
      fileLabel.SetTextColor(file*5+50);
      fileLabel.DrawLatex(0.82, 0.86-file*0.04,Form("%s %.2f",configName[file].c_str(),chi2Summary[file][sector]->Integral()/chi2Summary[file][sector]->GetXaxis()->GetNbins())); 
    }
    
    xLabel.DrawLatex(0.72, 0.08, "Q^{2} Bin Number");
    yLabel.DrawLatex(0.10, 0.68, "#chi^{2}/ndf");
    titleLabel.DrawLatex(0.4, 0.9, Form("Sector %d",sector)); 
    compareCanvas->Print(Form("%s/chi2Sector%d.png",imagePath.c_str(),sector));
  }


  for(int sector=0; sector<numberSector; sector++){
    
    for(int slice=0; slice<numberSlices; slice++){ 
      TCanvas *compareCanvas = new TCanvas("compareCanvas","",800,500); 
     
      for(int file=0; file<NCONF; file++){
	gPad->SetMargin(0.15, 0.25, 0.15, 0.15);
	crossSection[file][sector][slice]->SetMarkerColor(55 + 15*file);
	crossSection[file][sector][slice]->SetMarkerStyle(8);
	crossSection[file][sector][slice]->Draw("PEsame");
	fileLabel.SetTextColor(55 + 15*file); 
	fileLabel.DrawLatex(0.82, 0.86-file*0.04,configName[file].c_str()); 
      }
      model[0][sector][slice]->Draw("lsame");      

      
      xLabel.DrawLatex(0.62, 0.06, "W [GeV/c^{2}]");
      yLabel.DrawLatex(0.05, 0.68, "#frac{d#sigma}{dW dQ^{2}} [#mu Barn]");
      titleLabel.DrawLatex(0.3, 0.9, Form("Sector %d Slice %d",sector,slice)); 
      compareCanvas->Print(Form("%s/xsSector%dSlice%d.png",imagePath.c_str(),sector,slice));
    }
  }


}
