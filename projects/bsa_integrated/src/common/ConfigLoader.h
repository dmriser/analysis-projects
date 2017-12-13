#ifndef config_loader_h
#define config_loader_h

// standard libraries 
#include <iostream>

// external non-standard codes
#include "json.hpp"
using namespace nlohmann;

// this library 
#include "Config.h"

class ConfigLoader {

 public:
  static Config getConfiguration(std::string jsonFile); 

};

#endif
