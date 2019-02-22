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
#ifndef STDECODINGBASEALG_H
#define STDECODINGBASEALG_H 1

#include "Event/RawBank.h"
#include "GaudiKernel/DataObjectHandle.h"
#include "Kernel/STAlgBase.h"
#include "Kernel/STDAQDefinitions.h"
#include "Kernel/STLExtensions.h"

#include "Event/ODIN.h"
#include "Event/STCluster.h"

#include <string>
#include <vector>

/** @class STDecodingBaseAlg STDecodingBaseAlg.h
 *
 *  Algorithm to create STClusters from RawEvent object
 *
 *  @author M. Needham
 *  @author S. Ponce
 */

#include "Event/RawEvent.h"

#include "Event/STTELL1BoardErrorBank.h"
#include "Kernel/STClusterWord.h"
#include "Kernel/STDecoder.h"

#include <string>

class STTell1Board;

class STDecodingBaseAlg : public ST::AlgBase {

public:
  /// Standard constructor
  STDecodingBaseAlg( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override; ///< Algorithm initialization

protected:
  LHCb::RawBank::BankType bankType() const;

  bool forceVersion() const;

  unsigned int pcnVote( LHCb::span<const LHCb::RawBank*> banks ) const;

  bool checkDataIntegrity( STDecoder& decoder, const STTell1Board* aBoard, const unsigned int bankSize,
                           const STDAQ::version& bankVersion ) const;

  /** list of boards missing in action */
  std::vector<unsigned int> missingInAction( LHCb::span<const LHCb::RawBank*> banks ) const;

  /// Decodes error banks
  std::unique_ptr<LHCb::STTELL1BoardErrorBanks> decodeErrors( const LHCb::RawEvent& raw ) const;

  /** recover mode **/
  bool recoverMode() const;

  /** can be recovered recover **/
  bool canBeRecovered( const LHCb::STTELL1BoardErrorBank* bank, const STClusterWord& word,
                       const unsigned int pcn ) const;

  /// compute the spill offset
  void computeSpillOffset( const std::string& location );

  /** check the spill is read out **/
  bool validSpill( const LHCb::ODIN& odin ) const;

  /** return spill offset */
  LHCb::STCluster::Spill spill() const;

  unsigned int byteSize( LHCb::span<const LHCb::RawBank*> banks ) const;

  bool        m_skipErrors;
  std::string m_bankTypeString;

  int                     m_forcedVersion;
  bool                    m_checkValidSpill;
  LHCb::RawBank::BankType m_errorType;
  LHCb::RawBank::BankType m_bankType;

private:
  std::string            toSpill( const std::string& location ) const;
  LHCb::STCluster::Spill m_spillOffset;

  std::string m_errorBankString;

  bool m_recoverMode;

  mutable Gaudi::Accumulators::Counter<> m_skippedBanks{this, "skipped Banks"};
  mutable Gaudi::Accumulators::Counter<> m_evtsWithErrorBanks{this, "events with error banks"};
  mutable Gaudi::Accumulators::Counter<> m_totalErrorBanks{this, "total # error banks"};
};

inline LHCb::RawBank::BankType STDecodingBaseAlg::bankType() const { return m_bankType; }

inline bool STDecodingBaseAlg::forceVersion() const { return m_forcedVersion >= 0; }

inline bool STDecodingBaseAlg::recoverMode() const { return m_recoverMode; }

#include "Event/STTELL1Error.h"
#include "Kernel/LHCbConstants.h"
#include "Kernel/PPRepresentation.h"
#include "Kernel/STClusterWord.h"
#include "Kernel/STDAQDefinitions.h"
#include "Kernel/StripRepresentation.h"

inline bool STDecodingBaseAlg::canBeRecovered( const LHCb::STTELL1BoardErrorBank* bank, const STClusterWord& word,
                                               const unsigned int pcn ) const {

  STDAQ::PPRepresentation ppRep = STDAQ::PPRepresentation( STDAQ::StripRepresentation( word.channelID() ) );
  unsigned int            pp, beetle, port, strip;
  ppRep.decompose( pp, beetle, port, strip ); // split up the word
  const LHCb::STTELL1Error* errorInfo = bank->ppErrorInfo( pp );
  bool                      ok        = false;
  if ( errorInfo != 0 ) {
    if ( errorInfo->linkInfo( beetle, port, pcn ) == LHCb::STTELL1Error::FailureMode::kNone ) { ok = true; }
  }
  return ok;
}

inline LHCb::STCluster::Spill STDecodingBaseAlg::spill() const { return m_spillOffset; }

#endif // STDECODINGBASEALG_H
