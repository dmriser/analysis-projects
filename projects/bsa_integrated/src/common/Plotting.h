#ifndef plotting_h 
#define plotting_h 

#include <iostream>
#include <vector>
#include <map>

// this project 
#include "PhiFits.h"
#include "PhiHistograms.h"
#include "PtAverager.h"
#include "MissingMassHistograms.h"
#include "KinematicHistograms.h"
#include "Config.h"
#include "ConfigLoader.h"
#include "ConfigLoader.cxx"
#include "Types.h"

// h22 libraries 
#include "CommonTools.h"
#include "DBins.h"
#include "StandardHistograms.h"

// root 
#include "TCanvas.h"
#include "TF1.h"
#include "TH1.h"
#include "TLatex.h"
#include "TLine.h"
#include "TStyle.h"

class Plotting {

public:

  static void Setup(){
    gStyle->SetOptFit(0);
    gStyle->SetOptStat(0);
    //    gStyle->SetOptTitle(0);
    gStyle->SetLabelSize(0.03,"xyz");
    gStyle->SetTitleSize(0.035,"xyz");
    gStyle->SetTitleFont(22,"xyz");
    gStyle->SetLabelFont(22,"xyz");
    gStyle->SetTitleOffset(1.2,"y");
  }

  static void ApplyStyle(TH1F *h, int style){

    // apply some common options 
    //    h->SetLabelSize(0.1, "x");
    //    h->SetLabelSize(0.1, "y");
    //    h->SetLabelFont(42); 
    //    h->GetXaxis()->SetNdivisions(502);
    //    h->GetYaxis()->SetNdivisions(502);

    h->GetXaxis()->SetTitleSize(0.05); 
    h->GetYaxis()->SetTitleSize(0.05); 


    if (style == styles::marker_black){
      h->SetMarkerColor(kBlack);
      h->SetMarkerStyle(8);
      h->SetLineColor(kBlack); 
      h->SetLineWidth(1); 
    }

    else if (style == styles::marker_red){
      h->SetMarkerColor(99);
      h->SetMarkerStyle(8);
      h->SetLineColor(99); 
      h->SetLineWidth(1); 
    }

    else if (style == styles::filled_red){
      h->SetLineColor(99);
      h->SetFillColorAlpha(99, 0.3);
    }

    else if (style == styles::filled_blue){
      h->SetLineColor(55);
      h->SetFillColorAlpha(55, 0.3);
    }

  }

  static void PlotOnePhiHisto(PhiHistos *h, 
			      std::string name, 
			      std::string x, 
			      std::string y,
			      std::string options, 
			      int plotStyle,
			      std::string pdf){

    // retrieve binning from histograms 
    std::map<int, DLineBins> bins = h->GetBins();
    
    // setup a reasonably sized canvas 
    TCanvas *canvas = new TCanvas("canvas","",1600,1200);
    canvas->Divide(4,4);
    canvas->Print(Form("%s[",pdf.c_str()));

    // setup a printer 
    TLatex *title = new TLatex();
    title->SetNDC();
    title->SetTextFont(42);
    title->SetTextSize(0.05);

    int index = 1;

    for(std::pair<int, DLineBins> p : bins){
      int currentAxis       = p.first; 
      DLineBins currentBins = p.second; 


      if (currentAxis != axis::phi){
	for(int b=0; b<currentBins.GetNumber(); b++){
	  canvas->cd(index);
	  canvas->SetMargin(0.2, 0.2, 0.2, 0.2);

	  h->histos[currentAxis][b]->SetTitle(Form("%s;%s;%s",name.c_str(),x.c_str(),y.c_str())); 

	  ApplyStyle(h->histos[currentAxis][b], plotStyle);
	  h->histos[currentAxis][b]->Draw(options.c_str());
	  index++; 
	  
	  
	  title->DrawLatex(0.4, 0.01, x.c_str());
	  title->SetTextAngle(90.0);
	  title->DrawLatex(0.01, 0.4, y.c_str());
	  title->SetTextAngle(0.0);
	  title->DrawLatex(0.14, 0.80, Form("%s #epsilon [%.2f,%.2f]", 
					    axisNames[currentAxis].c_str(), 
					    currentBins.GetBin(b).GetMin(), 
					    currentBins.GetBin(b).GetMax()));
	  

	  // we need to print a page 
	  if (index > 16){
	    index = 1; 
	    canvas->Print(pdf.c_str());
	    canvas->Clear(); 
	    canvas->Divide(4,4); 
	  }

	}
      }
    }

    canvas->Print(pdf.c_str());
    canvas->Print(Form("%s]",pdf.c_str()));
  }

