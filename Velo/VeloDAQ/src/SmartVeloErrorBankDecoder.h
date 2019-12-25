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
#ifndef SMARTVELOERRORBANKDECODER_H
#define SMARTVELOERRORBANKDECODER_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

// data model
#include "Event/RawBank.h"
#include "Event/RawEvent.h"

#include "Tell1Kernel/VeloDecodeConf.h"
#include "Tell1Kernel/VeloDecodeCore.h"
#include "Tell1Kernel/VeloTell1Core.h"
#include "VeloEvent/VeloErrorBank.h"

// stl
#include <algorithm>
#include <iterator>
#include <vector>

/** @class SmartVeloErrorBankDecoder SmartVeloErrorBankDecoder.h
 *
 *
 *  @author Tomasz Szumlak
 *  @date   2008-08-22
 */

class SmartVeloErrorBankDecoder : public GaudiAlgorithm {
public:
  /// Standard constructor
  using GaudiAlgorithm::GaudiAlgorithm;

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution

private:
  /// iterator to the error bank body
  typedef const unsigned int*                 ErrorBankIT;
  typedef std::pair<ErrorBankIT, ErrorBankIT> ITPair;
  typedef std::map<unsigned int, ITPair>      BANKS;
  typedef std::map<unsigned int, ITPair>      SECTORS;

  enum marks { INIT_SHIFT = 4, EVT_WORDS = 5 };

  enum sectors { SOURCES = 4, HEADER = 5, ERROR_BANK_SIZE = 20 };
  // get the RawEvent
  StatusCode getRawEvent();
  // extract error banks
  StatusCode cacheErrorRawBanks();
  // count how many ppfpga sent errors
  SECTORS errorDetector( unsigned int tell1 );
  // decode header and store evt info blocks
  StatusCode storeErrorRawBanks();

private:
  const LHCb::RawEvent* m_rawEvent = nullptr; /// pointer to RawEvent container

  /// List of locations in the transient store to search the RawEvent object.
  Gaudi::Property<std::vector<std::string>> m_rawEventLocations{
      this, "RawEventLocations", {}, "List of possible locations of the RawEvent object in the transient store."};

  VeloErrorBanks*                       m_errorBank = nullptr; /// container to store error banks
  DataObjectWriteHandle<VeloErrorBanks> m_errorBankLoc{this, "ErrorBankLocation", VeloErrorBankLocation::Default};
  Gaudi::Property<bool>                 m_printBankHeader{this, "PrintBankHeader", false};
  BANKS                                 m_cachedBanks;
  std::map<unsigned int, unsigned int>  m_bankLength;
  unsigned int                          m_bankVersion  = 0;
  unsigned int                          m_bankType     = 0;
  unsigned int                          m_magicPattern = 0;
};
#endif // SMARTVELOERRORBANKDECODER_H
