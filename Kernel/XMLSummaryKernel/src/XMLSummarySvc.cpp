/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#include "XMLSummarySvc.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/IAppMgrUI.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/AppReturnCode.h"
#include "GaudiUtils/RegEx.h"
#include "GaudiUtils/IIODataManager.h"
#include "GaudiKernel/Memory.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DECLARE_COMPONENT(XMLSummarySvc)

using namespace std;

namespace {
  /// @FIXME: (MCl) work-around to fix the warning
  ///
  ///   warning: deprecated conversion from string constant to ‘char*’
  ///
  /// that occurs when a string constant (e.g.: "abc", "") is passed to a function
  /// expecting char *
  inline char *chr(const char*c){ return const_cast<char*>(c); }

  /// Helper class to protect calls to Python code with the GIL.
  struct PyGILGuard {
    PyGILGuard(): gstate(PyGILState_Ensure()) {}
    ~PyGILGuard() { PyGILState_Release(gstate); }
    PyGILState_STATE gstate;
  };
}

//
///////////////////////////////////////////////////////////////////////////
//

#define ON_DEBUG if (msgLevel(MSG::DEBUG))
#define DEBUG_MSG ON_DEBUG debug()

#define ON_VERBOSE if (msgLevel(MSG::VERBOSE))
#define VERBOSE_MSG ON_VERBOSE verbose()