  static void PlotTwoPhiHistos(PhiHistos *h1,
			       PhiHistos *h2,
			       std::string name, 
			       std::string x, 
			       std::string y,
			       std::string options1,
			       std::string options2,
			       int plotStyle1, 
			       int plotStyle2,
			       std::string pdf){
    
    // retrieve binning from histograms 
    std::map<int, DLineBins> bins = h1->GetBins();
    
    // setup a reasonably sized canvas 
    TCanvas *canvas = new TCanvas("canvas","",1600,1200);
    canvas->Divide(4,4);
    canvas->Print(Form("%s[",pdf.c_str()));

    // setup a printer 
    TLatex *title = new TLatex();
    title->SetNDC();
    title->SetTextFont(42);
    title->SetTextSize(0.05);

    int index = 1;

    for(std::pair<int, DLineBins> p : bins){
      int currentAxis       = p.first; 
      DLineBins currentBins = p.second; 


      if (currentAxis != axis::phi){
	for(int b=0; b<currentBins.GetNumber(); b++){
	  canvas->cd(index);
	  canvas->SetMargin(0.2, 0.2, 0.2, 0.2);

	  h1->histos[currentAxis][b]->SetTitle(Form("%s;%s;%s",name.c_str(),x.c_str(),y.c_str())); 
	  h2->histos[currentAxis][b]->SetTitle(Form("%s;%s;%s",name.c_str(),x.c_str(),y.c_str())); 

	  ApplyStyle(h1->histos[currentAxis][b], plotStyle1);
	  ApplyStyle(h2->histos[currentAxis][b], plotStyle2);
	  h1->histos[currentAxis][b]->Draw(options1.c_str());
	  h2->histos[currentAxis][b]->Draw(options2.c_str());
	  index++; 
	  

	  //	  title->DrawLatex(0.4, 0.01, x.c_str());
	  //	  title->SetTextAngle(90.0);
	  //	  title->DrawLatex(0.01, 0.4, y.c_str());
	  //	  title->SetTextAngle(0.0);
	  title->DrawLatex(0.14, 0.80, Form("%s #epsilon [%.2f,%.2f]", 
					    axisNames[currentAxis].c_str(), 
					    currentBins.GetBin(b).GetMin(), 
					    currentBins.GetBin(b).GetMax()));
	  
	  // we need to print a page 
	  if (index > 16){
	    index = 1; 
	    canvas->Print(pdf.c_str());
	    canvas->Clear(); 
	    canvas->Divide(4,4); 
	  }

	}
      }
    }

    canvas->Print(pdf.c_str());
    canvas->Print(Form("%s]",pdf.c_str()));
  }


