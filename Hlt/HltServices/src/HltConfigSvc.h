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
#ifndef HLTCONFIGSVC_H
#define HLTCONFIGSVC_H 1

// Include files
#include <string>
#include <map>
#include <vector>
#include <set>

#include "Kernel/TCK.h"


class StatusCode;
namespace Gaudi { namespace Parsers {
StatusCode parse( std::map<TCK,std::string>& result, const std::string& input );
} }

// from Gaudi
#include "PropertyConfigSvc.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiAlg/IGenericTool.h"
#include "GaudiKernel/ToolHandle.h"

struct Condition;

/** @class HltConfig HltConfig.h
 *
 *  functionality:
 *         configure the HLT algorithms...
 *
 *  @author Gerhard Raven
 *  @date   2007-10-24
 */
class HltConfigSvc : public extends<PropertyConfigSvc, IIncidentListener> {
public:

  using extends::extends;

  StatusCode initialize() override;    ///< Service initialization
  StatusCode start() override;    ///< Service initialization
  StatusCode finalize() override;      ///< Service finalization
  void handle(const Incident&) override;

private:

  StatusCode updateTCK();
  void createHltDecReports();

  void updateMap(Property&);
  void updateInitial(Property&);

  typedef std::map<TCK,std::string> TCKMap_t;

  Gaudi::Property<std::vector<std::string>>  m_outputContainerName{ this, "HltDecReportsLocations", {"/Event/Hlt1/DecReports","/Event/Hlt2/DecReports"} };  ///< location of HltDecReports in which to record configured TCK
  std::string                  m_prefetchDir;     ///< which set of configurations
                                                  ///< to search for same types as initial TCK
                                                  ///< and to prefetch...

  Gaudi::Property<TCKMap_t> m_tck2config { this, "TCK2ConfigMap" };     ///< from TCK to configuration ID
  Gaudi::Property<TCK> m_initialTCK {this, "initialTCK" };     ///< which TCK to start with...

  mutable TCKMap_t             m_tck2configCache; ///< from TCK to configuration ID
  TCK                          m_configuredTCK { 0 } ;   ///< which TCK is currently in use?
  SmartIF<IDataProviderSvc>    m_evtSvc ;          ///< get Evt Svc to get ODIN (which contains TCK)
  SmartIF<IIncidentSvc>        m_incidentSvc;     ///<
  ToolHandle<IGenericTool>     m_decodeOdin { "ODINDecodeTool",this };
  Gaudi::Property<bool>        m_checkOdin { this, "checkOdin", true };
  Gaudi::Property<bool>        m_maskL0TCK { this, "maskL0TCK", false};
  unsigned int                 m_id = ~0u;
  Gaudi::Property<bool>        m_hlt2Mode{ this, "Hlt2Mode", false};
  Gaudi::Property<std::string> m_tckConditionPath{ this, "Hlt2TCKCondition",  "/dd/Conditions/Online/LHCb/RunInfo/HLT2"};

  Condition*                   m_tckCondition;

  // resolve TCK -> toplevel config ID, then call method with ID
  ConfigTreeNode::digest_type tck2id(TCK    tck) const;

};
#endif // HLTCONFIGSVC_H
