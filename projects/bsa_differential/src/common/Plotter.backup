#ifndef plotter_h
#define plotter_h

#include <iostream>

#include "Bins.h"
#include "Constants.h"
#include "Histograms.h"
#include "IntegratedHistograms.h"
#include "PIDHistograms.h"
#include "Fits.h"
#include "SignalBackground.h"
#include "Types.h"

#include "TCanvas.h"
#include "TF1.h"
#include "TH1.h"
#include "TLatex.h"
#include "TLine.h"
#include "TPad.h"
#include "TGaxis.h"

#include "CommonTools.h"

class Plotter {
 public:
  Plotter(){
    fOutputPath = "/volatile/clas12/dmriser/plots/asymmetry/"; 
  }

  void SetOutputPath(std::string p){
    fOutputPath = p; 
  }

  void PlotGridZX(Histos *fHistos, Fits *fFits, int fMesonIndex){

    Bins *fBins = Bins::GetInstance(fMesonIndex); 

    TCanvas *can = new TCanvas("can","",1200,900); 

    gStyle->SetOptStat(0); 
    gStyle->SetErrorX(0.00); 
    gStyle->SetLabelSize(0.00,"y");
    gStyle->SetTitleSize(0.0,"y");
    gStyle->SetTitleFont(22,"xyz");
    gStyle->SetLabelFont(22,"xyz");
    gStyle->SetTitleOffset(1.2,"y");

    gStyle->SetCanvasColor(0);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetCanvasBorderSize(0);
    can->cd(); 
 
    TGaxis *zAxis = new TGaxis(0.32, 0.1, 0.9, 0.1, fBins->GetZBins()->GetMin(), fBins->GetZBins()->GetMax(), 5, "");
    zAxis->SetTitle("z");
    zAxis->SetLabelSize(0.03);
    zAxis->Draw();

    TGaxis *xAxis = new TGaxis(0.1, 0.32, 0.1, 0.9, fBins->GetXBins()->GetMin(), fBins->GetXBins()->GetMax(), 5, "");
    xAxis->SetTitle("x");
    xAxis->SetLabelSize(0.03);
    xAxis->Draw();    

    TPad *pads[constants::MAX_BINS_Z][constants::MAX_BINS_X];
    
    int nx = fBins->GetXBins()->GetNumber(); 
    int nz = fBins->GetZBins()->GetNumber(); 

    double padding = 0.01; 
    double xstop   = 0.9; 
    double xstart  = 0.18; 
    double ystop   = 0.9; 
    double ystart  = 0.18; 
    double xwidth  = ((xstop-xstart) - (nz-1)*padding)/(1+nz); 
    double ywidth  = ((ystop-ystart) - (nx-1)*padding)/(1+nx); 

    for (int x=0; x<nz+1; x++){
      for (int y=0; y<nx+1; y++){
	std::string padTitle(Form("pad_%d%d",x,y)); 
	
	double padStartX = xstart + (x)*xwidth + (x)*padding; 
	double padStopX  = padStartX + xwidth; 
	
	double padStartY = ystart + (y)*ywidth + (y)*padding; 
	double padStopY  = padStartY + ywidth; 

	can->cd(); 
	pads[y][x] = new TPad(padTitle.c_str(), padTitle.c_str(), padStartX, padStartY, padStopX, padStopY); 
	pads[y][x]->Draw(); 
	pads[y][x]->cd(); 
	pads[y][x]->SetGrid(); 
	pads[y][x]->SetMargin(0.01, 0.01, 0.01, 0.01);

	int color = kBlack; 
	if (x == 0)          { color = 99; }
	else if (y == 0)     { color = 55; }
	if (x == 0 && y == 0){ color = 77; }
	fHistos->h1_asym[y][x][0]->SetLineColor(color);
 	fHistos->h1_asym[y][x][0]->SetLineWidth(2);
	fHistos->h1_asym[y][x][0]->SetMarkerColor(color);
	fHistos->h1_asym[y][x][0]->SetMarkerStyle(24);
	fHistos->h1_asym[y][x][0]->SetMarkerSize(1);
	fHistos->h1_asym[y][x][0]->SetMinimum(-0.1);
	fHistos->h1_asym[y][x][0]->SetMaximum(0.1);

	fHistos->h1_asym[y][x][0]->Draw("pe");

	fFits->fit_asym[y][x][0]->SetLineColor(color);
	fFits->fit_asym[y][x][0]->Draw("same");
      }
    }

    can->cd(); 

    TLatex title; 
    title.SetNDC();
    title.SetTextSize(0.03); 
    title.DrawLatex(0.4, 0.95, Form("%s Asymmetry", constants::Names::latex[fMesonIndex].c_str())); 

    title.SetTextSize(0.02); 
    title.DrawLatex(0.05, 0.12, "#color[99]{Integrated z}"); 
    title.DrawLatex(0.05, 0.09, "#color[55]{Integrated x}"); 
    title.DrawLatex(0.05, 0.06, "#color[77]{Integrated both}"); 
    
    can->Print(Form("%sgrid_z_x_%s_%s.png",fOutputPath.c_str(), constants::Names::mesons[fMesonIndex].c_str(), fHistos->GetName().c_str())); 
  }

