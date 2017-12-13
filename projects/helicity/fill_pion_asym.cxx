////////////////////////////////////////
/*
 David Riser, University of Connecticut
 
 July 13, 2016
 
 Modified:
 March 8, 2017
 April 14, 2017 

 meson.cxx -> fill histograms for mesons 
 
 */
////////////////////////////////////////

#include <iostream>
#include <vector>
#include <map>

#include "CommonTools.h"
#include "Corrections.h"
#include "h22Event.h"
#include "h22Option.h"
#include "GenericAnalysis.h"
#include "MomCorr.h"
#include "Parameters.h"
#include "ParticleFilter.h"
#include "PhysicsEvent.h"
#include "PhysicsEventBuilder.h"
#include "StandardHistograms.h"

#include "TFile.h"
#include "TLorentzVector.h"
#include "TF1.h"
#include "TH1.h"

#define BEAM_POL 0.751

std::vector<std::string> loadFilesFromList(std::string fileList, int numFiles);
std::vector<std::string> loadFilesFromCommandLine(h22Options * theseOpts, int numFiles);

class AsymmetryCheck {
public:
  AsymmetryCheck(int run) : fRun(run), fAsymValue(0.0), fTotalMinus(0),
			    fTotalPlus(0), fSinPlus(0.0), fSinMinus(0.0), 
			    fSinMinusMoment(0.0), fSinPlusMoment(0.0) {
    InitHistos(); 
  }

  int GetRun() const {
    return fRun; 
  }

  void InitHistos(){
    phi_plus  = new TH1D(Form("h1_phiH_plus_run%d", fRun), Form("h1_phiH_plus_run%d", fRun),   18, -180, 180); 
    phi_minus = new TH1D(Form("h1_phiH_minus_run%d", fRun), Form("h1_phiH_minus_run%d", fRun), 18, -180, 180); 
    asym      = new TH1D(Form("h1_phiH_asym_run%d", fRun), Form("h1_phiH_asym_run%d", fRun),   18, -180, 180); 
    fit       = new TF1(Form("f_phiH_run%d",fRun), "[0]*sin((3.14159/180)*x)", -180, 180); 
  }

  double GetAsymmetry() const {
    return fAsymValue; 
  }

  double GetAsymmetryError() const {
    return fAsymError; 
  }

  double GetSinMoment(int hel) const {
    return (hel < 0 ? fSinMinusMoment : fSinPlusMoment); 
  }

  void Fill(PhysicsEvent &ev, int hel){
    if (hel > 0){
      phi_plus->Fill(ev.phiHadron); 
      fTotalPlus++; 
      fSinPlus += sin(ev.phiHadron *to_radians); 
    } else if (hel < 0) {
      phi_minus->Fill(ev.phiHadron); 
      fTotalMinus++; 
      fSinMinus += sin(ev.phiHadron *to_radians); 
    }
  }

  void MakeAsymmetry(){

    int xbins = asym->GetXaxis()->GetNbins(); 

    for (int b=1; b<=xbins; b++){
      double plus  = phi_plus->GetBinContent(b); 
      double minus = phi_minus->GetBinContent(b); 

      double sum = plus+minus; 
      double diff = plus-minus; 

      double err = sqrt( (pow(diff,2)/pow(sum,3)) + (1/sum) );

      if (sum > 0.0) { 
	asym->SetBinContent(b,diff/sum/BEAM_POL);
	asym->SetBinError(b, err); 
      }
    }
    
    //    fit->SetParameter(0, 0.03); 

    asym->Fit(Form("f_phiH_run%d",fRun), "RQ");
    fAsymValue = fit->GetParameter(0); 
    fAsymError = fit->GetParError(1); 

    // set the other way 
    fSinPlusMoment  = (double) fSinPlus/fTotalPlus/BEAM_POL; 
    fSinMinusMoment = (double) fSinMinus/fTotalMinus/BEAM_POL; 

  }

