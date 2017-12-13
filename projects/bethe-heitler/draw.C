{

  string imagePath    = "/volatile/clas/clas12/dmriser/plots/betheHeitler/";
  TFile *inputFile    = TFile::Open("/volatile/clas/clas12/dmriser/rootFiles/betheHeitler/BetheHeitlerEvents.root");
  THnSparseI *events  = (THnSparseI*) inputFile->Get("events"); 

  TH1D *mass          = events->Projection(1); 
  TH1D *w             = events->Projection(2); 
  TH1D *angle         = events->Projection(0); 
  TH1D *coplanarAngle = events->Projection(3);
  TH1D *angleEPrimeX  = events->Projection(4);
  TH2D *massAngle     = events->Projection(0,1); 
  TH2D *massW         = events->Projection(2,1); 

  gROOT->LoadMacro("functions.C"); 


  //  displayMassAngle();
  //  displayMassW(); 
  //  applyCuts(-0.15, 0.1, 177, 183); 

  //  displayW(); 

  //  displayElectronAngles();
  displayMissingMassCoplanarAngle();
  //  setRange("missingMass",-0.15, 0.1);
  //  displayCoplanarAngles(); 
  //  setRange("missingMass",-1.00, 5.0);
  //  setRange("coplanarAngle",175,185);
  //  displayMissingMass(); 


}