  void PlotGridZPt(Histos *fHistos, Fits *fFits, int fMesonIndex){

    Bins *fBins = Bins::GetInstance(fMesonIndex); 

    TCanvas *can = new TCanvas("can","",1200,900); 

    gStyle->SetOptStat(0); 
    gStyle->SetErrorX(0.00); 
    gStyle->SetLabelSize(0.00,"y");
    gStyle->SetTitleSize(0.0,"y");
    gStyle->SetTitleFont(22,"xyz");
    gStyle->SetLabelFont(22,"xyz");
    gStyle->SetTitleOffset(1.2,"y");

    gStyle->SetCanvasColor(0);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetCanvasBorderSize(0);
    can->cd(); 
 
    TGaxis *zAxis = new TGaxis(0.32, 0.1, 0.9, 0.1, fBins->GetZBins()->GetMin(), fBins->GetZBins()->GetMax(), 5, "");
    zAxis->SetTitle("z");
    zAxis->SetLabelSize(0.03);
    zAxis->Draw();

    TGaxis *ptAxis = new TGaxis(0.1, 0.32, 0.1, 0.9, fBins->GetPtBins()->GetMin(), fBins->GetPtBins()->GetMax(), 5, "");
    ptAxis->SetTitle("P_{T} (GeV/c)");
    ptAxis->SetLabelSize(0.03);
    ptAxis->Draw();    
 
    TPad *pads[constants::MAX_BINS_Z][constants::MAX_BINS_PT];
    
    int npt = fBins->GetPtBins()->GetNumber(); 
    int nz  = fBins->GetZBins()->GetNumber(); 

    double padding = 0.01; 
    double xstop   = 0.9; 
    double xstart  = 0.18; 
    double ystop   = 0.9; 
    double ystart  = 0.18; 
    double xwidth  = ((xstop-xstart) - (nz-1)*padding)/(1+nz); 
    double ywidth  = ((ystop-ystart) - (npt-1)*padding)/(1+npt); 

    for (int x=0; x<nz+1; x++){
      for (int y=0; y<npt+1; y++){
	std::string padTitle(Form("pad_%d%d",x,y)); 
	
	double padStartX = xstart + (x)*xwidth + (x)*padding; 
	double padStopX  = padStartX + xwidth; 
	
	double padStartY = ystart + (y)*ywidth + (y)*padding; 
	double padStopY  = padStartY + ywidth; 

	can->cd(); 
	pads[y][x] = new TPad(padTitle.c_str(), padTitle.c_str(), padStartX, padStartY, padStopX, padStopY); 
	pads[y][x]->Draw(); 
	pads[y][x]->cd(); 
	pads[y][x]->SetGrid(); 
	pads[y][x]->SetMargin(0.01, 0.01, 0.01, 0.01);

	int color = kBlack; 
	if (x == 0)          { color = 99; }
	else if (y == 0)     { color = 55; }
	if (x == 0 && y == 0){ color = 77; }
	fHistos->h1_asym[0][x][y]->SetLineColor(color);
 	fHistos->h1_asym[0][x][y]->SetLineWidth(2);
	fHistos->h1_asym[0][x][y]->SetMarkerColor(color);
	fHistos->h1_asym[0][x][y]->SetMarkerStyle(24);
	fHistos->h1_asym[0][x][y]->SetMarkerSize(1);
	fHistos->h1_asym[0][x][y]->SetMinimum(-0.1);
	fHistos->h1_asym[0][x][y]->SetMaximum(0.1);

	fHistos->h1_asym[0][x][y]->Draw("pe");

	fFits->fit_asym[0][x][y]->SetLineColor(color);
	fFits->fit_asym[0][x][y]->Draw("same");
      }
    }

    can->cd(); 

    TLatex title; 
    title.SetNDC();
    title.SetTextSize(0.03); 
    title.DrawLatex(0.4, 0.95, Form("%s Asymmetry", constants::Names::latex[fMesonIndex].c_str())); 

    title.SetTextSize(0.02); 
    title.DrawLatex(0.05, 0.12, "#color[99]{Integrated P_{T}}"); 
    title.DrawLatex(0.05, 0.09, "#color[55]{Integrated x}"); 
    title.DrawLatex(0.05, 0.06, "#color[77]{Integrated both}"); 
    
    can->Print(Form("%sgrid_z_pt_%s_%s.png",fOutputPath.c_str(), constants::Names::mesons[fMesonIndex].c_str(), fHistos->GetName().c_str())); 
  }
  
 protected:
  std::string    fOutputPath; 
};

class IntegratedPlotter {
 public:
 IntegratedPlotter() {
    fOutputPath = "/volatile/clas12/dmriser/plots/asymmetry/"; 
  }

 void Add(IntegratedHistos *h, int index){
   if (index < constants::NMESON){
     fHistos[index]     = h; 
     fMesonIndex[index] = index; 
   }
 }

