void testCutOnXAngleXMass(double angleCut, double massCut){
  int angleBin = events->GetAxis(0)->FindBin(angleCut);
  int massBin  = events->GetAxis(1)->FindBin(massCut);

  events->GetAxis(0)->SetRange(angleBin, events->GetAxis(0)->GetNbins());
  events->GetAxis(1)->SetRange(massBin,  events->GetAxis(1)->GetNbins());

  TH1D *wPass = events->Projection(2);
  //  massW       = events->Projection(2,1);

  // Set to region in top left which also passes 
  events->GetAxis(0)->SetRange(angleBin, events->GetAxis(0)->GetNbins());
  events->GetAxis(1)->SetRange(1, massBin);
  TH1D *wPass1 = events->Projection(2);

  // Set to region in top left which also passes 
  events->GetAxis(0)->SetRange(1, angleBin);
  events->GetAxis(1)->SetRange(massBin,  events->GetAxis(1)->GetNbins());
  TH1D *wPass2 = events->Projection(2);

  // Sum up passing 
  wPass->Add(wPass1); 
  wPass->Add(wPass2); 

  // Set to failing region 
  events->GetAxis(0)->SetRange(1, angleBin);
  events->GetAxis(1)->SetRange(1, massBin);
  TH1D *wFail = events->Projection(2);

  TLine *massLine = new TLine(massCut, 0, massCut, 65); 
  massLine->SetLineColor(99);
  massLine->SetLineStyle(4); 
  massLine->SetLineWidth(2);

  TLine *angleLine = new TLine(0, angleCut, 5, angleCut); 
  angleLine->SetLineColor(99);
  angleLine->SetLineStyle(4); 
  angleLine->SetLineWidth(2);

  TLatex *tit, *xtit, *ytit;
  tit = new TLatex();
  xtit = new TLatex();
  ytit = new TLatex();

  tit->SetNDC(); 
  tit->SetTextSize(0.05);

  xtit->SetNDC(); 
  xtit->SetTextSize(0.04);

  ytit->SetNDC(); 
  ytit->SetTextSize(0.04);
  ytit->SetTextAngle(90.0); 

  TCanvas *can1 = new TCanvas("can1","",800,500);
  w->SetLineColor(55);
  wPass->SetLineColor(75);
  wFail->SetLineColor(99);
  w     ->Draw();
  wPass ->Draw("same");
  wFail ->Draw("same");

  tit->DrawLatex(0.25, 0.9, "#color[55]{No Cuts}, #color[99]{Fail Cuts}, and #color[75]{Pass Cuts}"); 
  xtit->DrawLatex(0.7, 0.06, "W [GeV/c^{2}]");
  ytit->DrawLatex(0.06, 0.7, "Counts"); 

  TCanvas *can2 = new TCanvas("can2","",800,500); 
  gPad->SetLogz();
  gPad->SetGridx();
  gPad->SetGridy();
  massAngle ->Draw("colz");
  massLine  ->Draw();
  angleLine ->Draw();
  
  tit->DrawLatex(0.4, 0.9, "ep #rightarrow epX"); 
  xtit->DrawLatex(0.7, 0.06, "M_{X}^{2}");
  ytit->DrawLatex(0.06, 0.7, "#theta_{X}"); 
}

void setRange(string title, double min, double max){

  int axisToBeSet = -1;

  if (title == "xAngle")           { axisToBeSet = 0; }
  else if(title == "missingMass")  { axisToBeSet = 1; }
  else if(title == "w")            { axisToBeSet = 2; }
  else if(title == "coplanarAngle"){ axisToBeSet = 3; }
  else if(title == "ePrimeXAngle") { axisToBeSet = 4; }

  if (axisToBeSet > -1){
    int startBin = events->GetAxis(axisToBeSet)->FindBin(min);
    int endBin   = events->GetAxis(axisToBeSet)->FindBin(max);
    events->GetAxis(axisToBeSet)->SetRange(startBin, endBin);
    cout << "[setRange] Axis " << title << " is set. " << endl;
  }
  else {
    cout << "[setRange] Axis " << title << " is invalid. " << endl;
  }
}

void displayMissingMass(){

  TH1D *missingMassPass   = events->Projection(1);

  TLatex *tit = new TLatex();
  TLatex *xtit = new TLatex();
  TLatex *ytit = new TLatex();

  tit->SetNDC();
  tit->SetTextSize(0.05);
  tit->SetTextFont(42); 

  xtit->SetNDC();
  xtit->SetTextSize(0.04);
  xtit->SetTextFont(42); 

  ytit->SetNDC();
  ytit->SetTextSize(0.04);
  ytit->SetTextAngle(90.0);
  ytit->SetTextFont(42); 

  TCanvas *can1 = new TCanvas("can1","",800,500); 
  mass           ->SetLineColor(99);
  missingMassPass->SetLineColor(60);

  mass           ->Draw();
  missingMassPass->Draw("same");
  gPad->SetGridx(); 
  gPad->SetGridy(); 

  tit->DrawLatex(0.2, 0.9, " Missing Mass Sq. #color[99]{All} and #color[60]{Pass}"); 
  xtit->DrawLatex(0.48, 0.08, "M^{2}_{X}"); 
  ytit->DrawLatex(0.05, 0.48, "Counts");

  can1->Print(Form("%sMissingMass.png",imagePath.c_str())); 
}

