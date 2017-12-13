{
  gROOT->LoadMacro("utils.C");

  const int numberSector = 7;
  TFile * inputFile = TFile::Open("../out/purityResultPass1.root");

  TH1D *stability200[numberSector];
  TH1D *stability100[numberSector];
  TH1D *stability66[numberSector];
  TH1D *stability50[numberSector];
  TH1D *stability40[numberSector];
  TH1D *stability34[numberSector];
  TH1D *stability20[numberSector];
  TH1D *stability10[numberSector];

  readIntegratedHisto(stability200, inputFile, numberSector, "stability200Bins_wByQQ");
  readIntegratedHisto(stability100, inputFile, numberSector, "stability100Bins_wByQQ");
  readIntegratedHisto(stability66, inputFile, numberSector, "stability66Bins_wByQQ");
  readIntegratedHisto(stability50, inputFile, numberSector, "stability50Bins_wByQQ");
  readIntegratedHisto(stability40, inputFile, numberSector, "stability40Bins_wByQQ");
  readIntegratedHisto(stability34, inputFile, numberSector, "stability34Bins_wByQQ");
  readIntegratedHisto(stability20, inputFile, numberSector, "stability20Bins_wByQQ");
  readIntegratedHisto(stability10, inputFile, numberSector, "stability10Bins_wByQQ");
  
  TControlBar *bar = new TControlBar("vertical","Plot Stability");
  bar->AddButton("","");
  bar->AddButton("Stability 200 Bins","showRatio0To1(stability200, numberSector)");
  bar->AddButton("Stability 100 Bins","showRatio0To1(stability100, numberSector)");
  bar->AddButton("Stability  66 Bins","showRatio0To1(stability66, numberSector)");
  bar->AddButton("Stability  50 Bins","showRatio0To1(stability50, numberSector)");
  bar->AddButton("Stability  40 Bins","showRatio0To1(stability40, numberSector)");
  bar->AddButton("Stability  34 Bins","showRatio0To1(stability34, numberSector)");  
  bar->AddButton("Stability  20 Bins","showRatio0To1(stability20, numberSector)");
  bar->AddButton("Stability  10 Bins","showRatio0To1(stability10, numberSector)");
  bar->AddButton("","");
  bar->AddButton("Quit",".q");
  bar->Show();

}
