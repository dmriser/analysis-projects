void changeConfig(){
  config++;
  if (config == NCONF){
    config = 0; 
  }
  cout << " >>> Config changed to " << configName[config] << endl; 
}

void changeView(){
  view++; 

  if(view == 2){ view = 0; }
  cout << " >>> View changed to " << view << endl;
}

void toggleLogZ(){
  setLogZ = !setLogZ;
}

void init(){
  for(int c=0; c<NCONF; c++){
    cout << " >>> Creating histograms for " << configName[c] << endl;

    x[c]     = (TH1D*) events[c]->Projection(0); x[c]     ->SetName(Form("%s_x",configName[c].c_str()));
    y[c]     = (TH1D*) events[c]->Projection(1); y[c]     ->SetName(Form("%s_y",configName[c].c_str()));
    p[c]     = (TH1D*) events[c]->Projection(2); p[c]     ->SetName(Form("%s_p",configName[c].c_str()));
    w[c]     = (TH1D*) events[c]->Projection(3); w[c]     ->SetName(Form("%s_w",configName[c].c_str()));
    qq[c]    = (TH1D*) events[c]->Projection(4); qq[c]    ->SetName(Form("%s_qq",configName[c].c_str()));
    theta[c] = (TH1D*) events[c]->Projection(5); theta[c] ->SetName(Form("%s_theta",configName[c].c_str()));
    phi[c]   = (TH1D*) events[c]->Projection(6); phi[c]   ->SetName(Form("%s_phi",configName[c].c_str()));
    
    thetaPhi[c] = (TH2D*) events[c]->Projection(5,6); thetaPhi[c] ->SetName(Form("%s_thetaPhi",configName[c].c_str()));
    thetaP[c]   = (TH2D*) events[c]->Projection(5,2); thetaP[c]   ->SetName(Form("%s_thetaP",configName[c].c_str()));
    qqX[c]      = (TH2D*) events[c]->Projection(4,0); qqX[c]      ->SetName(Form("%s_qqX",configName[c].c_str()));
    qqW[c]      = (TH2D*) events[c]->Projection(4,3); qqW[c]      ->SetName(Form("%s_qqW",configName[c].c_str()));
    wY[c]       = (TH2D*) events[c]->Projection(1,3); wY[c]       ->SetName(Form("%s_wY",configName[c].c_str()));
    wNu[c]      = (TH2D*) events[c]->Projection(7,3); wNu[c]      ->SetName(Form("%s_wNu",configName[c].c_str()));
  }
}

void setRange(string varName, double min, double max){

  int var = 0;
  if (varName == "x")         { var = 0; }
  else if (varName == "y")    { var = 1; }
  else if (varName == "p")    { var = 2; }
  else if (varName == "w")    { var = 3; }
  else if (varName == "qq")   { var = 4; }
  else if (varName == "theta"){ var = 5; }
  else if (varName == "phi")  { var = 6; }
  else if (varName == "nu")  { var = 7; }

  for(int c=0; c<NCONF; c++){
    int startBin = events[c]->GetAxis(var)->FindBin(min);
    int stopBin  = events[c]->GetAxis(var)->FindBin(max);
    events[c]->GetAxis(var)->SetRange(startBin, stopBin);
  }

  cout << " >>> Range for " << varName << " restricted to " << min << "-" << max << endl; 
  init();
}

void normalize(){
  for(int c=0; c<NCONF; c++){
    x[c]    ->Scale(1.0/x[c]->Integral());
    y[c]    ->Scale(1.0/y[c]->Integral());
    p[c]    ->Scale(1.0/p[c]->Integral());
    w[c]    ->Scale(1.0/w[c]->Integral());
    qq[c]   ->Scale(1.0/qq[c]->Integral());
    theta[c]->Scale(1.0/theta[c]->Integral());
    phi[c]  ->Scale(1.0/phi[c]->Integral());
  }

  double max[7] = {x[0]    ->GetMaximum(), 
		   y[0]    ->GetMaximum(), 
		   p[0]    ->GetMaximum(), 
		   w[0]    ->GetMaximum(), 
		   qq[0]   ->GetMaximum(), 
		   theta[0]->GetMaximum(), 
		   phi[0]  ->GetMaximum()};
  
  for(int c=0; c<NCONF; c++){
    if (x[c]    ->GetMaximum() > max[0]){ max[0] = x[c]    ->GetMaximum(); }
    if (y[c]    ->GetMaximum() > max[1]){ max[1] = y[c]    ->GetMaximum(); }
    if (p[c]    ->GetMaximum() > max[2]){ max[2] = p[c]    ->GetMaximum(); }
    if (w[c]    ->GetMaximum() > max[3]){ max[3] = w[c]    ->GetMaximum(); }
    if (qq[c]   ->GetMaximum() > max[4]){ max[4] = qq[c]   ->GetMaximum(); }
    if (theta[c]->GetMaximum() > max[5]){ max[5] = theta[c]->GetMaximum(); }
    if (phi[c]  ->GetMaximum() > max[6]){ max[6] = phi[c]  ->GetMaximum(); }
  }
  
  for(int c=0; c<NCONF; c++){
    x[c]    ->SetMaximum(1.1*max[0]);
    y[c]    ->SetMaximum(1.1*max[1]);
    p[c]    ->SetMaximum(1.1*max[2]);
    w[c]    ->SetMaximum(1.1*max[3]);
    qq[c]   ->SetMaximum(1.1*max[4]);
    theta[c]->SetMaximum(1.1*max[5]);
    phi[c]  ->SetMaximum(1.1*max[6]);
  }
}

