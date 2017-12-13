// --------------------------------------------
//   doCrossSection.cxx
//   Feb 10, 2016
//   David Riser 
// --------------------------------------------

#include <iostream>
#include <vector>
using namespace std;

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

#include "BostedElasticWrapper.h"
#include "CommonTools.h"
#include "h22Option.h"
#include "common/Histograms.h"

class Analysis{
public:
  Analysis();
  ~Analysis();
  
protected:
  const static int numberSectors   = 6;
  const static int numberThetaBins = 100;
  const static int numberPhiBins   = 5;
  
  vector<double> thetaBins;
  vector<double> phiBins;
  vector<TH1D*>  histoContainer;
  
  ElasticHistograms *dataHistos;
  ElasticHistograms *recHistos;
  ElasticHistograms *genHistos;
  
  TH1D       *dataEvents_theta[numberSectors][numberPhiBins+1];
  TH1D       *recEvents_theta[numberSectors][numberPhiBins+1];
  TH1D       *genEvents_theta[numberSectors][numberPhiBins+1];
  TH1D       *acc_theta[numberSectors][numberPhiBins+1];
  TH1D       *w_theta[numberSectors][numberPhiBins+1];
  TH1D       *crossSection_theta[numberSectors][numberPhiBins+1];
  TH1D       *ratio_theta[numberSectors][numberPhiBins+1];
  TH1D       *model_theta;
  TH1D       *modelRad_theta;
  TH1D       *binCorr_theta;
  TH1D       *radCorr_theta;
  TH1D       *photonFlux_theta;
  TFile      *inputFile;
  
  double               totalCharge;
  double               thetaStep;
  double               phiStep;
  BostedElasticWrapper model;
  
public:
  void fillPhotonFlux(double beam);
  void loadEventFile(string filename);
  void makeProjections();
  void makeRatio();
  void makeBinCenteringCorrections();
  void makeRadiativeCorrections();
  void setupBinning();
  void scaleByLuminosity();
  void scaleCrossSection();
  void loadModel();
  void save(string outputFilename);
};

Analysis::Analysis(){

}

Analysis::~Analysis(){
    if(inputFile->IsOpen()){
        inputFile->Close();
    }
}

void Analysis::setupBinning(){
    thetaStep = (thetaMax-thetaMin)/numberThetaBins;
    phiStep   = (phiMax-phiMin)/numberPhiBins;

    for(int bin=0; bin<numberThetaBins+1; ++bin){
        thetaBins.push_back(bin*thetaStep+thetaMin);
    }

    for(int bin=0; bin<numberPhiBins+1; ++bin){
        phiBins.push_back(bin*phiStep+phiMin);
    }

    cout << "[Analysis::setupBinning] Finished theta bins with " << thetaBins.size() << " limits. " << endl;
    cout << "[Analysis::setupBinning] Finished phi bins with   " << phiBins.size()   << " limits. " << endl;
}

void Analysis::loadEventFile(string filename){
    inputFile        = TFile::Open(filename.c_str());
    dataHistos       = new ElasticHistograms("data",1);
    dataHistos->Load(inputFile);

    //  recHistos       = new ElasticHistograms("rec",1);
    //  recHistos->Load(inputFile);

    //  genHistos       = new ElasticHistograms("gen",1);
    //  genHistos->Load(inputFile);

    TH1D *totalChargeHisto = (TH1D*) inputFile->Get("totalCharge");
    totalCharge            = totalChargeHisto->GetBinContent(1);
}

