#include <iostream>
using namespace std;

#include "Tree.h"
#include "Tree.cxx"
#include "FillMonteCarlo.h"
#include "FillMonteCarlo.cxx"

#include "h22Option.h"
#include "Parameters.h"

int main(int argc, char *argv[]){

  h22Options opts; 

  Parameters pars, parsloose, parstight; 
  pars.loadParameters("/u/home/dmriser/Analysis_v2/lists/clasdis.pars"); 
  parsloose.loadParameters("/u/home/dmriser/Analysis_v2/lists/clasdis_loose.pars"); 
  parstight.loadParameters("/u/home/dmriser/Analysis_v2/lists/clasdis_tight.pars"); 
  
  opts.set(argc, argv);

  if (argc < 2){ return 0; }

  FillMonteCarlo treeWriter(&opts, &pars, &parstight, &parsloose); 
  for (int i=0; i<opts.ifiles.size(); ++i) { treeWriter.AddFile( opts.ifiles[i] ); }
  treeWriter.RunAnalysis();  
  treeWriter.Save(opts.args["OUT"].args); 
  
  treeWriter.getFilter()->electronSelector->PrintSummary();   
  treeWriter.getFilter()->positivePionSelector->PrintSummary();   
  treeWriter.getFilter()->positiveMesonCandidateSelector->PrintSummary();   

  return 0;
}
