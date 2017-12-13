{

  gROOT->LoadMacro("utils.C"); 
  gStyle->SetErrorX(0.0); 
  
  // --------------------------------------------------------------------
  //          user inputs 
  // --------------------------------------------------------------------
  const int NCONF               = 3; 
  int configColor[NCONF]        = {99, 72, 55};
  std::string configName[NCONF] = {"returnPass4", "returnPass5", "returnPass7"};
  std::string filePath[NCONF]   = {"/volatile/clas12/dmriser/rootFiles/asymmetry/asymmetry_return_pass4.root", 
				   "/volatile/clas12/dmriser/rootFiles/asymmetry/asymmetry_return_pass5.root",
				   "/volatile/clas12/dmriser/rootFiles/asymmetry/return_pass7.root"};

  std::string outputPath("/volatile/clas12/dmriser/plots/asymmetry");

  // --------------------------------------------------------------------
  //         end user inputs and begin logic 
  // --------------------------------------------------------------------


  // open the files 
  bool fileStatus = true; 

  TFile *inputFile[NCONF]; 
  for (int conf=0; conf<NCONF; conf++){
    inputFile[conf] = TFile::Open(filePath[conf].c_str());

    if (!inputFile[conf] || !inputFile[conf]->IsOpen()){
      fileStatus = false; 

      std::cerr << "something went wrong, flipping file status to false on: " << filePath[conf] << std::endl; 
    }
  }
  
  // physics inputs 
  const int npart            = 4; 
  std::string name[npart]    = {"pm", "pp", "km", "kp"};
  std::string texName[npart] = {"#pi^{-}", "#pi^{+}", "K^{-}", "K^{+}"};

  // styling 
  float labelSize = 0.045; 

  TH1D *x[NCONF][npart], 
    *q2[NCONF][npart], 
    *z[NCONF][npart], 
    *pt[NCONF][npart]; 

  if (fileStatus){
    for (int p=0; p<npart; p++){

      for (int conf=0; conf<NCONF; conf++){
	x[conf][p]  = (TH1D*) inputFile[conf]->Get(Form("integrated/%s/h1_x_q20_z0_pt0_base", name[p].c_str())); 
	x[conf][p]->SetLineColor(configColor[conf]); 
	x[conf][p]->SetLineWidth(2); 
	x[conf][p]->SetMarkerColor(configColor[conf]); 
	x[conf][p]->SetMarkerStyle(8); 
	x[conf][p]->SetMinimum(-0.07); 
	x[conf][p]->SetMaximum(0.07); 
	x[conf][p]->GetXaxis()->SetNdivisions(505); 
	x[conf][p]->SetLabelSize(labelSize, "x"); 
	x[conf][p]->SetLabelSize(labelSize, "y"); 
	
	q2[conf][p] = (TH1D*) inputFile[conf]->Get(Form("integrated/%s/h1_q2_z0_pt0_x0_base", name[p].c_str())); 
	q2[conf][p]->SetLineColor(configColor[conf]); 
	q2[conf][p]->SetMarkerColor(configColor[conf]); 
	q2[conf][p]->SetMarkerStyle(8); 
	q2[conf][p]->SetMinimum(-0.07); 
	q2[conf][p]->SetMaximum(0.07); 
	q2[conf][p]->GetXaxis()->SetNdivisions(505); 
	q2[conf][p]->SetLabelSize(labelSize, "x"); 
	q2[conf][p]->SetLabelSize(labelSize, "y"); 
	
	z[conf][p]  = (TH1D*) inputFile[conf]->Get(Form("integrated/%s/h1_z_pt0_x0_q20_base", name[p].c_str())); 
	z[conf][p]->SetLineColor(configColor[conf]); 
	z[conf][p]->SetMarkerColor(configColor[conf]); 
	z[conf][p]->SetMarkerStyle(8); 
	z[conf][p]->GetXaxis()->SetNdivisions(505); 
	z[conf][p]->SetMinimum(-0.07); 
	z[conf][p]->SetMaximum(0.07); 
	z[conf][p]->SetLabelSize(labelSize, "x"); 
	z[conf][p]->SetLabelSize(labelSize, "y"); 
	
	pt[conf][p] = (TH1D*) inputFile[conf]->Get(Form("integrated/%s/h1_pt_x0_q20_z0_base", name[p].c_str())); 
	pt[conf][p]->SetLineColor(configColor[conf]); 
	pt[conf][p]->SetMarkerColor(configColor[conf]); 
	pt[conf][p]->GetXaxis()->SetNdivisions(505); 
	pt[conf][p]->SetMinimum(-0.07); 
	pt[conf][p]->SetMaximum(0.07); 
	pt[conf][p]->SetMarkerStyle(8); 
	pt[conf][p]->SetLabelSize(labelSize, "x"); 
	pt[conf][p]->SetLabelSize(labelSize, "y"); 
      }
    }
  }

  // --------------------------------------------
  // setup lines for the middle and the limits on z 
  // --------------------------------------------
  TLine *xLine  = new TLine(x[0][0]->GetXaxis()->GetXmin(),  0.0, x[0][0]->GetXaxis()->GetXmax(),  0.0); 
  TLine *q2Line = new TLine(q2[0][0]->GetXaxis()->GetXmin(), 0.0, q2[0][0]->GetXaxis()->GetXmax(), 0.0); 
  TLine *zLine  = new TLine(z[0][0]->GetXaxis()->GetXmin(),  0.0, z[0][0]->GetXaxis()->GetXmax(),  0.0); 
  TLine *ptLine = new TLine(pt[0][0]->GetXaxis()->GetXmin(), 0.0, pt[0][0]->GetXaxis()->GetXmax(), 0.0); 
  TLine *zLeft  = new TLine(0.3, -0.07, 0.3, 0.07); 
  TLine *zRight = new TLine(0.7, -0.07, 0.7, 0.07); 

  zLeft->SetLineColor(99);
  zLeft->SetLineStyle(7); 
  zRight->SetLineColor(99);
  zRight->SetLineStyle(7); 

  xLine->SetLineColor(kBlack);
  xLine->SetLineStyle(7);  

  q2Line->SetLineColor(kBlack);
  q2Line->SetLineStyle(7);  

  zLine->SetLineColor(kBlack);
  zLine->SetLineStyle(7);  

  ptLine->SetLineColor(kBlack);
  ptLine->SetLineStyle(7);  

  // --------------------------------------------
  // setup the latex writer 
  TLatex lab; 
  lab.SetNDC(); 
  lab.SetTextSize(0.01); 
  // --------------------------------------------

  TCanvas *can = new TCanvas("can", "", 1600, 1600);
  can->cd(); 
  gPad->SetMargin(0.15, 0.15, 0.15, 0.15); 
  
  can->Divide(4, 4); 

  // annotate the configurations at the top 
  for (int conf=0; conf<NCONF; conf++){
    lab.DrawLatex(0.05+conf*0.15, 0.975, Form("config: #color[%d]{%s}", configColor[conf], configName[conf].c_str())); 
  }

  lab.SetTextSize(0.1);

  float inner = 0.1; 
  float outer = 0.1; 

  for (int p=0; p<npart; p++){
    for (int conf=0; conf<NCONF; conf++){
      int index = p*npart +1; 
      
      can->cd(index); 
      setupPad(index, 4, 4, inner, outer); 
      x[conf][p]->Draw("pesame"); 
      xLine->Draw(); 
      if (padIsBottom(index, 4, 4)) lab.DrawLatex(0.48, 0.18, "x"); 
      lab.DrawLatex(0.75, 0.75, Form("%s",texName[p].c_str())); 
      
      index++;
      can->cd(index); 
      setupPad(index, 4, 4, inner, outer); 
      q2[conf][p]->Draw("pesame"); 
      q2Line->Draw(); 
      if (padIsBottom(index, 4, 4)) lab.DrawLatex(0.30, 0.18, "Q^{2} (GeV^{2}/c^{2})"); 
      
      index++;
      can->cd(index); 
      setupPad(index, 4, 4, inner, outer); 
      z[conf][p]->Draw("pesame"); 
      zLine->Draw("same"); 
      zLeft->Draw("same");
      zRight->Draw("same");
      if (padIsBottom(index, 4, 4)) lab.DrawLatex(0.48, 0.18, "z"); 
      
      index++;
      can->cd(index); 
      setupPad(index, 4, 4, inner, outer); 
      pt[conf][p]->Draw("pesame");     
      ptLine->Draw(); 
      if (padIsBottom(index, 4, 4)) lab.DrawLatex(0.34, 0.18, "P_{T} (GeV/c)"); 
    }
  }

  can->Print(Form("%s/compare_integrated_all.pdf", outputPath.c_str()));

}
