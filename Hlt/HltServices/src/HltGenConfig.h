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
#ifndef HLTGENCONFIG_H
#define HLTGENCONFIG_H 1

#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/IAlgManager.h"
#include "GaudiKernel/IAlgTool.h"
#include "Kernel/IConfigAccessSvc.h"
#include "Kernel/IPropertyConfigSvc.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <map>

/** @class HltGenConfig HltGenConfig.h
 *
 *  functionality:
 *         generate configurations by interrogating
 *         a Gaudi job...
 *
 *  @author Gerhard Raven
 *  @date   2007-10-24
 */
using namespace std::literals::string_literals;
class HltGenConfig : public GaudiAlgorithm, public IToolSvc::Observer
{
  public:
    using GaudiAlgorithm::GaudiAlgorithm;

    StatusCode initialize() override; ///< Algorithm initialization
    StatusCode execute() override;    ///< Algorithm execution
    StatusCode finalize() override;   ///< Algorithm finalization

    void onCreate( const IAlgTool* tool ) override;

  private:
    typedef unsigned int TCK_t;

    SmartIF<IAlgManager> m_appMgr;
    Gaudi::Property<std::string> s_accessSvc { this, "ConfigAccessSvc"s, "ConfigTarFileAccessSvc"s };
    SmartIF<IConfigAccessSvc> m_accessSvc;
    Gaudi::Property<std::string> s_configSvc { this,  "PropertyConfigSvc"s, "PropertyConfigSvc"s };
    SmartIF<IPropertyConfigSvc> m_configSvc;
    Gaudi::Property<std::vector<std::string>> m_topConfig{ this, "ConfigTop",{  "Hlt"s  } } ; ///< where to start when configuring
    Gaudi::Property<std::vector<std::string>> m_svcConfig{ this, "ConfigSvc",{ "ToolSvc"s, "HltANNSvc"s, "Hlt::Service"s }}; ///< Which services to configure
    Gaudi::Property<std::string> m_hltType{ this, "HltType"s }; ///< what runtype do we label this config as?
    Gaudi::Property<std::string> m_release{ this, "MooreRelease"s }; ///< what is the Moore release we use as a label?
    Gaudi::Property<std::string> m_label{ this, "Label"s };   ///< what is the label?
    Gaudi::Property<std::map<std::string, std::vector<std::string>>> m_overrule {this, "Overrule"s };
    Gaudi::Property<std::vector<std::string>> m_envVars{ this, "EnvironmentVariables"s, {"PARAMFILESROOT"s}}; ///< Which environment variables do we put back in.
    std::unordered_map<std::string, std::string> m_envVarValues;
    mutable std::unordered_set<std::string> m_overruled;

    StatusCode generateConfig() const;

    ConfigTreeNode::digest_type generateConfig( const INamedInterface& obj ) const;
    template <typename I, typename INS, typename R>
    StatusCode getDependencies( I begin, I end, INS inserter , R resolver) const;

    // keep track of whom uses which tools
    std::multimap<std::string, const IAlgTool*>  m_toolmap;

    // obtain dependencies
    std::vector<PropertyConfig::digest_type>
    gatherDependencies( const INamedInterface& obj ) const;
};
#endif // HLTGENCONFIG_H
