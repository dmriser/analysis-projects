#include <vector>

#include "TObject.h"
#include "TFile.h"
#include "TRegexp.h"
#include "TString.h"
#include "TList.h"
#include "TF1.h"
#include "TH1.h"

std::vector<TH1D*> loadHistos(TFile *inputFile, string targetTitle){

  TList * tableOfContents = inputFile->GetListOfKeys();
  TIter next(tableOfContents);
  std::vector<TH1D*> foundHistos;

  TRegexp reg(targetTitle);
  TObject *objFromFile;

  while(objFromFile = next()){
    TString currentObjectName = objFromFile->GetName();

    if (currentObjectName.Contains(reg)){
      foundHistos.push_back( (TH1D*) inputFile->Get(currentObjectName) );
      cout << "[loadHistos] Found " << currentObjectName << endl;
    }
  }

  return foundHistos;
}

std::vector<TF1*> loadFits(TFile *inputFile, string targetTitle){

  TList * tableOfContents = inputFile->GetListOfKeys();
  TIter next(tableOfContents);
  std::vector<TF1*> foundHistos;

  TRegexp reg(targetTitle);
  TObject *objFromFile;

  while(objFromFile = next()){
    TString currentObjectName = objFromFile->GetName();

    if (currentObjectName.Contains(reg)){
      foundHistos.push_back( (TF1*) inputFile->Get(currentObjectName) );
      cout << "[loadFits] Found " << currentObjectName << endl;
    }
  }

  return foundHistos;
}
