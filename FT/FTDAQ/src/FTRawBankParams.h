#ifndef FTRAWBANKPARAMS_H 
#define FTRAWBANKPARAMS_H 1
#include <cstdint>

namespace FTRawBank {
  enum shifts {
    //    sipmShift     = 9,
    //    cellShift     = 0,
    //    fractionShift = 7,
    //    sizeShift     = 8,
    sipmShift     = 9,
    cellShift     = 2,
    fractionShift = 1,
    sizeShift     = 0,
  };

  static constexpr uint16_t nStations         =  3;
  static constexpr uint16_t nLayers           =  4;
  static constexpr uint16_t nQuarters         =  4;
  static constexpr uint16_t nTell40PerQuarter =  3;
  
  static constexpr uint16_t nbClusMaximum   = 15;  // 4 bits ??
  static constexpr uint16_t nbClusFFMaximum = 10;  // 4 bits ??
  static constexpr uint16_t fractionMaximum = 1;   // 1 bits allocted
  static constexpr uint16_t cellMaximum     = 127; // 0 to 127; coded on 7 bits
  static constexpr uint16_t sizeMaximum     = 1;   // 1 bits allocated
  static constexpr uint16_t sipmMaximum     = 127; // 7 bits

  enum BankProperties {
    NbBanks = 144,
    NbSiPMPerTell40 = 48 // should be max 3*16=48 (maxModulePerTell'à*sipms)
  };

}

#endif // FTRAWBANKPARAMS_H
