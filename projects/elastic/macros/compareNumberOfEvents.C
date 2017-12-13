{

  // ---------------------------------------------
  //    User Parameters 
  // ---------------------------------------------
  TFile *eventsFile = TFile::Open("../out/sectorFix.root"); 
  TFile *xsFile     = TFile::Open("../out.root"); 
  

  // ---------------------------------------------
  //    Other Things
  // ---------------------------------------------
  THnSparseI *rawEvents = (THnSparseI*) eventsFile->Get("dataEvents"); 
  TH1D *counts = rawEvents->Projection(0);

  TH1D *dataEvents[6]; 

  for (int sector=0; sector<6; sector++){
    dataEvents[sector] = (TH1D*) xsFile->Get(Form("dataEvents_thetaByPhi_sect%d_bin0",sector)); 
    cout << "[CompareNumberOfEvents] Projected Histo, Events = " << dataEvents[sector]->Integral() << " Sparse, Events = " << counts->GetBinContent(sector+1) << endl; 
  }


}