void displayCoplanarAngles(){

  TLatex *tit = new TLatex();
  TLatex *xtit = new TLatex();
  TLatex *ytit = new TLatex();

  tit->SetNDC();
  tit->SetTextSize(0.05);
  tit->SetTextFont(42); 

  xtit->SetNDC();
  xtit->SetTextSize(0.04);
  xtit->SetTextFont(42); 

  ytit->SetNDC();
  ytit->SetTextSize(0.04);
  ytit->SetTextAngle(90.0);
  ytit->SetTextFont(42); 

  TH1D *coplanarAnglePass = events->Projection(3);

  TCanvas *c3 = new TCanvas("c3","",800,500);
  gPad ->SetGridx();
  gPad ->SetGridy();
  coplanarAngle    ->SetLineColor(99); 
  coplanarAnglePass->SetLineColor(55); 
  coplanarAngle    ->Draw(); 
  coplanarAnglePass->Draw("same"); 

  tit ->DrawLatex(0.4,0.9,"ep #rightarrow epX");
  xtit->DrawLatex(0.75,0.06,"#phi_{e}-#phi_{P}");
  ytit->DrawLatex(0.08,0.72,"Counts");
  c3    ->Print(Form("%sCoplanarAngles.png",imagePath.c_str()));

}

void displayElectronAngles(){

  TH1D *eX      = events->Projection(0); 
  TH1D *ePrimeX = events->Projection(4); 

  TLatex *tit = new TLatex();
  TLatex *xtit = new TLatex();
  TLatex *ytit = new TLatex();

  tit->SetNDC();
  tit->SetTextSize(0.05);
  tit->SetTextFont(42); 

  xtit->SetNDC();
  xtit->SetTextSize(0.04);
  xtit->SetTextFont(42); 

  ytit->SetNDC();
  ytit->SetTextSize(0.04);
  ytit->SetTextAngle(90.0);
  ytit->SetTextFont(42); 

  double max = eX->GetMaximum(); 
  if(ePrimeX->GetMaximum() > max) max = ePrimeX->GetMaximum();

  TCanvas *can1 = new TCanvas("can1","",800,500); 
  eX       ->SetLineColor(99);
  ePrimeX  ->SetLineColor(65);
  eX       ->SetMaximum(1.1*max);
  ePrimeX  ->SetMaximum(1.1*max);
  eX       ->Draw();
  ePrimeX  ->Draw("same");

  tit->DrawLatex(0.4, 0.9, "ep #rightarrow epX #color[99]{#theta_{eX}} and #color[65]{#theta_{e'X}}"); 
  xtit->DrawLatex(0.48, 0.08, "#theta");
  ytit->DrawLatex(0.08, 0.48, "Counts");

  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy(); 
  can1->Print(Form("%sElectronAngles.png",imagePath.c_str())); 

}

