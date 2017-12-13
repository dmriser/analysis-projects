{

  gROOT->LoadMacro("tools.c"); 
  
  // input files 
  TFile *dataFile = TFile::Open("../out/test.root");
  TFile *simFile  = TFile::Open("../out/mc_test.root");

  // tree 
  TTree *dataEvents = (TTree*) dataFile->Get("events");
  TTree *simEvents  = (TTree*) simFile ->Get("events");

  // histos 
  TH1F *data = new TH1F("data", "data", 16, -3, 3);
  TH1F *rec  = new TH1F("rec", "rec", 16, -3, 3);
  TH1F *gen  = new TH1F("gen", "gen", 16, -3, 3);

  // fill 
  std::string cuts("w > 2.7 && q2 > 1.0 && missing_mass > 1.2");

  dataEvents->Draw("eta >> data", cuts.c_str(), ""); 
  simEvents ->Draw("eta >> rec", cuts.c_str(), ""); 
  simEvents ->Draw("gen_eta >> gen", "gen_w > 2.7 && gen_q2 > 1.0", ""); 
 
  // calculate acceptance 
  TH1F *acc = (TH1F*) rec->Clone(); 
  customDivide(acc, gen); 

  TH1F *dataCorr = (TH1F*) data->Clone(); 
  dataCorr->SetName("dataCorr"); 
  dataCorr->SetTitle("dataCorr"); 
  dataCorr->Divide(acc);

  style(data);
  style(dataCorr);
  style(rec);
  style(gen);
  style(acc);

  TCanvas *can = new TCanvas("can", "can", 1200, 1200); 
  can->Divide(2,2);
  
  can->cd(1);
  data->Draw(); 
  
  can->cd(2);
  rec->Draw(); 
  
  can->cd(3);
  gen->Draw(); 

  can->cd(4);
  acc->Draw();

  TCanvas *can2 = new TCanvas("can2", "", 1200, 500);

  can2->Divide(2,1); 

  can2->cd(1);
  data->Draw();

  can2->cd(2);
  dataCorr->Draw();

}
