#ifndef HCRAWBANKDECODER_HLT_H
#define HCRAWBANKDECODER_HLT_H 1

#include <array>
#include <bitset>
#include <string>
#include <vector>

// LHCb
#include "DAQKernel/DecoderHistoAlgBase.h"
#include "GaudiKernel/StatusCode.h"

class Condition;
class ISvcLocator;
namespace LHCb {
class RawBank;
}  // namespace LHCb

/** @class HCRawBankDecoderHlt.h
 *
 * Specialised decoder for Herschel raw banks for use in HLT1.
 *
 */

class HCRawBankDecoderHlt : public Decoder::HistoAlgBase {
 public:
  /// Standard constructor
  HCRawBankDecoderHlt(const std::string& name, ISvcLocator* pSvcLocator);
  /// Destructor
  virtual ~HCRawBankDecoderHlt();

  StatusCode initialize() override;  ///< Algorithm initialization
  StatusCode execute() override;     ///< Algorithm execution

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
  std::bitset<2*nChannels> m_masked;
  /// Station number for each channel.
  std::array<unsigned int,2*nChannels> m_station;

  /// Do the decoding.
  bool decode( const LHCb::RawBank& bank, std::vector<int>& sums) const ;
  /// Retrieve the mapping from the conditions database.
  StatusCode cacheMapping();
};

#endif
