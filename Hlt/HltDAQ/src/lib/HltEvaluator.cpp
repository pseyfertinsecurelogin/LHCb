// Include files
// from Boost
#include <boost/format.hpp>
#include "boost/algorithm/string/join.hpp"

// from Gaudi
#include "GaudiKernel/IIncidentSvc.h"
#include "AIDA/IHistogram1D.h"

// Event
#include "Event/RawEvent.h"
#include "Event/RawBank.h"
#include "Event/HltDecReports.h"
#include "Event/L0DUReport.h"
#include "Event/ODIN.h"

// Hlt Interfaces
#include "Kernel/RateCounter.h"

// from LoKi:
#include "LoKi/IHltFactory.h"

// local
#include "HltDAQ/HltEvaluator.h"

//-----------------------------------------------------------------------------
// Implementation file for class : HltEvaluator
//
// 2016-05-01 : Roel Aaij
//-----------------------------------------------------------------------------

namespace {
   using std::string;
}

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
HltEvaluator::HltEvaluator(const std::string& name, ISvcLocator* pSvcLocator)
   : base_class( name , pSvcLocator )
{
   declareProperty("ODINLocation", m_odin_location = LHCb::ODINLocation::Default);
   declareProperty("L0DUReportLocation", m_l0_location = LHCb::L0DUReportLocation::Default);
   declareProperty("Hlt1DecReportsLocation", m_hlt_location[0] = LHCb::HltDecReportsLocation::Default);
   declareProperty("Hlt2DecReportsLocation", m_hlt_location[1] = LHCb::HltDecReportsLocation::Default);
   declareProperty("Preambulo", m_preambulo_)->declareUpdateHandler(&HltEvaluator::updatePreambulo , this);
   declareProperty("TrendTimeSpan", m_timeSpan = 4000 );
   declareProperty("TrendBinWidth", m_binWidth = 10 );
   declareProperty("GetStartOfRunFromCondDB", m_useCondDB = true);
   declareProperty("MonitorSvc", m_monSvc = "HltMonitorSvc/Hlt2MonSvc");
}

//=============================================================================
StatusCode HltEvaluator::times(double& t, double& w, double& et) const
{
   // go from microseconds to seconds
   auto odin = get<LHCb::ODIN>(m_odin_location);
   if (!odin) return StatusCode::FAILURE;

   et = odin->gpsTime() / 1e6;
   t = odin->gpsTime() >= startOfRun() ? double(odin->gpsTime() - startOfRun()) : -double(startOfRun() - odin->gpsTime());
   // t in seconds
   t /= 1e6;

   // m_binWidth is in seconds, need rate in Hz
   w = 1. / binWidth();

   return StatusCode::SUCCESS;
}


//=============================================================================
void HltEvaluator::updatePreambulo(Property& /* p */)
{
   // concatenate the preambulo:
   m_preambulo = boost::algorithm::join(m_preambulo_ , "\n");
   /// mark as "to-be-updated"
   m_preambulo_updated = true;
   // no further action if not yet initialized
   if (Gaudi::StateMachine::INITIALIZED > FSMState()) return;
   // postpone the action
   if (!m_evals_updated) return;

   // perform the actual immediate decoding
   StatusCode sc = decode();
   Assert (sc.isFailure(), "Error from HltRoutingBits::decode()", sc);
}

//=============================================================================
// Initialization
//=============================================================================
StatusCode HltEvaluator::initialize() {
   StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
   if (sc.isFailure()) return sc;  // error printed already by GaudiAlgorithm

   auto incSvc = service<IIncidentSvc>("IncidentSvc");
   if (m_useCondDB) {
      m_updMgrSvc = service("UpdateManagerSvc");
      if(!m_updMgrSvc) {
         error()<< "Could not retrieve UpdateManagerSvc" << endmsg;
         return StatusCode::FAILURE;
      }
      m_updMgrSvc->registerCondition(this, "Conditions/Online/LHCb/RunParameters",
                                     &HltEvaluator::i_updateConditions, m_runpars);
      sc = m_updMgrSvc->update(this);
      if(!sc.isSuccess()) return sc;
   } else {
      // reset m_startOfRun to zero at start of run....
      incSvc->addListener(this, IncidentType::BeginRun, 0, false, false);
      incSvc->addListener(this, "RunChange",0, false, false);
   }
   incSvc->addListener(this, IncidentType::BeginEvent, 0, false, false);

   // Hlt Monitoring Service
   m_hltMonSvc = service(m_monSvc, false, true); // createIf=false, quiet=true
   // If we cannot retrieve it, HLT2 style monitoring is disabled.
   if (!m_hltMonSvc.isValid()) {
      info() << "Could not retrieve " << m_monSvc << endmsg;
   }

   // Decode functors
   return decode();
}

//=============================================================================
void HltEvaluator::handle(const Incident& incident) {
   if (!m_useCondDB && (incident.type() == IncidentType::BeginRun
                        || incident.type() == "RunChange")) {
      m_startOfRun = 0;
   }
   m_data.clear();
}

//=============================================================================
StatusCode HltEvaluator::i_updateConditions()
{
   if (m_runpars==nullptr) {
      error() << "Could not obtain Condition for run parameters from conditions DB" << endmsg;
      return StatusCode::FAILURE;
   }
   if (!m_runpars->exists("RunStartTime")) {
      error() << "Condition does not contain RunStartTime " << endmsg;
      return StatusCode::FAILURE;
   }

   //from seconds -> microseconds since 1/1/1970
   m_startOfRun = (long long unsigned int) (m_runpars->param<int>("RunStartTime") * 1e6);
   return StatusCode::SUCCESS;
}
