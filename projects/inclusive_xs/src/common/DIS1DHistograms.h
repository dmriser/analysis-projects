#ifndef dis_1d_histograms_h
#define dis_1d_histograms_h

#include <cmath>
#include <iostream>
#include <vector>
using std::cout; 
using std::endl; 
using std::string; 
using std::vector; 

#include "BaseDISHistograms.h"
#include "CommonTools.h"

#include "TCanvas.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "THnSparse.h"
#include "TKey.h"
#include "TList.h"
#include "TRegexp.h"
#include "TString.h"
#include "TStyle.h"

class DIS1DHistograms{
 public:
  DIS1DHistograms();
  ~DIS1DHistograms();

  TFile * inputFile; 
  TH1D *allwByQQ[7];
  TH1D *allQQByW[7];
  vector<vector<TH1D*> > wByQQ; 
  vector<vector<TH1D*> > QQByW; 

  int numberOfWBins; 
  double wMin, wMax, wWidth;

  int numberOfQQBins; 
  double qqMin, qqMax, qqWidth;

  string nameOfHistograms; 

 public:
  void Add(DIS1DHistograms *histos);
  void Create(BaseDISHistograms *baseHistograms); 
  void CreateByDivision(DIS1DHistograms *numerator, DIS1DHistograms *demoninator, string name, string title);
  void CreateFromExisting(DIS1DHistograms * sourceHistograms, string newName, string newTitle);
  void Divide(DIS1DHistograms * denominator);
  void DivideByZero(DIS1DHistograms *histos);
  void Load(string inputFilenameWithExtension, string title);
  void MultiplyByZero(DIS1DHistograms *histos);
  void PrintPDF(string pdfTitle);
  void Save(string outputFilenameWithExtension, string saveOption); 
  void Scale(double scaleValue);
  void ScaleByBinWidth();
  void ScaleByNumberSectors();
  void ScaleAllByNumberBins();
  void ScaleByPhotonFlux(double beamEnergy);
  void SetBinsOutsideRangeToValue(double min, double max,double value);
  void SetErrors(); 
  void SetNumberOfEventsSame(DIS1DHistograms *histos);
};

DIS1DHistograms::DIS1DHistograms(){
  numberOfWBins = 0; 
  wMin = 0; wMax = 0; wWidth = 0; 
  numberOfQQBins = 0; 
  qqMin = 0; qqMax = 0; qqWidth = 0; 
}

DIS1DHistograms::~DIS1DHistograms(){
  if (inputFile->IsOpen()){
    inputFile->Close();
  }
}

void DIS1DHistograms::Add(DIS1DHistograms *histos){

    for (int sector=0; sector<7; sector++){
      allwByQQ[sector]->Add(histos->allwByQQ[sector]);
      //      allQQByW[sector]->Add(histos->allQQByW[sector]);
    }

    for (int sector=0; sector<wByQQ.size(); sector++){
      for (int slice=0; slice<wByQQ[sector].size(); slice++){
	wByQQ[sector][slice]->Add( histos->wByQQ[sector][slice] );
      }
    }

    //    for (int sector=0; sector<QQByW.size(); sector++){
    //      for (int slice=0; slice<QQByW[sector].size(); slice++){
    //	QQByW[sector][slice]->Add( histos->QQByW[sector][slice] );
    //      }
    //    }

}

