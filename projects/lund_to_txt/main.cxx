#include <iostream>
using std::string;
using std::cout; 
using std::endl;

#include "LundToText.h"

int main(int argc, char *argv[]){

  if (argc < 2){
    cout << "[LundToText] Error: No file passed in." << endl; 
  }

  LundToText *lundConverter = new LundToText();
  lundConverter->LoadEvents(argv[1]);
  lundConverter->Execute( lundConverter->GetNumberOfEvents() );

  return 0;
} 
