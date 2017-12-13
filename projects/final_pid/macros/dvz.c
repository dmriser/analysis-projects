{

  std::string imagePath("/volatile/clas12/dmriser/plots/pid/hid/");

  TFile *histFile = TFile::Open("/volatile/clas12/dmriser/farm_out/final_pid_pass11/pid.root"); 
  const int NSECT  = 7; 

  float cutWidth = 8; 

  TH1F *dvz[NSECT];
  TLine *left[NSECT], *right[NSECT]; 

  for(int isect=0; isect<NSECT; isect++){
    dvz[isect] = (TH1F*) histFile->Get(Form("MesonHistograms/h1_dvz_%d_211_no_cut", isect));
    dvz[isect]->SetFillStyle(3004);
    dvz[isect]->SetLineColor(kBlack);
    dvz[isect]->SetFillColorAlpha(kBlack, 1.0);

    left[isect]  = new TLine(-cutWidth/2, 0, -cutWidth/2, dvz[isect]->GetMaximum()); 
    left[isect]->SetLineColor(kBlack);
    left[isect]->SetLineStyle(2); 

    right[isect] = new TLine(cutWidth/2, 0, cutWidth/2, dvz[isect]->GetMaximum()); 
    right[isect]->SetLineColor(kBlack);
    right[isect]->SetLineStyle(2); 

    printf("loading (%d) \n", isect); 
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

  TCanvas *c1 = new TCanvas("c1","",800,1100);
  c1->Divide(2,3);

  double xtitx = 0.0;
  double xtity = 0.0;

  for(int s=0; s<6; s++){
    c1->cd(s+1);

    dvz[s+1]  ->Draw(); 
    left[s+1] ->Draw("same");
    right[s+1]->Draw("same");

    if(s==0 || s==2 || s==4){
      gPad->SetLeftMargin(0.2);
      gPad->SetRightMargin(0.05);
      ytit.DrawLatex(0.1,0.48,"Counts");
      xtitx = 0.66;
    }

    else {
      gPad->SetRightMargin(0.2);
      gPad->SetLeftMargin(0.05);

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
      xtit.DrawLatex(0.46,0.1,"#Delta V_{z} (cm)");
      xtity = 0.81;
    }
    else{
      gPad->SetTopMargin(0.05);
      gPad->SetBottomMargin(0.05);
      xtity = 0.81;
    }
  }

  c1->cd();
  tit.DrawLatex(0.1, 0.95, "z-vertex difference for electrons and positives");
  c1->Print(Form("%sdvz.pdf",imagePath.c_str()));

}
