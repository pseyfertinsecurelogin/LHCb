#ifndef HCRAWBANKDECODER_H
#define HCRAWBANKDECODER_H 1

// AIDA
#include "AIDA/IHistogram1D.h"
#include "AIDA/IHistogram2D.h"

// LHCb
#include "GaudiAlg/GaudiHistoAlg.h"
#include "GaudiAlg/Transformer.h"

#include "Event/HCDigit.h"

namespace LHCb {
class RawEvent;
class ODIN;
}

/** @class HCRawBankDecoder.h
 *
 * Decoder for Herschel raw banks.
 *
 */

struct HCRawBankDecoderTraits {
    using BaseClass = GaudiHistoAlg;
};

class HCRawBankDecoder : public Gaudi::Functional::MultiTransformer<std::tuple<LHCb::HCDigits,LHCb::HCDigits>(const LHCb::RawEvent&, const LHCb::ODIN&),
                                                                    HCRawBankDecoderTraits>
{
 public:
  /// Standard constructor
  HCRawBankDecoder(const std::string& name, ISvcLocator* pSvcLocator);

  ///< Algorithm initialization
  StatusCode initialize() override;

  ///< The transform callable
  std::tuple<LHCb::HCDigits,LHCb::HCDigits>
  operator()(const LHCb::RawEvent&,const LHCb::ODIN&) const override;

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
