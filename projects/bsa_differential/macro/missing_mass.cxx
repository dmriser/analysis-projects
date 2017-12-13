{

  // ----------------------------------------------------------------------
  //   user parameters 
  // ----------------------------------------------------------------------

  std::string filePath = "/volatile/clas12/dmriser/rootFiles/asymmetry/return_pass7.root";

  // ----------------------------------------------------------------------
  //   business 
  // ----------------------------------------------------------------------

  const int NMESON = 4; 
  const int NCUT   = 3; 

  std::string mesonNames[NMESON] = {"pm", "pp", "km", "kp"};
  std::string latexNames[NMESON] = {"#pi^{-}", "#pi^{+}", "K^{-}", "K^{+}"};
  std::string cutNames[NCUT]     = {"PassPID", "PassSIDIS", "PassAll"};  

  // style 
  int colors[NCUT] = {99, 77, 55};

  // histos to get from file 
  TH1F *missingMass[NCUT][NMESON];

  // open file and get histos out 
  TFile *inputFile = TFile::Open(filePath.c_str()); 

  for (int m=0; m<NMESON; m++){
    for (int c=0; c<NCUT; c++){ 
      missingMass[c][m] = (TH1F*) inputFile->Get(Form("StandardHistograms/h1_mm2_sect0_%s_%s",cutNames[c].c_str(),mesonNames[m].c_str())); 
      missingMass[c][m]->SetLineColor(colors[c]); 
    }
  }
 
  // canvas 
  TCanvas *can = new TCanvas("can","",1200,750); 
  can->cd();

  // latex printer 
  TLatex *lab = new TLatex(); 
  lab->SetNDC();
  lab->SetTextSize(0.03);

  // legend 
  TLegend *leg = new TLegend(0.65, 0.66, 0.80, 0.82);
  leg->SetBorderSize(0);

  // manually add the labels 
  leg->AddEntry(missingMass[0][0], "Passed Meson ID");
  leg->AddEntry(missingMass[1][0], "Passed SIDIS Cuts");
  leg->AddEntry(missingMass[2][0], "Passed All");


  for (int m=0; m<NMESON; m++){
    can->Clear();
    gPad->SetMargin(0.12, 0.12, 0.12, 0.12); 

    for (int c=0; c<NCUT; c++){
      if (c==0) { missingMass[c][m]->Draw(); }
      else { missingMass[c][m]->Draw("same"); }
    }

    leg->Draw(); 
    lab->DrawLatex(0.05, 0.95, Form("Missing Mass for %s", latexNames[m].c_str()));
    lab->DrawLatex(0.46, 0.05, "M^{2}_{X}");

    can->Print(Form("/volatile/clas12/dmriser/plots/asymmetry/missingMass%s.pdf",
		    mesonNames[m].c_str()));
  }
  
}
