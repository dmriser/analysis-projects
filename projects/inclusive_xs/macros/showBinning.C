{

  // ------------------------------------------------------------------------
  // User Information changes here
  // ------------------------------------------------------------------------
  TFile *dataFile         = TFile::Open("/volatile/clas12/dmriser/rootFiles/eventMonitor/data.root"); 
  TFile *mcFile           = TFile::Open("/volatile/clas12/dmriser/rootFiles/eventMonitor/keppelRad.root"); 
  string imagePath        = "/volatile/clas12/dmriser/plots/inclusive/";
  string variationName[2] = {"Data","Monte Carlo"};
  const int numberXBins   = 35; 
  const int numberYBins   = 10;

  double xMin = 1.1; double xMax = 2.1; 
  double yMin = 1.7; double yMax = 4.2;

  // ------------------------------------------------------------------------
  // Doesn't Change 
  // ------------------------------------------------------------------------
  THnSparseI *dataEvents = (THnSparseI*) dataFile->Get("events"); 
  THnSparseI *mcEvents   = (THnSparseI*)   mcFile->Get("events"); 

  TH2D *hist[2];
  hist[0] = dataEvents->Projection(4,3); 
  hist[1] = mcEvents->Projection(4,3); 

  TCanvas *binnedCanvas = new TCanvas("binnedCanvas","",600,750); 
  binnedCanvas->Divide(1,2); 
 
  // Dress up the canvas with some information 
  TLatex xTitle, yTitle, title, binTitle; 
  title.SetNDC(); 
  title.SetTextFont(42);
  title.SetTextSize(0.05);  

  xTitle.SetNDC(); 
  xTitle.SetTextFont(42);
  xTitle.SetTextSize(0.04);  

  yTitle.SetNDC(); 
  yTitle.SetTextFont(42);
  yTitle.SetTextSize(0.04);  
  yTitle.SetTextAngle(90.0); 

  binTitle.SetNDC(); 
  binTitle.SetTextFont(42);
  binTitle.SetTextSize(0.03);  

  for (int type=0; type<2; type++){
    double absXMin = hist[type]->GetXaxis()->GetBinLowEdge(1);
    double absYMin = hist[type]->GetYaxis()->GetBinLowEdge(1);
    double absXMax = hist[type]->GetXaxis()->GetBinUpEdge(hist[type]->GetXaxis()->GetNbins());
    double absYMax = hist[type]->GetYaxis()->GetBinUpEdge(hist[type]->GetYaxis()->GetNbins());
    
    double xStep = (xMax-xMin)/numberXBins; 
    double yStep = (yMax-yMin)/numberYBins; 
    
    TLine *xLines[numberXBins+1]; 
    TLine *yLines[numberYBins+1];

    binnedCanvas->cd(type+1); 
    gPad->SetLogz(); 
    hist[type]->Draw("colz"); 

    // Create the lines and draw them on the histogram
    for (int xBin=0; xBin<numberXBins+1; xBin++){
      double x     = xBin*xStep + xMin; 
      xLines[xBin] = new TLine(x,absYMin,x,absYMax); 
      xLines[xBin]->Draw();
    }
    
    for (int yBin=0; yBin<numberYBins+1; yBin++){
      double y     = yBin*yStep + yMin; 
      yLines[yBin] = new TLine(absXMin,y,absXMax,y); 
      yLines[yBin]->Draw();
    }
    
    xTitle.DrawLatex(0.47, 0.065, "W (GeV/c^{2})"); 
    yTitle.DrawLatex(0.08, 0.45, "Q^{2} (GeV^{2}/c^{2})"); 
    title.DrawLatex(0.34, 0.88, Form("Inclusive Binning for %s",variationName[type].c_str())); 
    binTitle.DrawLatex(0.05, 0.05,Form("#delta x = %.2f, #delta y = %.2f",xStep,yStep)); 
  }

  binnedCanvas->Print(Form("%sBins.png",imagePath.c_str()));

}
