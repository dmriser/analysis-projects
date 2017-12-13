void changeConfig(){
  config++;
  if (config == NCONF){
    config = 0; 
  }
  cout << " >>> Config changed to " << config << endl; 
}

void draw(TH1D *histo, int type){
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
  titlePosX = 0.62;    titlePosY = 0.92;

  yLabelString = "Counts";
  titleString  = Form(" N_{events} = %.1e ", histo->GetEntries());

  if (type == 0) {
    histo->SetFillColorAlpha(color, 0.5);
    histo->SetLineColor(color);
    xLabelString = "x";
  }
  
  if (type == 1) {
    histo->SetFillColorAlpha(color, 0.5);
    histo->SetLineColor(color);
    xLabelString = "y";
  }
  
  if (type == 2) {
    histo->SetFillColorAlpha(color, 0.5);
    histo->SetLineColor(color);
    xLabelString = "p [GeV/c]";
  }
  
  if (type == 3) {
    histo->SetFillColorAlpha(color, 0.5);
    histo->SetLineColor(color);
    xLabelString = "W [GeV/c^{2}]";
  }
  
  if (type == 4) {
    histo->SetFillColorAlpha(color, 0.5);
    histo->SetLineColor(color);
    xLabelString = "Q^{2} [GeV^{2}/c^{2}]";
  }
  
  if (type == 5) {
    histo->SetFillColorAlpha(color, 0.5);
    histo->SetLineColor(color);
    xLabelString = "#theta [Deg]";
  }
  
  if (type == 6) {
    histo->SetFillColorAlpha(color, 0.5);
    histo->SetLineColor(color);
    xLabelString = "#phi [Deg]";
  }
  
    histo->Draw();      
    xLabel.DrawLatex(xLabelPosX, xLabelPosY, xLabelString.c_str());
    yLabel.DrawLatex(yLabelPosX, yLabelPosY, yLabelString.c_str());
    title .DrawLatex(titlePosX,   titlePosY, titleString.c_str());

    gPad->SetMargin(0.15,0.15,0.15,0.15);
    gPad->SetGridx();
    gPad->SetGridy();

}

void draw(TH2D *histo, int type){
  // type 
  // [0] - theta phi
  // [1] - qqX
  // [2] - qqW
  // [3] - wY

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
  titlePosX = 0.62;    titlePosY = 0.92;

  titleString  = Form(" N_{events} = %.1e ", histo->GetEntries());

  if (type == 0){
    xLabelString = "#phi [Deg]";
    yLabelString = "#theta [Deg]";
  } 

  else if (type == 1){
    xLabelString = "x";
    yLabelString = "Q^{2} [GeV^{2}/c^{2}]";
  } 

  else if (type == 2){
    xLabelString = "W [GeV/c^{2}]";
    yLabelString = "Q^{2} [GeV^{2}/c^{2}]";
  } 

  else if (type == 3){
    xLabelString = "W [GeV/c^{2}]";
    yLabelString = "y";
  } 

  histo->Draw("colz");
  xLabel.DrawLatex(xLabelPosX, xLabelPosY, xLabelString.c_str());
  yLabel.DrawLatex(yLabelPosX, yLabelPosY, yLabelString.c_str());
  title .DrawLatex(titlePosX,   titlePosY, titleString.c_str());

  gPad->SetMargin(0.15,0.15,0.15,0.15);
  gPad->SetLogz();
  gPad->SetGridx();
  gPad->SetGridy();

}
