{
  // ----------------------------------------------------
  // User Parameters 
  // ----------------------------------------------------
  TFile *input            = TFile::Open("/volatile/clas12/dmriser/farm_out/momentum_corrections_pass0/backup_momentum_corrections.root");
  string imagePath        = "/volatile/clas12/dmriser/plots/momentum_corrections";
  const int numberSectors = 6; 

  TH1F *wBefore[numberSectors];
  TH1F *wAfter[numberSectors];


  for(int s=0; s<numberSectors; s++){
    wBefore[s]  = (TH1F*) input->Get(Form("w_pid_sect%d", s + 1));
    wAfter[s]  = (TH1F*) input->Get(Form("w_corr_pid_sect%d", s + 1));
  }

  TLatex tit, xtit, ytit; 
  tit.SetNDC(); 
  tit.SetTextFont(102); 
  tit.SetTextSize(0.03);
  xtit.SetNDC(); 
  xtit.SetTextFont(102); 
  xtit.SetTextSize(0.05);
  ytit.SetNDC(); 
  ytit.SetTextFont(102); 
  ytit.SetTextSize(0.05);
  ytit.SetTextAngle(90.0);

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  TCanvas *c1 = new TCanvas("c1","",800,1100);
  c1->Divide(2,3);

  TF1 *fit = new TF1("fit","gaus",0.85,1.0); 
  fit->SetLineColor(kBlack);

  double xtitx = 0.0; 
  double xtity = 0.0; 

  for(int s=0; s<6; s++){
    c1->cd(s+1); 

    TLine *protonMass = new TLine(0.938,0.0,0.938,wAfter[s]->GetMaximum());  
    protonMass->SetLineStyle(8);
    protonMass->SetLineWidth(2);

    wBefore[s] ->SetLineColor(61); 
    wAfter[s]  ->SetLineColor(99); 
    wBefore[s] ->SetFillColorAlpha(61,0.25); 
    wAfter[s]  ->SetFillColorAlpha(99,0.25); 
    wAfter[s]  ->Fit("fit","RQ"); 
    wAfter[s]  ->Draw();
    wBefore[s] ->Draw("same");
    protonMass ->Draw();
    
    if(s==0 || s==2 || s==4){
      gPad->SetLeftMargin(0.2); 
      gPad->SetRightMargin(0.05); 
      ytit.DrawLatex(0.1,0.48,"Counts"); 

      xtitx = 0.66; 
    }
 
    else {
      gPad->SetRightMargin(0.2); 
      gPad->SetLeftMargin(0.1); 

      xtitx = 0.52; 
    }
 
    if(s==0 || s==1){
      gPad->SetTopMargin(0.2);
      gPad->SetBottomMargin(0.05);
      xtity = 0.68;
    } 
    else if (s==4 || s==5){
      gPad->SetTopMargin(0.05); 
      gPad->SetBottomMargin(0.2); 
      xtit.DrawLatex(0.46,0.1,"W (GeV/c^{2})"); 
      xtity = 0.81;
    }
    else{
      gPad->SetTopMargin(0.05); 
      gPad->SetBottomMargin(0.05);
      xtity = 0.81;
    }
      xtit.DrawLatex(xtitx,xtity,Form("#splitline{#mu = %.3f}{#sigma = %.3f}",fit->GetParameter(1),fit->GetParameter(2))); 
  } 

  c1->cd(); 
  tit.DrawLatex(0.1, 0.95, "Electron Momentum Corrections #color[61]{Before} and #color[99]{After}"); 
  c1->Print(Form("%s/w_before_after.png",imagePath.c_str())); 


}
