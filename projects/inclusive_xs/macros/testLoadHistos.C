{
  gROOT->LoadMacro("loadHistos.C"); 
  
  TFile *myFile = TFile::Open("/volatile/clas12/dmriser/rootFiles/inclusive/xs/goodRuns.root"); 
  
  vector<TObject*> histos = loadHistos(myFile,"binCorr");

  histos[0]->Draw();
}
