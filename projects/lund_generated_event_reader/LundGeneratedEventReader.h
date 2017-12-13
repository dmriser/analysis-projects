#ifndef lund_gen_reader_h 
#define lund_gen_reader_h 

// C++ libs 
#include <iostream>

// My Libs 
#include "LundReader.h"
#include "PhysicsEventBuilder.h"
#include "StandardHistograms.h"
#include "StatusBar.h"

class LundGeneratedEventReader : public LundReader {

 public:
  LundGeneratedEventReader();
  ~LundGeneratedEventReader();

 protected:
  PhysicsEventBuilder  *builder; 
  StandardHistograms   *histos; 
  StatusBar             bar; 
  std::string outputFilenameWithExtension; 

 public:
  void Initialize();
  void Loop(int numberOfEvents);
  void SetBeamEnergy(double beam);
  void Save(std::string outputFile);
  void Execute(int numberOfEvents);
};

#endif 
