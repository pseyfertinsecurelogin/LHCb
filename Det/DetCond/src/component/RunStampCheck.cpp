#include "GaudiKernel/Service.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/IEventProcessor.h"
#include "DetCond/ICondDBReader.h"
#include "DetDesc/RunChangeIncident.h"

#define ON_DEBUG if (UNLIKELY(msgLevel(MSG::DEBUG)))
#define DEBUG_MSG ON_DEBUG debug()

/** Simple service to check if the run stamp condition exists for the current
 *  event.
 *
 *  To ensure that the content of the conditions database includes alignments
 *  and calibrations for the event being processed, when these conditions are
 *  stored we also store a special condition with closed Interval Of Validity
 *  (IOV) covering only the run for which they are valid (RunStamp condition).
 *
 *  The time line of the RunStamp conditions is not completely covered, and the
 *  holes in the time line implicitly flag the events for which the alignments
 *  are not available.
 *
 *  When RunStampCheck is instantiated in a Gaudi application, it checks for
 *  each event time if the RunStamp condition exists or not, in which case the
 *  application is terminated with an error code.
 *
 *  So, to enable the check, it is enough to add to the options:
 *  \code{.py}
 *  from Configurables import ApplicationMgr, RunStampCheck
 *  ApplicationMgr().ExtSvc.append(RunStampCheck())
 *  \endcode
 *
 *  \see https://its.cern.ch/jira/browse/LBCORE-831
 *  \see https://its.cern.ch/jira/browse/LHCBPS-1421
 */
class RunStampCheck: public extends<Service, IIncidentListener> {
public:
  /// Constructor. Declares properties.
  using extends::extends;

  /// Connect to the required services and register as BeginEvent listener.
  StatusCode start() override {
    StatusCode sc = Service::start();
    if (UNLIKELY(!sc)) return sc;

    // ensure that we have the EventClocksvc (to get the current event time in
    // the DetectorDataSvc).
    if (UNLIKELY(!serviceLocator()->service("EventClockSvc"))) {
      error() << "Cannot get EventClockSvc" << endmsg;
      return StatusCode::FAILURE;
    }

    m_incSvc = serviceLocator()->service("IncidentSvc");
    if (UNLIKELY(!m_incSvc)) {
      error() << "Cannot get IncidentSvc" << endmsg;
      return StatusCode::FAILURE;
    }
    m_incSvc->addListener(this, IncidentType::RunChange);

    m_condDBReader = serviceLocator()->service(m_condDBReaderName);
    if (UNLIKELY(!m_condDBReader)) {
      error() << "Cannot get " << m_condDBReaderName.value() << endmsg;
      return StatusCode::FAILURE;
    }

    m_evtProc = serviceLocator();
    if (UNLIKELY(!m_evtProc)) {
      error() << "Cannot get IEventProcessor" << endmsg;
      return StatusCode::FAILURE;
    }

    return sc;
  }
  /// Deregister as BeginEvent listener and release reference to services.
  StatusCode stop() override {
    m_incSvc->removeListener(this, IncidentType::RunChange);
    m_incSvc.reset();
    m_condDBReader.reset();
    m_evtProc.reset();
    return Service::stop();
  }
  /// Handle the BeginEvent incident to check if the RunStamp condition exists.
  void handle(const Incident& inc) override {
    DEBUG_MSG << inc.type() << " incident received" << endmsg;

    const RunChangeIncident* rci = dynamic_cast<const RunChangeIncident*>(&inc);
    if (!rci) {
      error() << "Cannot dynamic_cast the incident to RunChangeIncident, "
                 "run change ignored" << endmsg;
      return;
    }

    auto when = rci->eventTime();
    DEBUG_MSG << "Checking '" << m_runStampCondition.value()
              << "' for event time " << when << endmsg;
    ICondDBReader::DataPtr p;
    std::string desc;
    Gaudi::Time since, until;
    StatusCode sc = m_condDBReader->getObject(m_runStampCondition, when,
        p, desc, since, until);
    if (!sc) {
      // we didn't manage to get the entry from the DB: we do not have data
      // for this run
      error() << "Database not up-to-date. No valid data for run at "
          << when.format(false, "%Y-%m-%d %H:%M:%S")
          << "." << when.nanoformat() << " UTC" << endmsg;
      m_evtProc->stopRun();
    } else
      DEBUG_MSG << "Found '" << m_runStampCondition.value()
                << "' valid in [" <<  since << ", " << until << ")" << endmsg;
  }
private:
  Gaudi::Property<std::string> m_runStampCondition{this, "RunStamp", "/Conditions/Online/LHCb/RunStamp.xml",
    "Path in the conditions database of the RunStamp condition."};
  Gaudi::Property<std::string> m_condDBReaderName{this, "CondDBReader", "CondDBCnvSvc",
    "Name of the ICondDBReader instance to query for the RunStamp."};

  /// reference to the incident service.
  SmartIF<IIncidentSvc> m_incSvc;
  /// reference to the CondDB reader.
  SmartIF<ICondDBReader> m_condDBReader;
  /// reference to the event processor.
  SmartIF<IEventProcessor> m_evtProc;
};

DECLARE_COMPONENT(RunStampCheck)
