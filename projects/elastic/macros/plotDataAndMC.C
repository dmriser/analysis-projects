{

  // -----------------------------------------
  //     User Parameters 
  // -----------------------------------------
  TFile *dataFile  = TFile::Open("/volatile/clas12/dmriser/rootFiles/elastic/data.root"); 
  TFile *simFile   = TFile::Open("/volatile/clas12/dmriser/rootFiles/elastic/mc.root"); 
  string imagePath = "/volatile/clas12/dmriser/plots/elastic/"; 

  // -----------------------------------------
  //     Business Logic 
  // -----------------------------------------
  TH1D *wProtonMassSim[6]; 
  TH1D *wProtonMassData[6]; 
  TH1D *thetaSim[6];
  TH1D *thetaData[6];
  TH1D *rphiSim[6];
  TH1D *rphiData[6];

  gROOT->LoadMacro("utils.C");  
  loadSectorTH1D(dataFile, wProtonMassData, "h1_wProtonMass_sect%d_data");
  loadSectorTH1D(simFile,   wProtonMassSim, "h1_wProtonMass_sect%d_rec");
  loadSectorTH1D(dataFile,       thetaData, "h1_ele_theta_sect%d_data");
  loadSectorTH1D(simFile,         thetaSim, "h1_ele_theta_sect%d_rec");
  loadSectorTH1D(dataFile,        rphiData, "h1_ele_relPhi_sect%d_data");
  loadSectorTH1D(simFile,          rphiSim, "h1_ele_relPhi_sect%d_rec");

  scaleSectorHisto(wProtonMassData, wProtonMassSim); 
  scaleSectorHisto(thetaData, thetaSim); 
  scaleSectorHisto(rphiData, rphiSim); 

  printSectorTwoHistos(wProtonMassData, wProtonMassSim, imagePath, 0);
  printSectorTwoHistos(thetaData, thetaSim, imagePath, 1);
  printSectorTwoHistos(rphiData, rphiSim, imagePath, 2);

}
