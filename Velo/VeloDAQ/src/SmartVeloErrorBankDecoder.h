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
  /// iterator to the error bank body
  typedef const unsigned int*                 ErrorBankIT;
  typedef std::pair<ErrorBankIT, ErrorBankIT> ITPair;
  typedef std::map<unsigned int, ITPair>      BANKS;
  typedef std::map<unsigned int, ITPair>      SECTORS;

  enum marks { INIT_SHIFT = 4, EVT_WORDS = 5 };

  enum sectors { SOURCES = 4, HEADER = 5, ERROR_BANK_SIZE = 20 };

  /// Standard constructor
  SmartVeloErrorBankDecoder( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution

protected:
  // get the RawEvent
  StatusCode getRawEvent();
  // extract error banks
  StatusCode cacheErrorRawBanks();
  // count how many ppfpga sent errors
  SECTORS errorDetector( unsigned int tell1 );
  // decode header and store evt info blocks
  StatusCode storeErrorRawBanks();

private:
  bool            m_isDebug;
  LHCb::RawEvent* m_rawEvent; /// pointer to RawEvent container

  /// Location in the transient store of the RawEvent object.
  /// @warning Obsolete: use m_rawEventLocations
  std::string m_rawEventLocation;

  /// List of locations in the transient store to search the RawEvent object.
  std::vector<std::string> m_rawEventLocations;

  VeloErrorBanks*                      m_errorBank; /// container to store error banks
  std::string                          m_errorBankLoc;
  int                                  m_printBankHeader;
  BANKS                                m_cachedBanks;
  std::map<unsigned int, unsigned int> m_bankLength;
  unsigned int                         m_bankVersion;
  unsigned int                         m_bankType;
  unsigned int                         m_magicPattern;
};
#endif // SMARTVELOERRORBANKDECODER_H
