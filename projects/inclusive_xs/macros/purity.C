{
  //  gROOT->LoadMacro("utils.C"); 

  // -----------------------------------------------
  //      Your parameters here 
  // -----------------------------------------------

  const int NCONF    = 5; 
  string name[NCONF] = {"80Bins20Bins","35Bins10Bins","35Bins6Bins","35Bins4Bins","30Bins10Bins"};
  string imagePath   = "/volatile/clas12/dmriser/plots/inclusive/purity/";

  TFile *inputFile = TFile::Open("/volatile/clas12/dmriser/rootFiles/inclusive/purity/pass3.root"); 
  TH2D *rec[NCONF];        
  TH2D *recAndGen[NCONF];  
  TH1D *purity[NCONF];
  TH1D *rec1d[NCONF];
  TH1D *recAndGen1d[NCONF];

  for(int c=0; c<NCONF; c++){
    rec[c]        = (TH2D*) inputFile->Get(Form("recEvents%s_wByQQ_s0",      name[c].c_str())); 
    recAndGen[c]  = (TH2D*) inputFile->Get(Form("recAndGenEvents%s_wByQQ_s0",name[c].c_str())); 

    int xbins = rec[c]->GetXaxis()->GetNbins();
    int ybins = rec[c]->GetYaxis()->GetNbins();

    const int numberBins = xbins*ybins; 
    purity[c]      = new TH1D(Form("purityLin%s",name[c].c_str()),"",numberBins,1,numberBins); 
    rec1d[c]       = new TH1D(Form("recLin%s",name[c].c_str()),"",numberBins,1,numberBins); 
    recAndGen1d[c] = new TH1D(Form("recAndGenLin%s",name[c].c_str()),"",numberBins,1,numberBins); 

    for(int bin=0; bin<numberBins; bin++){
      double p = (double)recAndGen[c]->GetBinContent(bin)/rec[c]->GetBinContent(bin);

      // Check we should add this 
      bool isGoodBin = false; 
      if (bin < xbins){ isGoodBin = true; }
      else if (bin > xbins && bin%xbins != 1 && bin%xbins != 2){ isGoodBin = true; }

      if(isGoodBin){
	rec1d[c]      ->SetBinContent(bin,      rec[c]->GetBinContent(bin)); 
	recAndGen1d[c]->SetBinContent(bin,recAndGen[c]->GetBinContent(bin)); 
	if (p < 1.01 && p > -0.01){ purity[c]->SetBinContent(bin,p); }
      }

    }

  }


  TCanvas *can[NCONF];
  for (int c=0; c<NCONF; c++){  
    can[c] = new TCanvas(Form("can%d",c),"",800,800); 
    can[c]->Divide(1,2); 
 
    can[c]->cd(1);
    purity[c]->Draw(); 
 
    TLatex title, xTitle, yTitle, caption; 
    title.SetNDC();  
    title.SetTextSize(0.05);
    title.SetTextFont(42);

    caption.SetNDC();  
    caption.SetTextSize(0.03);
    caption.SetTextFont(42);

    xTitle.SetNDC();  
    xTitle.SetTextSize(0.05);
    xTitle.SetTextFont(42);

    yTitle.SetNDC();  
    yTitle.SetTextSize(0.05);
    yTitle.SetTextFont(42);
    yTitle.SetTextAngle(90.0); 
    
    xTitle.DrawLatex(0.41, 0.08, "Global Bin Number"); 
    yTitle.DrawLatex(0.1, 0.47, "Purity"); 

    int xbins = rec[c]->GetXaxis()->GetNbins();
    int ybins = rec[c]->GetYaxis()->GetNbins();
    title.DrawLatex(0.35,0.9,Form("%d W-Bins, %d Q^{2}-Bins",xbins,ybins)); 

    double average = purity[c]->Integral()/purity[c]->GetXaxis()->GetNbins();
    caption.DrawLatex(0.05,0.02,Form("Average Purity = %.2f",average)); 

    can[c]->cd(2);
    rec1d[c]->SetLineColor(99);
    rec1d[c]->Draw();

    recAndGen1d[c]->SetLineColor(77);
    recAndGen1d[c]->Draw("same");

    gPad->SetLogy();
    gPad->SetGridy();
    gPad->SetTopMargin(0.01);

    can[c]->Print(Form("%sPurity%s.png",imagePath.c_str(),name[c].c_str())); 
  }

}
