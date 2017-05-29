// $Id: RecordStream.cpp,v 1.1 2008/12/10 18:37:36 marcocle Exp $
#define LHCB_RECORDSTREAM_CPP

#include <functional>

#include "Gaudi/Details/PluginServiceDetails.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/SmartIF.h"
#include "LHCbRecordStream.h"

class ISvcLocator;

// Define the algorithm factory for the standard output data writer
DECLARE_ALGORITHM_FACTORY(LHCbRecordStream)

// Standard Constructor
LHCbRecordStream::LHCbRecordStream(const std::string& name, ISvcLocator* pSvcLocator)
 : LHCbOutputStream(name, pSvcLocator)
{
  ///in the baseclass, always fire the incidents by default
  ///in LHCbRecordStream this will be set to false, and configurable
  m_fireIncidents  = false;
  declareProperty("FireIncidents",      m_fireIncidents=false);
}

StatusCode LHCbRecordStream::finalize() {
  MsgStream log(msgSvc(), name());
  log << MSG::INFO << "Set up File Summary Record" << endmsg;
  if( !m_fireIncidents && log.level() <= MSG::VERBOSE )
    log << MSG::VERBOSE << "will not fire incidents" << endmsg;
  StatusCode sc = LHCbOutputStream::execute();
  if( !sc.isSuccess() )  {
    log << MSG::WARNING << "Error writing run summary record....." << endmsg;
  }
  return LHCbOutputStream::finalize();
}
