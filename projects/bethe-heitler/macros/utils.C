void normalize(TH1D *h1, TH1D *h2){
  h1->Scale(1.0/h1->Integral());
  h2->Scale(1.0/h2->Integral());
  double max = h1->GetMaximum(); 
  if (h2->GetMaximum() > max){ max = h2->GetMaximum(); }
  h1->SetMaximum(1.1*max);
  h2->SetMaximum(1.1*max);
}

void normalize(TH1D *h1, TH1D *h2, TH1D *h3){
  h1->Scale(1.0/h1->Integral());
  h2->Scale(1.0/h2->Integral());
  h3->Scale(1.0/h3->Integral());
  double max = h1->GetMaximum(); 
  if (h2->GetMaximum() > max){ max = h2->GetMaximum(); }
  if (h3->GetMaximum() > max){ max = h3->GetMaximum(); }
  h1->SetMaximum(1.1*max);
  h2->SetMaximum(1.1*max);
  h3->SetMaximum(1.1*max);
}

void draw(int type, int style){

  // type 
  // [0] - x
  // [1] - y
  // [2] - p
  // [3] - w
  // [4] - qq
  // [5] - theta
  // [6] - phi
  // [7] - nu 

  // style 
  // [0] - Draw together the x plots for different W 
  // [1] - Draw together the x plots for different config

  TLegend *legend = new TLegend(0.1, 0.8, 0.2, 0.9); 

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
  titlePosX  = 0.32;   titlePosY = 0.92;

  yLabelString = "Probability";
  titleString  = "titleString";

  TCanvas *c = new TCanvas("c","",1200,400);
  c->Divide(3,1); 
 
  if (type == 0){
    x[0][0]->SetLineColor(99);
    x[0][1]->SetLineColor(75);
    x[0][2]->SetLineColor(55);
    x[1][0]->SetLineColor(99);
    x[1][1]->SetLineColor(75);
    x[1][2]->SetLineColor(55);
    x[2][0]->SetLineColor(99);
    x[2][1]->SetLineColor(75);
    x[2][2]->SetLineColor(55);

    x[0][1]->SetLineStyle(3);
    x[0][2]->SetLineStyle(4);
    x[1][1]->SetLineStyle(3);
    x[1][2]->SetLineStyle(4);
    x[2][1]->SetLineStyle(3);
    x[2][2]->SetLineStyle(4);

    if (style == 0){
      c->cd(1);
      normalize(x[0][0], x[0][1], x[0][2]);      

      x[0][0]->Draw("hist");
      x[0][1]->Draw("histsame");
      x[0][2]->Draw("histsame");

      c->cd(2);
      normalize(x[1][0], x[1][1], x[1][2]);      
      x[1][0]->Draw("hist");
      x[1][1]->Draw("histsame");
      x[1][2]->Draw("histsame");

      c->cd(3);
      normalize(x[2][0], x[2][1], x[2][2]);      
      x[2][0]->Draw("hist");
      x[2][1]->Draw("histsame");
      x[2][2]->Draw("histsame");

      legend->AddEntry(x[0][0],"All","l");
      legend->AddEntry(x[0][1],"W < 2","l");
      legend->AddEntry(x[0][2],"W > 2","l");
    }

    else if (style == 1){
      c->cd(1);
      normalize(x[0][0], x[1][0], x[2][0]);
      x[0][0]->Draw("hist");
      x[1][0]->Draw("histsame");
      x[2][0]->Draw("histsame");

      c->cd(2);
      normalize(x[0][1], x[1][1], x[2][1]);
      x[0][1]->Draw("hist");
      x[1][1]->Draw("histsame");
      x[2][1]->Draw("histsame");

      c->cd(3);
      normalize(x[0][1], x[1][1], x[2][1]);
      x[0][2]->Draw("hist");
      x[1][2]->Draw("histsame");
      x[2][2]->Draw("histsame");

    }
  }

  else if(type == 1){
    y[0][0]->SetLineColor(99);
    y[0][1]->SetLineColor(75);
    y[0][2]->SetLineColor(55);
    y[1][0]->SetLineColor(99);
    y[1][1]->SetLineColor(75);
    y[1][2]->SetLineColor(55);
    y[2][0]->SetLineColor(99);
    y[2][1]->SetLineColor(75);
    y[2][2]->SetLineColor(55);

    if (style == 0){
      c->cd(1);
      normalize(y[0][0], y[0][1], y[0][2]);      

      y[0][0]->Draw("hist");
      y[0][1]->Draw("histsame");
      y[0][2]->Draw("histsame");

      c->cd(2);
      normalize(y[1][0], y[1][1], y[1][2]);      
      y[1][0]->Draw("hist");
      y[1][1]->Draw("histsame");
      y[1][2]->Draw("histsame");

      c->cd(3);
      normalize(y[2][0], y[2][1], y[2][2]);      
      y[2][0]->Draw("hist");
      y[2][1]->Draw("histsame");
      y[2][2]->Draw("histsame");
    }

    else if (style == 1){
      c->cd(1);
      normalize(y[0][0], y[1][0], y[2][0]);
      y[0][0]->Draw("hist");
      y[1][0]->Draw("histsame");
      y[2][0]->Draw("histsame");

      c->cd(2);
      normalize(y[0][1], y[1][1], y[2][1]);
      y[0][1]->Draw("hist");
      y[1][1]->Draw("histsame");
      y[2][1]->Draw("histsame");

      c->cd(3);
      normalize(y[0][1], y[1][1], y[2][1]);
      y[0][2]->Draw("hist");
      y[1][2]->Draw("histsame");
      y[2][2]->Draw("histsame");

    }
  }
  
  xLabel.DrawLatex(xLabelPosX, xLabelPosY, xLabelString.c_str());
  yLabel.DrawLatex(yLabelPosX, yLabelPosY, yLabelString.c_str());
  title .DrawLatex(titlePosX,   titlePosY, titleString.c_str());

  gPad->SetMargin(0.15,0.15,0.15,0.15);
  gPad->SetGridx();
  gPad->SetGridy();
  legend->Draw();   

}

void scatter(int type, int style){
  // Type 
  // [0] - thetaP
  // [1] - yTheta
  TCanvas *scatterCan = new TCanvas("scatterCan","",800,800); 

  if(type == 0){
    if(style == 0){
      thetaP[1][2]->SetMarkerColor(kRed); 
      thetaP[2][2]->SetMarkerColor(kBlue); 
      thetaP[1][2]->Draw();
      thetaP[2][2]->Draw("same");
    }
  }

  else if (type == 1){
    yTheta[1][2]->SetMarkerColor(kRed);
    yTheta[2][2]->SetMarkerColor(kBlue);
    yTheta[1][2]->Draw();
    yTheta[2][2]->Draw("same");
  }

  gPad->SetGridx();
  gPad->SetGridy();

}
