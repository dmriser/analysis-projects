#ifndef config_h 
#define config_h 

// standard library includes 
#include <map>
#include <string>
#include <vector>

// binning tools 
#include "DBins.h"

// this project 
#include "Types.h"

// configuration structure 
struct Config {
  std::string name, outputPath, fileList, parameterFile, analysisHome; 
  int filesToProcess, mesonIndex, writeTree, numberFitPars; 

  std::map<std::string, std::map<std::string, double>> cuts; 
  std::map<std::string, std::vector<double>>           bins; 
  std::map<std::string, std::string>                   fitter;   

  // weird 
  std::map<int, DLineBins>                             axes; 
};

#endif 
