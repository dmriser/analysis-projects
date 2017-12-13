{
  gROOT->LoadMacro("utils.C");

  const int numberSector = 7;
  TFile * inputFile = TFile::Open("../out/purityResultPass1.root");

  TH1D *purity200[numberSector];
  TH1D *purity100[numberSector];
  TH1D *purity66[numberSector];
  TH1D *purity50[numberSector];
  TH1D *purity40[numberSector];
  TH1D *purity34[numberSector];
  TH1D *purity20[numberSector];
  TH1D *purity10[numberSector];

  readIntegratedHisto(purity200, inputFile, numberSector, "purity200Bins_xByQQ");
  readIntegratedHisto(purity100, inputFile, numberSector, "purity100Bins_xByQQ");
  readIntegratedHisto(purity66, inputFile, numberSector, "purity66Bins_xByQQ");
  readIntegratedHisto(purity50, inputFile, numberSector, "purity50Bins_xByQQ");
  readIntegratedHisto(purity40, inputFile, numberSector, "purity40Bins_xByQQ");
  readIntegratedHisto(purity34, inputFile, numberSector, "purity34Bins_xByQQ");
  readIntegratedHisto(purity20, inputFile, numberSector, "purity20Bins_xByQQ");
  readIntegratedHisto(purity10, inputFile, numberSector, "purity10Bins_xByQQ");
  
  TControlBar *bar = new TControlBar("vertical","Plot Purity");
  bar->AddButton("","");
  bar->AddButton("Purity 200 Bins","showRatio0To1(purity200, numberSector)");
  bar->AddButton("Purity 100 Bins","showRatio0To1(purity100, numberSector)");
  bar->AddButton("Purity  66 Bins","showRatio0To1(purity66, numberSector)");
  bar->AddButton("Purity  50 Bins","showRatio0To1(purity50, numberSector)");
  bar->AddButton("Purity  40 Bins","showRatio0To1(purity40, numberSector)");
  bar->AddButton("Purity  34 Bins","showRatio0To1(purity34, numberSector)");  
  bar->AddButton("Purity  20 Bins","showRatio0To1(purity20, numberSector)");
  bar->AddButton("Purity  10 Bins","showRatio0To1(purity10, numberSector)");
  bar->AddButton("","");
  bar->AddButton("Quit",".q");
  bar->Show();

}
