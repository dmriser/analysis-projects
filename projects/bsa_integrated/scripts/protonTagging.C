{


  // define conf and files 
  const int NCONF             = 4; 
  std::string fileName[NCONF] = {"/volatile/clas12/dmriser/analysis/asymmetry/k_lambda_no_proton/histos.root", 
				 "/volatile/clas12/dmriser/analysis/asymmetry/k_lambda_proton/histos.root", 
				 "/volatile/clas12/dmriser/analysis/asymmetry/k_lambda_dis_no_proton/histos.root", 
				 "/volatile/clas12/dmriser/analysis/asymmetry/k_lambda_dis_proton/histos.root"};


  std::string title[NCONF] = {"Dan Kin. (no proton)", 
			      "Dan Kin. (proton)", 
			      "DIS Kin. (no proton)", 
			      "DIS Kin. (proton)"};

  // load files 
  TFile *file[NCONF]; 
  for(int c=0; c<NCONF; c++){
    file[c] = (TFile*) TFile::Open(fileName[c].c_str());
  }


  TH1F *noCutsNoProton = (TH1F*) file[0]->Get("missingMass/missingMass_wide_PassMesonID");
  TH1F *noCutsProton   = (TH1F*) file[1]->Get("missingMass/missingMass_wide_PassMesonID");
  TH1F *cutsNoProton   = (TH1F*) file[2]->Get("missingMass/missingMass_wide_PassKinematic"); 
  
  TH1F *cutsRatio = (TH1F*) cutsNoProton->Clone(); 
  cutsRatio->Divide(noCutsNoProton); 

  TH1F *protonRatio = (TH1F*) noCutsProton->Clone();   
  protonRatio->Divide(noCutsNoProton); 


  TCanvas *can = new TCanvas("can","",1200,1200);
  can->Divide(2,2);

  TLatex *tit = new TLatex(); 
  tit->SetNDC(); 
  tit->SetTextFont(42);
  tit->SetTextSize(0.055);  
  
  can->cd(1); 
  noCutsNoProton->SetLineColor(99); 
  noCutsNoProton->SetFillColorAlpha(99, 0.2); 
  noCutsNoProton->Draw(); 

  cutsNoProton->SetLineColor(55);
  cutsNoProton->SetFillColorAlpha(55, 0.2); 
  cutsNoProton->Draw("same"); 

  tit->DrawLatex(0.1, 0.92, "Effect of DIS cuts on Missing Mass"); 


  can->cd(2); 
  noCutsNoProton->Draw(); 
  noCutsProton->SetFillColorAlpha(55, 0.2);
  noCutsProton->SetLineColor(55);
  noCutsProton->Draw("same"); 
  tit->DrawLatex(0.1, 0.92, "Effect of proton ID on Missing Mass"); 

  can->cd(3); 
  cutsRatio->SetFillColorAlpha(99, 0.2); 
  cutsRatio->SetLineColor(99); 
  cutsRatio->Draw(); 
  tit->DrawLatex(0.1, 0.92, "Ratio DIS/Full Kinematics"); 

  can->cd(4);
  protonRatio->SetFillColorAlpha(55, 0.2); 
  protonRatio->SetLineColor(55); 
  protonRatio->Draw(); 
  tit->DrawLatex(0.1, 0.92, "Ratio proton/no proton Kinematics"); 

  can->Print("proton.pdf"); 
}

