{

  TFile * file = TFile::Open("/volatile/clas12/dmriser/rootFiles/pid/eid/small.root");
  //  TFile * file = TFile::Open("PID.root");
  TCanvas * c1 = new TCanvas("c1", "", 1000, 900);

  std::string imagePath("/volatile/clas12/dmriser/plots/pid/eid"); 

  TF1 *ccCutLine = new TF1("ccCut","[0] - [1]*sqrt(1-x^2/[2])",-30,30); 
  ccCutLine->SetParameter(0, 46.0); 
  ccCutLine->SetParameter(1, 35.0); 
  ccCutLine->SetParameter(2, 360.0); 
  ccCutLine->SetLineWidth(2);
  ccCutLine->SetLineColor(99);  
  ccCutLine->SetLineStyle(9);  

  const static int NTYPE = 12; 
  const static int NSECT =  7;

  // cut types 
  string type[NTYPE] = {"allNegatives", "cuts", "Z_VERTEX","CC_FID","CC_PHI","CC_THETA","DC_R1_FID","DC_R3_FID","EC_FID","EC_IN_OUT","EC_SAMPLING", "allOthers"};
  string sect[NSECT] = {"all", "s1", "s2", "s3", "s4", "s5", "s6"};

  string cutApplied[NTYPE] = {"None", "All", "Z-Vertex", "CC Fiducial", "CC #phi-Matching", "#theta_{CC} Segment-Matching", "DC Fiducial (R1)", 
			      "DC Fiducial (R3)", "EC Fiducial UVW", "EC Inner-EDep", "Sampling Fraction", "All Others"};

  string xAxisTitle = "x";
  string yAxisTitle = "y";

  float xPosFrac = 0.585; 
  float yPosFrac = 0.95; 

  float xPosCut = 0.2; 
  float yPosCut = 0.87; 

  TLatex lab, xlabel, ylabel; 
  lab.SetNDC();
  lab.SetTextFont(42);
  lab.SetTextSize(0.08);

  xlabel.SetNDC();
  xlabel.SetTextFont(42);
  xlabel.SetTextSize(0.03);

  ylabel.SetNDC();
  ylabel.SetTextFont(42);
  ylabel.SetTextSize(0.03);
  ylabel.SetTextAngle(90.0);

  // 1-D
  TH1D * h1_nphe[NTYPE][NSECT];
  TH1F * h1_ec_edep_inner[NTYPE][NSECT];
  TH1F * h1_ec_edep_outer[NTYPE][NSECT];
  TH1F * h1_p[NTYPE][NSECT];
  TH1F * h1_z_vertex[NTYPE][NSECT];

  // 2-D
  TH2F * h2_cc_theta[NTYPE][NSECT];
  TH2F * h2_etot_p[NTYPE][NSECT];
  TH2F * h2_ang_fid[NTYPE][NSECT];
  TH2F * h2_ec_edep[NTYPE][NSECT];
  TH2F * h2_dcr1_fid[NTYPE][NSECT];
  TH2F * h2_dcr3_fid[NTYPE][NSECT];
  TH2F * h2_ec_fid[NTYPE][NSECT];
      
  string printer = "";
                        
  // --------------------------------------------
  //    Load the histograms from the file 
  //  -------------------------------------------- 
  for (int itype = 0; itype < NTYPE; itype++)
    for(int isect = 0; isect < 7; isect++){
	h1_nphe[itype][isect]          = (TH1D*) file->Get(Form("h1_nphe_%s_%s",type[itype].c_str(),sect[isect].c_str()));
	h1_ec_edep_inner[itype][isect] = (TH1F*) file->Get(Form("h1_ec_edep_inner_%s_%s",type[itype].c_str(),sect[isect].c_str()));
	h1_ec_edep_outer[itype][isect] = (TH1F*) file->Get(Form("h1_ec_edep_outer_%s_%s",type[itype].c_str(),sect[isect].c_str()));
	h1_p[itype][isect]             = (TH1F*) file->Get(Form("h1_p_%s_%s",type[itype].c_str(),sect[isect].c_str()));
	h1_z_vertex[itype][isect]      = (TH1F*) file->Get(Form("h1_z_vertex_%s_%s",type[itype].c_str(),sect[isect].c_str()));

	h2_cc_theta[itype][isect]      = (TH2F*) file->Get(Form("h_cc_theta_%s_%s",type[itype].c_str(),sect[isect].c_str()));
	h2_etot_p[itype][isect]        = (TH2F*) file->Get(Form("h_etot_p_%s_%s",type[itype].c_str(),sect[isect].c_str()));
	h2_ang_fid[itype][isect]       = (TH2F*) file->Get(Form("h_ang_fid_%s_%s",type[itype].c_str(),sect[isect].c_str()));
	h2_ec_edep[itype][isect]       = (TH2F*) file->Get(Form("h_ec_edep_%s_%s",type[itype].c_str(),sect[isect].c_str()));
	h2_dcr1_fid[itype][isect]      = (TH2F*) file->Get(Form("h_dcr1_fid_%s_%s",type[itype].c_str(),sect[isect].c_str()));
	h2_dcr3_fid[itype][isect]      = (TH2F*) file->Get(Form("h_dcr3_fid_%s_%s",type[itype].c_str(),sect[isect].c_str()));
	h2_ec_fid[itype][isect]        = (TH2F*) file->Get(Form("h_ec_fid_%s_%s",type[itype].c_str(),sect[isect].c_str()));
      }

  // --------------------------------------------
  //    Styling options 
  //  -------------------------------------------- 
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetTextFont(42);
  gStyle->SetTextSize(0.05);
  gStyle->SetLabelSize(0.05);

  // --------------------------------------------
  //    Draw the EC Inner 
  //  -------------------------------------------- 
  c1->Divide(2,3);

  TLine *left[6], *right[6];
  TLegend *legend[6];

  for (int isect=1; isect<7; isect++){
    c1->cd(isect);
    gPad->SetMargin(0.15,0.15,0.15,0.15);

    h2_ec_edep[11][isect]->Draw("colz"); 
    gPad->SetLogz(); 
    lab.DrawLatex(0.27, 0.01, "EC Inner Energy [GeV]");
    lab.SetTextAngle(90.0); 
    lab.DrawLatex(0.05, 0.15, "EC Outer Energy [GeV]");
    lab.SetTextAngle(0.0); 

    left[isect-1] = new TLine(0.06, 0.0, 0.06, 0.5);
    left[isect-1]->SetLineColor(99);
    left[isect-1]->SetLineStyle(2);
    left[isect-1]->SetLineWidth(2);
    left[isect-1]->Draw("same");
  }

  c1->Print(Form("%s/summary-ec-edep.pdf", imagePath.c_str()));
  c1->Clear(); 


  // --------------------------------------------
  //    Draw the histograms 
  //  -------------------------------------------- 
  c1->Divide(2,3);

  for (int isect=1; isect<7; isect++){
    c1->cd(isect);
    gPad->SetMargin(0.1,0.1,0.15,0.15);

    h1_z_vertex[0][isect] ->SetLineColor(kBlack);
    h1_z_vertex[0][isect] ->Draw();
    h1_z_vertex[11][isect]->SetLineColor(kBlack);
    h1_z_vertex[11][isect]->SetFillColor(kBlack);
    h1_z_vertex[11][isect]->SetFillStyle(3004);
    h1_z_vertex[11][isect]->Draw("same");
    lab.DrawLatex(0.27, 0.01, "corrected z-vertex (cm)");
 
    left[isect-1] = new TLine(-27.7302, 0.0, -27.7302, h1_z_vertex[0][isect]->GetMaximum());
    left[isect-1]->SetLineColor(99);
    left[isect-1]->SetLineStyle(2);
    left[isect-1]->SetLineWidth(2);
    left[isect-1]->Draw("same");
 
    right[isect-1] = new TLine(-22.6864, 0.0, -22.6864, h1_z_vertex[0][isect]->GetMaximum());
    right[isect-1]->SetLineColor(99);
    right[isect-1]->SetLineStyle(2);
    right[isect-1]->SetLineWidth(2);
    right[isect-1]->Draw("same");

    legend[isect-1] = new TLegend(0.65, 0.6, 0.82, 0.8);
    legend[isect-1]->AddEntry(h1_z_vertex[0][isect], "All Negatives");
    legend[isect-1]->AddEntry(h1_z_vertex[11][isect], "All Other Cuts");
    legend[isect-1]->SetBorderSize(0);
    legend[isect-1]->SetTextSize(0.05);
    legend[isect-1]->Draw();
  }

  c1->Print(Form("%s/summary-z-vertx.pdf", imagePath.c_str()));
  c1->Clear(); 

}
