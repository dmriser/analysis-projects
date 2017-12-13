#ifndef FC_FILE_CXX
#define FC_FILE_CXX

#include <iostream>
#include <vector>
using std::cout; 
using std::endl; 
using std::vector;

#include "Entry.cxx"


class File{

public:
  File(){ currentEntryNumber = 0; }
  ~File(){}

protected:
  int runNumber, fileNumber, currentEntryNumber;
  vector<Entry> entries;

public:
  void setRunNumber(int rn){ runNumber=rn; }
  void setFileNumber(int fn){ fileNumber=fn; }
  int getEntries(){ return entries.size(); }
  Entry getEntry(int index);  
  Entry getCurrentEntry();
  bool hasEntry(); 
  void addEntry(Entry entry){ entries.push_back(entry); }
  void clearEntries(){ entries.clear(); }
  int getRunNumber(){ return runNumber; }
  int getFileNumber(){ return fileNumber; }
};

Entry File::getEntry(int index){
  if (index < getEntries()){
    return entries[index];
  } else {
    Entry e;  e.Q = 0; e.N = 0;
    cout << "[File::getEntry] Error: Index exceeds current number of entries! " << endl; 
    return e;
  }
}

bool File::hasEntry(){
  if(currentEntry < getEntries()){ 
    currentEntry++;
    return true; 
  }

  currentEntry = 0;
  return false; 
}

Entry File::getCurrentEntry(){
  return entries[currentEntry];
}

#endif
