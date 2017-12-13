{

  gROOT->LoadMacro("utils.C"); 
  gStyle->SetErrorX(0.0); 

  TFile *inputFile = TFile::Open("/volatile/clas12/dmriser/rootFiles/asymmetry/asymmetry_pass25.root"); 

  const int npart            = 4; 
  std::string name[npart]    = {"pm", "pp", "km", "kp"};
 
  TH1D *x[npart], *q2[npart], *z[npart], *pt[npart]; 
  if (inputFile && inputFile->IsOpen()){
    for (int p=0; p<npart; p++){
      x[p]  = (TH1D*) inputFile->Get(Form("integrated/%s/h1_x_q20_z0_pt0_base", name[p].c_str())); 
      q2[p] = (TH1D*) inputFile->Get(Form("integrated/%s/h1_q2_z0_pt0_x0_base", name[p].c_str())); 
      z[p]  = (TH1D*) inputFile->Get(Form("integrated/%s/h1_z_pt0_x0_q20_base", name[p].c_str())); 
      pt[p] = (TH1D*) inputFile->Get(Form("integrated/%s/h1_pt_x0_q20_z0_base", name[p].c_str())); 


      std::ofstream outX(Form("x_%s.csv", name[p].c_str())); 
      for (int b=1; b<=x[p]->GetXaxis()->GetNbins(); b++){
	outX << b << ",";
      }
      outX << std::endl; 
      
      for (int b=1; b<=x[p]->GetXaxis()->GetNbins(); b++){
	outX << x[p]->GetBinCenter(b) << ",";
      }
      outX << std::endl; 
      
      for (int b=1; b<=x[p]->GetXaxis()->GetNbins(); b++){
	outX << x[p]->GetBinContent(b) << ",";
      }
      outX << std::endl; 

      for (int b=1; b<=x[p]->GetXaxis()->GetNbins(); b++){
	outX << x[p]->GetBinError(b) << ",";
      }
      outX << std::endl;  
      outX.close(); 

      std::ofstream outQ2(Form("q2_%s.csv", name[p].c_str())); 
      for (int b=1; b<=q2[p]->GetXaxis()->GetNbins(); b++){
	outQ2 << b << ",";
      }
      outQ2 << std::endl; 
      
      for (int b=1; b<=q2[p]->GetXaxis()->GetNbins(); b++){
	outQ2 << q2[p]->GetBinCenter(b) << ",";
      }
      outQ2 << std::endl; 
      
      for (int b=1; b<=q2[p]->GetXaxis()->GetNbins(); b++){
	outQ2 << q2[p]->GetBinContent(b) << ",";
      }
      outQ2 << std::endl; 

      for (int b=1; b<=q2[p]->GetXaxis()->GetNbins(); b++){
	outQ2 << q2[p]->GetBinError(b) << ",";
      }
      outQ2 << std::endl;  
      outQ2.close(); 

      std::ofstream outZ(Form("z_%s.csv", name[p].c_str())); 
      for (int b=1; b<=z[p]->GetXaxis()->GetNbins(); b++){
	outZ << b << ",";
      }
      outZ << std::endl; 
      
      for (int b=1; b<=z[p]->GetXaxis()->GetNbins(); b++){
	outZ << z[p]->GetBinCenter(b) << ",";
      }
      outZ << std::endl; 
      
      for (int b=1; b<=z[p]->GetXaxis()->GetNbins(); b++){
	outZ << z[p]->GetBinContent(b) << ",";
      }
      outZ << std::endl; 

      for (int b=1; b<=z[p]->GetXaxis()->GetNbins(); b++){
	outZ << z[p]->GetBinError(b) << ",";
      }
      outZ << std::endl;  
      outZ.close(); 

      std::ofstream outPt(Form("pt_%s.csv", name[p].c_str())); 
      for (int b=1; b<=pt[p]->GetXaxis()->GetNbins(); b++){
	outPt << b << ",";
      }
      outPt << std::endl; 
      
      for (int b=1; b<=pt[p]->GetXaxis()->GetNbins(); b++){
	outPt << pt[p]->GetBinCenter(b) << ",";
      }
      outPt << std::endl; 
      
      for (int b=1; b<=pt[p]->GetXaxis()->GetNbins(); b++){
	outPt << pt[p]->GetBinContent(b) << ",";
      }
      outPt << std::endl; 

      for (int b=1; b<=pt[p]->GetXaxis()->GetNbins(); b++){
	outPt << pt[p]->GetBinError(b) << ",";
      }
      outPt << std::endl;  
      outPt.close(); 
    }

  }

  

}
