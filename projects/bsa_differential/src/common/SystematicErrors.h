#ifndef sys_error_h
#define sys_error_h

#include "Bins.h"
#include "Histograms.h"

class SystematicError {
 public:

  // used for creating and saving 
 SystematicError(Histos *nom, Histos *var, std::string name, int index) : fName(name), fNomHistos(nom), fVarHistos(var), fMesonIndex(index) {
    fDiffHistos = new Histos(fName, fMesonIndex);     
    fBins = Bins::GetInstance(fMesonIndex); 
  }

  // used for loading 
 SystematicError(std::string name, int index) : fName(name), fMesonIndex(index) {
    fDiffHistos = new Histos(fName, fMesonIndex);     
    fBins = Bins::GetInstance(fMesonIndex); 
  }

 protected:
  std::string  fName; 
  Bins        *fBins; 
  int          fMesonIndex; 
  Histos      *fNomHistos; 
  Histos      *fVarHistos; 
  Histos      *fDiffHistos; 

};

#endif