// Under Construction: Adding the Q^{2} projection, but need
// to figure out naming scheme properly.
void DIS1DHistograms::Create(BaseDISHistograms *baseHistograms){

  nameOfHistograms = baseHistograms->baseName; 

  for (int sector=0; sector<7; sector++){
    vector<TH1D*> wContainer; 
    vector<TH1D*> qqContainer; 

    // Discover Properties of the Things Given to Us
    string wByQQBaseName  = baseHistograms->wByQQ[sector]->GetName(); 
    string wByQQBaseTitle = baseHistograms->wByQQ[sector]->GetTitle(); 
  
    numberOfWBins = baseHistograms->wByQQ[sector]->GetXaxis()->GetNbins();
    wMin = baseHistograms->wByQQ[sector]->GetXaxis()->GetBinLowEdge(1);
    wMax = baseHistograms->wByQQ[sector]->GetXaxis()->GetBinUpEdge(numberOfWBins);
    wWidth = (wMax-wMin)/numberOfWBins;

    numberOfQQBins = baseHistograms->wByQQ[sector]->GetYaxis()->GetNbins();
    qqMin = baseHistograms->wByQQ[sector]->GetYaxis()->GetBinLowEdge(1);
    qqMax = baseHistograms->wByQQ[sector]->GetYaxis()->GetBinUpEdge(numberOfQQBins);
    qqWidth = (qqMax-qqMin)/numberOfQQBins;

    allwByQQ[sector] = new TH1D(wByQQBaseName.c_str(),wByQQBaseTitle.c_str(),numberOfWBins,wMin,wMax);
    baseHistograms->wByQQ[sector]->ProjectionX(wByQQBaseName.c_str(),1,numberOfQQBins);
    
    for (int slice=0; slice<numberOfQQBins; slice++){
          string wTitle = Form("%s Slice %d",wByQQBaseTitle.c_str(),slice); 
          string wName  = Form("%s_slice%d",wByQQBaseName.c_str(),slice); 

          TH1D * wSlice = new TH1D(wName.c_str(),wTitle.c_str(),numberOfWBins,wMin,wMax);
          baseHistograms->wByQQ[sector]->ProjectionX(wName.c_str(),slice+1,slice+1);
	  
	  wContainer.push_back(wSlice);
    } 
    
    wByQQ.push_back(wContainer);
  }  
}

// Note to self
// This routine uses a lot of tools which I am not that familiar with, 
// it took me a while to write and I should make sure I can reproduce it. 
void DIS1DHistograms::Load(string inputFilenameWithExtension, string title){
  
  inputFile = new TFile(inputFilenameWithExtension.c_str());

  for (int sector=0; sector<7; sector++) {
    allwByQQ[sector] = (TH1D*) inputFile->Get(Form("%s_wByQQ_s%d",title.c_str(),sector));
  }

  // Load unspecified number of slices. 
  TList * tableOfContents = inputFile->GetListOfKeys(); 

  for (int sector=0; sector<7; sector++){
    TIter next(tableOfContents);
    TString targetTitle(Form("%s_wByQQ_s%d",title.c_str(),sector)); 
    vector<TH1D*> foundHistos;
    while(TObject *objectFromFile = next()){
      
      TString currentObjectName = objectFromFile->GetName();
      
      if (currentObjectName.Contains(targetTitle)){
	foundHistos.push_back((TH1D*) inputFile->Get(currentObjectName));
      }
    }
    wByQQ.push_back(foundHistos);
    cout << "[DIS1DHistograms::Load] For title=" << targetTitle << " sector=" << sector << " " << foundHistos.size() << " histograms were loaded. " << endl; 
  }
  
}

void DIS1DHistograms::MultiplyByZero(DIS1DHistograms *histos){
  for (int sector=0; sector<7; sector++){
    allwByQQ[sector]->Multiply(histos->allwByQQ[0]);

    for (int slice=0; slice<wByQQ[sector].size(); slice++){
      wByQQ[sector][slice]->Multiply(histos->wByQQ[0][slice]);
    }

    //    for (int slice=0; slice<QQByW[sector].size(); slice++){
    //      QQByW[sector][slice]->Multiply( histos->QQByW[0][slice] );
    //    }
    

  }

}

void DIS1DHistograms::DivideByZero(DIS1DHistograms *histos){

  for (int sector=0; sector<7; sector++){
    allwByQQ[sector]->Divide(histos->allwByQQ[0]);

    for (int slice=0; slice<wByQQ[sector].size(); slice++){
      wByQQ[sector][slice]->Divide(histos->wByQQ[0][slice]);
    }

  }

}

void DIS1DHistograms::SetNumberOfEventsSame(DIS1DHistograms *histos){  

    for (int sector=0; sector<7; sector++){
      allwByQQ[sector]->Scale( histos->allwByQQ[sector]->Integral()/allwByQQ[sector]->Integral() );
    }
  
    for (int sector=0; sector<wByQQ.size(); sector++){
      for (int slice=0; slice<wByQQ[sector].size(); slice++){
	wByQQ[sector][slice]->Scale( histos->wByQQ[sector][slice]->Integral()/wByQQ[sector][slice]->Integral() );
      }
    }

}