 void CompareTwo(IntegratedHistos *h1, IntegratedHistos *h2, std::string name1, std::string name2){
    TCanvas *can = new TCanvas("can","",1200,400); 
    can->Divide(3,1);     

    gPad->SetMargin(0.15, 0.15, 0.15, 0.15); 
    gStyle->SetOptStat(0); 
    gStyle->SetErrorX(0.00); 
    gStyle->SetLabelSize(0.00,"y");
    gStyle->SetTitleSize(0.0,"y");
    gStyle->SetTitleFont(22,"xyz");
    gStyle->SetLabelFont(22,"xyz");
    gStyle->SetTitleOffset(1.2,"y");

    gStyle->SetCanvasColor(0);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetCanvasBorderSize(0);

    int colors[2] = {55, 99};

    can->cd(1); 
    h1->h1_x[0][0]->SetFillColor(colors[0]);
    h1->h1_x[0][0]->SetLineColor(colors[0]);
    h1->h1_x[0][0]->SetLineWidth(2);
    h1->h1_x[0][0]->SetMarkerColor(colors[0]);
    h1->h1_x[0][0]->SetMarkerStyle(8);
    h1->h1_x[0][0]->SetMarkerSize(2);
    h1->h1_x[0][0]->SetMinimum(-0.08);
    h1->h1_x[0][0]->SetMaximum(0.08);
    h1->h1_x[0][0]->Draw("pesame");

    h2->h1_x[0][0]->SetFillColor(colors[1]);
    h2->h1_x[0][0]->SetLineColor(colors[1]);
    h2->h1_x[0][0]->SetLineWidth(2);
    h2->h1_x[0][0]->SetMarkerColor(colors[1]);
    h2->h1_x[0][0]->SetMarkerStyle(8);
    h2->h1_x[0][0]->SetMarkerSize(2);
    h2->h1_x[0][0]->SetMinimum(-0.08);
    h2->h1_x[0][0]->SetMaximum(0.08);
    h2->h1_x[0][0]->Draw("pesame");
    gPad->SetGrid();

    can->cd(2); 
    h1->h1_z[0][0]->SetFillColor(colors[0]);
    h1->h1_z[0][0]->SetLineColor(colors[0]);
    h1->h1_z[0][0]->SetLineWidth(2);
    h1->h1_z[0][0]->SetMarkerColor(colors[0]);
    h1->h1_z[0][0]->SetMarkerStyle(8);
    h1->h1_z[0][0]->SetMarkerSize(2);
    h1->h1_z[0][0]->SetMinimum(-0.08);
    h1->h1_z[0][0]->SetMaximum(0.08);
    h1->h1_z[0][0]->Draw("pesame");

    h2->h1_z[0][0]->SetFillColor(colors[1]);
    h2->h1_z[0][0]->SetLineColor(colors[1]);
    h2->h1_z[0][0]->SetLineWidth(2);
    h2->h1_z[0][0]->SetMarkerColor(colors[1]);
    h2->h1_z[0][0]->SetMarkerStyle(8);
    h2->h1_z[0][0]->SetMarkerSize(2);
    h2->h1_z[0][0]->SetMinimum(-0.08);
    h2->h1_z[0][0]->SetMaximum(0.08);
    h2->h1_z[0][0]->Draw("pesame");
    gPad->SetGrid();

    can->cd(3); 
    h1->h1_pt[0][0]->SetFillColor(colors[0]);
    h1->h1_pt[0][0]->SetLineColor(colors[0]);
    h1->h1_pt[0][0]->SetLineWidth(2);
    h1->h1_pt[0][0]->SetMarkerColor(colors[0]);
    h1->h1_pt[0][0]->SetMarkerStyle(8);
    h1->h1_pt[0][0]->SetMarkerSize(2);
    h1->h1_pt[0][0]->SetMinimum(-0.08);
    h1->h1_pt[0][0]->SetMaximum(0.08);
    h1->h1_pt[0][0]->Draw("pesame");

    h2->h1_pt[0][0]->SetFillColor(colors[1]);
    h2->h1_pt[0][0]->SetLineColor(colors[1]);
    h2->h1_pt[0][0]->SetLineWidth(2);
    h2->h1_pt[0][0]->SetMarkerColor(colors[1]);
    h2->h1_pt[0][0]->SetMarkerStyle(8);
    h2->h1_pt[0][0]->SetMarkerSize(2);
    h2->h1_pt[0][0]->SetMinimum(-0.08);
    h2->h1_pt[0][0]->SetMaximum(0.08);
    h2->h1_pt[0][0]->Draw("pesame");
    gPad->SetGrid();

    TLatex title; 
    title.SetNDC();
    title.SetTextSize(0.03); 

    title.DrawLatex(0.46, 0.87, "A_{LU}^{sin #phi}"); 

    can->Print(Form("%scompare_%s_%s.png",fOutputPath.c_str(), name1.c_str(), name2.c_str())); 
 }

