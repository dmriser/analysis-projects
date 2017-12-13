void loadSectorTH1D(TFile *file, TH1D *hist[6], string tit){
  for(int s=1; s<7; s++){
    hist[s-1] = (TH1D*) file->Get(Form(tit.c_str(),s)); 
  }
}

void setMaxSame(TH1D *h1, TH1D *h2){
  if(h1->GetMaximum() > h2->GetMaximum()){
    h1->SetMaximum(1.2 *h1->GetMaximum());
    h2->SetMaximum(1.2 *h1->GetMaximum());
  } else {
    h1->SetMaximum(1.2 *h2->GetMaximum());
    h2->SetMaximum(1.2 *h2->GetMaximum());
  }
}

void scaleHisto(TH1D *h1, TH1D *h2){
  h2->Scale(h1->Integral()/h2->Integral());
  setMaxSame(h1, h2); 
}

void scaleSectorHisto(TH1D *h1[6], TH1D *h2[6]){
  for(int h=0; h<6; h++){
    scaleHisto(h1[h], h2[h]);
  }
}

void styleHisto(TH1D *h, int type){
  // 0 = data
  // 1 = mc
  if (type == 0)      { 
    h->SetLineColor(99); 
  }
  
  else if (type == 1) { 
    h->SetLineColor(65); 
  }

}

void styleSectorHisto(TH1D *h[6], int type){
  for(int s=0; s<6; s++){
    styleHisto(h[s],type);
  }
}

void printSectorTwoHistos(TH1D *h1[6], TH1D *h2[6], string imagePath, int type){
  styleSectorHisto(h1, 0); 
  styleSectorHisto(h2, 1); 

  TLatex tit, xtit, ytit;
  tit.SetNDC(); 
  tit.SetTextFont(102);
  tit.SetTextSize(0.03);

  xtit.SetNDC(); 
  xtit.SetTextFont(102);
  xtit.SetTextSize(0.015);

  ytit.SetNDC(); 
  ytit.SetTextFont(102);
  ytit.SetTextSize(0.015);

  string name = ""; 
  if(type == 0){ name = "CompareW"; }
  if(type == 1){ name = "CompareTheta"; }
  if(type == 2){ name = "CompareRelPhi"; }
  
  string titleString = ""; 
  if(type == 0){ titleString = "Compare W"; }
  if(type == 1){ titleString = "Compare Theta"; }
  if(type == 2){ titleString = "Compare #Phi_{Rel}"; }

  TCanvas *can = new TCanvas("can","",850,1100); 
  can->Divide(2,3);

  tit.DrawLatex(0.45, 0.95, titleString.c_str()); 
  
  for(int s=0; s<6; s++){
    can->cd(s+1);
    h1[s]->Draw();
    h2[s]->Draw("same");
  }

  can->Print(Form("%s%s.png",imagePath.c_str(),name.c_str()));
}
