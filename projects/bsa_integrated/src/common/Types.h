#ifndef types_h
#define types_h

#include <iostream>
#include <map>

enum axis {
  x, 
  q2,
  z,
  pt, 
  phi
};

enum pass {
  mesonID, 
  kin, 
  all
};

enum helicity{
  plus,
  minus,
  unknown
};

enum styles {
  marker_black, 
  marker_red, 
  filled_blue, 
  filled_red
};

std::map<int, std::string> axisNames = {{axis::x,   "x"}, 
					{axis::q2,  "Q^{2}"},
					{axis::z,   "z"},
					{axis::pt,  "P_{T}"},
					{axis::phi, "#phi_{h}"}};

#endif