  void PlotSummary(){
    TCanvas *can = new TCanvas("can","",1200,900); 
    can->Divide(4, 3); 

    TLatex title; 
    title.SetNDC(); 
    title.SetTextSize(0.038); 

    int markerStyle = 26;
    //    int color[constants::NMESON] = {55, 77, 90, 99};
    int color[constants::NMESON] = {kBlack, kBlack, kBlack, kBlack};

 
    TPad *pads[3][4];

    int ncol = 4; 
    int nrow = 3;

    double padding = 0.025; 
    double xstop   = 0.9; 
    double xstart  = 0.1; 
    double ystop   = 0.9; 
    double ystart  = 0.1; 
    double xwidth  = ((xstop-xstart) - ncol*padding)/(ncol); 
    double ywidth  = ((ystop-ystart) - nrow*padding)/(nrow); 


    Bins *bins[4];
    bins[0] = Bins::GetInstance(0);
    bins[1] = Bins::GetInstance(1);
    bins[2] = Bins::GetInstance(2);
    bins[3] = Bins::GetInstance(3);
 
    for(int x=0; x<ncol; x++){
      std::string padTitle(Form("pad_0%d",x)); 
      
      double padStartX = xstart + x*xwidth + x*padding; 
      double padStopX  = padStartX + xwidth; 
      
      double padStartY = ystart; 
      double padStopY  = padStartY + ywidth; 

      TLine xline(bins[x]->GetXBins()->GetMin(), 0.0, bins[x]->GetXBins()->GetMax(), 0.0); 
      xline.SetLineColor(kBlack); 
      xline.SetLineStyle(8);
      xline.SetLineWidth(2);       

      TLine zline(bins[x]->GetZBins()->GetMin(), 0.0, bins[x]->GetZBins()->GetMax(), 0.0); 
      zline.SetLineColor(kBlack); 
      zline.SetLineStyle(8);
      zline.SetLineWidth(2);       

      TLine ptline(bins[x]->GetPtBins()->GetMin(), 0.0, bins[x]->GetPtBins()->GetMax(), 0.0); 
      ptline.SetLineColor(kBlack); 
      ptline.SetLineStyle(8);
      ptline.SetLineWidth(2);       
      
      can->cd(); 
      pads[0][x] = new TPad(padTitle.c_str(), padTitle.c_str(), padStartX, padStartY, padStopX, padStopY); 
      pads[0][x]->Draw(); 
      pads[0][x]->cd(); 
      pads[0][x]->SetGrid(); 
      pads[0][x]->SetMargin(0.01, 0.01, 0.01, 0.01);

      fHistos[x]->h1_x[0][0]->SetLineColor(color[x]); 
      fHistos[x]->h1_x[0][0]->SetFillColor(color[x]); 
      fHistos[x]->h1_x[0][0]->SetMarkerColor(color[x]); 
      fHistos[x]->h1_x[0][0]->SetMarkerStyle(markerStyle); 
      fHistos[x]->h1_x[0][0]->SetMarkerSize(1.0); 
      fHistos[x]->h1_x[0][0]->Draw("pesame"); 
      xline.Draw("same"); 

      padTitle = std::string(Form("pad_1%d",x)); 
      padStartY = ystart + ywidth + padding; 
      padStopY  = padStartY + ywidth; 

      can->cd(); 
      pads[1][x] = new TPad(padTitle.c_str(), padTitle.c_str(), padStartX, padStartY, padStopX, padStopY); 
      pads[1][x]->Draw(); 
      pads[1][x]->cd(); 
      pads[1][x]->SetGrid(); 
      pads[1][x]->SetMargin(0.01, 0.01, 0.01, 0.01);

      fHistos[x]->h1_z[0][0]->SetLineColor(color[x]); 
      fHistos[x]->h1_z[0][0]->SetFillColor(color[x]); 
      fHistos[x]->h1_z[0][0]->SetMarkerColor(color[x]); 
      fHistos[x]->h1_z[0][0]->SetMarkerStyle(markerStyle); 
      fHistos[x]->h1_z[0][0]->SetMarkerSize(1.0); 
      fHistos[x]->h1_z[0][0]->Draw("pesame"); 
      zline.Draw("same"); 

      padTitle = std::string(Form("pad_1%d",x)); 
      padStartY = ystart + 2*ywidth + 2*padding; 
      padStopY  = padStartY + ywidth; 

      can->cd(); 
      pads[2][x] = new TPad(padTitle.c_str(), padTitle.c_str(), padStartX, padStartY, padStopX, padStopY); 
      pads[2][x]->Draw(); 
      pads[2][x]->cd(); 
      pads[2][x]->SetGrid(); 
      pads[2][x]->SetMargin(0.01, 0.01, 0.01, 0.01);

      fHistos[x]->h1_pt[0][0]->SetLineColor(color[x]); 
      fHistos[x]->h1_pt[0][0]->SetFillColor(color[x]); 
      fHistos[x]->h1_pt[0][0]->SetMarkerColor(color[x]); 
      fHistos[x]->h1_pt[0][0]->SetMarkerStyle(markerStyle); 
      fHistos[x]->h1_pt[0][0]->SetMarkerSize(1.0); 
      fHistos[x]->h1_pt[0][0]->Draw("pesame"); 
      ptline.Draw("same"); 

      can->cd(); 
      title.DrawLatex(0.2 + 0.2*x, 0.9, Form("%s", constants::Names::latex[x].c_str())); 

      if (x == 0){
	title.DrawLatex(0.04, 0.75, "x");
	title.DrawLatex(0.04, 0.45, "z");
	title.DrawLatex(0.04, 0.18, "P_{T}");
      }

    }

    can->Print(Form("%sintegrated_summary.png",fOutputPath.c_str())); 
  }
 