void DIS1DHistograms::Save(string outputFilenameWithExtension, string saveOption){
  TFile * outputFile = TFile::Open(outputFilenameWithExtension.c_str(),saveOption.c_str());

  if (outputFile->IsOpen()){
    for (int sector=0; sector<7; sector++){
      allwByQQ[sector]->Write();
      cout << "Saving " << allwByQQ[sector]->GetName() << " to file=" << outputFilenameWithExtension << endl;
    }
  
    for (int sector=0; sector<wByQQ.size(); sector++){
      for (int slice=0; slice<wByQQ[sector].size(); slice++){
	wByQQ[sector][slice]->Write();
      }
    }
  }

  outputFile->Write();
  outputFile->Close();
}

void DIS1DHistograms::SetErrors(){
    for (int sector=0; sector<7; sector++){
      allwByQQ[sector]->Sumw2();
    }
    for (int sector=0; sector<wByQQ.size(); sector++){
      for (int slice=0; slice<wByQQ[sector].size(); slice++){
	wByQQ[sector][slice]->Sumw2();
      }
    }
}

void DIS1DHistograms::Divide(DIS1DHistograms * denominator){

  for (int sector=0; sector<7; sector++){
    allwByQQ[sector]->Divide(denominator->allwByQQ[sector]); 

    for (int slice=0; slice<wByQQ[sector].size(); slice++){
      wByQQ[sector][slice]->Divide(denominator->wByQQ[sector][slice]);
    }
  }

}

void DIS1DHistograms::CreateFromExisting(DIS1DHistograms * sourceHistograms, string newName, string newTitle){

  nameOfHistograms = newName; 
  numberOfWBins = sourceHistograms->numberOfWBins; 
  wMin = sourceHistograms->wMin; 
  wMax = sourceHistograms->wMax; 
  wWidth = sourceHistograms->wWidth; 

  numberOfQQBins = sourceHistograms->numberOfQQBins; 
  qqMin = sourceHistograms->qqMin; 
  qqMax = sourceHistograms->qqMax; 
  qqWidth = sourceHistograms->qqWidth; 

  for (int sector=0; sector<7; sector++){
    allwByQQ[sector] = (TH1D*) (sourceHistograms->allwByQQ[sector]->Clone());
    allwByQQ[sector]->SetName(Form("%s_wByQQ_s%d",newName.c_str(),sector));
    allwByQQ[sector]->SetTitle(Form("%s w vs. Q^{2} Sect. %d",newTitle.c_str(),sector));
    allwByQQ[sector]->SetDirectory(0);

    vector<TH1D*> wContainer; 
    for (int slice=0; slice<sourceHistograms->wByQQ[sector].size(); slice++){
      TH1D * wSlice = (TH1D*) sourceHistograms->wByQQ[sector][slice]->Clone();
      wSlice->SetName(Form("%s_wByQQ_s%d_slice%d",newName.c_str(),sector,slice));
      wSlice->SetTitle(Form("%s w vs. Q^{2} Sect. %d Slice %d",newTitle.c_str(),sector,slice));
      wContainer.push_back(wSlice);
    }
    
    wByQQ.push_back(wContainer);
  }
}

void DIS1DHistograms::CreateByDivision(DIS1DHistograms *numerator, DIS1DHistograms *denominator, string name, string title){

  numberOfWBins = numerator->numberOfWBins; 
  wMin = numerator->wMin; 
  wMax = numerator->wMax; 
  wWidth = numerator->wWidth; 

  numberOfQQBins = numerator->numberOfQQBins; 
  qqMin = numerator->qqMin; 
  qqMax = numerator->qqMax; 
  qqWidth = numerator->qqWidth; 

  CreateFromExisting(numerator,name,title);
  Divide(denominator);

  cout << "[DIS1DHistograms::CreateByDivision] " << allwByQQ[0]->GetName() << " has been created with entries=" << allwByQQ[0]->GetEntries() << endl;
}