void draw(int type){
  // type 
  // [0] - x
  // [1] - y
  // [2] - p
  // [3] - w
  // [4] - qq
  // [5] - theta
  // [6] - phi


  int color = 97;

  TLatex xLabel, yLabel, title;
  xLabel.SetNDC();
  xLabel.SetTextSize(0.04);

  yLabel.SetNDC();
  yLabel.SetTextSize(0.04);
  yLabel.SetTextAngle(90.0);

  title.SetNDC();
  title.SetTextSize(0.04);

  string xLabelString, yLabelString, titleString;
  double xLabelPosX, xLabelPosY;
  double yLabelPosX, yLabelPosY;
  double titlePosX,  titlePosY;

  xLabelPosX = 0.74;  xLabelPosY = 0.06;
  yLabelPosX = 0.045; yLabelPosY = 0.72;
  titlePosX = 0.32;    titlePosY = 0.92;

  yLabelString = "Counts";
  titleString  = Form(" %s, N_{events} = %.1e ",configName[config].c_str(),
		      x[config]->GetEntries());

  if (view == 1){ normalize(); }

  TCanvas *canvas = new TCanvas("canvas","",800,800);

  if (type == 0) {

    if (view == 0) {
      x[config]->SetFillColorAlpha(color, 0.5);
      x[config]->SetLineColor(color);
      x[config]->Draw();
    } 

    else if (view == 1){
      for (int c=0; c<NCONF; c++){
	//	x[c]->SetFillColorAlpha(fillColor[c], 0.5);
	x[c]->SetLineColor(fillColor[c]);
	x[c]->Draw("histsame");
      }
    }

    xLabelString = "x";
  }
  
  else if (type == 1) {

    if (view == 0){
      y[config]->SetFillColorAlpha(color, 0.5);
      y[config]->SetLineColor(color);
      y[config]->Draw();
    }
 
    else if (view == 1){
      for (int c=0; c<NCONF; c++){
	//	y[c]->SetFillColorAlpha(fillColor[c], 0.5);
	y[c]->SetLineColor(fillColor[c]);
	y[c]->Draw("histsame");
      }
    }
    xLabelString = "y";
  }
  
  else if (type == 2) {

    if (view == 0){
      p[config]->SetFillColorAlpha(color, 0.5);
      p[config]->SetLineColor(color);
      p[config]->Draw();
    }
 
    else if (view == 1){
      for (int c=0; c<NCONF; c++){
	//	p[c]->SetFillColorAlpha(fillColor[c], 0.5);
	p[c]->SetLineColor(fillColor[c]);
	p[c]->Draw("histsame");
      }
    }
    xLabelString = "p [GeV/c]";
  }
  
  else if (type == 3) {

    if (view == 0){
      w[config]->SetFillColorAlpha(color, 0.5);
      w[config]->SetLineColor(color);
      w[config]->Draw();
    }
 
    else if (view == 1){
      for (int c=0; c<NCONF; c++){
	//	w[c]->SetFillColorAlpha(fillColor[c], 0.5);
	w[c]->SetLineColor(fillColor[c]);
	w[c]->Draw("histsame");
      }
    }
    xLabelString = "W [GeV/c^{2}]";
  }
  
  else if (type == 4) {

    if (view == 0){
      qq[config]->SetFillColorAlpha(color, 0.5);
      qq[config]->SetLineColor(color);
      qq[config]->Draw();
    }
 
    else if (view == 1){
      for (int c=0; c<NCONF; c++){
	//	qq[c]->SetFillColorAlpha(fillColor[c], 0.5);
	qq[c]->SetLineColor(fillColor[c]);
	qq[c]->Draw("histsame");
      }
    }
    xLabelString = "Q^{2} [GeV^{2}/c^{2}]";
  }

  else if (type == 5) {

    if (view == 0){
      theta[config]->SetFillColorAlpha(color, 0.5);
      theta[config]->SetLineColor(color);
      theta[config]->Draw();
    }
 
    else if (view == 1){
      for (int c=0; c<NCONF; c++){
	//	theta[c]->SetFillColorAlpha(fillColor[c], 0.5);
	theta[c]->SetLineColor(fillColor[c]);
	theta[c]->Draw("histsame");
      }
    }
    xLabelString = "#theta [Degrees]";
  }

  else if (type == 6) {

    if (view == 0){
      phi[config]->SetFillColorAlpha(color, 0.5);
      phi[config]->SetLineColor(color);
      phi[config]->Draw();
    }
 
    else if (view == 1){
      for (int c=0; c<NCONF; c++){
	//	phi[c]->SetFillColorAlpha(fillColor[c], 0.5);
	phi[c]->SetLineColor(fillColor[c]);
	phi[c]->Draw("histsame");
      }
    }
    xLabelString = "#phi [Degrees]";
  }
  
  
    xLabel.DrawLatex(xLabelPosX, xLabelPosY, xLabelString.c_str());
    yLabel.DrawLatex(yLabelPosX, yLabelPosY, yLabelString.c_str());
    title .DrawLatex(titlePosX,   titlePosY, titleString.c_str());

    gPad->SetMargin(0.15,0.15,0.15,0.15);
    gPad->SetGridx();
    gPad->SetGridy();

    if(print){
      canvas->Update();
      canvas->SaveAs(Form("img/%s_%s.png",configName[config].c_str(),histogramTitles1d[type].c_str()));
    }

}

