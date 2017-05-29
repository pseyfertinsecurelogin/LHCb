#ifndef OTDAQ_OTTIMECREATOR_H
#define OTDAQ_OTTIMECREATOR_H 1

#include <string>

// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/IInterface.h"
#include "GaudiKernel/StatusCode.h"
// Kernel
#include "GaudiKernel/ToolHandle.h"
#include "OTDAQ/IOTRawBankDecoder.h"

class DeOTDetector;
class ISvcLocator;
// forward declarations
struct IOTRawBankDecoder;

/** @class OTTimeCreator OTTimeCreator.h
 *
 *
 *  @author Jacopo Nardulli
 *  @date   2004-06-06
 */

class OTTimeCreator : public GaudiAlgorithm {
public:

  /// Standard constructor
  OTTimeCreator( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode finalize() override;
  StatusCode execute() override;    ///< Algorithm execution

private:

  // job options
  std::string m_timeLocation;
  const DeOTDetector* m_tracker = nullptr;                  ///< Pointer to XML geometry
  ToolHandle<IOTRawBankDecoder> m_decoder = { "OTRawBankDecoder" };
};

#endif // OTDAQ_OTTIMECREATOR_H
