/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef FTRAWBANKPARAMS_H 
#define FTRAWBANKPARAMS_H 1
#include <cstdint>

namespace FTRawBank {
  enum shifts {
    linkShift     = 9,
    cellShift     = 2,
    fractionShift = 1,
    sizeShift     = 0,
  };

  static constexpr uint16_t nbClusMaximum   = 31;  // 5 bits
  static constexpr uint16_t nbClusFFMaximum = 10;  //
  static constexpr uint16_t fractionMaximum = 1;   // 1 bits allocted
  static constexpr uint16_t cellMaximum     = 127; // 0 to 127; coded on 7 bits
  static constexpr uint16_t sizeMaximum     = 1;   // 1 bits allocated

  enum BankProperties {
    NbBanks = 240,
    NbLinksPerBank = 24
  };

}

#endif // FTRAWBANKPARAMS_H
