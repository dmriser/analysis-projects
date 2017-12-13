#include <iostream>
#include <fstream>
#include <vector>

#include "DBins.h"

#include "CommonTools.h"
#include "StandardHistograms.h"

#include "common/Config.h"
#include "common/ConfigLoader.h"
#include "common/ConfigLoader.cxx"

#include "TH1.h"

#include "common/json.hpp"
using namespace nlohmann; 

struct BinnedAxis {
  int                number; 
  float              min, max; 
  std::vector<float> limits; 
};

inline void ApplySqrt(std::vector<float> &x){
  for(int i=0; i<x.size(); i++){
    x[i] = sqrt(x[i]);
  }
}

int main(int argc, char *argv[]){

  
  if (argc < 2){
    std::cout << "Please pass in configuration files." << std::endl; 
    return 0;
  }
  
  std::vector<Config> configs; 
  for(int iarg=1; iarg<argc; iarg++){
    std::string configFile(argv[iarg]); 
    
    configs.push_back( ConfigLoader::getConfiguration(configFile) ); 
  }

  for(Config c : configs){
    StandardHistograms histos("PassAll", 1); 
    histos.Load(Form("%s/%s/histos.root",c.analysisHome.c_str(),c.name.c_str()));

    std::map<int, BinnedAxis> binnedAxes; 
    binnedAxes[axis::x].number = Global::BinningTools::GetNumber((TH1F*) histos.h1_xbj[0]);
    binnedAxes[axis::x].min    = Global::BinningTools::GetQuantile((TH1F*) histos.h1_xbj[0], 0.01); 
    binnedAxes[axis::x].max    = Global::BinningTools::GetQuantile((TH1F*) histos.h1_xbj[0], 0.99); 
    binnedAxes[axis::x].limits = Global::BinningTools::GetBins((TH1F*) histos.h1_xbj[0],
							       ceil(binnedAxes[axis::x].number/c.axes[axis::phi].GetNumber()), 
							       binnedAxes[axis::x].min, 
							       binnedAxes[axis::x].max, 
							       0.3); 
    /* 

    binnedAxes[axis::q2].number = Global::BinningTools::GetNumber((TH1F*) histos.h1_xbj[0]);
    binnedAxes[axis::q2].min    = Global::BinningTools::GetQuantile((TH1F*) histos.h1_xbj[0], 0.01); 
    binnedAxes[axis::q2].max    = Global::BinningTools::GetQuantile((TH1F*) histos.h1_xbj[0], 0.99); 
    binnedAxes[axis::q2].limits = Global::BinningTools::GetBins((TH1F*) histos.h1_xbj[0],
								ceil(bins/conf.axes[axis::phi].GetNumber()), 
								binnedAxes[axis::x].min, 
								binnedAxes[axis::x].max, 
								0.3); 
    */

    binnedAxes[axis::z].number = Global::BinningTools::GetNumber((TH1F*) histos.h1_z[0]);
    binnedAxes[axis::z].min    = Global::BinningTools::GetQuantile((TH1F*) histos.h1_z[0], 0.01); 
    binnedAxes[axis::z].max    = Global::BinningTools::GetQuantile((TH1F*) histos.h1_z[0], 0.99); 
    binnedAxes[axis::z].limits = Global::BinningTools::GetBins((TH1F*) histos.h1_z[0],
							       ceil(binnedAxes[axis::z].number/c.axes[axis::phi].GetNumber()), 
							       binnedAxes[axis::z].min, 
							       binnedAxes[axis::z].max, 
							       0.2); 

    binnedAxes[axis::pt].number = Global::BinningTools::GetNumber((TH1F*) histos.h1_pt2[0]);
    binnedAxes[axis::pt].min    = Global::BinningTools::GetQuantile((TH1F*) histos.h1_pt2[0], 0.01); 
    binnedAxes[axis::pt].max    = Global::BinningTools::GetQuantile((TH1F*) histos.h1_pt2[0], 0.99); 
    binnedAxes[axis::pt].limits = Global::BinningTools::GetBins((TH1F*) histos.h1_pt2[0],
							       ceil(binnedAxes[axis::pt].number/c.axes[axis::phi].GetNumber()), 
							       binnedAxes[axis::pt].min, 
							       binnedAxes[axis::pt].max, 
							       0.2); 

    // take square root 
    ApplySqrt(binnedAxes[axis::pt].limits);

    // print the number of bins for each configuration 
    std::cout << "For configuration = " << c.name << " bins = " 
	      << ceil(binnedAxes[axis::x].number/c.axes[axis::phi].GetNumber()) 
	      << " in range [" << binnedAxes[axis::x].min << "," 
	      << binnedAxes[axis::x].max << "]" << std::endl; 

    std::cout << "For configuration = " << c.name << " bins = " 
	      << ceil(binnedAxes[axis::z].number/c.axes[axis::phi].GetNumber()) 
	      << " in range [" << binnedAxes[axis::z].min << "," 
	      << binnedAxes[axis::z].max << "]" << std::endl; 

    std::cout << "For configuration = " << c.name << " bins = " 
	      << ceil(binnedAxes[axis::pt].number/c.axes[axis::phi].GetNumber()) 
	      << " in range [" << binnedAxes[axis::pt].min << "," 
	      << binnedAxes[axis::pt].max << "]" << std::endl; 

    /* 
    // print the results 
    std::cout << "[";
    for(float f : binnedAxes[axis::x].limits){
      std::cout << f << ", "; 
    }
    std::cout << "]" << std::endl;

    std::cout << "[";
    for(float f : binnedAxes[axis::z].limits){
      std::cout << f << ", "; 
    }
    std::cout << "]" << std::endl;

    std::cout << "[";
    for(float f : binnedAxes[axis::pt].limits){
      std::cout << f << ", "; 
    }
    std::cout << "]" << std::endl;
    */ 

    json j; 
    j["bins"]["x"]  = binnedAxes[axis::x] .limits; 
    j["bins"]["z"]  = binnedAxes[axis::z] .limits; 
    j["bins"]["pt"] = binnedAxes[axis::pt].limits; 

    std::ofstream o(Form("/u/home/dmriser/Analysis_v2/projects/asym2/binning/%s.json", c.name.c_str()));
    o << std::setw(4) << j << std::endl;
    o.close();

  }
 
  return 0;
}
