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
#ifndef GAUDISVC_XMLSUMMARYSVC_H
#define GAUDISVC_XMLSUMMARYSVC_H

#include "GaudiKernel/AppReturnCode.h"
#include "GaudiKernel/ICounterSummarySvc.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Service.h"
#include "GaudiKernel/Stat.h"
#include "GaudiKernel/StatEntity.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiUtils/IFileCatalog.h"
#include "Python.h"
#include <algorithm>
#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace Gaudi {
  class IIODataManager;
}

/** @class XMLSummarySvc XMLSummarySvc.h
 *
 *  Service controlling the python writing of
 *  an XML summary.
 *  Inherits from the Incident handler->gets EOF events
 *  To collect all counters it must be named "CounterSummarySvc"
 *
 *  @code
 *   sum=XMLSummarySvc("CounterSummarySvc")
 *   sum.xmlfile="summary.xml"
 *   ApplicationMgr().ExtSvc+=sum
 *
 *   #OR
 *   LHCbApp().XMLSummary="summary.xml"
 *  @endcode
 *
 *  options CounterList, StatEntityList
 *  options xmlfile, xsdfile
 *  options UpdateFreq
 *
 *  The file xmlfile is written:
 *    at initialisation/finalisation
 *    at beginning/ending/failing an input file
 *    at the first call to handle
 *    once every UpdateFreq calls to handle
 *
 *  @author Robert Lambert
 *  @date   2009-09-11
 */
class XMLSummarySvc : public extends<Service, ICounterSummarySvc, IIncidentListener> {
public:
  using extends::extends;

  // a list of pairs is used, as referring to the entry by key is not required
  // sorting is also not required, and would waste time
  // these internal typedefs are used as a shortcut only
  typedef std::pair<std::string, const StatEntity> NameStatPair;
  // first=name, second=counter
  typedef std::pair<std::pair<std::string, const StatEntity>, const Gaudi::CounterSummary::SaveType> NameStatTypePair;
  // first.first=name, first.second=counter, second=SaveType
  typedef std::list<std::pair<std::pair<std::string, const StatEntity>, const Gaudi::CounterSummary::SaveType>>
      NameStatList;

  StatusCode initialize() override;
  StatusCode finalize() override;

  StatusCode stop() override;

  // ==========================================================================
  // ICounterSummarySvc interface
  // ==========================================================================

  void addCounter( const std::string alg, const std::string name, const StatEntity& count,
                   const Gaudi::CounterSummary::SaveType saveType = Gaudi::CounterSummary::SaveSimpleCounter ) override;

  void addCounter( const std::string alg, const std::string name, const Stat& count,
                   const Gaudi::CounterSummary::SaveType saveType = Gaudi::CounterSummary::SaveSimpleCounter ) override;
  // ==========================================================================
  // IIncindentListener interface
  // ==========================================================================
  void handle( const Incident& ) override;
  // ==========================================================================

private:
  Gaudi::Property<std::string>              m_xmlfile{this, "xmlfile", "summary.xml", "name of XML file to write"};
  Gaudi::Property<std::string>              m_xsdfile{this, "xsdfile", "$XMLSUMMARYBASEROOT/xml/XMLSummary.xsd",
                                         "name of the schema to use"};
  Gaudi::Property<std::vector<std::string>> m_counterList{
      this, "CounterList", {".*"}, "regex list of integer counters to write"};
  Gaudi::Property<std::vector<std::string>> m_statEntityList{
      this, "StatEntityList", {".*"}, "regex list of StatEntity counters to write"};
  Gaudi::Property<int>              m_freq{this, "UpdateFreq", 500, "force writeout of the file every x incidents"};
  Gaudi::Property<std::string>      m_beginIncident{this, "BeginEventIncident", IncidentType::BeginEvent,
                                               "type of incident to regard as begin event"};
  Gaudi::Property<std::string>      m_endIncident{this, "EndEventIncident", IncidentType::EndEvent,
                                             "type of incident to regard as end event"};
  Gaudi::Property<std::vector<int>> m_successExitCodes{
      this, "SuccessExitCodes", {}, "non-zero exit codes that should be remapped to success"};

  IIncidentSvc*          m_incSvc        = nullptr; /// the incident service
  Gaudi::IIODataManager* m_ioDataManager = nullptr; /// the pointer to the data manager service

  PyObject*  m_summary = nullptr; /// the pointer to the python object
  StatEntity m_handled;           /// simple counter of #handled events
  StatEntity m_ended;             /// simple counter of #begin-end

  /// the list of added counters
  NameStatList m_addedCounters;

  std::string m_filename; /// the current open filename

  /// has the xml object been configured?
  bool m_configured = false;
  /// is the object still configured?
  inline bool isConfigured() const;
  /// has stop been called?
  bool m_stopped = false;

  /// has at least one input file been opened in this job?
  bool m_hasinput = false;

  /// counters are filled in the finalise method, using these internal functions
  StatusCode fillcounters();
  StatusCode fillcounter( const NameStatTypePair& count );

  /// fill in the virtual memory usage
  StatusCode fillUsage();

  /// shorcut to prepare the incident service
  StatusCode prepareIncSvc();

  /// shorcut to define the summary object
  StatusCode prepareXML();
  /// shorcut to write the summary
  StatusCode writeXML( const MSG::Level lev = MSG::VERBOSE );
  StatusCode printXML( const MSG::Level lev = MSG::VERBOSE ) const;

  std::string file2GUID( const std::string& filename );
  std::string AFN2name( const std::string& filename ) const;

  /// Map of FID to PFN
  typedef std::map<std::string, std::string> FidMap;
  /// Map of FID to PFN
  std::map<std::string, std::string> m_fidMap;
};

#endif
