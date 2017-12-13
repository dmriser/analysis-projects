{
  // -----------------------------------------------------
  //      User Parameters 
  // -----------------------------------------------------

  TFile *input = TFile::Open("/volatile/clas12/dmriser/rootFiles/inclusive/both/Phi4to12v2.root");


  // -----------------------------------------------------
  //      Logic Stuffs 
  // -----------------------------------------------------
  gROOT->LoadMacro("loadHistos.C");

  vector<TH1D*> data = loadHistos(input, "h1_ele_relPhi_sect[1-6]_dataAll");
  vector<TH1D*> rec  = loadHistos(input, "h1_ele_relPhi_sect[1-6]_allRec");


  for(int h=0; h<data.size(); h++){
    data[h]->SetLineColor(99); 
    rec[h] ->SetLineColor(55); 
  }

  TCanvas *c = new TCanvas("c","",800,800); 
  c->Divide(3,3);

  for(int h=0; h<6; h++){
    c->cd(h+1);

    gPad->SetMargin(0.05, 0.05, 0.05, 0.05);
    if(padIsLeft(h+1,3,3)){
      gPad->SetLeftMargin(0.2);
    } else if (padIsRight(h+1,3,3)){
      gPad->SetRightMargin(0.2);
    }
    if (padIsTop(h+1,3,3)){
      gPad->SetTopMargin(0.2);
    } else if (padIsBottom(h+1,3,3)){
      gPad->SetBottomMargin(0.2);
    }


    rec[h]->Scale(data[h]->Integral()/rec[h]->Integral());

    double max = rec[h]->GetMaximum(); 
    if (data[h]->GetMaximum() > max) { max = data[h]->GetMaximum(); }
    max *= 1.2; 

    data[h]->SetMaximum(max); 
    rec[h] ->SetMaximum(max); 

    data[h]->Draw();
    rec[h] ->Draw("same");
  }

}
