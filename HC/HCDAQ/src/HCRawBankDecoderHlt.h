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
#ifndef HCRAWBANKDECODER_HLT_H
#define HCRAWBANKDECODER_HLT_H 1

// LHCb
#include "DAQKernel/DecoderHistoAlgBase.h"

struct Condition;

/** @class HCRawBankDecoderHlt.h
 *
 * Specialised decoder for Herschel raw banks for use in HLT1.
 *
 */

class HCRawBankDecoderHlt : public Decoder::HistoAlgBase {
public:
  /// Standard constructor
  HCRawBankDecoderHlt( const std::string& name, ISvcLocator* pSvcLocator );
  /// Destructor
  virtual ~HCRawBankDecoderHlt();

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution

private:
  Condition* m_cond = nullptr;

  /// TES location of output digits.
  std::string m_digitLocation;

  /// Crate number for B side stations.
  unsigned int m_crateB = 0;
  /// Crate number for F side stations.
  unsigned int m_crateF = 1;

  /// Masking flag for each channel.

  enum { nChannels = 64 };
  std::bitset<2 * nChannels> m_masked;
  /// Station number for each channel.
  std::array<unsigned int, 2 * nChannels> m_station;

  /// Do the decoding.
  bool decode( const LHCb::RawBank& bank, std::vector<int>& sums ) const;
  /// Retrieve the mapping from the conditions database.
  StatusCode cacheMapping();
};

#endif
