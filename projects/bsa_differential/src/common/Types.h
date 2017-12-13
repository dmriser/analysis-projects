#ifndef types_h
#define types_h

namespace Meson {
  enum {
    kPionNegative, 
    kPionPositive, 
    kKaonNegative,
    kKaonPositive
  };
}

namespace Helicity {
  enum {
    kNegative,
    kPositive,
    kUnknown
  };
}

namespace EventStatus {
  enum {
    kPID, 
    kSIDIS,
    kPass
  };
}

#endif