  void Save(TFile *f){
    if (f->IsOpen()){
      phi_plus ->Write(); 
      phi_minus->Write(); 
      asym     ->Write(); 
      fit      ->Write(); 
      std::cout << "Saving to file run " << fRun << std::endl; 
    } else {
      std::cerr << "File isnt open for writing asymmetry histos." << std::endl; 
    }
  }

  TH1D *phi_plus;
  TH1D *phi_minus;
  TH1D *asym; 
  TF1  *fit; 

protected:
  int fRun; 
  double fAsymValue, fAsymError; 
  
  long int fTotalMinus, fTotalPlus; 
  double fSinMinus, fSinPlus; 
  double fSinMinusMoment, fSinPlusMoment; 
  
};

class Analysis : public GenericAnalysis {

public:
    Analysis(h22Options *opts, Parameters *params) : GenericAnalysis(opts), pars(params) { }
    ~Analysis(){ }

public:
  Parameters          *pars;
  ParticleFilter      *filter;
  Corrections          corr;
  PhysicsEventBuilder *builder;   
  MomCorr_e1f         *momCorr; 
  AsymmetryCheck      *asymmetry; 

  void Initialize();
  void ProcessEvent();
  void Save(std::string outfile);
  void DoSave(std::string outfile); 
  void InitHistos();
  bool CurrentParticleIsNotElectronCandidate(std::vector<int> &electronCandidates, int index);

  double GetAsymmetry() const {
    return asymmetry->GetAsymmetry(); 
  }

  double GetAsymmetryError() const {
    return asymmetry->GetAsymmetryError(); 
  }

  int GetRun() const {
    return asymmetry->GetRun(); 
  }

  

};

void Analysis::InitHistos() {
  int run = GetRunNumber(); 
  asymmetry = new AsymmetryCheck(run); 
}

void Analysis::Initialize(){
  InitHistos();
  
  filter = new ParticleFilter(pars);
  filter->set_info(GSIM, GetRunNumber());

  builder = new PhysicsEventBuilder(); 

  // not really safe for use on the farm because I don't set vars 
  // so not sure what to do about this.  maybe passing in the path
  // as a parameter would be ok? 
  std::string path = Global::Environment::GetAnalysisPath(); 
  std::string momCorrPath = Form("%s/momCorr/",path.c_str());
  momCorr = new MomCorr_e1f(momCorrPath);
}

void Analysis::ProcessEvent(){
    // Load up hadrons if we've electron.
  std::vector<int> electronCandidates = filter->getVectorOfParticleIndices(event, 11);

  if ( !electronCandidates.empty()){
    
    // Take the fastest one
    int electronIndex = electronCandidates[0];
    event.SetElectronIndex(electronIndex); 
    corr.correctEvent(&event, GetRunNumber(), GSIM); 
    std::vector<int> kpCandidates = filter->getVectorOfParticleIndices(event, -321);

    if (!kpCandidates.empty()) {
    TLorentzVector electron  = event.GetTLorentzVector(electronIndex, 11); 
    electron = momCorr->PcorN(electron, -1, 11);    

    TLorentzVector kaon = event.GetTLorentzVector(kpCandidates[0], -321);
    //    pion = momCorr->PcorN(pion, 1, 211);

    PhysicsEvent candidateEvent = builder->getPhysicsEvent(electron, kaon);
    
    if (candidateEvent.w > 2.00 && candidateEvent.qq > 1.00 && candidateEvent.mm2 > 1.1
	&& candidateEvent.z > 0.3 && kaon.P() < 2.75) {
      asymmetry->Fill(candidateEvent, event.corr_hel); 
    }
  }
  }
}


bool Analysis::CurrentParticleIsNotElectronCandidate(std::vector<int> &electronCandidates,int index){
  return (std::find(electronCandidates.begin(), electronCandidates.end(), index) == electronCandidates.end());
}

void Analysis::Save(std::string outfile){
  /*
  asymmetry->MakeAsymmetry(); 

  TFile *f = new TFile(outfile.c_str(), "UPDATE"); 

  if (f->IsOpen()){
    asymmetry->Save(f); 
  } else {
    std::cerr << "[Analysis::Save] Output root file couldn't be opened. " << std::endl; 
  }

  //  f->Write(); 
  f->Close(); 
  */
}

