#ifndef phi_histos_h
#define phi_histos_h

// standard includes 
#include <iostream>
#include <vector>

// this library includes
#include "DBins.h"
#include "PhysicsEvent.h"

// this project
#include "Config.h"
#include "Constants.h"
#include "Types.h"

// external 
#include "TDirectory.h"
#include "TFile.h"
#include "TKey.h"
#include "TList.h"
#include "TRegexp.h"
#include "TString.h"

class PhiHistos {
  
 public:
  PhiHistos(){
   

    fFile = new TFile(); 
  }

  ~PhiHistos(){
    if(fFile){
      if(fFile->IsOpen()){
	fFile->Close();
      }
    }
    
  }

  void Init(std::map<int, DLineBins> bins,
	    std::string name){
    fName = name; 
    fBins = bins; 

    // there are more elegant ways, but im trying to 
    // be a physicist 
    for(int b=0; b<fBins[axis::x].GetNumber(); b++){
      std::string title(Form("phi_x%d_%s", b, fName.c_str()));

      histos[axis::x].push_back(new TH1F(title.c_str(), 
					 title.c_str(),
					 fBins[axis::phi].GetNumber(),
					 fBins[axis::phi].GetLimits().data()));      
    }
    for(int b=0; b<fBins[axis::q2].GetNumber(); b++){
      std::string title(Form("phi_q2%d_%s", b, fName.c_str()));

      histos[axis::q2].push_back(new TH1F(title.c_str(), 
					 title.c_str(),
					 fBins[axis::phi].GetNumber(),
					 fBins[axis::phi].GetLimits().data()));      
    }
    for(int b=0; b<fBins[axis::z].GetNumber(); b++){
      std::string title(Form("phi_z%d_%s", b, fName.c_str()));

      histos[axis::z].push_back(new TH1F(title.c_str(), 
					 title.c_str(),
					 fBins[axis::phi].GetNumber(),
					 fBins[axis::phi].GetLimits().data()));      
    }
    for(int b=0; b<fBins[axis::pt].GetNumber(); b++){
      std::string title(Form("phi_pt%d_%s", b, fName.c_str()));

      histos[axis::pt].push_back(new TH1F(title.c_str(), 
					  title.c_str(),
					  fBins[axis::phi].GetNumber(),
					  fBins[axis::phi].GetLimits().data()));      
    }
  }

  void Fill(PhysicsEvent & ev){
    std::map<int,int> indices;
    

    // What bin does this event belong in?
    indices[axis::x]  = fBins[axis::x] .FindBin(ev.x); 
    indices[axis::q2] = fBins[axis::q2].FindBin(ev.qq); 
    indices[axis::z]  = fBins[axis::z] .FindBin(ev.z); 
    indices[axis::pt] = fBins[axis::pt].FindBin(ev.pT); 

    // Is this a safe bin?
    if(IsSafe(indices)){

      // Houston we have liftoff.
      histos[axis::x] [indices[axis::x]] ->Fill(ev.phiHadron);
      histos[axis::q2][indices[axis::q2]]->Fill(ev.phiHadron);
      histos[axis::z] [indices[axis::z]] ->Fill(ev.phiHadron);
      histos[axis::pt][indices[axis::pt]]->Fill(ev.phiHadron);
    }

  }

  // is the index safe ?
  bool IsSafe(std::map<int,int> indices){
    for(std::pair<int,int> index : indices){
      if( (index.second < 0) || (index.second >= fBins[index.first].GetNumber()) ){
	return false; 
      }
    }

    return true;
  }

  void Save(TFile *out){
    
    if (out->IsOpen()){
      out->mkdir("phi");
      out->cd("phi");

      for(int b=0; b<fBins[axis::x].GetNumber(); b++){
	histos[axis::x][b]->Write();
      }
      
      for(int b=0; b<fBins[axis::q2].GetNumber(); b++){
	histos[axis::q2][b]->Write();
      }
      
      for(int b=0; b<fBins[axis::z].GetNumber(); b++){
	histos[axis::z][b]->Write();
      }
      
      for(int b=0; b<fBins[axis::pt].GetNumber(); b++){
	histos[axis::pt][b]->Write();
      }
      
      out->Write();
      out->cd("/");
    }
  }

