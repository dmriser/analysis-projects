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
