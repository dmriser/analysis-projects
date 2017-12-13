{

  gROOT->LoadMacro("utils.C");

  const int numberSector = 7;
  const int numberSlices = 10;

  TH1D *acceptance[numberSector][numberSlices];
  TH1D *crossSection[numberSector][numberSlices];
  TH1D *crossSectionRatio[numberSector][numberSlices];
  TH1D *binCorrection[numberSector][numberSlices];
  TH1D *binCorrectedCrossSection[numberSector][numberSlices];
  TH1D *binCorrectedCrossSectionRatio[numberSector][numberSlices];


  TFile *inputFile = TFile::Open("../out/crossSectionResultNewModelPass1.root");
  
  readHisto(acceptance, inputFile, numberSector, numberSlices, "acceptance_xByQQ");
  readHisto(crossSection, inputFile, numberSector, numberSlices, "crossSection_xByQQ");
  readHisto(crossSectionRatio, inputFile, numberSector, numberSlices, "crossSectionRatio_xByQQ");
  readHisto(binCorrection, inputFile, numberSector, numberSlices, "binCenterCorrection_xByQQ");
  readHisto(binCorrectedCrossSection, inputFile, numberSector, numberSlices, "binCorrectedCrossSection_xByQQ");
  readHisto(binCorrectedCrossSectionRatio, inputFile, numberSector, numberSlices, "binCorrectedCrossSectionRatio_xByQQ");
  
  TControlBar *bar = new TControlBar("vertical","Plot Slices");
  bar->AddButton("","");
  bar->AddButton("Acceptance","showHisto(acceptance, numberSector, numberSlices)");
  bar->AddButton("Bin Centering Correction","showHisto(binCorrection, numberSector, numberSlices)");
  bar->AddButton("","");
  bar->AddButton("Cross Section","showHisto(crossSection, numberSector, numberSlices)");
  bar->AddButton("Cross Section Ratio","showHisto(crossSectionRatio, numberSector, numberSlices)");
  bar->AddButton("","");
  bar->AddButton("Bin Corrected Cross Section","showHisto(binCorrectedCrossSection, numberSector, numberSlices)");
  bar->AddButton("Bin Corrected Cross Section Ratio","showHisto(binCorrectedCrossSectionRatio, numberSector, numberSlices)");
  bar->AddButton("","");
  bar->AddButton("Quit",".q");
  bar->Show();

}
