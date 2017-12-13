#include "TFile.h"
#include "TH1.h"
#include "TTree.h"

void checkGenEvents(){

  TFile *inputFile = TFile::Open("debug_jobs6.root");
  TTree *h10;
  inputFile->GetObject("h10",h10);

  TH2I *wByQQ = new TH2I("wByQQ","",200,1.0,3,200,1,4.5);
  TH1I *invMass = new TH1I("invMass","",100,1.0,3);
  TH1I *virtuality = new TH1I("virtuality","",100,1.0,4.5);
  TH1I *y = new TH1I("y","",100,0.0,1.0);
  TH1I *wYCut = new TH1I("wYCut","",100,1.0,3.0);
  TH1I *nu = new TH1I("nu","",100,0.0,5.5);

  h10->Draw("2*es*ep*(1-cos(thete*3.14159/180)):w >> wByQQ");
  h10->Draw("w >> invMass");
  h10->Draw("w >> wYCut","(es-ep)/es < 0.8");
  h10->Draw("(es-ep)/es >> y");
  h10->Draw("(es-ep) >> nu");
  h10->Draw("2*es*ep*(1-cos(thete*3.14159/180)) >> virtuality");

  TCanvas *checkCanvas = new TCanvas("checkCanvas","",800,800); 
  //  checkCanvas->Divide(3,1);

  checkCanvas->cd(1);
  //  wByQQ->Draw("colz");
  invMass->SetLineColor(kBlue);
  wYCut->SetLineColor(kRed);
  invMass->Draw(); 
  wYCut->Draw("same");

  /*
  checkCanvas->cd(2);
  invMass->Draw();

  checkCanvas->cd(3);
  virtuality->Draw();
  */

}
