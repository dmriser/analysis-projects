#include <iostream>

// this project 
#include "common/Config.h"
#include "common/ConfigLoader.h"
#include "common/ConfigLoader.cxx"
#include "common/PhiHistograms.h"
#include "common/Plotting.h"
#include "common/PtAverager.h"
#include "common/KinematicHistograms.h"
#include "common/MissingMassHistograms.h"

// h22 libs
#include "StandardHistograms.h"

int main(int argc, char *argv[]){

  if(argc < 2){
    std::cerr << "Pass in configuration files." << std::endl; 
    exit(0);
  }


  std::vector<Config> configs; 
  for(int iarg=1; iarg<argc; iarg++){
    std::string jsonFilename(argv[iarg]);
    configs.push_back(ConfigLoader::getConfiguration(jsonFilename)); 
  }

 for(Config c : configs){

   PhiHistos *plus  = new PhiHistos(); 
   PhiHistos *minus = new PhiHistos(); 
   
   plus ->Load(c.axes, Form("%s/%s/histos.root", c.analysisHome.c_str(), c.name.c_str()), "countsPlus"); 
   minus->Load(c.axes, Form("%s/%s/histos.root", c.analysisHome.c_str(), c.name.c_str()), "countsMinus"); 
   
   PhiHistos *asym = new PhiHistos();
   asym->Load(c.axes, Form("%s/%s/fits.root", c.analysisHome.c_str(), c.name.c_str()), "asym");
   
   PhiHistos *err = new PhiHistos(); 
   err->Load(c.axes, Form("%s/%s/fits.root", c.analysisHome.c_str(), c.name.c_str()), "fit_confidenceInterval");
   
   KinematicHistos *kin = new KinematicHistos(); 
   kin->Load(c.axes, Form("%s/%s/fits.root", c.analysisHome.c_str(), c.name.c_str()), "integratedAsymmetry"); 

   KinematicHistos *kinPt = new KinematicHistos(); 
   kinPt->Load(c.axes, Form("%s/%s/fits.root", c.analysisHome.c_str(), c.name.c_str()), "integratedAsymmetryPt"); 
   
   MissingMassHistos *mass = new MissingMassHistos(); 
   mass->Load(c.axes, Form("%s/%s/histos.root", c.analysisHome.c_str(), c.name.c_str()), "PassKinematic"); 
   
   StandardHistograms *standard = new StandardHistograms("PassAll",1);
   standard->Load(Form("%s/%s/histos.root", c.analysisHome.c_str(), c.name.c_str()));

   PtAverager *ptAverager = new PtAverager(c);
   ptAverager->Load(Form("%s/%s/histos.root", c.analysisHome.c_str(), c.name.c_str()));
   ptAverager->CalculateAverage();

   // I don't really like this here
   // but idk.
   TFile *input = TFile::Open(Form("%s/%s/histos.root", c.analysisHome.c_str(), c.name.c_str())); 
   TH1I *checkPoint = (TH1I*) input->Get("checkPoints");;

   Plotting::Setup();
   Plotting::PlotTwoPhiHistos(asym, err,
			      "A_{LU}^{sin(#phi)}",
			      "#phi_{h}",
			      "A_{LU}^{sin(#phi)}",
			      "histpe",
			      "e3same",
			      styles::marker_black, 
			      styles::filled_blue,
			      Form("%s/%s/plots/asymmetry/phi.pdf",c.analysisHome.c_str(),c.name.c_str())); 
   
   Plotting::PlotTwoPhiHistos(plus, minus,
			      "Phi-Dist.",
			      "#phi_{h}",
			      "Counts",
			      "hist",
			      "histsame",
			      styles::filled_red, 
			      styles::filled_blue,
			      Form("%s/%s/plots/phi/counts.pdf",c.analysisHome.c_str(),c.name.c_str())); 
   
   
   Plotting::PlotAveragePt(ptAverager, 
			   c, 
			   Form("%s/%s/plots/average_pt.pdf",c.analysisHome.c_str(),c.name.c_str())); 

   Plotting::PlotAsymmetryZ(kin, 
			   c, 
			   Form("%s/%s/plots/asymmetry/z.pdf",c.analysisHome.c_str(),c.name.c_str()));    

   Plotting::PlotAsymmetryZ(kinPt, 
			   c, 
			   Form("%s/%s/plots/asymmetry/z_div_pt.pdf",c.analysisHome.c_str(),c.name.c_str()));    

   Plotting::PlotOneKinematicHisto(kin,
				   "A_{LU}^{sin(#phi)}",
				   "A_{LU}^{sin(#phi)}",
				   "histpe",
				   styles::marker_black,
				   Form("%s/%s/plots/asymmetry/integrated.pdf",c.analysisHome.c_str(),c.name.c_str())); 

   Plotting::PlotOneKinematicHisto(kinPt,
				   "A_{LU}^{sin(#phi)}",
				   "A_{LU}^{sin(#phi)}",
				   "histpe",
				   styles::marker_black,
				   Form("%s/%s/plots/asymmetry/integrated_div_pt.pdf",c.analysisHome.c_str(),c.name.c_str())); 

   
   Plotting::PlotOneMissingMassHisto(mass,
				     c.cuts["missing_mass"]["min"],
				     c.cuts["missing_mass"]["max"],
				     "Pass Kinematic Cuts",
				     "hist",
				     styles::filled_red,
				     Form("%s/%s/plots/missingMass/passKinematics.pdf",c.analysisHome.c_str(),c.name.c_str())); 

   Plotting::PlotPtVsZ(c,standard,"",Form("%s/%s/plots/z_pt2.pdf",c.analysisHome.c_str(),c.name.c_str())); 
   Plotting::PlotPtVsZBinned(c,standard,"",Form("%s/%s/plots/z_pt2_binned.pdf",c.analysisHome.c_str(),c.name.c_str())); 
   Plotting::PlotQ2VsX(c,standard,"",Form("%s/%s/plots/x_q2.pdf",c.analysisHome.c_str(),c.name.c_str())); 
   Plotting::PlotQ2VsXBinned(c,standard,"",Form("%s/%s/plots/x_q2_binned.pdf",c.analysisHome.c_str(),c.name.c_str())); 

   Plotting::PlotCheckPoints(checkPoint, Form("Config name: %s",c.name.c_str()), Form("%s/%s/plots/check_points.pdf",c.analysisHome.c_str(),c.name.c_str()));
 }
 
 return 0;
}
