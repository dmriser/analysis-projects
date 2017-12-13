#include "FileIO.cxx"
#include "Entry.cxx"
#include "File.cxx"
#include "Run.cxx"


int main(int argc, char *argv[]){

  FileIO fileLoader; 

  fileLoader.loadFile("/volatile/clas12/dmriser/analysis/e1f_analysis/fcup/clas_038222.A02.fcup");

  return 0;
}