void Analysis::DoSave(std::string outfile){
  asymmetry->MakeAsymmetry(); 

  TFile *f = new TFile(outfile.c_str(), "update"); 
 
  if (f->IsOpen()){
    asymmetry->Save(f); 
  } else {
    std::cerr << "[Analysis::Save] Output root file couldn't be opened. " << std::endl; 
  }

  //  f->Write(); 
  f->Close(); 


  filter->GetSelector(321)->PrintSummary();
}

struct AsymmetryResult {
  int    run; 
  double value; 
  double error; 
  double plus_moment; 
  double minus_moment; 
};

int main(int argc, char * argv[]){
  
  // Setup Options
  h22Options opts;
  opts.args["PARS"].args = "/u/home/dmriser/analysis-main/lists/data.pars";
  opts.args["PARS"].type = 1;
  opts.args["PARS"].name = "Parameter file";
  opts.args["LIST"].args = "UNSET";
  opts.args["LIST"].type = 1;
  opts.args["LIST"].name = "List of files";
  opts.args["MODE"].args = "RUN";
  opts.args["MODE"].type = 1;
  opts.args["MODE"].name = "Run-Mode";
  opts.set(argc,argv);

  Parameters pars;
  pars.loadParameters(opts.args["PARS"].args);
  
  std::vector<std::string> files; 
  if (opts.args["LIST"].args != "UNSET"){
    files = loadFilesFromList(opts.args["LIST"].args, 10000);
  } else {
    files = loadFilesFromCommandLine(&opts, 10000);
  }
  
  if (opts.args["MODE"].args == "RUN") {
    std::vector<AsymmetryResult> results; 

    for (int i=0; i<files.size(); i++) {   
      Analysis *Ana = new Analysis(&opts, &pars); 
      Ana->AddFile(files[i]); 
      Ana->RunAnalysis();
      Ana->DoSave("Asymmetry.root"); 

      AsymmetryResult result; 
      result.run   = Ana->GetRun(); 
      result.value = Ana->GetAsymmetry(); 
      result.error = Ana->GetAsymmetryError();
      result.plus_moment = Ana->asymmetry->GetSinMoment(1); 
      result.minus_moment = Ana->asymmetry->GetSinMoment(-1); 
      results.push_back(result); 

      delete Ana; 
    }

    // save results 
    std::ofstream resultFile; 
    resultFile.open("asymmetry_results.dat",std::ios::trunc); 
    
    for (int i=0; i<results.size(); i++){
      resultFile.width(10); resultFile << results[i].run; 
      resultFile.width(16); resultFile << results[i].value; 
      resultFile.width(16); resultFile << results[i].error;
      resultFile.width(16); resultFile << results[i].plus_moment;
      resultFile.width(16); resultFile << results[i].minus_moment << std::endl; 
    }

    resultFile.close(); 

  } else {
    Analysis *Ana = new Analysis(&opts, &pars); 
    for (int i=0; i<files.size(); i++) {   
      Ana->AddFile(files[i]); 
    }
    Ana->RunAnalysis();
    Ana->DoSave("Asymmetry.root"); 
    delete Ana; 
  }

  return 0;
}

std::vector<std::string> loadFilesFromList(std::string fileList, int numFiles){
  std::vector<std::string> theseFiles;
  
  std::ifstream inputFile;
  inputFile.open(fileList.c_str());

  int ifile = 0; string line;
  while (getline(inputFile, line) && ifile < numFiles){
    theseFiles.push_back(line);
    ifile++;
  }

  inputFile.close();
  return theseFiles;
}

std::vector<std::string> loadFilesFromCommandLine(h22Options * theseOpts, int numFiles){
  std::vector<std::string> theseFiles;

  for(int ifile = 0; ifile < theseOpts->ifiles.size(); ifile++){
    theseFiles.push_back(theseOpts->ifiles[ifile]);

    if (ifile == numFiles){ break; }
  }

  return theseFiles;
}