  void PlotX(){
    TCanvas *can = new TCanvas("can","",1200,900); 
    
    gPad->SetMargin(0.15, 0.15, 0.15, 0.15); 
    gStyle->SetOptStat(0); 
    gStyle->SetErrorX(0.00); 
    gStyle->SetLabelSize(0.00,"y");
    gStyle->SetTitleSize(0.0,"y");
    gStyle->SetTitleFont(22,"xyz");
    gStyle->SetLabelFont(22,"xyz");
    gStyle->SetTitleOffset(1.2,"y");

    gStyle->SetCanvasColor(0);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetCanvasBorderSize(0);
    can->cd(); 

    int colors[constants::NMESON] = {99, 82, 71, 55};
    for (int m=0; m<constants::NMESON; m++){
      fHistos[m]->h1_x[0][0]->SetFillColor(colors[m]);
      fHistos[m]->h1_x[0][0]->SetLineColor(colors[m]);
      fHistos[m]->h1_x[0][0]->SetLineWidth(2);
      fHistos[m]->h1_x[0][0]->SetMarkerColor(colors[m]);
      fHistos[m]->h1_x[0][0]->SetMarkerStyle(8);
      fHistos[m]->h1_x[0][0]->SetMarkerSize(2);
      fHistos[m]->h1_x[0][0]->SetMinimum(-0.08);
      fHistos[m]->h1_x[0][0]->SetMaximum(0.08);
      fHistos[m]->h1_x[0][0]->Draw("pesame");
    }

    gPad->SetGrid();

    TLatex title; 
    title.SetNDC();
    title.SetTextSize(0.03); 

    title.DrawLatex(0.2, 0.87, "A_{LU}^{sin #phi}"); 
    for(int m=0; m<constants::NMESON; m++)
      title.DrawLatex(0.55 + 0.03*m, 0.87, Form("#color[%d]{%s}", colors[m], constants::Names::latex[fMesonIndex[m]].c_str())); 

    title.DrawLatex(0.47, 0.05, "x_{Bjorken}"); 
    title.SetTextAngle(90.0);
    title.DrawLatex(0.07, 0.47, "A_{LU}^{sin(#phi)}"); 

    can->Print(Form("%sx.png",fOutputPath.c_str())); 
  }

 
  void PlotZ(){
    TCanvas *can = new TCanvas("can","",1200,900); 
    
    gPad->SetMargin(0.15, 0.15, 0.15, 0.15); 
    gStyle->SetOptStat(0); 
    gStyle->SetErrorX(0.00); 
    gStyle->SetLabelSize(0.00,"y");
    gStyle->SetTitleSize(0.0,"y");
    gStyle->SetTitleFont(22,"xyz");
    gStyle->SetLabelFont(22,"xyz");
    gStyle->SetTitleOffset(1.2,"y");

    gStyle->SetCanvasColor(0);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetCanvasBorderSize(0);
    can->cd(); 

    int colors[constants::NMESON] = {99, 82, 71, 55};
    for (int m=0; m<constants::NMESON; m++){
      fHistos[m]->h1_z[0][0]->SetFillColor(colors[m]);
      fHistos[m]->h1_z[0][0]->SetLineColor(colors[m]);
      fHistos[m]->h1_z[0][0]->SetLineWidth(2);
      fHistos[m]->h1_z[0][0]->SetMarkerColor(colors[m]);
      fHistos[m]->h1_z[0][0]->SetMarkerStyle(8);
      fHistos[m]->h1_z[0][0]->SetMarkerSize(2);
      fHistos[m]->h1_z[0][0]->SetMinimum(-0.08);
      fHistos[m]->h1_z[0][0]->SetMaximum(0.08);
      fHistos[m]->h1_z[0][0]->Draw("pesame");
    }

    gPad->SetGrid();

    TLatex title; 
    title.SetNDC();
    title.SetTextSize(0.03); 

    title.DrawLatex(0.2, 0.87, "A_{LU}^{sin #phi}"); 
    for(int m=0; m<constants::NMESON; m++)
      title.DrawLatex(0.55 + 0.03*m, 0.87, Form("#color[%d]{%s}", colors[m], constants::Names::latex[fMesonIndex[m]].c_str())); 

    title.DrawLatex(0.47, 0.05, "z"); 
    title.SetTextAngle(90.0);
    title.DrawLatex(0.07, 0.47, "A_{LU}^{sin(#phi)}"); 

    can->Print(Form("%sz.png",fOutputPath.c_str())); 
  }