  static void PlotOneKinematicHisto(KinematicHistos *h, 
				    std::string name, 
				    std::string y,
				    std::string options, 
				    int plotStyle,
				    std::string pdf){

    // retrieve binning from histograms 
    std::map<int, DLineBins> bins = h->GetBins();
    
    // setup a reasonably sized canvas 
    TCanvas *canvas = new TCanvas("canvas","",1600,500);
    canvas->Divide(4,1);
    canvas->Print(Form("%s[",pdf.c_str()));

    // setup a printer 
    TLatex *title = new TLatex();
    title->SetNDC();
    title->SetTextFont(42);
    title->SetTextSize(0.05);

    int index = 1; 
    for(std::pair<int, DLineBins> p : bins){
      int currentAxis       = p.first; 
      
      if (currentAxis != axis::phi){
	canvas->cd(index);
	canvas->SetMargin(0.2, 0.2, 0.2, 0.2);
	
	ApplyStyle(h->histos[currentAxis], plotStyle);
	h->histos[currentAxis]->Draw(options.c_str());
	index++; 	  
	
	title->DrawLatex(0.4, 0.01, axisNames[currentAxis].c_str());
	title->SetTextAngle(90.0);
	title->DrawLatex(0.01, 0.4, y.c_str());
	title->SetTextAngle(0.0);
	//	  title->DrawLatex(0.14, 0.80, Form("%s #epsilon [%.2f,%.2f]", 
	//					    axisNames[currentAxis].c_str(), 
	//					    currentBins.GetBin(b).GetMin(), 
	//					    currentBins.GetBin(b).GetMax()));
	
      }
    }
 
    canvas->Print(pdf.c_str());
    canvas->Print(Form("%s]",pdf.c_str()));
  }

  static void PlotAveragePt(PtAverager *ptAverage, 
			    Config config, 
			    std::string pdf){
    
    TCanvas *canvas = new TCanvas("canvas", "", 1600, 1200); 
    canvas->cd();

    TLatex *label = new TLatex(); 
    label->SetNDC();
    label->SetTextFont(42);
    label->SetTextSize(0.03);

    ptAverage->average->SetLineColor(kBlack);
    ptAverage->average->SetMarkerColor(kBlack);
    ptAverage->average->SetMarkerStyle(8);
    ptAverage->average->SetLineWidth(2);
    ptAverage->average->Draw("pe");

    label->DrawLatex(0.4, 0.95, Form("Configuration: %s",config.name.c_str()));
    label->DrawLatex(0.49, 0.02, "z");

    label->SetTextAngle(90.0);
    label->DrawLatex(0.02, 0.7, "< P_{T} >");

    canvas->Print(pdf.c_str());

  }

  static void PlotAsymmetryZ(KinematicHistos *h, 
			     Config config, 
			     std::string pdf){
    
    TCanvas *canvas = new TCanvas("canvas", "", 1600, 1200); 
    canvas->cd();

    TLatex *label = new TLatex(); 
    label->SetNDC();
    label->SetTextFont(42);
    label->SetTextSize(0.03);

    h->histos[axis::z]->SetLineColor(kBlack);
    h->histos[axis::z]->SetMarkerColor(kBlack);
    h->histos[axis::z]->SetMarkerStyle(8);
    h->histos[axis::z]->SetLineWidth(2);
    h->histos[axis::z]->Draw("pe");

    label->DrawLatex(0.4, 0.95, Form("Configuration: %s",config.name.c_str()));
    label->DrawLatex(0.49, 0.02, "z");

    label->SetTextAngle(90.0);
    label->DrawLatex(0.02, 0.7, "BSA");

    canvas->Print(pdf.c_str());

  }

