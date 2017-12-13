{

  gROOT->LoadMacro("utils.C");

  const int numberSector = 7;
  TFile * inputFile = TFile::Open("../out/crossSectionWithResonancePass1.root");

  TH1D *acceptance_xByQQ[numberSector];
  TH1D *crossSection_xByQQ[numberSector];
  TH1D *crossSectionRatio_xByQQ[numberSector];
  TH1D *binCorrectedCrossSection_xByQQ[numberSector];
  TH1D *binCorrectedCrossSectionRatio_xByQQ[numberSector];
  
  readIntegratedHisto(acceptance_xByQQ, inputFile, numberSector,"acceptance_xByQQ");
  readIntegratedHisto(crossSection_xByQQ, inputFile, numberSector,"crossSection_xByQQ");
  readIntegratedHisto(crossSectionRatio_xByQQ, inputFile, numberSector,"crossSectionRatio_xByQQ");
  readIntegratedHisto(binCorrectedCrossSection_xByQQ, inputFile, numberSector,"binCorrectedCrossSection_xByQQ");
  readIntegratedHisto(binCorrectedCrossSectionRatio_xByQQ, inputFile, numberSector,"binCorrectedCrossSectionRatio_xByQQ");

  TControlBar * bar = new TControlBar("vertical","Plot Integrated Histos");
  bar->AddButton("","");
  bar->AddButton("Acceptance","showIntegratedHisto(acceptance_xByQQ,numberSector)");
  bar->AddButton("","");
  bar->AddButton("Cross Section","showIntegratedHisto(crossSection_xByQQ,numberSector)");
  bar->AddButton("Cross Section Ratio","showIntegratedHisto(crossSectionRatio_xByQQ,numberSector)");
  bar->AddButton("","");
  bar->AddButton("Bin Corrected Cross Section","showIntegratedHisto(binCorrectedCrossSection_xByQQ,numberSector)");
  bar->AddButton("Bin Corrected Cross Section Ratio","showIntegratedHisto(binCorrectedCrossSectionRatio_xByQQ,numberSector)");
  bar->AddButton("","");
  bar->AddButton("Quit",".q");
  bar->Show();
}
