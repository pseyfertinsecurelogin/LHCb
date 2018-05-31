// $Id: $
#ifndef FTRAWBANKENCODER_H 
#define FTRAWBANKENCODER_H 1

// Include files
#include <cstdint>
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "IFTReadoutTool.h"

/** @class FTRawBankEncoder FTRawBankEncoder.h
 *  Encode the FTCLusters into raw banks
 *
 *  @author Olivier Callot
 *  @date   2012-05-11
 */
class FTRawBankEncoder : public GaudiAlgorithm {
public: 
  using GaudiAlgorithm::GaudiAlgorithm;

  StatusCode execute   () override;    ///< Algorithm execution

private:
  PublicToolHandle<IFTReadoutTool> m_readoutTool = { this, "FTReadoutTool", "FTReadoutTool" };

  Gaudi::Property<std::string> m_inputLocation  {this, "InputLocation"  , LHCb::FTClusterLocation::Default };
  Gaudi::Property<std::string> m_outputLocation {this, "OutputLocation" , LHCb::RawEventLocation::Default  };
};
#endif // FTRAWBANKENCODER_H
