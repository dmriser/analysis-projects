#ifndef lund_to_text_h 
#define lund_to_text_h 

// C++ libs 
#include <iostream>

// My Libs 
#include "LundReader.h"
#include "PhysicsEventBuilder.h"

class LundToText : public LundReader{

 public:
  LundToText();
  ~LundToText();

 protected:
  PhysicsEventBuilder builder; 
  std::string outputFilenameWithExtension; 

 public:
  void Initialize();
  void Loop(int numberOfEvents);
  void Save();
  void Execute(int numberOfEvents);
};

#endif 
