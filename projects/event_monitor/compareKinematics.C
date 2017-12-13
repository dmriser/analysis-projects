{

  const int NCONF   = 3;
  string filename[NCONF]   = {"data.root","keppelRad.root","elastGen.root"};
  string configName[NCONF] = {"data","keppelRad","elastGen"};
  int configColor[NCONF]   = {55, 75, 95};


  gROOT->LoadMacro("tools.C");

  TFile      *files[NCONF];
  THnSparseI *events[NCONF];
  TH1D       *x[NCONF], *xLess[NCONF], *xGreater[NCONF];
  TH1D       *y[NCONF], *yLess[NCONF], *yGreater[NCONF];
  TH1D       *nu[NCONF], *nuLess[NCONF], *nuGreater[NCONF];
  TH1D       *qq[NCONF];
  TH1D       *w[NCONF];
  TH1D       *theta[NCONF], *thetaLess[NCONF], *thetaGreater[NCONF];
  TH1D       *phi[NCONF];
  TH1D       *p[NCONF];
  
  TH2D       *thetaPhi[NCONF];
  TH2D       *thetaP[NCONF], *thetaPLess[NCONF], *thetaPGreater[NCONF];
  TH2D       *qqX[NCONF];
  TH2D       *qqW[NCONF];
  TH2D       *wY[NCONF];
  TH2D       *yTheta[NCONF], *yThetaLess[NCONF], *yThetaGreater[NCONF];

  for(int c=0; c<NCONF; c++){
    files[c]  = TFile::Open(filename[c].c_str());
    events[c] = (THnSparseI*) files[c]->Get("events");
    events[c]->SetName(Form("%s_events",configName[c].c_str()));
    events[c]->SetTitle(Form("%s_events",configName[c].c_str()));

    // Grab histograms
    x[c]            = (TH1D*) events[c]->Projection(0); x[c]       ->SetName(Form("%s_x",configName[c].c_str()));
    y[c]            = (TH1D*) events[c]->Projection(1); y[c]       ->SetName(Form("%s_y",configName[c].c_str()));
    p[c]            = (TH1D*) events[c]->Projection(2); p[c]     ->SetName(Form("%s_p",configName[c].c_str()));
    w[c]            = (TH1D*) events[c]->Projection(3); w[c]     ->SetName(Form("%s_w",configName[c].c_str()));
    qq[c]           = (TH1D*) events[c]->Projection(4); qq[c]    ->SetName(Form("%s_qq",configName[c].c_str()));
    theta[c]        = (TH1D*) events[c]->Projection(5); theta[c] ->SetName(Form("%s_theta",configName[c].c_str()));
    phi[c]          = (TH1D*) events[c]->Projection(6); phi[c]   ->SetName(Form("%s_phi",configName[c].c_str()));
    nu[c]           = (TH1D*) events[c]->Projection(7); nu[c]    ->SetName(Form("%s_nu",configName[c].c_str()));

    thetaPhi[c] = (TH2D*) events[c]->Projection(5,6); thetaPhi[c] ->SetName(Form("%s_thetaPhi",configName[c].c_str()));
    thetaP[c]   = (TH2D*) events[c]->Projection(5,2); thetaP[c]   ->SetName(Form("%s_thetaP",configName[c].c_str()));
    qqX[c]      = (TH2D*) events[c]->Projection(4,0); qqX[c]      ->SetName(Form("%s_qqX",configName[c].c_str()));
    qqW[c]      = (TH2D*) events[c]->Projection(4,3); qqW[c]      ->SetName(Form("%s_qqW",configName[c].c_str()));
    wY[c]       = (TH2D*) events[c]->Projection(1,3); wY[c]       ->SetName(Form("%s_wY",configName[c].c_str()));
    yTheta[c]   = (TH2D*) events[c]->Projection(1,5); yTheta[c]   ->SetName(Form("%s_yTheta",configName[c].c_str()));

    // Setting for W < 2
    int cutBin = events[c]->GetAxis(3)->FindBin(2.0);
    int endBin = events[c]->GetAxis(3)->GetNbins();
    events[c]->GetAxis(3)->SetRange(1,cutBin);
    xLess[c]      = (TH1D*) events[c]->Projection(0); xLess[c]        ->SetName(Form("%s_xLess",configName[c].c_str()));
    yLess[c]      = (TH1D*) events[c]->Projection(1); yLess[c]        ->SetName(Form("%s_yLess",configName[c].c_str()));
    thetaLess[c]  = (TH1D*) events[c]->Projection(5); thetaLess[c]    ->SetName(Form("%s_thetaLess",configName[c].c_str()));
    nuLess[c]     = (TH1D*) events[c]->Projection(7); nuLess[c]       ->SetName(Form("%s_nuLess",configName[c].c_str()));
    thetaPLess[c] = (TH2D*) events[c]->Projection(5,2); thetaPLess[c] ->SetName(Form("%s_thetaPLess",configName[c].c_str()));
    yThetaLess[c] = (TH2D*) events[c]->Projection(1,5); yThetaLess[c] ->SetName(Form("%s_yThetaLess",configName[c].c_str()));

    // Setting for W > 2
    events[c]->GetAxis(3)->SetRange(cutBin,endBin);
    xGreater[c]      = (TH1D*) events[c]->Projection(0); xGreater[c]        ->SetName(Form("%s_xGreater",configName[c].c_str()));
    yGreater[c]      = (TH1D*) events[c]->Projection(1); yGreater[c]        ->SetName(Form("%s_yGreater",configName[c].c_str()));
    thetaGreater[c]  = (TH1D*) events[c]->Projection(5); thetaGreater[c]    ->SetName(Form("%s_thetaGreater",configName[c].c_str()));
    nuGreater[c]     = (TH1D*) events[c]->Projection(7); nuGreater[c]       ->SetName(Form("%s_nuGreater",configName[c].c_str()));
    thetaPGreater[c] = (TH2D*) events[c]->Projection(5,2); thetaPGreater[c] ->SetName(Form("%s_thetaPGreater",configName[c].c_str()));
    yThetaGreater[c] = (TH2D*) events[c]->Projection(1,5); yThetaGreater[c] ->SetName(Form("%s_yThetaGreater",configName[c].c_str()));
    
    events[c]->GetAxis(3)->SetRange(1,endBin);

    y[c]     ->SetLineColor(configColor[c]);
    yLess[c]    ->SetLineColor(configColor[c]);
    yGreater[c] ->SetLineColor(configColor[c]);
    yLess[c]    ->SetLineStyle(3);
    yGreater[c] ->SetLineStyle(4);
    w[c]     ->SetLineColor(configColor[c]);
    x[c]        ->SetLineColor(configColor[c]);
    xLess[c]    ->SetLineColor(configColor[c]);
    xGreater[c] ->SetLineColor(configColor[c]);
    xLess[c]    ->SetLineStyle(3);
    xGreater[c] ->SetLineStyle(4);
 
    yTheta[c]->SetMarkerColor(configColor[c]);
  }

  normalize(x[0], x[1], x[2]);
  normalize(x[0], xLess[0], xGreater[0]);
  normalize(x[1], xLess[1], xGreater[1]);
  normalize(x[2], xLess[2], xGreater[2]);
  normalize(y[0], y[1], y[2]);
  normalize(y[0], yLess[0], yGreater[0]);
  normalize(y[1], yLess[1], yGreater[1]);
  normalize(y[2], yLess[2], yGreater[2]);
  normalize(w[0], w[1], w[2]);

  TCanvas *compareCanvas = new TCanvas("compareCanvas","",1000,1000);
  compareCanvas->Divide(3,3);

  TLatex xTitle, yTitle, title;
  title.SetNDC();
  title.SetTextFont(21);
  title.SetTextSize(0.04);

  xTitle.SetNDC();
  xTitle.SetTextFont(21);
  xTitle.SetTextSize(0.05);

  yTitle.SetNDC();
  yTitle.SetTextFont(21);
  yTitle.SetTextSize(0.05);
  yTitle.SetTextAngle(90.0);

  compareCanvas->cd(0);
  title.DrawLatex(0.46, 0.95, "y vs. #theta");

  compareCanvas->cd(1);
  gPad->SetLogz();
  gPad->SetGridx();
  gPad->SetGridy();
  yTheta[0]->Draw("colz");
  yTitle.DrawLatex(0.08, 0.45, configName[0].c_str());
  xTitle.DrawLatex(0.45, 0.9, "All"); 

  compareCanvas->cd(2);
  gPad->SetLogz();
  gPad->SetGridx();
  gPad->SetGridy();
  yThetaLess[0]->Draw("colz");
  xTitle.DrawLatex(0.4, 0.9, "W < 2 GeV/c^2"); 
  xTitle.DrawLatex(0.65, 0.1, "#theta [deg]");
  yTitle.DrawLatex(0.08, 0.65, "y");
 
  compareCanvas->cd(3);
  gPad->SetLogz();
  gPad->SetGridx();
  gPad->SetGridy();
  yThetaGreater[0]->Draw("colz");
  xTitle.DrawLatex(0.4, 0.9, "W > 2 GeV/c^2"); 

  compareCanvas->cd(4);
  gPad->SetLogz();
  gPad->SetGridx();
  gPad->SetGridy();
  yTheta[1]->Draw("colz");
  yTitle.DrawLatex(0.08, 0.45, configName[1].c_str());

  compareCanvas->cd(5);
  gPad->SetLogz();
  gPad->SetGridx();
  gPad->SetGridy();
  yThetaLess[1]->Draw("colz");

  compareCanvas->cd(6);
  gPad->SetLogz();
  gPad->SetGridx();
  gPad->SetGridy();
  yThetaGreater[1]->Draw("colz");

  compareCanvas->cd(7);
  gPad->SetLogz();
  gPad->SetGridx();
  gPad->SetGridy();
  yTheta[2]->Draw("colz");
  yTitle.DrawLatex(0.08, 0.45, configName[2].c_str());

  compareCanvas->cd(8);
  gPad->SetLogz();
  gPad->SetGridx();
  gPad->SetGridy();
  yThetaLess[2]->Draw("colz");

  compareCanvas->cd(9);
  gPad->SetLogz();
  gPad->SetGridx();
  gPad->SetGridy();
  yThetaGreater[2]->Draw("colz");


}
