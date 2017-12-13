#include <iostream>
#include <vector>
using std::cout;
using std::endl;

// MyLibs
#include "h22Option.h"
#include "Parameters.h"

int main(int argc, char *argv[]){

  // Set options 
  h22Options options;
  options.args["OUT"].args = "UNSET";
  options.set(argc, argv);

  // Check for proper use
  if (options.args["OUT"].args == "UNSET" || options.ifiles.size() < 2){
    cout << "[ComebineParams] Proper Usage: -OUT=File.pars input1.pars input2.pars ... " << endl;
  } 

  else {
    Parameters pars;
    for(int ifile=0; ifile<options.ifiles.size(); ++ifile){
      pars.loadParameters(options.ifiles[ifile]);
      cout << "[ComebineParams] Adding file: " << options.ifiles[ifile] << endl;
    }
    
    pars.saveParameters(options.args["OUT"].args);
    cout << "[ComebineParams] Output saved to: " << options.args["OUT"].args << endl;
  }

  return 0; 
}
