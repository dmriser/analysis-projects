{

  gROOT->LoadMacro("utils.C");

  const int numberSector = 7;
  const int numberSlices = 10;

  float q2_min = 1.7; 
  float q2_max = 4.2; 
  float width  = (q2_max-q2_min)/numberSlices; 

  TFile * inputFile = TFile::Open("/volatile/clas12/dmriser/rootFiles/inclusive/xs/xsBugFixNoES.root");

  TH1D *crossSection_wByQQ[numberSector][numberSlices];
  
  readHisto(crossSection_wByQQ, inputFile, numberSector, numberSlices, "binCorrectedCrossSection_wByQQ");

  std::cout << "q2_bin,q2,w_bin,w,value,error" << std::endl; 
  for(int s=0; s<numberSlices; s++){
    for(int b=1; b<=crossSection_wByQQ[1][s]->GetXaxis()->GetNbins(); b++){
      float q2 = q2_min + s*width + width/2.0; 
      std::cout << s+1 << "," << Form("%.4e", q2) << "," << getBinString(crossSection_wByQQ[1][s], b) << std::endl;     
    }
  }


}
