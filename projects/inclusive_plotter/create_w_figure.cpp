void create_w_figure(){

  std::string input_filename("/volatile/clas12/dmriser/rootFiles/inclusive/both/bugFix.root"); 

  // open file 
  TFile *input_file = TFile::Open(input_filename.c_str()); 

  // open histos 
  TH2F *data = (TH2F*) input_file->Get("dataEvents_wByQQ_s0"); 
  TH2F *rec  = (TH2F*) input_file->Get("recEventsRad_wByQQ_s0"); 

  // no style 
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  TCanvas *canvas = new TCanvas("canvas", "", 1600, 800); 
  canvas->Divide(2,1);

  TLatex *latex = new TLatex();
  latex->SetNDC();
  latex->SetTextFont(42);
  latex->SetTextSize(0.03);

  canvas->cd(1);
  data->Draw("colz"); 
  gPad->SetLogz();

  latex->DrawLatex(0.45, 0.02, "W (GeV/c^{2})");
  latex->SetTextAngle(90.0);
  latex->DrawLatex(0.05, 0.45, "Q^{2} (GeV^{2}/c^{2})"); 
  latex->SetTextAngle(0.0);
  latex->DrawLatex(0.4, 0.95, "Data Events"); 

  canvas->cd(2);
  rec->Draw("colz"); 
  gPad->SetLogz();

  latex->DrawLatex(0.45, 0.02, "W (GeV/c^{2})");
  latex->SetTextAngle(90.0);
  latex->DrawLatex(0.05, 0.45, "Q^{2} (GeV^{2}/c^{2})"); 
  latex->SetTextAngle(0.0);
  latex->DrawLatex(0.4, 0.95, "Rec. MC Events"); 

  canvas->Print("w_qq.pdf");
}
