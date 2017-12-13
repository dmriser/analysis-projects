
#include <iostream>
#include <vector>

#include "CommonTools.h"
#include "StandardHistograms.h"

#include "TH1.h"

void FindBins(TH1D *h, int nbins, float min, float max, float tolerance);

int main(int nargs, char *args[]){

  
  if (nargs < 2){
    std::cout << " Please pass in the root file which contains standard histograms. " << std::endl; 
    return 0;
  }
  
  
  std::string inputFile(args[1]); 

  StandardHistograms kp("kp", 1); 
  kp.Load(inputFile);

  FindBins(kp.h1_xbj[0], 4, 0.1, 0.6, 0.05);
  FindBins(kp.h1_z[0], 6, 0.0, 1.0, 0.05);
  FindBins(kp.h1_pt2[0], 4, 0.0, 1.0, 0.05);
  FindBins(kp.h1_q2[0], 6, 1.0, 5.0, 0.05);

  StandardHistograms km("km", 1); 
  km.Load(inputFile);

  FindBins(km.h1_xbj[0], 3, 0.1, 0.6, 0.05);
  FindBins(km.h1_z[0], 3, 0.0, 0.7, 0.05);
  FindBins(km.h1_pt2[0], 3, 0.0, 1.0, 0.05);

  StandardHistograms pp("pp", 1); 
  pp.Load(inputFile);

  FindBins(pp.h1_xbj[0], 6, 0.1, 0.6, 0.1);
  FindBins(pp.h1_z[0], 6, 0.1, 0.7, 0.1);
  FindBins(pp.h1_pt2[0], 6, 0.0, 1.0, 0.1);

  StandardHistograms pm("pm", 1); 
  pm.Load(inputFile);

  FindBins(pm.h1_xbj[0], 4, 0.1, 0.6, 0.05);
  FindBins(pm.h1_z[0], 4, 0.1, 0.7, 0.05);
  FindBins(pm.h1_pt2[0], 4, 0.0, 1.0, 0.05);
 
  return 0;
}

void FindBins(TH1D *h, int nbins, float min, float max, float tolerance){
  
  std::vector<float> binLimits; 
  std::vector<float> binContent; 
  binLimits.push_back(min); 

  int startBin = h->GetXaxis()->FindBin(min);
  int stopBin = h->GetXaxis()->FindBin(max);

  if (stopBin > startBin && (stopBin-startBin) > nbins){
    
    float total = 0.0; 
    for (int b=startBin; b<=stopBin; b++){
      total += h->GetBinContent(b); 
    }

    // setup target number of entries per bin 
    // and the stopping criteria target +/- target*tolerance
    float target = total/nbins; 
    float targetTolerance = target*tolerance; 
    std::cout << "[FindBin] trying to get entries = " << target << " for each bin. " << std::endl;

    float binTotal = 0.0; 
    for(int b=startBin; b<stopBin; b++){
      if (binLimits.size() == nbins){
	binLimits.push_back(max);


	float theRest = total;
	for (int b=0; b<binContent.size(); b++){ 
	  theRest -= binContent[b]; 
	}

	binContent.push_back(theRest); 
	break;
      }

      binTotal += h->GetBinContent(b);
      
      if (fabs(target-binTotal) < targetTolerance || binTotal > target){
	binLimits.push_back(h->GetBinCenter(b)); 
	binContent.push_back(binTotal); 
	binTotal = 0.0; 
      }
    }

    if (binLimits.size() != nbins+1){
      std::cerr << "Something bad happened" << std::endl; 
    } else {
      std::cout << "Found binning for " << h->GetName();
      for(int b=0; b<binLimits.size(); b++){
	std::cout.width(14); std::cout << binLimits[b]; 
      }
      std::cout << std::endl; 
      std::cout << "With fills: ";
      for(int b=0; b<binContent.size(); b++){
	std::cout.width(14); std::cout << binContent[b]; 
      }	
      std::cout << std::endl; 
    }

  } else {
    std::cerr << "[FindBins] Problem finding limits for the histogram axis " << std::endl; 
  }
}