  // passing the bins means you have to have 
  // the config file but its ok.  it makes life
  // easier in the loading phase.
  void Load(std::map<int, DLineBins> bins, 
	    std::string inputFilenameWithExtension, 
	    std::string title){
    
    // setup things first 
    fBins = bins; 
    
    // setup file 
    fFile = TFile::Open(inputFilenameWithExtension.c_str());

    // load the business 
    for(int b=0; b<fBins[axis::x] .GetNumber(); b++){ histos[axis::x] .push_back( (TH1F*) fFile->Get(Form("phi/phi_x%d_%s", b,title.c_str())) ); }
    for(int b=0; b<fBins[axis::q2].GetNumber(); b++){ histos[axis::q2].push_back( (TH1F*) fFile->Get(Form("phi/phi_q2%d_%s",b,title.c_str())) ); }
    for(int b=0; b<fBins[axis::z] .GetNumber(); b++){ histos[axis::z] .push_back( (TH1F*) fFile->Get(Form("phi/phi_z%d_%s", b,title.c_str())) ); }
    for(int b=0; b<fBins[axis::pt].GetNumber(); b++){ histos[axis::pt].push_back( (TH1F*) fFile->Get(Form("phi/phi_pt%d_%s",b,title.c_str())) ); }

    std::cout << "[PhiHistos::Load] Loaded histograms = " << histos[axis::x] .size() << std::endl; 
    std::cout << "[PhiHistos::Load] Loaded histograms = " << histos[axis::q2].size() << std::endl; 
    std::cout << "[PhiHistos::Load] Loaded histograms = " << histos[axis::z] .size() << std::endl; 
    std::cout << "[PhiHistos::Load] Loaded histograms = " << histos[axis::pt].size() << std::endl; 
  }
  
  void CreateAsymmetry(PhiHistos *plus, PhiHistos *minus){
    
    for(std::pair<int,std::vector<TH1F*> > axis : histos){
      axis.second.clear();
    }

    // should be the same as minus or 
    // it will break 
    fBins = plus->GetBins();

    // for each histogram in each axis do every bin 
    for (int h=0; h<fBins[axis::x].GetNumber(); h++){
      TH1F *currentHisto = new TH1F(Form("phi_x%d_asym",h),"",
				     fBins[axis::phi].GetNumber(), 
				     fBins[axis::phi].GetLimits().data()); 

      MakeAsymmetryHisto(currentHisto, 
			 plus->histos[axis::x][h], 
			 minus->histos[axis::x][h], 0.1); 
      
      // histogram is done 
      histos[axis::x].push_back(currentHisto);
    }

    // for each histogram in each axis do every bin 
    for (int h=0; h<fBins[axis::q2].GetNumber(); h++){
      TH1F *currentHisto = new TH1F(Form("phi_q2%d_asym",h),"",
				     fBins[axis::phi].GetNumber(), 
				     fBins[axis::phi].GetLimits().data()); 

      MakeAsymmetryHisto(currentHisto, 
			 plus->histos[axis::q2][h], 
			 minus->histos[axis::q2][h], 0.1); 
      
      // histogram is done 
      histos[axis::q2].push_back(currentHisto);
    }

    // for each histogram in each axis do every bin 
    for (int h=0; h<fBins[axis::z].GetNumber(); h++){
      TH1F *currentHisto = new TH1F(Form("phi_z%d_asym",h),"",
				     fBins[axis::phi].GetNumber(), 
				     fBins[axis::phi].GetLimits().data()); 

      MakeAsymmetryHisto(currentHisto, 
			 plus->histos[axis::z][h], 
			 minus->histos[axis::z][h], 0.1); 
      
      // histogram is done 
      histos[axis::z].push_back(currentHisto);
    }

    // for each histogram in each axis do every bin 
    for (int h=0; h<fBins[axis::pt].GetNumber(); h++){
      TH1F *currentHisto = new TH1F(Form("phi_pt%d_asym",h),"",
				     fBins[axis::phi].GetNumber(), 
				     fBins[axis::phi].GetLimits().data()); 

      MakeAsymmetryHisto(currentHisto, 
			 plus->histos[axis::pt][h], 
			 minus->histos[axis::pt][h], 0.1); 
      
      // histogram is done 
      histos[axis::pt].push_back(currentHisto);
    }
 
  }

  std::map<int, DLineBins> GetBins() const {
    return fBins; 
  }

  // the histograms have index 
  // axis, heliciy 
  std::map<int, std::vector<TH1F*> > histos; 

 protected:

  void MakeAsymmetryHisto(TH1F *asym, TH1F *plus, TH1F *minus, float pointErrorLimit){
      for(int b=1; b<=plus->GetXaxis()->GetNbins(); b++){
	double p  = plus ->GetBinContent(b);
	double m = minus->GetBinContent(b);
	double sum   = p+m;
	double diff  = p-m;
	double err   = sqrt((1-pow(diff/sum,2))/sum);

	// this should be handled as an input 
	if(sum > 0.0 && err < pointErrorLimit){
	  asym->SetBinContent(b, diff/sum/constants::beam_pol); 
	  asym->SetBinError(b, err); 
	} else {
	  asym->SetBinContent(b,0);
	  asym->SetBinError(b,0);
	}
      }
  }

  std::string               fName; 
  std::map<int, DLineBins>  fBins; 

  TFile * fFile; 
};

#endif
