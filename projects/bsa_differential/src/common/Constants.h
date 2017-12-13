#ifndef constants_h
#define constants_h

#include <iostream>

#include "Types.h"

namespace constants {
  const static int NSECT        = 6;
  const static int NMESON       = 4;
  const static int NHEL         = 2;
  const static int MAX_BINS_X   = 8; 
  const static int MAX_BINS_Q2  = 8; 
  const static int MAX_BINS_Z   = 8; 
  const static int MAX_BINS_PT  = 8; 
  const static int NSTATUS      = 3;
  const static float BEAM_POL   = 0.749;
  const static float KAONPMAX   = 2.15; 
  const static float DIS_WMIN   = 2.0;  
  const static float DIS_Q2MIN  = 1.0; 
  const static float DIS_ZMIN   = 0.1; 
  const static float DIS_ZMAX   = 0.7; 
  const static float DIS_MM2_PION_MIN = 1.1; 
  const static float DIS_MM2_KAON_MIN = 2.65; 
  const static float LOWER_Z = 0.3; 
  const static float UPPER_Z = 0.7; 

  struct Names{
    const static std::string mesons[NMESON]; 
    const static std::string latex[NMESON]; 
    const static std::string status[NSTATUS]; 
  };

  struct PID {
    const static int pid[NMESON];
  };

  struct cuts {
    const static float MM2[NMESON];
  };

}

const std::string constants::Names::mesons[NMESON]  = {"pm","pp","km","kp"};
const std::string constants::Names::latex[NMESON]   = {"#pi^{-}","#pi^{+}","K^{-}","K^{+}"};
const std::string constants::Names::status[NSTATUS] = {"PassPID", "PassSIDIS", "PassAll"};
const int constants::PID::pid[NMESON]               = {-211, 211, -321, 321};
const float constants::cuts::MM2[NMESON]            = {1.1, 1.1, 1.1, 2.65};

#endif
