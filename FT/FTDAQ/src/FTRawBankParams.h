#ifndef FTRAWBANKPARAMS_H 
#define FTRAWBANKPARAMS_H 1
#include <cstdint>

namespace FTRawBank {
  enum shifts {
    sipmShift     = 4,  // for the header: SiPM Number + number of clusters
    cellShift     = 0,
    fractionShift = 7,
    //sipmIdShift   = 9,
    sizeShift     = 8,
    //chargeShift   = 13
  };
  
  static constexpr uint16_t nbClusMaximum   = 15;  // 4 bits ??
  static constexpr uint16_t fractionMaximum = 1;   // 1 bits allocted
  static constexpr uint16_t cellMaximum     = 127; // 0 to 127; coded on 7 bits
  //static constexpr uint16_t sipmIdMaximum   = 3;   // 2 bits allocated
  static constexpr uint16_t sizeMaximum     = 1;   // 2 bits allocated
  //static constexpr uint16_t chargeMaximum   = 7;   // 3 bits allocated
  static constexpr uint16_t largeClusterSize  = 6; // TODO: tune this parameter

  enum BankProperties {
    NbBanks = 48,
    NbSiPMPerTELL40 = 128 // should be max 6*16=96 (modules*sipms)
  };

}

#endif // FTRAWBANKPARAMS_H