  void PlotPt(){
    TCanvas *can = new TCanvas("can","",1200,900); 
    
    gPad->SetMargin(0.15, 0.15, 0.15, 0.15); 
    gStyle->SetOptStat(0); 
    gStyle->SetErrorX(0.00); 
    gStyle->SetLabelSize(0.00,"y");
    gStyle->SetTitleSize(0.0,"y");
    gStyle->SetTitleFont(22,"xyz");
    gStyle->SetLabelFont(22,"xyz");
    gStyle->SetTitleOffset(1.2,"y");

    gStyle->SetCanvasColor(0);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetCanvasBorderSize(0);
    can->cd(); 

    int colors[constants::NMESON] = {99, 92, 85, 78};
    for (int m=0; m<constants::NMESON; m++){
      fHistos[m]->h1_pt[0][0]->SetFillColor(colors[m]);
      fHistos[m]->h1_pt[0][0]->SetLineColor(colors[m]);
      fHistos[m]->h1_pt[0][0]->SetLineWidth(2);
      fHistos[m]->h1_pt[0][0]->SetMarkerColor(colors[m]);
      fHistos[m]->h1_pt[0][0]->SetMarkerStyle(8);
      fHistos[m]->h1_pt[0][0]->SetMarkerSize(2);
      fHistos[m]->h1_pt[0][0]->SetMinimum(-0.08);
      fHistos[m]->h1_pt[0][0]->SetMaximum(0.08);
      fHistos[m]->h1_pt[0][0]->Draw("pesame");
    }

    gPad->SetGrid();

    TLatex title; 
    title.SetNDC();
    title.SetTextSize(0.03); 

    title.DrawLatex(0.2, 0.87, "A_{LU}^{sin #phi}"); 
    for(int m=0; m<constants::NMESON; m++)
      title.DrawLatex(0.55 + 0.03*m, 0.87, Form("#color[%d]{%s}", colors[m], constants::Names::latex[fMesonIndex[m]].c_str())); 

    title.DrawLatex(0.47, 0.05, "P_{T} (GeV/c)"); 
    title.SetTextAngle(90.0);
    title.DrawLatex(0.07, 0.47, "A_{LU}^{sin(#phi)}"); 

    can->Print(Form("%spt.png",fOutputPath.c_str())); 
  }

  void PlotXByZ(int index){
    TCanvas *can = new TCanvas("can","",1200,900); 
    
    Bins  *bins = Bins::GetInstance(index); 

    gPad->SetMargin(0.15, 0.15, 0.15, 0.15); 
    gStyle->SetOptStat(0); 
    gStyle->SetErrorX(0.00); 
    gStyle->SetLabelSize(0.00,"y");
    gStyle->SetTitleSize(0.0,"y");
    gStyle->SetTitleFont(22,"xyz");
    gStyle->SetLabelFont(22,"xyz");
    gStyle->SetTitleOffset(1.2,"y");

    gStyle->SetCanvasColor(0);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetCanvasBorderSize(0);
    can->cd(); 

    for (int b=1; b<bins->GetZBins()->GetNumber()+1; b++){
      fHistos[index]->h1_x[b][0]->SetFillColor(50 + b*5);
      fHistos[index]->h1_x[b][0]->SetLineColor(50 + b*5);
      fHistos[index]->h1_x[b][0]->SetLineWidth(2);
      fHistos[index]->h1_x[b][0]->SetMarkerColor(50 + b*5);
      fHistos[index]->h1_x[b][0]->SetMarkerStyle(8);
      fHistos[index]->h1_x[b][0]->SetMarkerSize(2);
      fHistos[index]->h1_x[b][0]->SetMinimum(-0.1);
      fHistos[index]->h1_x[b][0]->SetMaximum(0.1);
      fHistos[index]->h1_x[b][0]->Draw("pesame");
    }

    gPad->SetGrid();

    TLatex title; 
    title.SetNDC();
    title.SetTextSize(0.03); 

    title.DrawLatex(0.2, 0.87, "A_{LU}^{sin #phi}"); 

    for (int b=0; b<bins->GetZBins()->GetNumber(); b++){
      title.DrawLatex(0.55 + b*0.05, 0.87, Form("#color[%d]{z =  %.3f}", 50 + (b+1)*5, bins->GetZBins()->GetBin(b).GetCenter())); 
    }

    title.DrawLatex(0.47, 0.05, "x_{Bjorken}"); 
    title.SetTextAngle(90.0);
    title.DrawLatex(0.07, 0.47, "A_{LU}^{sin(#phi)}"); 

    can->Print(Form("%sx_by_z_%s.png",fOutputPath.c_str(), constants::Names::mesons[index].c_str())); 
  }

 protected:
  IntegratedHistos *fHistos[constants::NMESON]; 
  std::string       fOutputPath; 
  int               fMesonIndex[constants::NMESON]; 
  

};

class PidPlotter {
 public: 
 PidPlotter() {
    fOutputPath = std::string("/volatile/clas12/dmriser/plots/asymmetry/");
    Global::Visualization::SetCustomPalette(); 
  }

  void SetOutputPath(std::string p){
    fOutputPath = p; 
  }

