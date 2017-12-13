{


  // ----------------------------------------------
  //      user parameters 
  // ----------------------------------------------
  std::string path("/volatile/clas12/dmriser/farm_out"); 
  
  const int NCONF = 2; 
  std::string project[NCONF] = {"pi_n_tree_v5", 
				"pi_n_tree_v3"};

  std::string configTitle[NCONF] = {"With Cut", 
				    "Without"};

  const int NFILES = 10;

  std::string disCuts("w > 2.0 && q2 > 1.0");
  std::string epnCuts("missing_mass > 0.88 && missing_mass < 1.01");

  // ----------------------------------------------
  //      business 
  // ----------------------------------------------
  TChain *chain[NCONF]; 

  TH1F *p[NCONF]; 
  TH1F *mm[NCONF]; 
  TH1F *z[NCONF]; 
  TH1F *w[NCONF]; 

  for (int c=0; c<NCONF; c++){    

    // setup tchain 
    chain[c] = new TChain("events");
    
    // setup histos 
    std::string pTitle(Form("p_%s", project[c].c_str()));
    std::string mmTitle(Form("mm_%s", project[c].c_str()));
    std::string wTitle(Form("w_%s", project[c].c_str()));
    std::string zTitle(Form("z_%s", project[c].c_str()));

    p[c]  = new TH1F(pTitle.c_str(),  pTitle.c_str(), 200, 0.0, 5.5); 
    w[c]  = new TH1F(wTitle.c_str(),  wTitle.c_str(), 200, 1.8, 3.1); 
    mm[c] = new TH1F(mmTitle.c_str(), mmTitle.c_str(), 200, 0.7, 1.2); 
    z[c]  = new TH1F(zTitle.c_str(),  zTitle.c_str(), 200, 0.0, 1.05); 

    // load the correct number of files 
    for(int file=1; file<=NFILES; file++){

      std::string pathToFile( Form("%s/%s/%s.%d.root", path.c_str(), 
				   project[c].c_str(), 
				   project[c].c_str(), 
				   file) );
      
      chain[c]->AddFile(pathToFile.c_str());
    }

    // summarize what we loaded and start projecting out events into histograms 
    std::cout << "Loaded files for project: " << project[c] << " w/events = " << chain[c]->GetEntries() << std::endl; 

    chain[c]->Draw(Form("p_pip >> %s", pTitle.c_str()), disCuts.c_str(), "");
    chain[c]->Draw(Form("w >> %s", wTitle.c_str()), disCuts.c_str(), "");
    chain[c]->Draw(Form("missing_mass >> %s", mmTitle.c_str()), disCuts.c_str(), "");
    chain[c]->Draw(Form("z >> %s", zTitle.c_str()), disCuts.c_str(), "");

    p[c]->SetLineColor(kBlack); 
    p[c]->Scale( 1/p[c]->Integral() ); 

    w[c]->SetLineColor(kBlack); 
    w[c]->Scale( 1/w[c]->Integral() ); 

    mm[c]->SetLineColor(kBlack); 
    mm[c]->Scale( 1/mm[c]->Integral() ); 

    z[c]->SetLineColor(kBlack); 
    z[c]->Scale( 1/z[c]->Integral() ); 
  }


  // plot the results 
  TLatex *lab = new TLatex(); 
  lab->SetNDC(); 
  lab->SetTextFont(42); 
  lab->SetTextSize(0.03); 

  TLegend *leg = new TLegend(0.62, 0.75, 0.82, 0.82); 
  leg->SetBorderSize(0); 
  leg->SetTextFont(42); 
  leg->AddEntry(p[0], "#Delta V_{z} < 5.0"); 
  leg->AddEntry(p[1], "No cut"); 

  TCanvas *can = new TCanvas("can", "", 1600, 1600);
  can->Divide(2, 2); 
  
  can->cd(1);
  p[1]->SetFillStyle(3004); 
  p[1]->SetFillColorAlpha(kBlack, 1.0); 
  p[1]->Draw(); 
  p[0]->Draw("same"); 
  lab->DrawLatex(0.47, 0.05, "P [GeV/c]"); 
  lab->SetTextAngle(90.0); 
  lab->DrawLatex(0.06, 0.46, "Probability"); 
  lab->SetTextAngle(0.0); 
  leg->Draw(); 

  can->cd(2);
  z[1]->SetFillStyle(3004); 
  z[1]->SetFillColorAlpha(kBlack, 1.0); 
  z[1]->Draw(); 
  z[0]->Draw("same"); 
  lab->DrawLatex(0.50, 0.05, "z_{h}"); 
  lab->SetTextAngle(90.0); 
  lab->DrawLatex(0.06, 0.46, "Probability"); 
  lab->SetTextAngle(0.0); 
  leg->Draw(); 

  can->cd(3); 
  mm[1]->SetFillStyle(3004); 
  mm[1]->SetFillColorAlpha(kBlack, 1.0); 
  mm[0]->Draw(); 
  mm[1]->Draw("same"); 
  lab->DrawLatex(0.45, 0.05, "M_{X} [GeV/c^{2}]^{2}"); 
  lab->SetTextAngle(90.0); 
  lab->DrawLatex(0.06, 0.46, "Probability"); 
  lab->SetTextAngle(0.0); 
  leg->Draw(); 

  can->cd(4); 
  w[1]->SetFillStyle(3004); 
  w[1]->SetFillColorAlpha(kBlack, 1.0); 
  w[1]->Draw(); 
  w[0]->Draw("same"); 
  lab->DrawLatex(0.47, 0.05, "W [GeV/c^{2}]^{2}"); 
  lab->SetTextAngle(90.0); 
  lab->DrawLatex(0.06, 0.46, "Probability"); 
  lab->SetTextAngle(0.0); 
  leg->Draw(); 

  can->Print("compare.pdf"); 
}