  static void PlotOneMissingMassHisto(MissingMassHistos *h, 
				      float min, 
				      float max,
				      std::string name, 
				      std::string options, 
				      int plotStyle,
				      std::string pdf){

    // retrieve binning from histograms 
    std::map<int, DLineBins> bins = h->GetBins();
    
    // setup a reasonably sized canvas 
    TCanvas *canvas = new TCanvas("canvas","",1600,1200);
    canvas->Divide(4,4);
    canvas->Print(Form("%s[",pdf.c_str()));

    // setup a printer 
    TLatex *title = new TLatex();
    title->SetNDC();
    title->SetTextFont(42);
    title->SetTextSize(0.05);

    int index = 1;
    for(std::pair<int, DLineBins> p : bins){
      int currentAxis       = p.first; 
      DLineBins currentBins = p.second; 


      if (currentAxis != axis::phi){
	for(int b=0; b<currentBins.GetNumber(); b++){
	  canvas->cd(index);
	  canvas->SetMargin(0.2, 0.2, 0.2, 0.2);

	  ApplyStyle(h->zoom[currentAxis][b], plotStyle);
	  h->zoom[currentAxis][b]->Draw(options.c_str());

	  index++; 
	  

	  title->DrawLatex(0.14, 0.80, Form("%s #epsilon [%.2f,%.2f]", 
					    axisNames[currentAxis].c_str(), 
					    currentBins.GetBin(b).GetMin(), 
					    currentBins.GetBin(b).GetMax()));

	  // for drawing the cuts 
	  TLine *minLine = new TLine(min, h->zoom[currentAxis][b]->GetMinimum(), 
				     min, h->zoom[currentAxis][b]->GetMaximum()); 
	  TLine *maxLine = new TLine(max, h->zoom[currentAxis][b]->GetMinimum(), 
				     max, h->zoom[currentAxis][b]->GetMaximum()); 
	  
	  minLine->SetLineWidth(2);
	  minLine->SetLineStyle(8);
	  minLine->SetLineColor(kBlack); 
	  minLine->Draw(); 
	  
	  maxLine->SetLineWidth(2);
	  maxLine->SetLineStyle(8);
	  maxLine->SetLineColor(kBlack); 
	  maxLine->Draw(); 

	  // we need to print a page 
	  if (index > 16){
	    index = 1; 
	    canvas->Print(pdf.c_str());
	    canvas->Clear(); 
	    canvas->Divide(4,4); 
	  }

	}
      }
    }

    canvas->Print(pdf.c_str());
    canvas->Print(Form("%s]",pdf.c_str()));
  }

  static void PlotPtVsZ(Config conf, 
			StandardHistograms *histos, 
			std::string title, 
			std::string pdf){

    Global::Visualization::SetCustomPalette(); 

    gStyle->SetOptTitle(0); 

    TCanvas *can = new TCanvas("can","",1200,800);
    can->cd();
    can->SetMargin(0.15, 0.15, 0.15, 0.15);

    TLatex *tit = new TLatex(); 
    tit->SetNDC(); 
    //    tit->SetTextFont(122);
    tit->SetTextSize(0.03);
    
    histos->h2_z_pt2[0]->Draw("colz");
    gPad->SetLogz(); 
    gPad->SetGrid(); 

    tit->DrawLatex(0.48, 0.05, "z_{h}");
    tit->SetTextAngle(90.0);
    tit->DrawLatex(0.05, 0.48, "P_{T}^{2} [GeV^{2}/c^{2}]");
    tit->SetTextAngle(0.0);
    tit->DrawLatex(0.36, 0.865, Form("Event Sample for Configuration: %s", conf.name.c_str()));

    can->Print(pdf.c_str());
    
    // return to our options 
    Setup();
    
  }