  // ============================================= 
  void PlotGridZPt(PidHistos *fHistos, int fMesonIndex){

    Bins *fBins = Bins::GetInstance(fMesonIndex); 

    TCanvas *can = new TCanvas("can","",1200,900); 

    gStyle->SetOptStat(0); 
    gStyle->SetErrorX(0.00); 
    gStyle->SetLabelSize(0.00,"y");
    gStyle->SetTitleSize(0.0,"y");
    gStyle->SetTitleFont(22,"xyz");
    gStyle->SetLabelFont(22,"xyz");
    gStyle->SetTitleOffset(1.2,"y");

    gStyle->SetCanvasColor(0);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetCanvasBorderSize(0);
    can->cd(); 
 
    TGaxis *zAxis = new TGaxis(0.32, 0.1, 0.9, 0.1, fBins->GetZBins()->GetMin(), fBins->GetZBins()->GetMax(), 5, "");
    zAxis->SetTitle("z");
    zAxis->SetLabelSize(0.03);
    zAxis->Draw();

    TGaxis *ptAxis = new TGaxis(0.1, 0.32, 0.1, 0.9, fBins->GetPtBins()->GetMin(), fBins->GetPtBins()->GetMax(), 5, "");
    ptAxis->SetTitle("P_{T} (GeV/c)");
    ptAxis->SetLabelSize(0.03);
    ptAxis->Draw();    
 
    TPad *pads[constants::MAX_BINS_Z][constants::MAX_BINS_PT];
    
    int npt = fBins->GetPtBins()->GetNumber(); 
    int nz  = fBins->GetZBins()->GetNumber(); 

    double padding = 0.01; 
    double xstop   = 0.9; 
    double xstart  = 0.18; 
    double ystop   = 0.9; 
    double ystart  = 0.18; 
    double xwidth  = ((xstop-xstart) - (nz-1)*padding)/(1+nz); 
    double ywidth  = ((ystop-ystart) - (npt-1)*padding)/(1+npt); 

    for (int x=0; x<nz+1; x++){
      for (int y=0; y<npt+1; y++){
	std::string padTitle(Form("pad_%d%d",x,y)); 
	
	double padStartX = xstart + (x)*xwidth + (x)*padding; 
	double padStopX  = padStartX + xwidth; 
	
	double padStartY = ystart + (y)*ywidth + (y)*padding; 
	double padStopY  = padStartY + ywidth; 

	can->cd(); 
	pads[y][x] = new TPad(padTitle.c_str(), padTitle.c_str(), padStartX, padStartY, padStopX, padStopY); 
	pads[y][x]->Draw(); 
	pads[y][x]->cd(); 
	pads[y][x]->SetGrid(); 
	pads[y][x]->SetLogz(); 
	pads[y][x]->SetMargin(0.01, 0.01, 0.01, 0.01);


	fHistos->h2_p_b[0][x][y]->Draw("colz");
      }
    }

    can->cd(); 

    TLatex title; 
    title.SetNDC();
    title.SetTextSize(0.03); 
    title.DrawLatex(0.4, 0.95, Form("%s Binned PID Plots", constants::Names::latex[fMesonIndex].c_str())); 

    can->Print(Form("%sgrid_pid_z_pt_%s_%s.png",fOutputPath.c_str(), constants::Names::mesons[fMesonIndex].c_str(), fHistos->GetName().c_str())); 
  }

  void PlotGridZX(PidHistos *fHistos, int fMesonIndex){
    Bins *fBins = Bins::GetInstance(fMesonIndex); 

    TCanvas *can = new TCanvas("can","",1200,900); 

    gStyle->SetOptStat(0); 
    gStyle->SetErrorX(0.00); 
    gStyle->SetLabelSize(0.00,"y");
    gStyle->SetTitleSize(0.0,"y");
    gStyle->SetTitleFont(22,"xyz");
    gStyle->SetLabelFont(22,"xyz");
    gStyle->SetTitleOffset(1.2,"y");

    gStyle->SetCanvasColor(0);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetCanvasBorderSize(0);
    can->cd(); 
 
    TGaxis *zAxis = new TGaxis(0.32, 0.1, 0.9, 0.1, fBins->GetZBins()->GetMin(), fBins->GetZBins()->GetMax(), 5, "");
    zAxis->SetTitle("z");
    zAxis->SetLabelSize(0.03);
    zAxis->Draw();

    TGaxis *xAxis = new TGaxis(0.1, 0.32, 0.1, 0.9, fBins->GetXBins()->GetMin(), fBins->GetXBins()->GetMax(), 5, "");
    xAxis->SetTitle("x");
    xAxis->SetLabelSize(0.03);
    xAxis->Draw();    

    TPad *pads[constants::MAX_BINS_Z][constants::MAX_BINS_X];
    
    int nx = fBins->GetXBins()->GetNumber(); 
    int nz = fBins->GetZBins()->GetNumber(); 

    double padding = 0.01; 
    double xstop   = 0.9; 
    double xstart  = 0.18; 
    double ystop   = 0.9; 
    double ystart  = 0.18; 
    double xwidth  = ((xstop-xstart) - (nz-1)*padding)/(1+nz); 
    double ywidth  = ((ystop-ystart) - (nx-1)*padding)/(1+nx); 

    for (int x=0; x<nz+1; x++){
      for (int y=0; y<nx+1; y++){
	std::string padTitle(Form("pad_%d%d",x,y)); 
	
	double padStartX = xstart + (x)*xwidth + (x)*padding; 
	double padStopX  = padStartX + xwidth; 
	
	double padStartY = ystart + (y)*ywidth + (y)*padding; 
	double padStopY  = padStartY + ywidth; 

	can->cd(); 
	pads[y][x] = new TPad(padTitle.c_str(), padTitle.c_str(), padStartX, padStartY, padStopX, padStopY); 
	pads[y][x]->Draw(); 
	pads[y][x]->cd(); 
	pads[y][x]->SetGrid(); 
	pads[y][x]->SetLogz(); 
	pads[y][x]->SetMargin(0.01, 0.01, 0.01, 0.01);

	fHistos->h2_p_b[y][x][0]->Draw("colz"); 
      }
    }

    can->cd(); 

    TLatex title; 
    title.SetNDC();
    title.SetTextSize(0.03); 
    title.DrawLatex(0.4, 0.95, Form("%s Binned PID Plots", constants::Names::latex[fMesonIndex].c_str())); 
    
    can->Print(Form("%sgrid_pid_z_x_%s_%s.png",fOutputPath.c_str(), constants::Names::mesons[fMesonIndex].c_str(), fHistos->GetName().c_str())); 
  }