void Analysis::makeProjections(){
    for(int sect=0; sect<numberSectors; sect++){
        TH2D *thisDataHisto = dataHistos->getThetaByPhi(sect);
        //    TH2D *thisRecHisto  = recHistos->getThetaByPhi(sect);
        //    TH2D *thisGenHisto  = genHistos->getThetaByPhi(sect);

        for(int bin=0; bin<numberPhiBins; bin++){
            int lowBin = 1+floor(numberOfPhiBins/numberPhiBins)*bin;
            int upBin  = floor(numberOfPhiBins/numberPhiBins)*(1+bin);

            cout << "[Analysis::makeProjections] Getting lowBin = " << lowBin << " to upBin = " << upBin << " for Sector = " << sect << endl;

            dataEvents_theta[sect][bin] = new TH1D(Form("dataEvents_thetaByPhi_sect%d_bin%d",sect,bin),Form("dataEvents_thetaByPhi_sect%d_bin%d",sect,bin), numberOfThetaBins, thetaMin, thetaMax);
            thisDataHisto->ProjectionY(Form("dataEvents_thetaByPhi_sect%d_bin%d",sect,bin),lowBin,upBin);
            dataEvents_theta[sect][bin]->Rebin((int) dataEvents_theta[sect][bin]->GetXaxis()->GetNbins()/numberThetaBins);

            //      recEvents_theta[sect][bin] = new TH1D(Form("recEvents_thetaByPhi_sect%d_bin%d",sect,bin),Form("recEvents_thetaByPhi_sect%d_bin%d",sect,bin), numberOfThetaBins, thetaMin, thetaMax);
            //      thisRecHisto->ProjectionY(Form("recEvents_thetaByPhi_sect%d_bin%d",sect,bin),lowBin,upBin);
            //      recEvents_theta[sect][bin]->Rebin(recEvents_theta[sect][bin]->GetXaxis()->GetNbins()/numberThetaBins);


            //      genEvents_theta[sect][bin] = new TH1D(Form("genEvents_thetaByPhi_sect%d_bin%d",sect,bin),Form("genEvents_thetaByPhi_sect%d_bin%d",sect,bin), numberOfThetaBins, thetaMin, thetaMax);
            //      thisGenHisto->ProjectionY(Form("genEvents_thetaByPhi_sect%d_bin%d",sect,bin),lowBin,upBin);
            //      genEvents_theta[sect][bin]->Rebin(genEvents_theta[sect][bin]->GetXaxis()->GetNbins()/numberThetaBins);

            //      acc_theta[sect][bin] = (TH1D*) recEvents_theta[sect][bin]->Clone();
            //      acc_theta[sect][bin]->Divide(genEvents_theta[sect][bin]);
            //      acc_theta[sect][bin]->SetName(Form("acc_thetaByPhi_sect%d_bin%d",sect,bin));

            crossSection_theta[sect][bin] = (TH1D*) dataEvents_theta[sect][bin]->Clone();
            crossSection_theta[sect][bin]->SetName(Form("crossSection_thetaByPhi_sect%d_bin%d",sect,bin));
            //      crossSection_theta[sect][bin]->Divide(acc_theta[sect][bin]);

            histoContainer.push_back(dataEvents_theta[sect][bin]);
            //      histoContainer.push_back(recEvents_theta[sect][bin]);
            //      histoContainer.push_back(genEvents_theta[sect][bin]);
            //      histoContainer.push_back(acc_theta[sect][bin]);
            histoContainer.push_back(crossSection_theta[sect][bin]);

            cout << "[Analysis::makeProjections] Created histo " << dataEvents_theta[sect][bin] << " Sect=" << sect << " Bin=" << bin << " w/ Entries=" << dataEvents_theta[sect][bin]->GetEntries() << endl;
        }
    }
}

void Analysis::fillPhotonFlux(double beam){
    int numberBins   = dataEvents_theta[0][0]->GetXaxis()->GetNbins();
    photonFlux_theta = new TH1D("photonFlux_theta","",numberBins,thetaMin,thetaMax);

    for(int tBin=1; tBin<=numberBins; tBin++){
        double mid    = tBin*thetaStep/2+thetaMin;
        double t      = photonFlux_theta->GetBinCenter(tBin);
        double epr    = beam/(1+beam*(1-cos(t*to_radians)));
        double q2     = 4*beam*epr*pow(sin(t*to_radians/2),2);
        double factor = calculatePhotonFlux(beam,0.938,q2);

        cout << "[Analysis::fillPhotonFlux] ";
        cout.width(16); cout << t;
        cout.width(16); cout << epr;
        cout.width(16); cout << q2;
        cout.width(16); cout << factor << endl;

        photonFlux_theta->SetBinContent(tBin,factor);
        photonFlux_theta->SetBinError(tBin,0.00);
    }

    histoContainer.push_back(photonFlux_theta);

    for(int sect=0; sect<numberSectors; sect++){
        for(int bin=0; bin<numberPhiBins; bin++){
            crossSection_theta[sect][bin]->Divide(photonFlux_theta);
        }
    }

    cout << "[Analysis::fillPhotonFlux] Created " << photonFlux_theta << endl;
}

void Analysis::makeBinCenteringCorrections(){
    int numberBins = dataEvents_theta[0][0]->GetXaxis()->GetNbins();

    binCorr_theta = new TH1D("binCorr_theta","",numberBins,thetaMin,thetaMax);
    for(int tBin=1; tBin<=numberBins; tBin++){
        double low = model.getRadiatedValue(5.498,(tBin-1)*thetaStep+thetaMin, 5.0/865.0, 1.1);
        double mid = model.getRadiatedValue(5.498,(tBin-0.5)*thetaStep+thetaMin, 5.0/865.0, 1.1);
        double up  = model.getRadiatedValue(5.498,tBin*thetaStep+thetaMin, 5.0/865.0, 1.1);
        double avg = (up+low)/2;

        binCorr_theta->SetBinContent(tBin,mid/avg);
        binCorr_theta->SetBinError(tBin,0.00);
    }

    histoContainer.push_back(binCorr_theta);

    for(int sect=0; sect<numberSectors; sect++){
        for(int bin=0; bin<numberPhiBins; bin++){
            crossSection_theta[sect][bin]->Divide(binCorr_theta);
        }
    }


}

