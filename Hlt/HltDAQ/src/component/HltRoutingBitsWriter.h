#ifndef HLTCOMMON_HLTROUTINGBITSWRITER_H
#define HLTCOMMON_HLTROUTINGBITSWRITER_H 1

// Include files
#include <array>
#include <functional>

// boost
#include <boost/variant.hpp>

// from Gaudi
#include "GaudiAlg/GaudiHistoAlg.h"
#include "GaudiKernel/IUpdateManagerSvc.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/GaudiException.h"
#include "DetDesc/Condition.h"

#include "Kernel/IHltMonitorSvc.h"

#include "LoKi/OdinTypes.h"
#include "LoKi/L0Types.h"
#include "LoKi/HLTTypes.h"

#include "HltDAQ/HltEvaluator.h"

/** @class HltRoutingBitsWriter HltRoutingBitsWriter.h
 *
 *
 *  @author Gerhard Raven
 *  @date   2008-07-29
 */
class HltRoutingBitsWriter : public HltEvaluator {
public:
   /// Standard constructor
   HltRoutingBitsWriter( const std::string& name, ISvcLocator* pSvcLocator );

   StatusCode execute   () override;    ///< Algorithm execution

private:

   /// Decode
   StatusCode decode() override;

   void zeroEvaluators();
   void updateBits( Property& /* p */ );

   std::unordered_map<unsigned int, EvalVariant> m_evaluators;
   Gaudi::Property<bool> m_updateBank { this, "UpdateExistingRawBank", false };
   Gaudi::Property<std::string> m_raw_location { this, "RawEventLocation", LHCb::RawEventLocation::Default };
   Gaudi::Property<std::map<unsigned int,std::string>> m_bits { this, "RoutingBits" };

};
#endif // HLTROUTINGBITSWRITER_H