  void PlotGridZXWithFits(PidHistos *fHistos, SignalBackgroundFitter *fitter, int fMesonIndex){
    Bins *fBins = Bins::GetInstance(fMesonIndex); 

    TCanvas *can = new TCanvas("can","",1200,900); 

    gStyle->SetOptStat(0); 
    gStyle->SetErrorX(0.00); 
    gStyle->SetLabelSize(0.00,"y");
    gStyle->SetTitleSize(0.0,"y");
    gStyle->SetTitleFont(22,"xyz");
    gStyle->SetLabelFont(22,"xyz");
    gStyle->SetTitleOffset(1.2,"y");

    gStyle->SetCanvasColor(0);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetCanvasBorderSize(0);
    can->cd(); 
 
    TGaxis *zAxis = new TGaxis(0.32, 0.1, 0.9, 0.1, fBins->GetZBins()->GetMin(), fBins->GetZBins()->GetMax(), 5, "");
    zAxis->SetTitle("z");
    zAxis->SetLabelSize(0.03);
    zAxis->Draw();

    TGaxis *xAxis = new TGaxis(0.1, 0.32, 0.1, 0.9, fBins->GetXBins()->GetMin(), fBins->GetXBins()->GetMax(), 5, "");
    xAxis->SetTitle("x");
    xAxis->SetLabelSize(0.03);
    xAxis->Draw();    

    TPad *pads[constants::MAX_BINS_Z][constants::MAX_BINS_X];
    
    int nx = fBins->GetXBins()->GetNumber(); 
    int nz = fBins->GetZBins()->GetNumber(); 

    double padding = 0.01; 
    double xstop   = 0.9; 
    double xstart  = 0.18; 
    double ystop   = 0.9; 
    double ystart  = 0.18; 
    double xwidth  = ((xstop-xstart) - (nz-1)*padding)/(1+nz); 
    double ywidth  = ((ystop-ystart) - (nx-1)*padding)/(1+nx); 

    TLatex title; 
    title.SetNDC();
    title.SetTextSize(0.15); 

    for (int x=0; x<nz+1; x++){
      for (int y=0; y<nx+1; y++){
	std::string padTitle(Form("pad_%d%d",x,y)); 
	
	double padStartX = xstart + (x)*xwidth + (x)*padding; 
	double padStopX  = padStartX + xwidth; 
	
	double padStartY = ystart + (y)*ywidth + (y)*padding; 
	double padStopY  = padStartY + ywidth; 

	can->cd(); 
	pads[y][x] = new TPad(padTitle.c_str(), padTitle.c_str(), padStartX, padStartY, padStopX, padStopY); 
	pads[y][x]->Draw(); 
	pads[y][x]->cd(); 
	pads[y][x]->SetMargin(0.01, 0.01, 0.01, 0.01);

	fHistos->h1_tof_mass[y][x][0]->Draw("histw"); 
	fitter->fit_p[y][x][0]->SetLineColor(88); 
	fitter->fit_p[y][x][0]->SetLineWidth(0.6);
	fitter->fit_p[y][x][0]->Draw("same");
	fitter->fit_k[y][x][0]->SetLineColor(77); 
	fitter->fit_k[y][x][0]->SetLineWidth(0.6);
	fitter->fit_k[y][x][0]->Draw("same");
	fitter->fit_bg[y][x][0]->SetLineColor(55); 
	fitter->fit_bg[y][x][0]->SetLineWidth(0.6);
	fitter->fit_bg[y][x][0]->Draw("same");
	fitter->fit_tot[y][x][0]->SetLineColor(99); 
	fitter->fit_tot[y][x][0]->SetLineWidth(0.6);
	fitter->fit_tot[y][x][0]->Draw("same");

	title.DrawLatex(0.35, 0.82, Form("#chi^{2} = %.3f", fitter->fit_tot[y][x][0]->GetChisquare()/fitter->fit_tot[y][x][0]->GetNDF()));
      }
    }

    can->cd(); 

    title.SetTextSize(0.03); 
    title.DrawLatex(0.4, 0.95, Form("%s Binned PID Plots", constants::Names::latex[fMesonIndex].c_str())); 
    
    can->Print(Form("%sgrid_tofmass_z_x_%s_%s.pdf",fOutputPath.c_str(), constants::Names::mesons[fMesonIndex].c_str(), fHistos->GetName().c_str())); 
  }
  
 protected:
  std::string fOutputPath; 

};



#endif
