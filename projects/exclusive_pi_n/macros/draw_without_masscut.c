{

  TChain *chain = new TChain("events");

  // --------------------------------------------
  //   setup program options 
  // --------------------------------------------
  int FILE_MAX = 5; 
  int FILE_MIN = 1; 
  std::string filePath("/volatile/clas12/dmriser/farm_out/pi_n_tree_v5");

  // --------------------------------------------
  //   main section of code 
  // --------------------------------------------
  for(int file=FILE_MIN; file<=FILE_MAX; file++){
    chain->AddFile( Form("%s/pi_n_tree_v5.%d.root", filePath.c_str(), file) );   
  }

  // Declare and project events into histograms.
  // If you put too many here its better to just 
  // make an event loop.

  std::cout << "Projecting events into 2-D histograms..." << std::endl; 
  TH2F *z_pt_all = new TH2F("z_pt_all", "", 200, 0.0, 1.05, 200, 0.0, 1.50); 
  chain->Draw("pt:z >> z_pt_all", "w > 2.0 && w < 3.5", "");

  TH2F *z_pt_low = new TH2F("z_pt_low", "", 200, 0.0, 1.05, 200, 0.0, 1.50); 
  chain->Draw("pt:z >> z_pt_low", "w > 2.0 && w < 2.3", "");

  TH2F *z_pt_mid = new TH2F("z_pt_mid", "", 200, 0.0, 1.05, 200, 0.0, 1.50); 
  chain->Draw("pt:z >> z_pt_mid", "w > 2.3 && w < 2.6", "");

  TH2F *z_pt_high = new TH2F("z_pt_high", "", 200, 0.0, 1.05, 200, 0.0, 1.50); 
  chain->Draw("pt:z >> z_pt_high", "w > 2.6 && w < 3.5", "");

  // do the same histograms but for the neutron mass peak 
  std::cout << "Projecting events into 2-D histograms with cuts..." << std::endl; 

  // for cutting 
  std::string missingMassCut("missing_mass < 1.01 && missing_mass > 0.88"); 

  TH2F *z_pt_res = new TH2F("z_pt_res", "", 200, 0.0, 1.05, 200, 0.0, 1.50); 
  chain->Draw("pt:z >> z_pt_res", Form("w > 2.0 && w < 3.5 && %s", missingMassCut.c_str()), "");

  TH2F *z_pt_res_low = new TH2F("z_pt_res_low", "", 200, 0.0, 1.05, 200, 0.0, 1.50); 
  chain->Draw("pt:z >> z_pt_res_low", Form("w > 2.0 && w < 2.3 && %s", missingMassCut.c_str()), "");

  TH2F *z_pt_res_mid = new TH2F("z_pt_res_mid", "", 200, 0.0, 1.05, 200, 0.0, 1.50); 
  chain->Draw("pt:z >> z_pt_res_mid", Form("w > 2.3 && w < 2.6 && %s", missingMassCut.c_str()), "");

  TH2F *z_pt_res_high = new TH2F("z_pt_res_high", "", 200, 0.0, 1.05, 200, 0.0, 1.50); 
  chain->Draw("pt:z >> z_pt_res_high", Form("w > 2.6 && w < 3.5 && %s", missingMassCut.c_str()), "");

  std::cout << "Projecting events into 1-D histograms for z..." << std::endl; 
  TH1F *z_dis = new TH1F("z_dis", "", 200, 0.0, 1.05);
  TH1F *z_exc = new TH1F("z_exc", "", 200, 0.0, 1.05);
  chain->Draw("z >> z_dis", "missing_mass > 1.15", ""); 
  chain->Draw("z >> z_exc", "missing_mass < 1.01 && missing_mass > 0.88", ""); 
 
  std::cout << "Projecting events into 1-D histograms for P..." << std::endl; 
  TH1F *p_dis = new TH1F("p_dis", "", 200, 0.0, 5.5); 
  TH1F *p_exc = new TH1F("p_exc", "", 200, 0.0, 5.5); 
  chain->Draw("p_pip >> p_dis", "missing_mass > 1.15", ""); 
  chain->Draw("p_pip >> p_exc", missingMassCut.c_str(), ""); 

  TLatex *lab = new TLatex();
  lab->SetNDC(); 
  lab->SetTextFont(42);
  lab->SetTextSize(0.03);

  TCanvas *can = new TCanvas("can", "", 1600, 1600);
  can->Divide(2, 2);

  can->cd(1);
  gPad->SetLogz();
  z_pt_all->Draw("colz");
  lab->DrawLatex(0.35, 0.02, "All DIS #pi^{+} Events"); 

  can->cd(2);
  gPad->SetLogz();
  z_pt_low->Draw("colz");
  lab->DrawLatex(0.48, 0.02, "W #in [2.0, 2.3]"); 

  can->cd(3);
  gPad->SetLogz();
  z_pt_mid->Draw("colz");
  lab->DrawLatex(0.48, 0.02, "W #in [2.3, 2.6]"); 

  can->cd(4);
  gPad->SetLogz();
  z_pt_high->Draw("colz");
  lab->DrawLatex(0.48, 0.02, "W #in [2.6, 3.5]"); 

  can->Print("zpt.pdf");

  // draw again for cuts 
  can->Clear(); 
  can->Divide(2, 2);

  can->cd(1);
  gPad->SetLogz();
  z_pt_res->Draw("colz");
  lab->DrawLatex(0.35, 0.02, "ep #rightarrow e #pi^{+} N"); 

  can->cd(2);
  gPad->SetLogz();
  z_pt_res_low->Draw("colz");
  lab->DrawLatex(0.48, 0.02, "W #in [2.0, 2.3]"); 

  can->cd(3);
  gPad->SetLogz();
  z_pt_res_mid->Draw("colz");
  lab->DrawLatex(0.48, 0.02, "W #in [2.3, 2.6]"); 

  can->cd(4);
  gPad->SetLogz();
  z_pt_res_high->Draw("colz");
  lab->DrawLatex(0.48, 0.02, "W #in [2.6, 3.5]"); 

  can->Print("zpt_exc.pdf");

  TCanvas *can2 = new TCanvas("can2", "", 1600, 1600);

  z_dis->SetLineColor(kBlack);
  z_dis->Scale( 1/z_dis->GetEntries() ); 
  z_dis->Draw();

  z_exc->SetLineColor(kBlack);
  z_exc->SetFillColorAlpha(kBlack, 1.0);
  z_exc->SetFillStyle(3004);
  z_exc->Scale( 1/z_exc->GetEntries() ); 
  z_exc->Draw("same");

  lab->DrawLatex(0.45, 0.09, "z_{h}"); 
  lab->SetTextAngle(90.0);
  lab->DrawLatex(0.04, 0.45, "Probability");

  TLegend *leg = new TLegend(0.6, 0.72, 0.8, 0.8);
  leg->AddEntry(z_dis, "ep #rightarrow e #pi^{+} X"); 
  leg->AddEntry(z_exc, "ep #rightarrow e #pi^{+} N"); 
  leg->SetBorderSize(0); 
  leg->SetTextFont(42); 
  leg->Draw();

  can2->Print("z.pdf");
  can2->Clear(); 

  // momentum figure
  p_dis->SetLineColor(kBlack);
  p_dis->Scale( 1/p_dis->GetEntries() ); 
  p_dis->Draw();

  p_exc->SetLineColor(kBlack);
  p_exc->SetFillColorAlpha(kBlack, 1.0);
  p_exc->SetFillStyle(3004);
  p_exc->Scale( 1/p_exc->GetEntries() ); 
  p_exc->Draw("same");

  lab->SetTextAngle(0.0);
  lab->DrawLatex(0.45, 0.09, "P [GeV/c]"); 
  lab->SetTextAngle(90.0);
  lab->DrawLatex(0.04, 0.45, "Probability");

  leg->Draw(); 
  can2->Print("p.pdf"); 

}
