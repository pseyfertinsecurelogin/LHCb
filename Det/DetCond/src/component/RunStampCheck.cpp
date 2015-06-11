#include "GaudiKernel/Service.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/IDetDataSvc.h"
#include "GaudiKernel/IEventProcessor.h"
#include "DetCond/ICondDBReader.h"

class RunStampCheck: public extends1<Service, IIncidentListener> {
public:
  RunStampCheck(const std::string& name, ISvcLocator* svcloc):
    base_class(name, svcloc) {
    declareProperty("RunStamp", m_runStampCondition,
        "Path in the conditions database of the RunStamp condition.");
    declareProperty("CondDBReader", m_condDBReaderName,
        "Name of the ICondDBReader instance to query for the RunStamp.");
  }
  StatusCode start() override {
    StatusCode sc = Service::start();
    if (UNLIKELY(!sc)) return sc;

    m_incSvc = serviceLocator()->service("IncidentSvc");
    if (UNLIKELY(!m_incSvc)) {
      error() << "Cannot get IncidentSvc" << endmsg;
      return StatusCode::FAILURE;
    }
    m_incSvc->addListener(this, IncidentType::BeginEvent);

    m_condDBReader = serviceLocator()->service(m_condDBReaderName);
    if (UNLIKELY(!m_condDBReader)) {
      error() << "Cannot get " << m_condDBReaderName << endmsg;
      return StatusCode::FAILURE;
    }

    m_detSvc = serviceLocator()->service("DetectorDataSvc");
    if (UNLIKELY(!m_detSvc)) {
      error() << "Cannot get DetectorDataSvc" << endmsg;
      return StatusCode::FAILURE;
    }

    m_evtProc = serviceLocator();
    if (UNLIKELY(!m_detSvc)) {
      error() << "Cannot get IEventProcessor" << endmsg;
      return StatusCode::FAILURE;
    }

    // reset the IOV for the current run (to something not valid)
    m_currentRunIOV = {Gaudi::Time::epoch(), Gaudi::Time::epoch()};
    return sc;
  }
  StatusCode stop() override {
    m_incSvc->removeListener(this, IncidentType::BeginEvent);
    m_incSvc.reset();
    m_condDBReader.reset();
    m_detSvc.reset();
    m_evtProc.reset();
    return Service::stop();
  }
  void handle(const Incident&) override {
    auto when = m_detSvc->eventTime();
    // run the check only if the current event time falls outside the boundaries
    // of the current run
    if (when < m_currentRunIOV.since || when >= m_currentRunIOV.until) {
      if (UNLIKELY(msgLevel(MSG::DEBUG)))
        debug() << "Checking '" << m_runStampCondition
                << "' for event time " << when << endmsg;
      ICondDBReader::DataPtr p;
      std::string desc;
      StatusCode sc = m_condDBReader->getObject(m_runStampCondition, when,
          p, desc,
          m_currentRunIOV.since, m_currentRunIOV.until);
      if (!sc) {
        // we didn't manage to get the entry from the DB: we do not have data
        // for this run
        error() << "Database not up-to-date. No valid data for run at "
            << when << endmsg;
        m_evtProc->stopRun();
      } else if (UNLIKELY(msgLevel(MSG::DEBUG))) {
        debug() << "Found '" << m_runStampCondition
          << "' valid in [" <<  m_currentRunIOV.since
          << ", " << m_currentRunIOV.until << ")" << endmsg;
      }
    }
  }
private:
  /// Path in the conditions database of the RunStamp condition.
  std::string m_runStampCondition = "/Conditions/Online/LHCb/RunStamp.xml";
  /// Path in the conditions database of the RunStamp condition.
  std::string m_condDBReaderName = "CondDBCnvSvc";
  /// reference to the incident service.
  SmartIF<IIncidentSvc> m_incSvc;
  /// reference to the CondDB reader.
  SmartIF<ICondDBReader> m_condDBReader;
  /// reference to the detector transient store.
  SmartIF<IDetDataSvc> m_detSvc;
  /// reference to the event processor.
  SmartIF<IEventProcessor> m_evtProc;

  /// IOV of the current RunStamp.
  ICondDBReader::IOV m_currentRunIOV;
};

DECLARE_COMPONENT(RunStampCheck)
