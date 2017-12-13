#ifndef FC_FILEIO_CXX
#define FC_FILEIO_CXX

// c++ standard 
#include <iostream>
#include <fstream>
#include <vector>
using std::cout; 
using std::endl; 
using std::string;
using std::ifstream;
using std::ofstream;

// From this code
#include "Entry.cxx"
#include "File.cxx"
#include "Run.cxx"

// String manipulation from ROOT
#include "TString.h"
#include "TRegexp.h"

class FileIO{
public:
  FileIO(){}
  ~FileIO(){}

protected:
  Run run;

public:
  void loadFile(TString filePath);

};

void FileIO::loadFile(TString filePath){
  
  ifstream inputFile(filePath);

  if(inputFile.is_open()){
    TRegex runnoRegex("[1-9][0-9][0-9][0-9][0-9][0-9]");
    TRegex fileNumberRegex(".A[0-9][0-9]");
    TString runnoString      = runnoRegex(filePath);
    TString fileNumberString = fileNumberRegex(filePath);

    cout << "[FileIO::loadFile] Found run number " << runnoString << " " << fileNumberString << endl;

    inputFile.close();
  } else {
    cout << "[FileIO::loadFile] Error: Failed to open " << filePath << endl;
  }

}

#endif

