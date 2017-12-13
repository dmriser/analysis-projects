{
  TFile *f = TFile::Open("/volatile/clas12/dmriser/analysis/asymmetry/test/histos.root");

  TH1F *av = (TH1F*) f->Get("average_pt/average_pt");
  av->Draw();
}
