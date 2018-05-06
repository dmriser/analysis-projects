#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

std::vector<std::string> parseLine(std::string line){
  std::vector<std::string> tokenizedLine;
  std::string buffer;

  istringstream tokens(line);
  while(!tokens.eof())
    {
      string buffer;
      tokens >> buffer;
      tokenizedLine.push_back(buffer);
    }

  return tokenizedLine;
}

std::map<std::string, std::vector<float> > loadParameterFile(std::string inputFilename){

  std::map<std::string, std::vector<float> > parameters; 

  ifstream file(inputFilename.c_str());

  std::string currentLine;
  while(getline(file, currentLine)){

    // Trying to skip whitespace lines
    if (currentLine.size() > 10 && currentLine[0] != '#'){

      vector<std::string> line = parseLine(currentLine);
      getline(file, currentLine);
      vector<std::string> nextLine = parseLine(currentLine);

      std::string parameterName = line[0];
      int numberOfValues = line.size()-3;
      int numberOfErrors = nextLine.size()-3;

      for (int i=0; i<numberOfValues; i++){ parameters[parameterName].push_back( atof(line[i+3].c_str()) ); } // Setting values
    }
  }

  file.close();

  return parameters; 
}

void setupSamplingFractionBounds(std::map<std::string, std::vector<float> > params, 
		     TF1 *mean[6], TF1 *min[6], TF1 *max[6], float xMin, float xMax){

  // this is the width 
  float NSIGMA = params["EL_EC_NSIGMA"][0];

  // load 
  for(int sect=0; sect<6; sect++){
    mean[sect] = new TF1(Form("sf_mean_%d", sect), "pol3", xMin, xMax);
    min[sect]  = new TF1(Form("sf_min_%d", sect), "pol3",  xMin, xMax);
    max[sect]  = new TF1(Form("sf_max_%d", sect), "pol3",  xMin, xMax);

    mean[sect]->SetParameter(3, params["EL_SF_MU_A"][sect]);
    mean[sect]->SetParameter(2, params["EL_SF_MU_B"][sect]);
    mean[sect]->SetParameter(1, params["EL_SF_MU_C"][sect]);
    mean[sect]->SetParameter(0, params["EL_SF_MU_D"][sect]);
    min[sect]->SetParameter(3, params["EL_SF_MU_A"][sect]-NSIGMA*params["EL_SF_SIGMA_A"][sect]);
    min[sect]->SetParameter(2, params["EL_SF_MU_B"][sect]-NSIGMA*params["EL_SF_SIGMA_B"][sect]);
    min[sect]->SetParameter(1, params["EL_SF_MU_C"][sect]-NSIGMA*params["EL_SF_SIGMA_C"][sect]);
    min[sect]->SetParameter(0, params["EL_SF_MU_D"][sect]-NSIGMA*params["EL_SF_SIGMA_D"][sect]);
    max[sect]->SetParameter(3, params["EL_SF_MU_A"][sect]+NSIGMA*params["EL_SF_SIGMA_A"][sect]);
    max[sect]->SetParameter(2, params["EL_SF_MU_B"][sect]+NSIGMA*params["EL_SF_SIGMA_B"][sect]);
    max[sect]->SetParameter(1, params["EL_SF_MU_C"][sect]+NSIGMA*params["EL_SF_SIGMA_C"][sect]);
    max[sect]->SetParameter(0, params["EL_SF_MU_D"][sect]+NSIGMA*params["EL_SF_SIGMA_D"][sect]);
  }

  std::cout << "Sampling fraction parameters loaded from file." << std::endl; 
}

void setupCCThetaBounds(std::map<std::string, std::vector<float> > params, 
		     TF1 *mean[6], TF1 *min[6], TF1 *max[6], float xMin, float xMax){

  // this is the width 
  float NSIGMA = params["EL_CCT_NSIGMA"][0];

  // load 
  for(int sect=0; sect<6; sect++){
    mean[sect] = new TF1(Form("cct_mean_%d", sect), "pol3", xMin, xMax);
    min[sect]  = new TF1(Form("cct_min_%d", sect), "pol3",  xMin, xMax);
    max[sect]  = new TF1(Form("cct_max_%d", sect), "pol3",  xMin, xMax);

    mean[sect]->SetParameter(3, params["EL_CCT_MU_A"][sect]);
    mean[sect]->SetParameter(2, params["EL_CCT_MU_B"][sect]);
    mean[sect]->SetParameter(1, params["EL_CCT_MU_C"][sect]);
    mean[sect]->SetParameter(0, params["EL_CCT_MU_D"][sect]);
    min[sect]->SetParameter(3, params["EL_CCT_MU_A"][sect]-NSIGMA*params["EL_CCT_SIGMA_A"][sect]);
    min[sect]->SetParameter(2, params["EL_CCT_MU_B"][sect]-NSIGMA*params["EL_CCT_SIGMA_B"][sect]);
    min[sect]->SetParameter(1, params["EL_CCT_MU_C"][sect]-NSIGMA*params["EL_CCT_SIGMA_C"][sect]);
    min[sect]->SetParameter(0, params["EL_CCT_MU_D"][sect]-NSIGMA*params["EL_CCT_SIGMA_D"][sect]);
    max[sect]->SetParameter(3, params["EL_CCT_MU_A"][sect]+NSIGMA*params["EL_CCT_SIGMA_A"][sect]);
    max[sect]->SetParameter(2, params["EL_CCT_MU_B"][sect]+NSIGMA*params["EL_CCT_SIGMA_B"][sect]);
    max[sect]->SetParameter(1, params["EL_CCT_MU_C"][sect]+NSIGMA*params["EL_CCT_SIGMA_C"][sect]);
    max[sect]->SetParameter(0, params["EL_CCT_MU_D"][sect]+NSIGMA*params["EL_CCT_SIGMA_D"][sect]);
  }

  std::cout << "CC-Theta parameters loaded from file." << std::endl; 
}
