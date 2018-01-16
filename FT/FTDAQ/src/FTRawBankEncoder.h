// $Id: $
#ifndef FTRAWBANKENCODER_H 
#define FTRAWBANKENCODER_H 1

// Include files
#include <cstdint>
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"


/** @class FTRawBankEncoder FTRawBankEncoder.h
 *  Encode the FTCLusters into raw banks
 *
 *  @author Olivier Callot
 *  @date   2012-05-11
 */
class FTRawBankEncoder : public GaudiAlgorithm {
public: 
  /// Standard constructor
  //FTRawBankEncoder( const std::string& name, ISvcLocator* pSvcLocator );
  using GaudiAlgorithm::GaudiAlgorithm;

  StatusCode execute   () override;    ///< Algorithm execution

private:
  Gaudi::Property<std::string> m_inputLocation  {this, "InputLocation"  , LHCb::FTClusterLocation::Default };
  Gaudi::Property<std::string> m_outputLocation {this, "OutputLocation" , LHCb::RawEventLocation::Default  };
};
#endif // FTRAWBANKENCODER_H
