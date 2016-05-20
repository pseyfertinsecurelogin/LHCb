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

   ~HltRoutingBitsWriter( ) override;   ///< Destructor

   StatusCode initialize() override;    ///< Algorithm execution
   StatusCode execute   () override;    ///< Algorithm execution

protected:

   /// Decode
   StatusCode decode() override;

private:

   void zeroEvaluators();
   void updateBits( Property& /* p */ );

   std::unordered_map<unsigned int, EvalVariant> m_evaluators;
   bool m_updateBank;

};
#endif // HLTROUTINGBITSWRITER_H