  static void PlotPtVsZBinned(Config conf, 
			      StandardHistograms *histos, 
			      std::string title, 
			      std::string pdf){

    Global::Visualization::SetCustomPalette(); 

    gStyle->SetOptTitle(0); 

    TCanvas *can = new TCanvas("can","",1200,800);
    can->cd();
    can->SetMargin(0.15, 0.15, 0.15, 0.15);

    TLatex *tit = new TLatex(); 
    tit->SetNDC(); 
    //    tit->SetTextFont(122);
    tit->SetTextSize(0.03);
    
    histos->h2_z_pt2[0]->Draw("colz");
    gPad->SetLogz(); 
    
    for(int bin=0; bin<conf.axes[axis::z].GetNumber(); bin++){
      TLine *leftEdge = new TLine(conf.axes[axis::z].GetBin(bin).GetMin(), 
				  pow(conf.axes[axis::pt].GetMin(),2), 
				  conf.axes[axis::z].GetBin(bin).GetMin(), 
				  pow(conf.axes[axis::pt].GetMax(),2)); 

      leftEdge->SetLineWidth(1); 
      leftEdge->SetLineColor(kBlack);
      leftEdge->SetLineStyle(2);
      leftEdge->Draw();
    }

    // have to draw the last one 
    TLine *rightEdge = new TLine(conf.axes[axis::z].GetMax(), 
				 pow(conf.axes[axis::pt].GetMin(),2), 
				 conf.axes[axis::z].GetMax(), 
				 pow(conf.axes[axis::pt].GetMax(),2)); 
    
    rightEdge->SetLineWidth(1); 
    rightEdge->SetLineColor(kBlack);
    rightEdge->SetLineStyle(2);
    rightEdge->Draw();

    for(int bin=0; bin<conf.axes[axis::pt].GetNumber(); bin++){
      TLine *bottomEdge = new TLine(conf.axes[axis::z].GetMin(), 
				    pow(conf.axes[axis::pt].GetBin(bin).GetMin(),2), 
				    conf.axes[axis::z].GetMax(), 
				    pow(conf.axes[axis::pt].GetBin(bin).GetMin(),2));

      bottomEdge->SetLineWidth(1); 
      bottomEdge->SetLineColor(kBlack);
      bottomEdge->SetLineStyle(2);
      bottomEdge->Draw();
    }
    
    // and the top 
    TLine *topEdge = new TLine(conf.axes[axis::z].GetMin(), 
			       pow(conf.axes[axis::pt].GetMax(),2), 
			       conf.axes[axis::z].GetMax(),
			       pow(conf.axes[axis::pt].GetMax(),2));
    
    topEdge->SetLineWidth(1); 
    topEdge->SetLineColor(kBlack);
    topEdge->SetLineStyle(2);
    topEdge->Draw();
    

    tit->DrawLatex(0.48, 0.05, "z_{h}");
    tit->SetTextAngle(90.0);
    tit->DrawLatex(0.05, 0.48, "P_{T}^{2} [GeV^{2}/c^{2}]");
    tit->SetTextAngle(0.0);
    tit->DrawLatex(0.36, 0.865, Form("Event Sample for Configuration: %s", conf.name.c_str()));

    can->Print(pdf.c_str());
    can->Clear();
    
    // return to our options 
    Setup();
    
  }

  static void PlotQ2VsX(Config conf, 
			StandardHistograms *histos, 
			std::string title, 
			std::string pdf){

    Global::Visualization::SetCustomPalette(); 

    gStyle->SetOptTitle(0); 

    TCanvas *can = new TCanvas("can","",1200,800);
    can->cd();
    can->SetMargin(0.15, 0.15, 0.15, 0.15);

    TLatex *tit = new TLatex(); 
    tit->SetNDC(); 
    //    tit->SetTextFont(122);
    tit->SetTextSize(0.03);
    
    histos->h2_xbj_q2[0]->Draw("colz");
    gPad->SetLogz(); 
    gPad->SetGrid(); 

    tit->DrawLatex(0.48, 0.05, "x");
    tit->SetTextAngle(90.0);
    tit->DrawLatex(0.05, 0.48, "Q^{2} [GeV^{2}/c^{2}]");
    tit->SetTextAngle(0.0);
    tit->DrawLatex(0.36, 0.865, Form("Event Sample for Configuration: %s", conf.name.c_str()));

    can->Print(pdf.c_str());
     
    // return to our options 
    Setup();    
  }