void displayW(){

  TH1D *wPass = events->Projection(2);
  double max = w    ->GetMaximum();
  double min = wPass->GetMaximum(); 

  // Set the photon to be along the beamline or along the lepton 
  setRange("xAngle",-2,5); 
  TH1D *wPassISR = events->Projection(2);
  setRange("xAngle",5,60); 
  TH1D *wPassFSR = events->Projection(2);
  setRange("xAngle",-20,70); 

  TLatex *tit = new TLatex();
  TLatex *xtit = new TLatex();
  TLatex *ytit = new TLatex();

  tit->SetNDC();
  tit->SetTextSize(0.05);
  tit->SetTextFont(42); 

  xtit->SetNDC();
  xtit->SetTextSize(0.04);
  xtit->SetTextFont(42); 

  ytit->SetNDC();
  ytit->SetTextSize(0.04);
  ytit->SetTextAngle(90.0);
  ytit->SetTextFont(42); 

  TCanvas *can2 = new TCanvas("can2","",800,500); 
  wPass   ->SetLineColor(65);
  wPassISR->SetLineColor(99);
  wPassFSR->SetLineColor(77);
  wPass   ->SetMaximum(min*1.1);
  wPass->Draw();
  wPassISR->Draw("same");
  wPassFSR->Draw("same");
  tit ->DrawLatex(0.25, 0.9, "ep #rightarrow epX #color[65]{Pass B.H.Cuts}"); 
  xtit->DrawLatex(0.7, 0.08, "W [GeV/c^{2}]");
  ytit->DrawLatex(0.08, 0.75, "Counts");
  //  gPad->SetGridx();
  //  gPad->SetGridy();

  TLegend *legendaryLegendOfAllLegends = new TLegend(0.21, 0.62, 0.47, 0.83); 
  legendaryLegendOfAllLegends->AddEntry(wPass, "Bethe Heitler", "l"); 
  legendaryLegendOfAllLegends->AddEntry(wPassISR, "#theta_{eX} < 10", "l"); 
  legendaryLegendOfAllLegends->AddEntry(wPassFSR, "#theta_{eX} > 10", "l"); 
  legendaryLegendOfAllLegends->SetBorderSize(0);
  legendaryLegendOfAllLegends->SetFillColor(0);
  legendaryLegendOfAllLegends->Draw(); 

  can2->Print(Form("%sWSpectrum.png",imagePath.c_str())); 

  /*
  TCanvas *can1 = new TCanvas("can1","",800,500); 
  w       ->SetLineColor(99);
  w       ->SetMaximum(1.1*max);
  wPass   ->SetMaximum(1.1*max);
  w       ->Draw();
  wPass   ->Draw("same");

  tit ->DrawLatex(0.25, 0.9, "ep #rightarrow epX #color[99]{All} and #color[65]{Pass B.H.Cuts}"); 
  xtit->DrawLatex(0.7, 0.08, "W [GeV/c^{2}]");
  ytit->DrawLatex(0.08, 0.75, "Counts");

  gPad->SetGridx();
  gPad->SetGridy();
  */
}

void displayMassAngle(){
  massAngle = events->Projection(0,1); 

  // Mass Angle 2D-Plot
  TCanvas *c1 = new TCanvas("c1","",800,500);
  TLatex *title = new TLatex();
  title->SetNDC();
  title->SetTextSize(0.05);

  TLatex *xtitle = new TLatex();
  xtitle->SetNDC();
  xtitle->SetTextSize(0.04);

  TLatex *ytitle = new TLatex();
  ytitle->SetNDC();
  ytitle->SetTextSize(0.04);
  ytitle->SetTextAngle(90.0);

  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  massAngle->Draw("colz");

  title ->DrawLatex(0.4,0.9,"ep #rightarrow epX");
  xtitle->DrawLatex(0.75,0.06,"M^{2}_{X}");
  ytitle->DrawLatex(0.08,0.72,"#theta_{X}");
  c1    ->Print(Form("%sMissingMassVsAngle.png",imagePath.c_str()));
}

void displayMassW(){
  massW = events->Projection(2,1); 

  TLatex *title = new TLatex();
  title->SetNDC();
  title->SetTextSize(0.05);

  TLatex *xtitle = new TLatex();
  xtitle->SetNDC();
  xtitle->SetTextSize(0.04);

  TLatex *ytitle = new TLatex();
  ytitle->SetNDC();
  ytitle->SetTextSize(0.04);
  ytitle->SetTextAngle(90.0);

  // Mass W 2D-Plot
  TCanvas *c3 = new TCanvas("c3","",800,500);
  gPad ->SetGridx();
  gPad ->SetGridy();
  gPad ->SetLogz();
  massW->Draw("colz");

  title ->DrawLatex(0.4,0.9,"ep #rightarrow epX");
  xtitle->DrawLatex(0.75,0.06,"M^{2}_{X}");
  ytitle->DrawLatex(0.08,0.72,"W [GeV/c^{2}]");
  c3    ->Print(Form("%sMissingMassVsW.png",imagePath.c_str()));
}

void displayMissingMassCoplanarAngle(){
  mmPhi = events->Projection(3,1); 

  TLatex *title = new TLatex();
  title->SetNDC();
  title->SetTextSize(0.05);

  TLatex *xtitle = new TLatex();
  xtitle->SetNDC();
  xtitle->SetTextSize(0.04);

  TLatex *ytitle = new TLatex();
  ytitle->SetNDC();
  ytitle->SetTextSize(0.04);
  ytitle->SetTextAngle(90.0);

  // Mass W 2D-Plot
  TCanvas *c3 = new TCanvas("c3","",800,500);
  gPad ->SetGridx();
  gPad ->SetGridy();
  gPad ->SetLogz();
  mmPhi->Draw("colz");

  title ->DrawLatex(0.4,0.9,"ep #rightarrow epX");
  xtitle->DrawLatex(0.75,0.06,"M^{2}_{X}");
  ytitle->DrawLatex(0.08,0.72,"#phi_{e}-#phi_{P}");
  c3    ->Print(Form("%sMissingMassPhi.png",imagePath.c_str()));


}

void applyCuts(double massMin, double massMax, double angleMin, double angleMax){
  setRange("missingMass", massMin, massMax);
  setRange("coplanarAngle", angleMin, angleMax);
}
