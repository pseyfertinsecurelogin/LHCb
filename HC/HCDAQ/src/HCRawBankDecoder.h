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
#ifndef HCRAWBANKDECODER_H
#define HCRAWBANKDECODER_H 1

// AIDA
#include "AIDA/IHistogram1D.h"
#include "AIDA/IHistogram2D.h"

// LHCb
#include "GaudiAlg/GaudiHistoAlg.h"
#include "GaudiAlg/Transformer.h"

#include "Event/HCDigit.h"
#include "Event/RawEvent.h"
#include "Event/RawBank.h"
#include "Event/ODIN.h"

namespace LHCb {
class ODIN;
}

/** @class HCRawBankDecoder.h
 *
 * Decoder for Herschel raw banks.
 *
 */

class HCRawBankDecoder final : public Gaudi::Functional::MultiTransformer<std::tuple<LHCb::HCDigits,LHCb::HCDigits>(const LHCb::RawEvent&, const LHCb::ODIN&),
                                                                          Gaudi::Functional::Traits::BaseClass_t<GaudiHistoAlg>>
{
 public:
  /// Standard constructor
  HCRawBankDecoder(const std::string& name, ISvcLocator* pSvcLocator);

  StatusCode initialize() override;  ///< Algorithm initialization

  ///< The transform callable
  std::tuple<LHCb::HCDigits,LHCb::HCDigits>
  operator()(const LHCb::RawEvent&,const LHCb::ODIN&) const override final;

 private:

  bool m_skipTrigger;
  bool m_skipAdc;
  bool m_skipErrorBanks;

  bool m_monitoring;

  IHistogram2D* m_hLinkErrors;
  IHistogram2D* m_hTell1Errors;
  std::vector<IHistogram1D*> m_hBxDiff;

  void decode(const LHCb::RawBank& bank,
              LHCb::HCDigits& digits,
              LHCb::HCDigits& l0_digits) const;
  void decodeErrorBank(const LHCb::RawBank& bank, const int bxid) const;

};

#endif
