{

  gStyle->SetOptStat(1); 

  TFile *f = TFile::Open("/volatile/clas12/dmriser/analysis/asymmetry/test/test.root"); 

  TH1F *mm = (TH1F*) f->Get("missingMass/missingMass_wide_PassKinematic");

  TLatex lab;
  lab.SetNDC(); 
  lab.SetTextFont(42); 
  lab.SetTextSize(0.05); 


  TCanvas *can = new TCanvas("can","",1200,1200); 

  mm->SetLineColor(kBlack);
  mm->SetLineWidth(2);
  mm->Draw(); 

  lab.DrawLatex(0.45, 0.05, "M_{X} (eK^{+})"); 
  
  can->Print("mmek-dr.pdf"); 
}
