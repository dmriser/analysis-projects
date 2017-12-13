{

  const int NCONF          = 3;
  string filename[NCONF]   = {"data.root","keppelRad.root","elastGen.root"};
  string configName[NCONF] = {"data","keppelRad","elastGen"};
  int configColor[NCONF]   = {55, 75, 95};

  string saveName = "pLineAt2";


  gROOT->LoadMacro("utils.C");

  TFile      *files[NCONF];
  THnSparseI *events[NCONF];
  TH1D       *x[NCONF][3];
  TH1D       *y[NCONF][3];
  TH1D       *nu[NCONF][3];
  TH1D       *qq[NCONF][3];
  TH1D       *w[NCONF][3];
  TH1D       *theta[NCONF][3];
  TH1D       *phi[NCONF][3];
  TH1D       *p[NCONF][3];
  
  TH2D       *thetaPhi[NCONF][3];
  TH2D       *thetaP[NCONF][3];
  TH2D       *qqX[NCONF][3];
  TH2D       *qqW[NCONF][3];
  TH2D       *wY[NCONF][3];
  TH2D       *xY[NCONF][3];
  TH2D       *yTheta[NCONF][3];

  for(int c=0; c<NCONF; c++){
    files[c]  = TFile::Open(filename[c].c_str());
    events[c] = (THnSparseI*) files[c]->Get("events");
    events[c]->SetName(Form("%s_events",configName[c].c_str()));
    events[c]->SetTitle(Form("%s_events",configName[c].c_str()));

    cout << "[CompareKinematics] Loading histograms for " << configName[c] << endl;

    // Grab histograms
    x[c][0]     = (TH1D*) events[c]->Projection(0); x[c][0]     ->SetName(Form("%s_x0",configName[c].c_str()));
    y[c][0]     = (TH1D*) events[c]->Projection(1); y[c][0]     ->SetName(Form("%s_y0",configName[c].c_str()));
    p[c][0]     = (TH1D*) events[c]->Projection(2); p[c][0]     ->SetName(Form("%s_p0",configName[c].c_str()));
    w[c][0]     = (TH1D*) events[c]->Projection(3); w[c][0]     ->SetName(Form("%s_w0",configName[c].c_str()));
    qq[c][0]    = (TH1D*) events[c]->Projection(4); qq[c][0]    ->SetName(Form("%s_qq0",configName[c].c_str()));
    theta[c][0] = (TH1D*) events[c]->Projection(5); theta[c][0] ->SetName(Form("%s_theta0",configName[c].c_str()));
    phi[c][0]   = (TH1D*) events[c]->Projection(6); phi[c][0]   ->SetName(Form("%s_phi0",configName[c].c_str()));
    nu[c][0]    = (TH1D*) events[c]->Projection(7); nu[c][0]    ->SetName(Form("%s_nu0",configName[c].c_str()));

    thetaPhi[c][0] = (TH2D*) events[c]->Projection(5,6); thetaPhi[c][0] ->SetName(Form("%s_thetaPhi0",configName[c].c_str()));
    thetaP[c][0]   = (TH2D*) events[c]->Projection(5,2); thetaP[c][0]   ->SetName(Form("%s_thetaP0",configName[c].c_str()));
    qqX[c][0]      = (TH2D*) events[c]->Projection(4,0); qqX[c][0]      ->SetName(Form("%s_qqX0",configName[c].c_str()));
    qqW[c][0]      = (TH2D*) events[c]->Projection(4,3); qqW[c][0]      ->SetName(Form("%s_qqW0",configName[c].c_str()));
    wY[c][0]       = (TH2D*) events[c]->Projection(1,3); wY[c][0]       ->SetName(Form("%s_wY0",configName[c].c_str()));
    xY[c][0]       = (TH2D*) events[c]->Projection(1,0); xY[c][0]       ->SetName(Form("%s_xY0",configName[c].c_str()));
    yTheta[c][0]   = (TH2D*) events[c]->Projection(1,5); yTheta[c][0]   ->SetName(Form("%s_yTheta0",configName[c].c_str()));

    // Setting for W < 2
    int cutBin = events[c]->GetAxis(3)->FindBin(2.0);
    int endBin = events[c]->GetAxis(3)->GetNbins();
    events[c]->GetAxis(3)->SetRange(1,cutBin);
    x[c][1]     = (TH1D*) events[c]->Projection(0); x[c][1]     ->SetName(Form("%s_x1",configName[c].c_str()));
    y[c][1]     = (TH1D*) events[c]->Projection(1); y[c][1]     ->SetName(Form("%s_y1",configName[c].c_str()));
    p[c][1]     = (TH1D*) events[c]->Projection(2); p[c][1]     ->SetName(Form("%s_p1",configName[c].c_str()));
    w[c][1]     = (TH1D*) events[c]->Projection(3); w[c][1]     ->SetName(Form("%s_w1",configName[c].c_str()));
    qq[c][1]    = (TH1D*) events[c]->Projection(4); qq[c][1]    ->SetName(Form("%s_qq1",configName[c].c_str()));
    theta[c][1] = (TH1D*) events[c]->Projection(5); theta[c][1] ->SetName(Form("%s_theta1",configName[c].c_str()));
    phi[c][1]   = (TH1D*) events[c]->Projection(6); phi[c][1]   ->SetName(Form("%s_phi1",configName[c].c_str()));
    nu[c][1]    = (TH1D*) events[c]->Projection(7); nu[c][1]    ->SetName(Form("%s_nu1",configName[c].c_str()));

    thetaPhi[c][1] = (TH2D*) events[c]->Projection(5,6); thetaPhi[c][1] ->SetName(Form("%s_thetaPhi1",configName[c].c_str()));
    thetaP[c][1]   = (TH2D*) events[c]->Projection(5,2); thetaP[c][1]   ->SetName(Form("%s_thetaP1",configName[c].c_str()));
    qqX[c][1]      = (TH2D*) events[c]->Projection(4,0); qqX[c][1]      ->SetName(Form("%s_qqX1",configName[c].c_str()));
    qqW[c][1]      = (TH2D*) events[c]->Projection(4,3); qqW[c][1]      ->SetName(Form("%s_qqW1",configName[c].c_str()));
    wY[c][1]       = (TH2D*) events[c]->Projection(1,3); wY[c][1]       ->SetName(Form("%s_wY1",configName[c].c_str()));
    xY[c][1]       = (TH2D*) events[c]->Projection(1,0); xY[c][1]       ->SetName(Form("%s_xY1",configName[c].c_str()));
    yTheta[c][1]   = (TH2D*) events[c]->Projection(1,5); yTheta[c][1]   ->SetName(Form("%s_yTheta1",configName[c].c_str()));


    // Setting for W > 2
    events[c]->GetAxis(3)->SetRange(cutBin,endBin);
    x[c][2]     = (TH1D*) events[c]->Projection(0); x[c][2]     ->SetName(Form("%s_x2",configName[c].c_str()));
    y[c][2]     = (TH1D*) events[c]->Projection(1); y[c][2]     ->SetName(Form("%s_y2",configName[c].c_str()));
    p[c][2]     = (TH1D*) events[c]->Projection(2); p[c][2]     ->SetName(Form("%s_p2",configName[c].c_str()));
    w[c][2]     = (TH1D*) events[c]->Projection(3); w[c][2]     ->SetName(Form("%s_w2",configName[c].c_str()));
    qq[c][2]    = (TH1D*) events[c]->Projection(4); qq[c][2]    ->SetName(Form("%s_qq2",configName[c].c_str()));
    theta[c][2] = (TH1D*) events[c]->Projection(5); theta[c][2] ->SetName(Form("%s_theta2",configName[c].c_str()));
    phi[c][2]   = (TH1D*) events[c]->Projection(6); phi[c][2]   ->SetName(Form("%s_phi2",configName[c].c_str()));
    nu[c][2]    = (TH1D*) events[c]->Projection(7); nu[c][2]    ->SetName(Form("%s_nu2",configName[c].c_str()));

    thetaPhi[c][2] = (TH2D*) events[c]->Projection(5,6); thetaPhi[c][2] ->SetName(Form("%s_thetaPhi2",configName[c].c_str()));
    thetaP[c][2]   = (TH2D*) events[c]->Projection(5,2); thetaP[c][2]   ->SetName(Form("%s_thetaP2",configName[c].c_str()));
    qqX[c][2]      = (TH2D*) events[c]->Projection(4,0); qqX[c][2]      ->SetName(Form("%s_qqX2",configName[c].c_str()));
    qqW[c][2]      = (TH2D*) events[c]->Projection(4,3); qqW[c][2]      ->SetName(Form("%s_qqW2",configName[c].c_str()));
    wY[c][2]       = (TH2D*) events[c]->Projection(1,3); wY[c][2]       ->SetName(Form("%s_wY2",configName[c].c_str()));
    xY[c][2]       = (TH2D*) events[c]->Projection(1,0); xY[c][2]       ->SetName(Form("%s_xY2",configName[c].c_str()));
    yTheta[c][2]   = (TH2D*) events[c]->Projection(1,5); yTheta[c][2]   ->SetName(Form("%s_yTheta2",configName[c].c_str()));

    // Return to full W Range    
    events[c]->GetAxis(3)->SetRange(1,endBin);
  }

  /*
  TControlBar *bar = new TControlBar("vertical","David");
  bar->AddButton("",""); 
  bar->AddButton("Show x(0)","draw(0,0)");   
  bar->AddButton("Show x(1)","draw(0,1)");   
  bar->AddButton("Show y(0)","draw(1,0)");   
  bar->AddButton("Show y(1)","draw(1,1)");   
  bar->AddButton("","");   
  bar->AddButton("Scatter P vs. Theta","scatter(0,0)");   
  bar->AddButton("Scatter Y vs. Theta","scatter(1,0)");   
  bar->AddButton("Quit",".q"); 
  bar->Show();
  */


  TCanvas *can1 = new TCanvas("can1","",1200,1200); 

  TLatex *xTitle = new TLatex(); 
  xTitle->SetNDC(); 
  xTitle->SetTextSize(0.05);
  xTitle->SetTextFont(42); 

  TLatex *yTitle = new TLatex(); 
  yTitle->SetNDC(); 
  yTitle->SetTextSize(0.05);
  yTitle->SetTextFont(42); 
  yTitle->SetTextAngle(90.0);

  TLatex *title = new TLatex(); 
  title->SetNDC(); 
  title->SetTextSize(0.05);
  title->SetTextFont(42); 

  TLine *line = new TLine(2.0,20,2.0,60); 
  line->SetLineWidth(2);
  line->SetLineColor(99);
  line->SetLineStyle(4);

  can1->Divide(3,3);

  can1->cd(1);
  thetaP[0][0]->Draw("colz");
  title->DrawLatex(0.45, 0.9, "All W Range"); 
  xTitle->DrawLatex(0.65, 0.1, "P [GeV/c]") ;
  yTitle->DrawLatex(0.08, 0.45, "Data");
  gPad->SetGridx();
  gPad->SetGridy();

  can1->cd(2);
  thetaP[0][1]->Draw("colz");
  title->DrawLatex(0.45, 0.9, "W < 2.0 GeV/c^{2}"); 
  xTitle->DrawLatex(0.65, 0.1, "P [GeV/c]");
  gPad->SetGridx();
  gPad->SetGridy();

  can1->cd(3);
  thetaP[0][2]->Draw("colz");
  title->DrawLatex(0.45, 0.9, "w > 2.0 GeV/c^{2}"); 
  xTitle->DrawLatex(0.65, 0.1, "P [GeV/c]");
  gPad->SetGridx();
  gPad->SetGridy();
  line->Draw();

  can1->cd(4);
  thetaP[1][0]->Draw("colz");
  xTitle->DrawLatex(0.65, 0.1, "P [GeV/c]");
  yTitle->DrawLatex(0.08, 0.45, "Keppel");
  gPad->SetGridx();
  gPad->SetGridy();

  can1->cd(5);
  thetaP[1][1]->Draw("colz");
  xTitle->DrawLatex(0.65, 0.1, "P [GeV/c]");
  gPad->SetGridx();
  gPad->SetGridy();

  can1->cd(6);
  thetaP[1][2]->Draw("colz");
  xTitle->DrawLatex(0.65, 0.1, "P [GeV/c]");
  gPad->SetGridx();
  gPad->SetGridy();
  line->Draw();

  can1->cd(7);
  thetaP[2][0]->Draw("colz");
  xTitle->DrawLatex(0.65, 0.1, "P [GeV/c]");
  yTitle->DrawLatex(0.08, 0.45, "ElastGen");
  gPad->SetGridx();
  gPad->SetGridy();

  can1->cd(8);
  thetaP[2][1]->Draw("colz");
  xTitle->DrawLatex(0.65, 0.1, "P [GeV/c]");
  gPad->SetGridx();
  gPad->SetGridy();

  can1->cd(9);
  thetaP[2][2]->Draw("colz");
  xTitle->DrawLatex(0.65, 0.1, "P [GeV/c]");
  gPad->SetGridx();
  gPad->SetGridy();
  line->Draw();

  can1->Print(Form("thetaP_%s.png",saveName.c_str()));




  //////////////////////////  //////////////////////////  //////////////////////////  //////////////////////////  //////////////////////////  //////////////////////////
  TCanvas *can2 = new TCanvas("can2","",1200,1200); 

  TF1 *curve = new TF1("curve","[0]*x + [1]");
  curve->SetParameter(0,1.3); 
  curve->SetParameter(1,0.55); 
  curve->SetLineWidth(2);
  curve->SetLineColor(99);
  curve->SetLineStyle(4);

  TLatex *xTitle2 = new TLatex(); 
  xTitle2->SetNDC(); 
  xTitle2->SetTextSize(0.05);
  xTitle2->SetTextFont(42); 

  TLatex *yTitle2 = new TLatex(); 
  yTitle2->SetNDC(); 
  yTitle2->SetTextSize(0.05);
  yTitle2->SetTextFont(42); 
  yTitle2->SetTextAngle(90.0);

  TLatex *title2 = new TLatex(); 
  title2->SetNDC(); 
  title2->SetTextSize(0.05);
  title2->SetTextFont(42); 

  can2->Divide(3,3);

  can2->cd(1);
  xY[0][0]->Draw("colz");
  title2->DrawLatex(0.45, 0.9, "All W Range"); 
  xTitle2->DrawLatex(0.8, 0.1, "x") ;
  yTitle2->DrawLatex(0.08, 0.75, "y");
  yTitle2->DrawLatex(0.08, 0.45, "Data");
  gPad->SetGridx();
  gPad->SetGridy();

  can2->cd(2);
  xY[0][1]->Draw("colz");
  title2->DrawLatex(0.45, 0.9, "W < 2.0 GeV/c^{2}"); 
  xTitle2->DrawLatex(0.8, 0.1, "x") ;
  yTitle2->DrawLatex(0.08, 0.75, "y");
  gPad->SetGridx();
  gPad->SetGridy();

  can2->cd(3);
  xY[0][2]->Draw("colz");
  title2->DrawLatex(0.45, 0.9, "w > 2.0 GeV/c^{2}"); 
  xTitle2->DrawLatex(0.8, 0.1, "x") ;
  yTitle2->DrawLatex(0.08, 0.75, "y");
  gPad->SetGridx();
  gPad->SetGridy();
  curve->Draw("same");

  can2->cd(4);
  xY[1][0]->Draw("colz");
  yTitle2->DrawLatex(0.08, 0.45, "Keppel");
  xTitle2->DrawLatex(0.8, 0.1, "x") ;
  yTitle2->DrawLatex(0.08, 0.75, "y");
  gPad->SetGridx();
  gPad->SetGridy();

  can2->cd(5);
  xY[1][1]->Draw("colz");
  xTitle2->DrawLatex(0.8, 0.1, "x") ;
  yTitle2->DrawLatex(0.08, 0.75, "y");
  gPad->SetGridx();
  gPad->SetGridy();

  can2->cd(6);
  xY[1][2]->Draw("colz");
  xTitle2->DrawLatex(0.8, 0.1, "x") ;
  yTitle2->DrawLatex(0.08, 0.75, "y");
  gPad->SetGridx();
  gPad->SetGridy();
  curve->Draw("same");

  can2->cd(7);
  xY[2][0]->Draw("colz");
  xTitle2->DrawLatex(0.8, 0.1, "x") ;
  yTitle2->DrawLatex(0.08, 0.75, "y");
  yTitle2->DrawLatex(0.08, 0.45, "ElastGen");
  gPad->SetGridx();
  gPad->SetGridy();

  can2->cd(8);
  xY[2][1]->Draw("colz");
  xTitle2->DrawLatex(0.8, 0.1, "x") ;
  yTitle2->DrawLatex(0.08, 0.75, "y");
  gPad->SetGridx();
  gPad->SetGridy();

  can2->cd(9);
  xY[2][2]->Draw("colz");
  xTitle2->DrawLatex(0.8, 0.1, "x") ;
  yTitle2->DrawLatex(0.08, 0.75, "y");
  gPad->SetGridx();
  gPad->SetGridy();
  curve->Draw("same");

  can2->Print(Form("xy_%s.png",saveName.c_str()));

  ///////////  ///////////  ///////////  ///////////  ///////////  ///////////  ///////////  ///////////
  TCanvas *can3 = new TCanvas("can3","",550,800); 
  can3->Divide(1,3);

  double pMin = (1-0.7)*5.498; 

  TLine *lin = new TLine(pMin,12,pMin,60); 
  lin->SetLineWidth(3);
  lin->SetLineColor(99);
  //  lin->SetLineStyle(4);

  can3->cd(1); 
  thetaP[0][0]->Draw("colz"); 
  lin->Draw();
  gPad->SetGridx(); 
  gPad->SetGridy(); 
  title->SetTextSize(0.07);
  title ->DrawLatex(0.4, 0.9, "ep #rightarrow eX #theta_{e} vs. P'_{e}");
  yTitle->DrawLatex(0.05, 0.4,"E1-F Data"); 
  yTitle->DrawLatex(0.1, 0.7,"#theta [Deg]"); 
  xTitle->DrawLatex(0.46, 0.01, "P [GeV/c]");
  gPad->SetBottomMargin(0.05);

  can3->cd(2); 
  gPad->SetGridx(); 
  gPad->SetGridy(); 
  thetaP[1][0]->Draw("colz"); 
  lin->Draw();
  gPad->SetTopMargin(0.05);
  gPad->SetBottomMargin(0.05);
  yTitle->DrawLatex(0.05, 0.3,"Inelastic Simulation"); 
  xTitle->DrawLatex(0.46, 0.01, "P [GeV/c]");
  yTitle->DrawLatex(0.1, 0.7,"#theta [Deg]"); 

  can3->cd(3); 
  gPad->SetGridx(); 
  gPad->SetGridy(); 
  thetaP[2][0]->Draw("colz"); 
  lin->Draw();
  gPad->SetTopMargin(0.05);
  xTitle->DrawLatex(0.46, 0.08, "P [GeV/c]");
  yTitle->DrawLatex(0.05, 0.15,"Elastic/Bethe-Heitler Simulation"); 
  yTitle->DrawLatex(0.1, 0.7,"#theta [Deg]"); 

  can3->Print("/volatile/clas/clas12/dmriser/plots/inclusive/phaseSpace.png"); 

}