void draw2d(int type){
  // type 
  // [0] - theta phi
  // [1] - qqX
  // [2] - qqW
  // [3] - wY
  // [4] - theta phi

  TLatex xLabel, yLabel, title;
  xLabel.SetNDC();
  xLabel.SetTextSize(0.04);

  yLabel.SetNDC();
  yLabel.SetTextSize(0.04);
  yLabel.SetTextAngle(90.0);

  title.SetNDC();
  title.SetTextSize(0.04);

  string xLabelString, yLabelString, titleString;
  double xLabelPosX, xLabelPosY;
  double yLabelPosX, yLabelPosY;
  double titlePosX,  titlePosY;

  xLabelPosX = 0.72;  xLabelPosY = 0.06;
  yLabelPosX = 0.06;  yLabelPosY = 0.72;
  titlePosX = 0.32;    titlePosY = 0.92;

  titleString  = Form(" %s, N_{events} = %.1e ",configName[config].c_str(),
		      x[config]->GetEntries());

  TCanvas *c = new TCanvas("c","",800,800);

  if (type == 0){
    thetaPhi[config]->Draw("colz");
    xLabelString = "#phi [Deg]";
    yLabelString = "#theta [Deg]";
  } 

  else if (type == 1){
    qqX[config]->Draw("colz");
    xLabelString = "x";
    yLabelString = "Q^{2} [GeV^{2}/c^{2}]";
  } 

  else if (type == 2){
    qqW[config]->Draw("colz");
    xLabelString = "W [GeV/c^{2}]";
    yLabelString = "Q^{2} [GeV^{2}/c^{2}]";
  } 

  else if (type == 3){
    wY[config]->Draw("colz");
    xLabelString = "W [GeV/c^{2}]";
    yLabelString = "y";
  } 

  else if (type == 4){
    thetaP[config]->Draw("colz");
    xLabelString = "P [GeV/c]";
    yLabelString = "#theta [Deg]";
  } 

  else if (type == 5){
    wNu[config]->Draw("colz");
    xLabelString = "W [GeV/c^{2}]";
    yLabelString = "#nu [GeV/c]";
  } 

  xLabel.DrawLatex(xLabelPosX, xLabelPosY, xLabelString.c_str());
  yLabel.DrawLatex(yLabelPosX, yLabelPosY, yLabelString.c_str());
  title .DrawLatex(titlePosX,   titlePosY, titleString.c_str());

  gPad->SetMargin(0.15,0.15,0.15,0.15);
  if (setLogZ) { gPad->SetLogz(); }
  gPad->SetGridx();
  gPad->SetGridy();

  if(print){
    c->Update();
    c->SaveAs(Form("img/%s_%s.png",configName[config].c_str(),histogramTitles2d[type].c_str()));
  }

}

void printAll(){
  print = true;
  int oldConfig = config; 

  for (int c=0; c<NCONF; c++){
    config = c;
    for(int type=0; type<7; type++){
      draw(type);
    }
    
    for(int type=0; type<5; type++){
      draw2d(type);
    }
  }

  config = oldConfig; 
  print = false; 
}

/*
void normalize(TH1D *h1, TH1D *h2){
  h1->Scale(h1->Integral());
  h2->Scale(h2->Integral());
  double max = h1->GetMaximum(); 
  if (h2->GetMaximum() > max){ max = h2->GetMaximum(); }
  h1->SetMaximum(1.1*max);
  h2->SetMaximum(1.1*max);
}

void normalize(TH1D *h1, TH1D *h2, TH1D *h3){
  h1->Scale(h1->Integral());
  h2->Scale(h2->Integral());
  h3->Scale(h3->Integral());
  double max = h1->GetMaximum(); 
  if (h2->GetMaximum() > max){ max = h2->GetMaximum(); }
  if (h3->GetMaximum() > max){ max = h3->GetMaximum(); }
  h1->SetMaximum(1.1*max);
  h2->SetMaximum(1.1*max);
  h3->SetMaximum(1.1*max);
}
*/
