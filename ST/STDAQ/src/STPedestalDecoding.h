#ifndef STPEDESTALDECODING_H
#define STPEDESTALDECODING_H 1

#include <string>

#include "Event/RawBank.h"
#include "GaudiKernel/StatusCode.h"
// Include files
// from Gaudi
#include "Kernel/STAlgBase.h"

class ISvcLocator;

/** @class STPedestalDecoding STPedestalDecoding.h
 *
 *  Algorithm to decode the pedestal bank in the RawEvent buffer into
 *  STTELL1Data objects. Job options:
 *  - \b InputLocation: Location of RawEvent
 *  - \b OutputLocation: Location of output pedestal data, e.g. TTPedestal
 *  \sa{http://edms.cern.ch/doc/695007}.
 *
 *  @author Mathias Knecht, Jeroen van Tilburg
 *  @date   2007-09-11
 */
class STPedestalDecoding : public ST::AlgBase {

public:
  /// Standard constructor
  STPedestalDecoding( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:

  LHCb::RawBank::BankType m_bankType;

  // job options
  std::string m_inputLocation;  ///< Location of RawEvent
  std::string m_outputLocation; ///< Location of output pedestal (STTELL1Data)

};
#endif // STPEDESTALDECODING_H
