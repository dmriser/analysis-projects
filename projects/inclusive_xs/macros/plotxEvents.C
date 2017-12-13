{

  gROOT->LoadMacro("utils.C");

  TFile * inputFile = TFile::Open("../out/pass1.root");
  const int numberSector = 7;

  TH2D *dataEvents[numberSector];
  TH2D *recEvents[numberSector];
  TH2D *genEvents[numberSector];

  readEvents(dataEvents, inputFile, numberSector, "dataEvents_xByQQ");
  readEvents(recEvents, inputFile, numberSector, "recEventsRad_xByQQ");
  readEvents(genEvents, inputFile, numberSector, "genEventsRad_xByQQ");

  TControlBar *bar = new TControlBar("vertical","Plot Events");
  bar->AddButton("","");
  bar->AddButton("Data Events","showEvents(dataEvents, numberSector)");
  bar->AddButton("Rec Events","showEvents(recEvents, numberSector)");
  bar->AddButton("Gen Events","showEvents(genEvents, numberSector)");
  bar->AddButton("","");
  bar->Show();

}
