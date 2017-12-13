#ifndef bins_h
#define bins_h

#include <iostream>

#include "Types.h"

#include "DBins.h"

class Bins {
 public:
  Bins(){
  }

  void SetName(std::string n){
    fName = n; 
  }

  void SetXBins(DLineBins *b){
    xbins = b;
  }

  void SetQ2Bins(DLineBins *b){
    q2bins = b;
  }

  void SetZBins(DLineBins *b){
    zbins = b;
  }

  void SetPtBins(DLineBins *b){
    ptbins = b;
  }

  void SetPhiBins(DLineBins *b){
    phibins = b;
  }
  
  std::string GetName() const {
    return fName; 
  }

  DLineBins *GetXBins() const {
    return xbins; 
  }

  DLineBins *GetQ2Bins() const {
    return q2bins; 
  }
  
  DLineBins *GetZBins() const {
    return zbins; 
  }
  
  DLineBins *GetPtBins() const {
    return ptbins; 
  }
  
  DLineBins *GetPhiBins() const {
    return phibins; 
  }

  static Bins *GetInstance(int index);

 protected:

  std::string fName; 
  DLineBins *xbins; 
  DLineBins *q2bins; 
  DLineBins *zbins; 
  DLineBins *ptbins; 
  DLineBins *phibins; 

};

class KaonPositiveBins : public Bins {
 public:
  KaonPositiveBins(){
    std::vector<double> vx;
    vx.push_back(0.100);
    vx.push_back(0.165);
    vx.push_back(0.215);
    vx.push_back(0.255);
    vx.push_back(0.305);
    vx.push_back(0.365);
    vx.push_back(0.600);

    std::vector<double> vq2; 
    vq2.push_back(1.00); 
    vq2.push_back(1.24); 
    vq2.push_back(1.47); 
    vq2.push_back(1.69); 
    vq2.push_back(2.01); 
    vq2.push_back(2.50); 
    vq2.push_back(5.00); 

    std::vector<double> vz;    
    vz.push_back(0.000);
    vz.push_back(0.265);
    vz.push_back(0.335);
    vz.push_back(0.405);
    vz.push_back(0.485);
    vz.push_back(0.585);
    vz.push_back(0.990);
    
    /*      
    vz.push_back(0.300);
    vz.push_back(0.325);
    vz.push_back(0.365);
    vz.push_back(0.405);
    vz.push_back(0.465);
    vz.push_back(0.545);
    vz.push_back(0.700);
    */

    std::vector<double> vpt;
    vpt.push_back(0.000);
    vpt.push_back(0.220);
    vpt.push_back(0.330);
    vpt.push_back(0.410);
    vpt.push_back(0.500);
    vpt.push_back(0.610);
    vpt.push_back(1.000);

    xbins   = new DLineBins(6, vx);
    q2bins   = new DLineBins(6, vq2);
    zbins   = new DLineBins(6, vz);
    ptbins  = new DLineBins(6, vpt);
    phibins = new DLineBins(18, -180.0, 180.0); 

    std::cout << "KaonPositiveBins have been created." << std::endl; 
  }
};

class KaonNegativeBins : public Bins {
 public:
  KaonNegativeBins(){
    std::vector<double> vx;
    vx.push_back(0.100);
    vx.push_back(0.215);
    vx.push_back(0.295);
    vx.push_back(0.600);

    std::vector<double> vq2; 
    vq2.push_back(1.00); 
    vq2.push_back(2.00); 
    vq2.push_back(4.50); 

    std::vector<double> vz;    
    vz.push_back(0.000);
    vz.push_back(0.265);
    vz.push_back(0.335);
    vz.push_back(0.405);
    vz.push_back(0.485);
    vz.push_back(0.585);
    vz.push_back(0.990);
    
    std::vector<double> vpt;
    vpt.push_back(0.000);
    vpt.push_back(0.412);
    vpt.push_back(0.556);
    vpt.push_back(1.000);
    
    xbins   = new DLineBins(3, vx);
    q2bins   = new DLineBins(2, vq2);
    zbins   = new DLineBins(6, vz);
    ptbins  = new DLineBins(3, vpt);
    phibins = new DLineBins(18, -180.0, 180.0); 

    std::cout << "KaonNegativeBins have been created." << std::endl; 
  }
};

