{

  // --------------------------------------------------
  //    User Parameters 
  // --------------------------------------------------
  TFile *inputFile = TFile::Open("/volatile/clas12/dmriser/rootFiles/inclusive/xs/goodRuns.root"); 
  string imagePath = "/volatile/clas12/dmriser/plots/inclusive/";
  const int NSECT  = 7;
  const int NSLICE = 10; 


  // --------------------------------------------------
  //    The Rest
  // --------------------------------------------------
  TH1D *data[NSECT][NSLICE];
  TH1D *sim[NSECT][NSLICE];
  TH1D *ratio[NSECT][NSLICE];

  for(int sect=0; sect<NSECT; sect++){
    for(int slice=0; slice<NSLICE; slice++){
      data[sect][slice]  = (TH1D*) inputFile->Get(Form("dataEvents_wByQQ_s%d_slice%d",sect,slice));
      sim[sect][slice]   = (TH1D*) inputFile->Get(Form("recEventsRad_wByQQ_s%d_slice%d",sect,slice));
      ratio[sect][slice] = (TH1D*) data[sect][slice]->Clone(); 
      ratio[sect][slice]->SetName(Form("ratio_s%d_slice%d",sect,slice));
      ratio[sect][slice]->SetTitle(Form("ratio_s%d_slice%d",sect,slice));
      sim[sect][slice]  ->Scale( data[sect][slice]->Integral()/sim[sect][slice]->Integral()); 
      ratio[sect][slice]->Divide( sim[sect][slice] );


      // Styling Options 
      data[sect][slice]->SetLineWidth(2);
      sim[sect][slice] ->SetLineWidth(2);
      data[sect][slice]->SetLineColor(61); 
      sim[sect][slice] ->SetLineColor(99);

      ratio[sect][slice]->SetMinimum(0.6);
      ratio[sect][slice]->SetMaximum(1.3);
      ratio[sect][slice]->SetMarkerStyle(8);
    }
  }

  TCanvas *c1 = new TCanvas("c1","",800,800); 

  TLatex tit, xtit, ytit; 
  tit.SetNDC(); 
  tit.SetTextFont(42);
  tit.SetTextSize(0.04);

  xtit.SetNDC(); 
  xtit.SetTextFont(42);
  xtit.SetTextSize(0.04);

  ytit.SetNDC(); 
  ytit.SetTextFont(42);
  ytit.SetTextSize(0.04);
  ytit.SetTextAngle(90.0);

  TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
  pad1->SetBottomMargin(0);
  pad1->SetGridx();        
  pad1->Draw();            
  pad1->cd();              
  data[0][4]->SetStats(0);
  data[0][4]->Draw("hist");
  sim[0][4] ->Draw("histsame");

  data[0][4]->GetYaxis()->SetLabelSize(0.0);

  TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
  axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
  axis->SetLabelSize(15);
  axis->Draw();

  // lower plot will be in pad
  c1->cd();
  TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
  pad2->SetTopMargin(0);
  pad2->SetBottomMargin(0.2);
  pad2->SetGridx(); 
  pad2->Draw();
  pad2->cd();       

  ratio[0][4]->Draw("pe");
  ratio[0][4]->SetMarkerStyle(8);
  ratio[0][4]->GetYaxis()->SetNdivisions(505);
  ratio[0][4]->GetYaxis()->SetLabelFont(43);
  ratio[0][4]->GetYaxis()->SetLabelSize(15);

  ratio[0][4]->GetXaxis()->SetNdivisions(505);
  ratio[0][4]->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
  ratio[0][4]->GetXaxis()->SetLabelSize(15);

  c1->cd(); 
  tit .DrawLatex(0.4,0.9,"#color[99]{Data} and #color[61]{MC}"); 
  xtit.DrawLatex(0.5,0.1,"W (GeV/c^{2})");
  ytit.DrawLatex(0.1,0.35,"Counts (Scaled to Data in each Q^{2} bin)");
  ytit.DrawLatex(0.1,0.1,"Ratio");
  
  c1->Print("testRatio.png");
}
