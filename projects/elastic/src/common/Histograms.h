#ifndef elastic_histograms_h
#define elastic_histograms_h

// ------------------------------
//      C++ Stand
// ------------------------------
#include <iostream>
#include <vector>
using std::vector; 
using std::string;
using std::cout; 
using std::endl; 

// ------------------------------
//       Cern ROOT
// ------------------------------
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"

// ------------------------------
//      This Project
// ------------------------------
#include "CommonTools.h"
#include "PhysicsEvent.h"

const static int numberOfSectors   = 6;
const static int numberOfThetaBins = 100;
const static int numberOfPhiBins   = 100;

#if __cplusplus >= 201103L
static constexpr double thetaMin       = 22.0;
static constexpr double thetaMax       = 48.0;
static constexpr double phiMin         = -12.0;
static constexpr double phiMax         = 12.0;
#else
const static double thetaMin       = 22.0;
const static double thetaMax       = 48.0;
const static double phiMin         = -12.0;
const static double phiMax         = 12.0;
#endif

class ElasticHistograms{
public:
    ElasticHistograms(std::string basename, int runMode);
    ~ElasticHistograms();

    std::string name;
    int         mode;

    void Initialize();
    void Load(TFile *file);

public:
    TH2D              *thetaPhi[numberOfSectors];
    std::vector<TH1D*> thetaSlices;

    std::string getName(){ return name; }
    int getMode()        { return mode; }
    void Save(TFile *file);
    void Fill(PhysicsEvent event);
    TH2D *getThetaByPhi(int sector);

};

ElasticHistograms::ElasticHistograms(string basename, int runMode) : name(basename), mode(runMode){

} 

void ElasticHistograms::Initialize(){
    for(int sector=0; sector<numberOfSectors; sector++){
        thetaPhi[sector] = new TH2D(Form("%sThetaByPhiSector%d",name.c_str(),sector),"",numberOfPhiBins,phiMin,phiMax,numberOfThetaBins,thetaMin,thetaMax);
    }
}

void ElasticHistograms::Load(TFile *file){
    if(file->IsOpen()){
        for(int sector=0; sector<numberOfSectors; sector++){
            thetaPhi[sector] = (TH2D*) file->Get(Form("%sThetaByPhiSector%d",name.c_str(),sector));
            cout << "[ElasticHistograms::Load] Opened " << thetaPhi[sector] << " w/ Entries = " << thetaPhi[sector]->GetEntries() << endl;
        }
    } else{
        cout << "[ElasticHistograms::Load] File not open. " << endl;
    }
}


void ElasticHistograms::Fill(PhysicsEvent event){
    thetaPhi[phiToSector(event.detectedElectron.Phi()*to_degrees)-1]->Fill(getRelativePhi(event.detectedElectron.Phi()*to_degrees), event.detectedElectron.Theta()*to_degrees);
}

TH2D* ElasticHistograms::getThetaByPhi(int sector){
    if(sector < numberOfSectors){
        return thetaPhi[sector];
    }

    else return new TH2D();
}

void ElasticHistograms::Save(TFile *file){

    if(file->IsOpen()){
        for(int sector=0; sector<numberOfSectors; sector++){
            thetaPhi[sector]->Write();
        }

        for(int i=0; i<thetaSlices.size(); i++){
            thetaSlices[i]->Write();
        }
    } else {
        cout << "[ElasticHistograms] File not opened " << endl;
    }

}

#endif
