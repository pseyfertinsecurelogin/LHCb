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
  /// Standard constructor
  //FTRawBankEncoder( const std::string& name, ISvcLocator* pSvcLocator );
  using GaudiAlgorithm::GaudiAlgorithm;

  inline IFTReadoutTool* getReadoutTool() const;
  inline IFTReadoutTool* readoutTool() const;

  StatusCode execute   () override;    ///< Algorithm execution

private:
  mutable IFTReadoutTool* m_readoutTool = 0;

  Gaudi::Property<std::string> m_inputLocation  {this, "InputLocation"  , LHCb::FTClusterLocation::Default };
  Gaudi::Property<std::string> m_outputLocation {this, "OutputLocation" , LHCb::RawEventLocation::Default  };
};
#endif // FTRAWBANKENCODER_H
