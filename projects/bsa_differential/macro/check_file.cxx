bool checkFile(std::string file){
  TFile *f = TFile::Open(file.c_str());


  if (f){
    if (f->IsOpen()){


      if(gDirectory->cd("integrated")){
	std::cout << file << ", Status = GOOD" << std::endl;
      } else {
	std::cout << file << ", Status = BAD" << std::endl;
      }

    } else {

      std::cout << file << ", Status = BAD" << std::endl;
      return false;
    }

    return false; 
  }


  // should not get here
  return false; 
}
