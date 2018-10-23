/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
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

  IL0DUConfigProvider*   m_confTool = nullptr;
  IL0CondDBProvider*     m_conddb   = nullptr;
  mutable std::atomic<const LHCb::L0DUConfig*> m_config = { nullptr };

  Gaudi::Property<bool> m_ensureKnownTCK   { this, "EnsureKnownTCK", false };
  Gaudi::Property<std::string> m_configName{ this, "L0DUConfigProviderName", "L0DUConfig" };
  Gaudi::Property<std::string> m_configType{ this, "L0DUConfigProviderType", "L0DUMultiConfigProvider" };

  mutable Gaudi::Accumulators::AveragingCounter<unsigned int> m_rawbankSize{ this, "L0DU RawBank Size (Bytes)" };
};
#endif // L0DUFROMRAWHLT1TOOL_H