class PionPositiveBins : public Bins {
 public:
  PionPositiveBins(){
    std::vector<double> vx;
    vx.push_back(0.100);
    vx.push_back(0.165);
    vx.push_back(0.215);
    vx.push_back(0.255);
    vx.push_back(0.305);
    vx.push_back(0.365);
    vx.push_back(0.600);

    std::vector<double> vq2; 
    vq2.push_back(1.00); 
    vq2.push_back(1.24); 
    vq2.push_back(1.47); 
    vq2.push_back(1.69); 
    vq2.push_back(2.01); 
    vq2.push_back(2.50); 
    vq2.push_back(5.00); 

    /*    
    std::vector<double> vz;
    vz.push_back(0.300);
    vz.push_back(0.325);
    vz.push_back(0.365);
    vz.push_back(0.405);
    vz.push_back(0.465);
    vz.push_back(0.545);
    vz.push_back(0.700);
    */
    
    std::vector<double> vz;
    vz.push_back(0.000);
    vz.push_back(0.265);
    vz.push_back(0.335);
    vz.push_back(0.405);
    vz.push_back(0.485);
    vz.push_back(0.585);
    vz.push_back(0.990);


    std::vector<double> vpt;
    vpt.push_back(0.000);
    vpt.push_back(0.220);
    vpt.push_back(0.330);
    vpt.push_back(0.410);
    vpt.push_back(0.500);
    vpt.push_back(0.610);
    vpt.push_back(1.000);

    xbins   = new DLineBins(6, vx);
    q2bins  = new DLineBins(6, vq2);
    zbins   = new DLineBins(6, vz);
    ptbins  = new DLineBins(6, vpt);
    phibins = new DLineBins(18, -180.0, 180.0); 

    std::cout << "PionPositiveBins have been created." << std::endl; 
  }
};

class PionNegativeBins : public Bins {
 public:
  PionNegativeBins(){

    std::vector<double> vx;
    vx.push_back(0.100);
    vx.push_back(0.215);
    vx.push_back(0.295);
    vx.push_back(0.600);

    std::vector<double> vq2; 
    vq2.push_back(1.00); 
    vq2.push_back(2.00); 
    vq2.push_back(4.50); 

    std::vector<double> vz;    
    vz.push_back(0.000);
    vz.push_back(0.265);
    vz.push_back(0.335);
    vz.push_back(0.405);
    vz.push_back(0.485);
    vz.push_back(0.585);
    vz.push_back(0.990);

    std::vector<double> vpt;
    vpt.push_back(0.000);
    vpt.push_back(0.412);
    vpt.push_back(0.556);
    vpt.push_back(1.000);

    xbins   = new DLineBins(3, vx);
    q2bins  = new DLineBins(2, vq2);
    zbins   = new DLineBins(6, vz);
    ptbins  = new DLineBins(3, vpt);
    phibins = new DLineBins(18, -180.0, 180.0); 

    std::cout << "PionNegativeBins have been created." << std::endl; 
  }
};

class NullBins : public Bins {
 public:
  NullBins(){
    xbins   = new DLineBins(2, 0.0, 1.0); 
    q2bins  = new DLineBins(2, 0.0, 1.0); 
    zbins   = new DLineBins(2, 0.0, 1.0); 
    ptbins  = new DLineBins(2, 0.0, 1.0); 
    phibins = new DLineBins(2, 0.0, 1.0); 

    std::cout << "NullBins have been created." << std::endl; 
  }
};

Bins* Bins::GetInstance(int index){

  if (index == Meson::kKaonPositive){
    return new KaonPositiveBins;
  } else if (index == Meson::kPionPositive){
    return new PionPositiveBins;
  } else if (index == Meson::kKaonNegative){
    return new KaonNegativeBins;
  } else if (index == Meson::kPionNegative){
    return new PionNegativeBins;
  }
  

  return new NullBins; 
}

#endif
