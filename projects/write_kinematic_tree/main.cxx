#include <iostream>
using namespace std;

#include "Tree.h"
#include "Tree.cxx"
#include "KinematicTree.h"
#include "KinematicTree.cxx"

#include "h22Option.h"
#include "Parameters.h"

int main(int argc, char *argv[]){

  h22Options opts; 

  Parameters pars, parsloose, parstight; 
  pars.loadParameters("/u/home/dmriser/analysis-main/lists/data_tofmass.pars"); 
  parsloose.loadParameters("/u/home/dmriser/analysis-main/lists/dataLoose.pars"); 
  parstight.loadParameters("/u/home/dmriser/analysis-main/lists/dataTight.pars"); 
  
  opts.set(argc, argv);

  if (argc < 2){ return 0; }

  KinematicTree treeWriter(&opts, &pars, &parstight, &parsloose, 0); 
  for (int i=0; i<opts.ifiles.size(); ++i) { treeWriter.AddFile( opts.ifiles[i] ); }
  treeWriter.RunAnalysis();  
  treeWriter.Save(opts.args["OUT"].args); 

  return 0;
}