void Analysis::makeRadiativeCorrections(){
    int numberBins = dataEvents_theta[0][0]->GetXaxis()->GetNbins();

    radCorr_theta = new TH1D("radCorr_theta","",numberBins,thetaMin,thetaMax);
    for(int tBin=1; tBin<=numberBins; tBin++){
        double rad   = model.getRadiatedValue(5.498,tBin*thetaStep/2+thetaMin, 5.0/865.0, 1.1);
        double norad = model.getValue(5.498,tBin*thetaStep/2+thetaMin);
        radCorr_theta->SetBinContent(tBin,rad/norad);
        radCorr_theta->SetBinError(tBin,0.00);
    }

    histoContainer.push_back(radCorr_theta);

    for(int sect=0; sect<numberSectors; sect++){
        for(int bin=0; bin<numberPhiBins; bin++){
            crossSection_theta[sect][bin]->Divide(radCorr_theta);
        }
    }

}

void Analysis::makeRatio(){
    for(int sect=0; sect<numberSectors; sect++){
        for(int bin=0; bin<numberPhiBins; bin++){
            ratio_theta[sect][bin] = (TH1D*) crossSection_theta[sect][bin]->Clone();
            ratio_theta[sect][bin]->SetName(Form("ratio_thetaByPhi_sect%d_bin%d",sect,bin));
            //      ratio_theta[sect][bin]->Divide(modelRad_theta);
            ratio_theta[sect][bin]->Divide(model_theta);
            histoContainer.push_back(ratio_theta[sect][bin]);
            cout << "[Analysis::makeProjection] Created histo " << ratio_theta[sect][bin] << " Sect=" << sect << " Bin=" << bin << " w/ Entries=" << ratio_theta[sect][bin]->GetEntries() << endl;
        }
    }
}

void Analysis::scaleByLuminosity(){
    cout << "[Analysis::scaleByLuminostiy] Total charge from file = " << totalCharge << " uC" << endl;

    double normalizationScale = cm_to_outhouse*(hydrogen_molar_weight*electron_c*1e6)/(5.00*avogadro*hydrogen_density);
    normalizationScale /= totalCharge;

    for(int sect=0; sect<numberSectors; sect++){
        for(int bin=0; bin<numberPhiBins; bin++){
            crossSection_theta[sect][bin]->Scale(normalizationScale);
        }
    }
}

void Analysis::scaleCrossSection(){

    for(int sect=0; sect<numberSectors; sect++){
        for(int bin=0; bin<numberPhiBins; bin++){

            crossSection_theta[sect][bin]->Scale(1.0/(thetaStep*phiStep*to_radians*to_radians));
            for(int tBin=1; tBin<=crossSection_theta[sect][bin]->GetXaxis()->GetNbins(); tBin++){
                double t       = crossSection_theta[sect][bin]->GetBinCenter(tBin);
                double content = crossSection_theta[sect][bin]->GetBinContent(tBin);
                double factor  = sin(t*to_radians);
                double events  = dataEvents_theta[sect][bin]->GetBinContent(tBin);
                crossSection_theta[sect][bin]->SetBinContent(tBin,content/factor);
            }
        }
    }
}

void Analysis::loadModel(){
    int numberBins = dataEvents_theta[0][0]->GetXaxis()->GetNbins();

    model_theta = new TH1D("model_theta","",numberBins,thetaMin,thetaMax);
    model_theta->SetName("model_theta");
    modelRad_theta = new TH1D("modelRad_theta","",numberBins,thetaMin,thetaMax);
    modelRad_theta->SetName("modelRad_theta");
    for(int tBin=1; tBin<=numberBins; tBin++){
        modelRad_theta->SetBinContent(tBin,model.getRadiatedValue(5.498,tBin*thetaStep/2+thetaMin, 5.0/865.0, 1.1));
        modelRad_theta->SetBinError(tBin,0.00);
        model_theta->SetBinContent(tBin,model.getValue(5.498,tBin*thetaStep/2+thetaMin));
        model_theta->SetBinError(tBin,0.00);
    }

    histoContainer.push_back(model_theta);
    histoContainer.push_back(modelRad_theta);
}

void Analysis::save(string outputFilename){
    TFile *out = new TFile(outputFilename.c_str(),"recreate");

    for(int h=0; h<histoContainer.size(); ++h){
        histoContainer[h]->Write();
    }

    out->Close();
}

int main(int argc, char *argv[]){

    h22Options *options = new h22Options();
    options->args["INPUT"].args = "UNSET";
    options->args["INPUT"].type = 1;
    options->args["INPUT"].name = "Input file with THnSparse events";
    options->set(argc, argv);

    Analysis analysis;
    analysis.loadEventFile(options->args["INPUT"].args);
    analysis.setupBinning();
    analysis.makeProjections();
    analysis.loadModel();
    analysis.makeRadiativeCorrections();
    analysis.makeBinCenteringCorrections();
    analysis.scaleByLuminosity();
    analysis.scaleCrossSection();
    analysis.makeRatio();
    analysis.save(options->args["OUT"].args);

    return 0;
}