void DIS1DHistograms::Scale(double scaleValue){

  for (int sector=0; sector<7; sector++){
    allwByQQ[sector]->Scale(scaleValue);

    for (int slice=0; slice<wByQQ[sector].size(); slice++){
      wByQQ[sector][slice]->Scale(scaleValue);
      //      if (sector != 0) { wByQQ[sector][slice]->Scale(6.0); } 
    }
  }
}

void DIS1DHistograms::ScaleByNumberSectors(){

  for (int sector=0; sector<7; sector++){
    
    if (sector != 0){
      allwByQQ[sector]->Scale(6.0);
    }

    for (int slice=0; slice<wByQQ[sector].size(); slice++){
      if (sector != 0) { wByQQ[sector][slice]->Scale(6.0); } 
    }
  }
}

void DIS1DHistograms::ScaleByBinWidth(){

  for (int sector=0; sector<7; sector++){
    allwByQQ[sector]->Scale(1/wWidth);
    
    for (int slice=0; slice<wByQQ[sector].size(); slice++){
      wByQQ[sector][slice]->Scale(1/(wWidth*qqWidth));
    }
  }
}

void DIS1DHistograms::PrintPDF(string pdfTitle){
  
  // getting size of grid 
  int gridSize=0; 
  while(gridSize*(gridSize+1) < wByQQ[0].size()){
    gridSize++;
  }

  cout << "[DIS1DHistograms::PrintPDF] found grid size " << gridSize << endl;

  gStyle->SetOptStat(0);

  TCanvas * canvas = new TCanvas("canvas","",1000,800);
  canvas->Divide(gridSize+1,gridSize);
  canvas->Print(Form("%s[",pdfTitle.c_str()));

  for (int sector=0; sector<7; sector++){
    for (int slice=0; slice<wByQQ[sector].size()-1; slice++){
      canvas->cd(slice+1);
      wByQQ[sector][slice]->Draw();
    }
    canvas->Print(pdfTitle.c_str());
  }
  canvas->Clear();

  canvas->Print(Form("%s]",pdfTitle.c_str()));
}

void DIS1DHistograms::ScaleAllByNumberBins(){
  
  for (int s=0; s<7; s++){
    allwByQQ[s]->Scale((double)1.0/numberOfQQBins);
  }

}

void DIS1DHistograms::ScaleByPhotonFlux(double beamEnergy){  
  for (int sector=0; sector<7; sector++){
    for (int qqBin=0; qqBin< numberOfQQBins; qqBin++){
      double qq = qqMin + qqBin*qqWidth;
      for (int wBin=0; wBin< numberOfWBins; wBin++){
	double scaledContent = wByQQ[sector][qqBin]->GetBinContent(wBin+1);
	double w = wBin*wWidth + wMin; 
	cout << "xs=" << scaledContent;
	scaledContent /= calculatePhotonFlux(beamEnergy,w,qq);
	cout << "xs/GAMMA=" << scaledContent << endl;
	wByQQ[sector][qqBin]->SetBinContent(wBin+1,scaledContent);
      }
    }
  }
}

void DIS1DHistograms::SetBinsOutsideRangeToValue(double min, double max, double value){
  for (int sector=0; sector<7; sector++){

    // For combined 
    for (int xAxisBin=0; xAxisBin< numberOfWBins; xAxisBin++){
      if (allwByQQ[sector]->GetBinContent(xAxisBin+1) < min || allwByQQ[sector]->GetBinContent(xAxisBin+1) > max){
	allwByQQ[sector]->SetBinContent(xAxisBin+1,value);
      } 
    }

    // For slices 
    for (int qqBin=0; qqBin<  numberOfQQBins; qqBin++){
      for (int xAxisBin=0; xAxisBin< numberOfWBins; xAxisBin++){
	if (wByQQ[sector][qqBin]->GetBinContent(xAxisBin+1) < min || wByQQ[sector][qqBin]->GetBinContent(xAxisBin+1) > max ){
	  wByQQ[sector][qqBin]->SetBinContent(xAxisBin+1,value);
	} 
      }
    }
  }
}

#endif
