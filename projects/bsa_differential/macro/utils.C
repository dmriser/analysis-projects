void loadIntegratedHisto(TFile *f, TH1D *h, std::string particle, 
			 std::string name, int axis, int bin1, 
			 int bin2, int bin3){

  if (f && f->IsOpen()){
    if (axis == 1){ 
      h = (TH1D*) f->Get(Form("integrated/%s/h1_x_q2%d_z%d_pt%d_%s",
			      particle.c_str(), bin1, bin2, bin3,
			      name.c_str())); 
    } else if (axis == 2){ 
      h = (TH1D*) f->Get(Form("integrated/%s/h1_q2_z%d_pt%d_x%d_%s",
			      particle.c_str(), bin1, bin2, bin3,
			      name.c_str())); 
    } else if (axis == 3){ 
      h = (TH1D*) f->Get(Form("integrated/%s/h1_z_pt%d_x%d_q2%d_%s",
			      particle.c_str(), bin1, bin2, bin3,
			      name.c_str())); 
    } else if (axis == 4){ 
      h = (TH1D*) f->Get(Form("integrated/%s/h1_pt_x%d_q2%d_z%d_%s",
			      particle.c_str(), bin1, bin2, bin3,
			      name.c_str())); 
    }
    
    std::cout << "Loaded " << h->GetName() << " w/entries = "<< h->GetEntries() << std::endl; 
  }
}

bool padIsLeft(int pad, int width, int height){
  return (pad%width == 1);
}

bool padIsRight(int pad, int width, int height){
  return (pad%width == 0);
}

bool padIsTop(int pad, int width, int height){
  return (pad <= width);
}

bool padIsBottom(int pad, int width, int height){
  return (pad > width*(height-1));
}

void setupPad(int pad, int width, int height, float innerSpace, float outerSpace){
  
  if (padIsLeft(pad, width, height)){
    gPad->SetLeftMargin(outerSpace); 
    gPad->SetRightMargin(innerSpace); 
  } else if (padIsRight(pad, width, height)){
    gPad->SetLeftMargin(innerSpace); 
    gPad->SetRightMargin(outerSpace); 
  } else {
    gPad->SetLeftMargin(innerSpace); 
    gPad->SetRightMargin(innerSpace);     
  }

  if (padIsTop(pad, width, height)){
    gPad->SetTopMargin(outerSpace);
    gPad->SetBottomMargin(innerSpace);
  } else if (padIsBottom(pad, width, height)){
    gPad->SetTopMargin(innerSpace);
    gPad->SetBottomMargin(outerSpace);
  } else {
    gPad->SetTopMargin(innerSpace);
    gPad->SetBottomMargin(innerSpace);
  }

}
