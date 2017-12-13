{

  gROOT->LoadMacro("loadHistos.C"); 
  TFile *inputFile = TFile::Open("/volatile/clas12/dmriser/rootFiles/inclusive/xs/goodRuns.root"); 
  
  vector<TH1D*> xs = loadHistos(inputFile,"radCorrCrossSectionRatio_wByQQ_s[0-6]_slice[0-9]");

  for(int s=0; s<xs.size(); s++){
      xs[s]->SetMarkerStyle(8);
      xs[s]->SetMarkerColor(99);
  }

  drawOneHisto(xs[4],"E1-F Inclusive Sample","W (GeV/c^{2})","Ratio",0); 

}
