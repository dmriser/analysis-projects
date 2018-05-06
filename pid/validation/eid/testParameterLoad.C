#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "utils.C"

void test(){

  // to load parameters 
  //  gROOT->LoadMacro("utils.C");

  // the file 
  std::string filePath = "/u/home/dmriser/clas/analysis-main/lists/parameters/data/final.pars";

  // try to load them 
  std::map<std::string, std::vector<float> > parameters = loadParameterFile(filePath);


  // try to print something 
  for(float x : parameters["EL_SF_MU_A"]){
    std::cout << x << std::endl; 
  }
}