StatusCode
XMLSummarySvc::initialize()
{

  StatusCode sc = Service::initialize();
  if (!sc.isSuccess()) return sc;

  DEBUG_MSG << "initialize" << endmsg;

  //prepare the incident service
  sc = prepareIncSvc();
  if (!sc.isSuccess()) warning() << "could not init incSvc" << endmsg;

  //prepare the IODataManager service
  sc=service("IODataManager", m_ioDataManager);//, false);//,false);
  if(!sc.isSuccess() || m_ioDataManager== nullptr) return StatusCode::FAILURE;

  //get the python object, then initialize it
  sc=prepareXML();
  if(!sc.isSuccess()) return sc;

  {
    PyGILGuard gil;
    PyObject_CallMethod(m_summary, chr("set_step"), chr("s"), chr("initialize"));
  }

  //temp
  m_filename="PFN:/path/filename.dst";
  //todo fill the list of file/GUIDs from somewhere
  std::vector<std::string> filenames;
  //try this
  auto connections = m_ioDataManager->connections(nullptr);
  for(const auto& conn: connections)
  {
    if(conn && !conn->pfn().empty())
      filenames.push_back(conn->pfn());
  }


  {
    PyGILGuard gil;
    //fill the initial list of filenames
    for(const auto& fn: filenames)
    {
      PyObject_CallMethod(m_summary,
                          chr("fill_input"),
                          chr("s"),
                          chr(fn.c_str())
                          );
    }
  }
  //output the file, and write to the stdout
  printXML(MSG::DEBUG).ignore();
  writeXML(MSG::INFO).ignore();

  DEBUG_MSG << "initialized successfully" << endmsg;
  return StatusCode::SUCCESS;

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

StatusCode
XMLSummarySvc::finalize()
{

  DEBUG_MSG << "finalize" << endmsg;
  if(!isConfigured())
  {
    DEBUG_MSG << "no longer configured" << endmsg;
    return Service::finalize();
  }

  //add own counter
  addCounter(name(),"handled",m_handled);

  //need to get return code from AppMgr

  int gaudiReturn = Gaudi::ReturnCode::Success;

  SmartIF<IProperty> appmgr(serviceLocator()->service("ApplicationMgr"));
  gaudiReturn=Gaudi::getAppReturnCode(appmgr);

  if ( gaudiReturn != Gaudi::ReturnCode::Success &&
       std::any_of(begin(m_successExitCodes), end(m_successExitCodes),
                   [gaudiReturn] (const int x) { return x == gaudiReturn; }) ) {
    // remap exit code to success
    Gaudi::setAppReturnCode(appmgr, Gaudi::ReturnCode::Success, true).ignore();
    gaudiReturn = Gaudi::ReturnCode::Success;
  }
  //IntegerProperty returnCode("ReturnCode",0);

  //const IntegerProperty returnCode=(const IntegerProperty) getProperty("ReturnCode");//Gaudi::Utils::getProperty(&returnCode);

  //log << MSG::VERBOSE << "state " << FSMState()
  //    << " running " << Gaudi::StateMachine::RUNNING << endmsg;
  //IInterface* iface = Gaudi::createApplicationMgr();
  //SmartIF<IProperty> appmgr(iface);

  //int gaudiReturn=returnCode.value();
  //Gaudi::getAppReturnCode(this);
  //getProperty(this,"ReturnCode");
  //Gaudi::getAppReturnCode(m_propertyMgr);
  DEBUG_MSG << "gaudi return code" << gaudiReturn << endmsg;


  //write collected counters
  {
    PyGILGuard gil;
    PyObject_CallMethod(m_summary, chr("set_step"), chr("s,i"), chr("finalize"),
                        int(gaudiReturn == Gaudi::ReturnCode::Success));
  }

  info() << "filling counters..." << endmsg;
  fillcounters().ignore();
  info() << "counters filled OK" << endmsg;

  //finally add memory usage
  fillUsage().ignore();

  printXML(MSG::DEBUG).ignore();
  writeXML(MSG::INFO).ignore();

  //stop once finalize is called!
  m_summary=nullptr;

  return Service::finalize();

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void XMLSummarySvc::addCounter(
                               const std::string alg,
                               const std::string name,
                               const StatEntity & count,
                               const Gaudi::CounterSummary::SaveType saveType)
{
  std::string full_name=alg+"/"+name;
  NameStatPair c(full_name,count);
  NameStatTypePair ct(c,saveType);
  m_addedCounters.push_back(ct);

  return;
}

void XMLSummarySvc::addCounter(
                               const std::string alg,
                               const std::string name,
                               const Stat & count,
                               const Gaudi::CounterSummary::SaveType saveType)
{
  StatEntity newEnt=*(count.entity());
  addCounter(alg,name,newEnt,saveType);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
inline bool XMLSummarySvc::isConfigured() const
{
  if (m_summary==nullptr || !m_configured) return false;
  PyGILGuard gil;
  PyObject* res = PyObject_CallMethod(m_summary, chr("test"), chr(""));
  if (res==nullptr || res==Py_None) return false;
  return res==Py_True;

}
void XMLSummarySvc::handle( const Incident& incident )
{
  //check extended file incidents are defined
#ifdef GAUDI_FILE_INCIDENTS

  if (!isConfigured())
  {
    DEBUG_MSG << "the summary object is no longer available "
        << incident.type() << " Ignored" << endmsg;
    return;

  }

  ON_VERBOSE if(incident.type()!=m_endIncident && incident.type()!=m_beginIncident)
    verbose() << incident.type() << ":" << incident.source() << endmsg;

  //std::string GUID="";m_GUID;
  std::string filename=m_filename;
  std::string method="fill_input";
  std::string status="";
  int addevents=0;

  if(incident.type()==IncidentType::BeginInputFile)
    {
      status="part";
      //update current filename and GUID
      filename=m_filename=incident.source();
      m_hasinput=true;
    }
  else if(incident.type()==IncidentType::BeginOutputFile)
    {
      status="none";
      method="fill_output";
      //update current filename and GUID
      filename=incident.source();
    }
  else if(incident.type()==IncidentType::EndInputFile)
    {
      status="full";
      //use end name so that no race conditions occur between end file and end event
      filename=m_filename=incident.source();
      m_hasinput=true;

    }
  else if(incident.type()==IncidentType::EndOutputFile)
    {
      status="full";
      method="fill_output";
      filename=incident.source();
    }
  else if(incident.type()==IncidentType::FailInputFile)
    {
      status="fail";
      //do not update filename, so that end events are never attributed
      //to failed files, possibly could produce a race condition
      filename=incident.source();
      m_hasinput=true;

    }
  else if(incident.type()==IncidentType::FailOutputFile)
    {
      status="fail";
      method="fill_output";
      filename=incident.source();
    }
  // perhaps BeginEvent should also change the active file to wherever
  // /Event is stored ...
  //else if(incident.type()==m_beginIncident)
  //  {
  //    status="part";
  //    addevents=1;
  //  }
  else if(incident.type()==m_endIncident)
    {
      status="part";
      addevents=1;
      m_ended++;
    }
  else if(incident.type()==IncidentType::WroteToOutputFile)
    {
      status="part";
      method="fill_output";
      addevents=1;
      filename=incident.source();
    }


  //only fill input if there is input to fill!, i.e. if EndEvent but not m_hasinput, then skip it
  if(incident.type()!=m_endIncident || m_hasinput)
  {

    //actually add to the summary
    std::string GUID=file2GUID(filename);
    ON_VERBOSE if(incident.type()!=m_endIncident && incident.type()!=m_beginIncident)
      verbose() << method <<"(" << filename << "," << GUID << "," << status << "," << addevents << ")" << endmsg;

    PyGILGuard gil;
    PyObject_CallMethod(m_summary,
                        chr(method.c_str()),
                        chr("s,s,s,d"),
                        chr(filename.c_str()),
                        chr(GUID.c_str()),
                        chr(status.c_str()),
                        double(addevents));
  }

  fillUsage().ignore();
  m_handled++;

  //never write at begin event!
  if ( incident.type()!=m_beginIncident && (
       //write all major file events
       (incident.type()==IncidentType::EndInputFile
       || incident.type()==IncidentType::FailInputFile
       || incident.type()==IncidentType::FailOutputFile
       || incident.type()==IncidentType::BeginInputFile
        || m_handled.flag()==1)
       //write every freq end events
       || (incident.type()==m_endIncident && m_freq>0 && int(m_ended.flag())%m_freq ==0) )
       )
    {

      PyGILGuard gil;
      PyObject_CallMethod(m_summary,
                          chr("set_step"),
                          chr("s"),
                          chr("execute"));
      printXML();
      writeXML();
    }

#endif //GAUDI_FILE_INCIDENTS

  return;


}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

StatusCode XMLSummarySvc::fillcounters()
{

  if (!isConfigured())
  {
    MsgStream log( msgSvc(), name() );
    log << MSG::DEBUG << "the summary object is no longer available" << endmsg;
    return StatusCode::FAILURE;

  }
  //fill all counters, in correct order
  Gaudi::CounterSummary::SaveType saveType=Gaudi::CounterSummary::SaveSimpleCounter;
  while(true)
  {
    for (const auto& cnt: m_addedCounters)
    {
      if(cnt.second == saveType) fillcounter(cnt).ignore();
    }

    if(saveType==Gaudi::CounterSummary::SaveAlwaysStatEntity) break;
    saveType=Gaudi::CounterSummary::SaveType(saveType+1);
  }

  return StatusCode::SUCCESS;
}

StatusCode XMLSummarySvc::fillcounter(const NameStatTypePair & count)
{
  if (!isConfigured())
  {
    MsgStream log( msgSvc(), name() );
    log << MSG::DEBUG << "the summary object is no longer available" << endmsg;
    return StatusCode::FAILURE;

  }

  std::string type_name="fill_counter";
  bool stat_ent=false;
  bool check=false;
  switch (count.second)
    {
    case Gaudi::CounterSummary::SaveSimpleCounter:
      type_name="fill_counter";
      check=Gaudi::Utils::RegEx::matchOr(count.first.first, m_counterList.value());
      break;
    case Gaudi::CounterSummary::SaveStatEntity:
      type_name="fill_counter";
      check=Gaudi::Utils::RegEx::matchOr(count.first.first, m_statEntityList.value());
      stat_ent=true;
      break;
    case Gaudi::CounterSummary::SaveAlwaysSimpleCounter:
      type_name="fill_lumi";
      check=true;
      break;
    case Gaudi::CounterSummary::SaveAlwaysStatEntity:
      type_name="fill_lumi";
      stat_ent=true;
      check=true;
      break;
    default:
      check=false;
      break;
    }
  //not supposed to write this one out
  //MsgStream log( msgSvc(), name() );
  if(!check) return StatusCode::SUCCESS;

  PyGILGuard gil;
  if(stat_ent)
    {
      //log << MSG::VERBOSE << "making stat entity " << endmsg;
      PyObject_CallMethod(m_summary, chr(type_name.c_str()),
                          chr("s,d,d,d,d,d"),
                          chr(count.first.first.c_str()),
                          double(count.first.second.flag()),
                          double(count.first.second.nEntries()),
                          double(count.first.second.flag2()),
                          double(count.first.second.flagMin()),
                          double(count.first.second.flagMax())
                          );
      //printXML(MSG::VERBOSE).ignore();
      return StatusCode::SUCCESS;
    }
  //log << MSG::VERBOSE << "making stimple counter "  << endmsg;
  PyObject_CallMethod(m_summary, chr(type_name.c_str()),
                      chr("s,d"),
                      chr(count.first.first.c_str()),
                      double(count.first.second.flag())
                      );
  //printXML(MSG::VERBOSE).ignore();

  return StatusCode::SUCCESS;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

StatusCode XMLSummarySvc::writeXML(const MSG::Level lev)
{
  if (m_xmlfile=="")
  {
    msgStream(lev) << "no xml file to be written " << m_xmlfile << endmsg;
    return StatusCode::SUCCESS;
  }
  if (!isConfigured())
  {
    DEBUG_MSG << "the summary object is no longer available" << endmsg;
    msgStream(lev) << "xml file not written as PyObject was lost" << endmsg;
    return StatusCode::FAILURE;
  }

  VERBOSE_MSG << "ready to write xml file " << m_xmlfile.value() << " " << m_summary << endmsg;
  PyGILGuard gil;
  PyObject_CallMethod(m_summary, chr("write"), chr("s"), chr(m_xmlfile.value().c_str()));
  msgStream(lev) << "Wrote xml file " << m_xmlfile.value() << endmsg;
  return StatusCode::SUCCESS;
}
StatusCode XMLSummarySvc::printXML(const MSG::Level lev) const
{
  if (!msgLevel(lev)) return StatusCode::SUCCESS;

  if (!isConfigured())
  {
    DEBUG_MSG << "the summary object is no longer available" << endmsg;
    msgStream(lev) << "xml file not written as PyObject was lost" << endmsg;
    return StatusCode::FAILURE;

  }
  VERBOSE_MSG << "ready to write to screen " << m_xmlfile.value() << " " << m_summary << endmsg;

  PyGILGuard gil;
  PyObject* res = PyObject_CallMethod(m_summary, chr("xml"), chr(""));
  if (res==nullptr || res==Py_None || !PyString_Check(res))
  {
    DEBUG_MSG << "Cannot print XML" << endmsg;
    return StatusCode::FAILURE;
  }

  msgStream(lev) << PyString_AsString(res) << endmsg;
  return StatusCode::SUCCESS;
}

StatusCode XMLSummarySvc::prepareIncSvc()
{
  StatusCode sc=service("IncidentSvc", m_incSvc, false);
  if(!sc.isSuccess() || m_incSvc== nullptr) return StatusCode::FAILURE;

  m_incSvc->addListener( this, m_endIncident);
  //m_incSvc->addListener( this, m_beginIncident);

  //check extended file incidents are defined
#ifdef GAUDI_FILE_INCIDENTS
  m_incSvc->addListener( this, IncidentType::BeginInputFile);
  m_incSvc->addListener( this, IncidentType::BeginOutputFile);
  m_incSvc->addListener( this, IncidentType::WroteToOutputFile);
  m_incSvc->addListener( this, IncidentType::FailOutputFile);
  m_incSvc->addListener( this, IncidentType::FailInputFile);
  m_incSvc->addListener( this, IncidentType::EndOutputFile);
  m_incSvc->addListener( this, IncidentType::EndInputFile);

#endif //GAUDI_FILE_INCIDENTS

  DEBUG_MSG << "registered with incSvc" << endmsg;

  return StatusCode::SUCCESS;
}

StatusCode XMLSummarySvc::prepareXML()
{

  // Initialize the python session if needed
  if (!Py_IsInitialized()) Py_Initialize();

  PyGILGuard gil;
  //Import the python module
  //import XMLSummaryBase.summary
  PyObject* pName=PyString_FromString("XMLSummaryBase.summary");
  m_summary = PyImport_Import(pName);
  if (m_summary==nullptr) return StatusCode::FAILURE;

  //Get the summary class object from the module
  //XMLSummaryBase.summary.Summary
  m_summary = PyObject_GetAttrString(m_summary, "Summary");
  if (m_summary==nullptr) return StatusCode::FAILURE;

  //Call the class to make an instance
  //XMLSummaryBase.summary.Summary(m_xsdfile)
  PyObject* schema=PyString_FromString(chr(m_xsdfile.value().c_str()));
  m_summary=PyObject_CallFunctionObjArgs(m_summary, schema, nullptr);
  if (m_summary==nullptr) return StatusCode::FAILURE;

  DEBUG_MSG << "xml summary object created" << endmsg;
  m_configured=true;
  return StatusCode::SUCCESS;
}
std::string XMLSummarySvc::file2GUID(const std::string & filename)
{
  FidMap::iterator j = m_fidMap.find(filename);
  if (j != m_fidMap.end() ) return j->second;
  std::string shortname=AFN2name(filename);
  j = m_fidMap.find(shortname);
  if (j != m_fidMap.end() ) return j->second;

  const Gaudi::IDataConnection * con=m_ioDataManager->connection(shortname);
  if(!con) con=m_ioDataManager->connection(shortname);
  if(!con) return "";
  m_fidMap[filename]=con->fid();
  m_fidMap[shortname]=con->fid();
  return con->fid();
}
std::string XMLSummarySvc::AFN2name(const std::string & filename) const
{
  std::string me="LFN:";
  size_t pos=filename.find(me);
  if ((pos==0) && (pos+me.size()<filename.size()))
    return filename.substr(pos+me.size());
  me="PFN:";
  pos=filename.find(me);
  if ((pos==0) && (pos+me.size()<filename.size()))
    return filename.substr(pos+me.size());
  return filename;

}

StatusCode XMLSummarySvc::fillUsage()
{

  if (!isConfigured())
  {
    DEBUG_MSG << "the summary object is no longer available" << endmsg;
    return StatusCode::FAILURE;

  }
  int mem = System::virtualMemoryPeak();
  PyGILGuard gil;
  PyObject_CallMethod(m_summary, chr("fill_memory"),
                      chr("d,s"),
                      double(mem),
                      chr("KB")
                      );
  return StatusCode::SUCCESS;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
StatusCode XMLSummarySvc::stop()
{
  DEBUG_MSG << "STOPPED" << endmsg;
  m_stopped=true;
  return Service::stop();

}
