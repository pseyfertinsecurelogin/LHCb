#ifndef L0DUDECODER_H
#define L0DUDECODER_H 1

// Include files
#include <string>
#include <atomic>
// from Gaudi
#include "GaudiAlg/Transformer.h"
#include "L0Interfaces/IL0DUConfigProvider.h"
#include "L0Interfaces/IL0CondDBProvider.h"

// from Event
#include "Event/L0DUReport.h"
#include "Event/RawEvent.h"

/** @class L0DUDecoder L0DUDecoder.h
 *
 *  @author Gerhard Raven, based L0DUFromRawHlt1Tool by Olivier Callot
 *  @date   2016-11-22
 */

class L0DUDecoder : public Gaudi::Functional::Transformer< LHCb::L0DUReport( const LHCb::RawEvent& ) >
{
public:
  /// Standard constructor
  L0DUDecoder( const std::string& name, ISvcLocator* pSvcLocator);

  StatusCode initialize() override;
  LHCb::L0DUReport operator()(const LHCb::RawEvent& ) const override;

private:

  std::string m_configName;
  std::string m_configType;

  IL0DUConfigProvider*   m_confTool = nullptr;
  IL0CondDBProvider*     m_conddb   = nullptr;
  mutable std::atomic<LHCb::L0DUConfig*> m_config = { nullptr };
  bool m_ensureKnownTCK = false;
};
#endif // L0DUFROMRAWHLT1TOOL_H
