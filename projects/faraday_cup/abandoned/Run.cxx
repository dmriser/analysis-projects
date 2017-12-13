#ifndef FC_RUN_CXX
#define FC_RUN_CXX

#include <iostream>
#include <vector>
using std::cout; 
using std::endl;
using std::vector; 

#include "Entry.cxx"
#include "File.cxx"

class Run{

public:
  Run(){ currentFile = 0; }
  ~Run(){}

protected:
  int runNumber, currentFile;
  vector<File> files;

public:
  void addFile(File f){ files.push_back(f); }
  void clearFiles(){ files.clear(); }
  void setRunNumber(int r){ runNumber=r; }
  
  bool allFilesAreSameRun();
  int getNumberOfFiles(){ return files.size(); }
  int getRunNumber(){ return runNumber; }

  bool hasFiles();
  File getCurrentFile();
  File getFile(int index);
};

bool Run::hasFiles(){
  bool result = true;
  if(currentFile < getNumberOfFiles()){
    currentFile++;
    return true;
  } else {
    currentFile = 0;
    return false;
  }
}

File Run::getCurrentEntry(){
  return files[currentFile];
}

File Run::getFile(int index){
  if(index < getNumberOfFiles){ 
    return files[index]; 
  } else{ 
    cout << "[Run::getFile] Error: Requested file exceeds number of files! " << endl;
    File f; return f; 
  }
}

bool Run::allFilesAreSameRun(){
  bool result = true; 

  if(getNumberOfFiles() > 0){
    int currentRunNumber = files[0].getRunNumber();
    for(int ifile=0; ifile<getNumberOfFiles(); ifile++){
      if (files[ifile].getRunNumber() != currentRunNumber){
	result = false;
      }
    }
  }

  return result;
}



#endif
