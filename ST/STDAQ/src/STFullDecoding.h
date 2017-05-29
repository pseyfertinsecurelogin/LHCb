#ifndef STFULLDECODING_H
#define STFULLDECODING_H 1

#include <string>

#include "Event/RawBank.h"
#include "GaudiKernel/StatusCode.h"
// Include files
#include "Kernel/STAlgBase.h"

class ISvcLocator;

/** @class STFullDecoding STFullDecoding.h
 *
 *  Algorithm to decode the NZS ST data in the RawEvent buffer into STTELL1Data
 *  objects. Job options:
 *  - \b PrintErrorInfo: Flag to print out errors from event info.
 *  - \b InputLocation: Location of RawEvent
 *  - \b OutputLocation: Location of NZS output data, e.g. TTFull or ITFull
 *  - \b EventInfoLocation: Location of event info data
 *  \sa{https://edms.cern.ch/document/692431/3}.
 *
 *  @author Mathias Knecht, Jeroen van Tilburg
 *  @date   2007-09-11
 */
class STFullDecoding : public ST::AlgBase {

public:

  /// Standard constructor
  STFullDecoding( const std::string& name, ISvcLocator* pSvcLocator );
  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:

  LHCb::RawBank::BankType m_bankType;

  // job options
  bool m_printErrorInfo;           ///< Flag to print out errors from event info
  std::string m_inputLocation;     ///< Location of RawEvent
  std::string m_outputLocation;    ///< Location of NZS output data, e.g. TTFull
  std::string m_eventInfoLocation; ///< Location of event info data

};
#endif // STFULLDECODING_H
