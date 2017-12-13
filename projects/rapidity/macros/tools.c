#include "TH1.h"

void style(TH1F *h){
  h->SetFillColor(kBlack);
  h->SetLineColor(kBlack);
  h->SetFillStyle(3004);
}

void customDivide(TH1F *num, TH1F *denom){
  for(int bin=1; bin<=num->GetXaxis()->GetNbins(); bin++){
    float a = num->GetBinContent(bin); 
    float b = denom->GetBinContent(bin); 

    if (b > 0){
      float content = a/b; 
      float err     = content * sqrt(1/a + 1/b); 

      if (content > 0.05){
	num->SetBinContent(bin, content); 
	num->SetBinError(bin, err); 
      } 
      else {
	num->SetBinContent(bin, 0.0); 
	num->SetBinError(bin,  0.0); 
      }
    }
  }
}
