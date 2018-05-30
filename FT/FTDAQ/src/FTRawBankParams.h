#ifndef FTRAWBANKPARAMS_H 
#define FTRAWBANKPARAMS_H 1
#include <cstdint>

namespace FTRawBank {
  enum shifts {
    sipmShift     = 9,
    cellShift     = 2,
    fractionShift = 1,
    sizeShift     = 0,
  };

  static constexpr uint16_t nStations         =  3;
  static constexpr uint16_t nLayers           =  4;
  static constexpr uint16_t nQuarters         =  4;
  static constexpr uint16_t nBanks40PerQuarter=  5;
  static constexpr uint16_t nTotQuarters      =  nStations*nLayers*nQuarters;

  static constexpr uint16_t nbClusMaximum   = 31;  // 5 bits
  static constexpr uint16_t nbClusFFMaximum = 10;  //
  static constexpr uint16_t fractionMaximum = 1;   // 1 bits allocted
  static constexpr uint16_t cellMaximum     = 127; // 0 to 127; coded on 7 bits
  static constexpr uint16_t sizeMaximum     = 1;   // 1 bits allocated
  static constexpr uint16_t sipmMaximum     = 3; // 2 bits

  enum BankProperties {
    NbBanks = 240,
    NbSiPMPerBank = 24
  };

}

#endif // FTRAWBANKPARAMS_H