  static void PlotQ2VsXBinned(Config conf, 
			      StandardHistograms *histos, 
			      std::string title, 
			      std::string pdf){

    Global::Visualization::SetCustomPalette(); 

    gStyle->SetOptTitle(0); 

    TCanvas *can = new TCanvas("can","",1200,800);
    can->cd();
    can->SetMargin(0.15, 0.15, 0.15, 0.15);

    TLatex *tit = new TLatex(); 
    tit->SetNDC(); 
    //    tit->SetTextFont(122);
    tit->SetTextSize(0.03);
    
    histos->h2_xbj_q2[0]->Draw("colz");
    gPad->SetLogz(); 
    
    for(int bin=0; bin<conf.axes[axis::x].GetNumber(); bin++){
      TLine *leftEdge = new TLine(conf.axes[axis::x].GetBin(bin).GetMin(), 
				  conf.axes[axis::q2].GetMin(), 
				  conf.axes[axis::x].GetBin(bin).GetMin(), 
				  conf.axes[axis::q2].GetMax()); 

      leftEdge->SetLineWidth(1); 
      leftEdge->SetLineColor(kBlack);
      leftEdge->SetLineStyle(2);
      leftEdge->Draw();
    }

    // have to draw the last one 
    TLine *rightEdge = new TLine(conf.axes[axis::x].GetMax(), 
				 conf.axes[axis::q2].GetMin(), 
				 conf.axes[axis::x].GetMax(), 
				 conf.axes[axis::q2].GetMax()); 
    
    rightEdge->SetLineWidth(1); 
    rightEdge->SetLineColor(kBlack);
    rightEdge->SetLineStyle(2);
    rightEdge->Draw();

    for(int bin=0; bin<conf.axes[axis::q2].GetNumber(); bin++){
      TLine *bottomEdge = new TLine(conf.axes[axis::x].GetMin(), 
				    conf.axes[axis::q2].GetBin(bin).GetMin(), 
				    conf.axes[axis::x].GetMax(), 
				    conf.axes[axis::q2].GetBin(bin).GetMin());

      bottomEdge->SetLineWidth(1); 
      bottomEdge->SetLineColor(kBlack);
      bottomEdge->SetLineStyle(2);
      bottomEdge->Draw();
    }
    
    // and the top 
    TLine *topEdge = new TLine(conf.axes[axis::x].GetMin(), 
			       conf.axes[axis::q2].GetMax(), 
			       conf.axes[axis::x].GetMax(),
			       conf.axes[axis::q2].GetMax());
    
    topEdge->SetLineWidth(1); 
    topEdge->SetLineColor(kBlack);
    topEdge->SetLineStyle(2);
    topEdge->Draw();
    

    tit->DrawLatex(0.48, 0.05, "x");
    tit->SetTextAngle(90.0);
    tit->DrawLatex(0.05, 0.48, "Q^{2} [GeV^{2}/c^{2}]");
    tit->SetTextAngle(0.0);
    tit->DrawLatex(0.36, 0.865, Form("Event Sample for Configuration: %s", conf.name.c_str()));

    can->Print(pdf.c_str());
    can->Clear();
    
    // return to our options 
    Setup();
    
  }

  static void PlotCheckPoints(TH1I *hist, std::string title, std::string pdf){
    
    TCanvas *can = new TCanvas("can","",1200,800); 
    can->cd();

    hist->SetFillColorAlpha(99, 0.2); 
    hist->SetLineColor(99); 
    hist->Draw();
    gPad->SetLogy();

    TLatex *tit = new TLatex(); 
    tit->SetNDC(); 
    tit->SetTextFont(102);
    tit->SetTextSize(0.04); 
    tit->DrawLatex(0.38, 0.95, title.c_str()); 

    tit->DrawLatex(0.12, 0.86, Form("#splitline{Electron ID}{%.3e}", hist->GetBinContent(1)));
    tit->DrawLatex(0.32, 0.86, Form("#splitline{Meson ID}{%.3e}", hist->GetBinContent(2)));
    tit->DrawLatex(0.52, 0.86, Form("#splitline{Kinematics}{%.3e}", hist->GetBinContent(3)));
    tit->DrawLatex(0.72, 0.86, Form("#splitline{Missing Mass}{%.3e}", hist->GetBinContent(4)));


    can->Print(pdf.c_str());
  }

};

#endif 
