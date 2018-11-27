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
// Include files
#include <algorithm>

// boost
#include <boost/algorithm/string.hpp>

// from Gaudi
#include "GaudiKernel/IAlgManager.h"
#include "GaudiKernel/IJobOptionsSvc.h"
#include "GaudiKernel/IToolSvc.h"
#include "GaudiKernel/IAuditor.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/AlgTool.h"
#include "GaudiKernel/Service.h"
#include "GaudiKernel/System.h"

#include "GaudiKernel/TypeNameString.h"

// local
#include "HltGenConfig.h"
#include "Kernel/PropertyConfig.h"

namespace {
   namespace ba = boost::algorithm;
}

//-----------------------------------------------------------------------------
// Implementation file for class : HltGenConfig
//
// 2007-10-24 : Gerhard Raven
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( HltGenConfig )

//=============================================================================
// Initialization
//=============================================================================
StatusCode HltGenConfig::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize();
  m_appMgr = service("ApplicationMgr");
  m_accessSvc = service(s_accessSvc, true);
  m_configSvc = service("PropertyConfigSvc", true);
  auto toolSvc = service<IToolSvc>("ToolSvc", true);
  toolSvc->registerObserver(this);
  // FIXME: need to unregister at some point!!!
  if (m_hltType.empty()) {
    error() << "You must specify the hltType label for a configuration..."
            << endmsg;
    return StatusCode::FAILURE;
  }
  if (m_label.empty()) {
    error()
        << "You must specify short, descriptive label for a configuration..."
        << endmsg;
    return StatusCode::FAILURE;
  }

  // Save the environment variables
  for (const auto& var : m_envVars) {
     auto val = System::getEnv(var.c_str());
     if (val.empty()) {
        throw GaudiException(std::string{"Failed to obtain environment variable "} + var,
                             name(), StatusCode::FAILURE);
     }
     m_envVarValues[val] = var;
  }
  return sc;
}

std::vector<PropertyConfig::digest_type>
HltGenConfig::gatherDependencies(const INamedInterface &obj) const {
  std::vector<PropertyConfig::digest_type> depRefs;
  INamedInterface *ini = const_cast<INamedInterface *>(&obj); // we do treat obj logically const,
                                                              // even if we call code which seems
                                                              // wants non-const version of obj
  // in case of Algorithms, do some extra work...
  SmartIF<IAlgorithm> ia(ini);
  if (ia.isValid()) {
    auto subs = dynamic_cast<const Algorithm &>(*ia).subAlgorithms();
    std::transform( begin(*subs), end(*subs),
                    std::back_inserter(depRefs),
                    [&](const Algorithm*  dep) {
      debug() << "adding sub-algorithm " << dep->name() << " as dependant to " << obj.name() << endmsg;
      auto digest = generateConfig(*dep);
      if (digest.invalid())  {
          throw std::runtime_error(
                    "problem creating dependant configuration for " + dep->name() );
      }
      return digest;
    } );
  }

  // check whether this obj uses any private tools -- if so, add them as
  // dependencies
  auto tools = m_toolmap.equal_range(obj.name());
  std::transform( tools.first, tools.second,
                  std::back_inserter(depRefs),
                  [&](const std::pair<const std::string, const IAlgTool*>& i) {
    debug() << "adding tool " << i.second->name() << " as dependency of " << obj.name() << endmsg;
    auto digest = generateConfig(*i.second);
    if (digest.invalid()) {
      throw std::runtime_error(
               "problem creating dependant configuration for " + i.second->name() );
    }
    return digest;
  });
  return depRefs;
}

ConfigTreeNode::digest_type
HltGenConfig::generateConfig(const INamedInterface &obj) const {
  // iterate over dependants, create config for them first (as we depend on
  // them)
  // ..
  // i.e. must go 'depth first', no choice here...
  auto depRefs = gatherDependencies(obj);

  // create and write the leaf object
  auto currentConfig = m_configSvc->currentConfiguration(obj);
  if (currentConfig.digest().invalid()) {
      throw std::runtime_error( "got bad configuration for "  +  obj.name() );
  }

  // check whether there is a modification request for this component...
  std::vector<std::string> overrule;

  // Find known environment variables in options and replace them with the environment variable again.
  using iter_t = ba::find_iterator<std::string::const_iterator>;
  for (const auto& valVar : m_envVarValues) {
    for (const auto& prop : currentConfig.properties()) {
      auto it = ba::make_find_iterator(prop.second, ba::first_finder(valVar.first, ba::is_iequal()));
      for(;it != iter_t(); ++it) {
        overrule.push_back(prop.first + ":" + ba::replace_range_copy(prop.second, *it, std::string{"$"} + valVar.second));
      }
    }
  }

  // Overrules from property
  auto it = m_overrule.find(obj.name());
  if (it != end(m_overrule)) {
     std::copy(begin(it->second), end(it->second), std::back_inserter(overrule));
  }

  // If some overrules were found, apply them
  if (!overrule.empty()) {
    if (!m_overruled.count(obj.name())) {
      warning() << " applying overrule to " << obj.name() << " : " << overrule
                << endmsg;
      m_overruled.emplace(obj.name());
    }
    currentConfig = currentConfig.copyAndModify(begin(overrule), end(overrule));
    if (!currentConfig.digest().valid()) {
      throw std::runtime_error( " overruling of " + obj.name() + " failed " );
    }
  }

  // Write the actual config
  auto propRef = m_accessSvc->writePropertyConfig(currentConfig);
  if (propRef.invalid()) {
    throw std::runtime_error( "problem writing PropertyConfig for " + obj.name() );
  }

  // create the tree node for this leaf object, and its dependencies, and write
  // it
  auto nodeRef = m_accessSvc->writeConfigTreeNode({ propRef, depRefs });
  if (nodeRef.invalid()) {
    throw std::runtime_error ( "problem writing ConfigTreeNode for " + obj.name() );
  }
  return nodeRef;
}

template <typename I, typename INS, typename R>
StatusCode HltGenConfig::getDependencies(I i, I end, INS inserter, R resolver) const
{
  for (; i != end; ++i) {
    info() << "Generating config for " << *i << endmsg;
    auto comp = resolver(*i);
    if (!comp) {
      error() << "Unable to get " << System::typeinfoName(typeid(comp)) << " "
              << *i << endmsg;
      return StatusCode::FAILURE;
    }
    auto digest = generateConfig(*comp);
    info() << " id for " << *i << " : " << digest << endmsg;
    if (!digest.valid()) {
      error() << "got invalid digest for " << *i << endmsg;
      return StatusCode::FAILURE;
    }
    *inserter++ = digest;
  }
  return StatusCode::SUCCESS;
}

StatusCode HltGenConfig::generateConfig() const {
  std::vector<ConfigTreeNode::digest_type> depRefs;

  auto sl = serviceLocator().get();
  StatusCode sc = getDependencies( begin(m_svcConfig), end(m_svcConfig),
                                   std::back_inserter(depRefs),
                                   [&](const std::string &name) {
        IService *svc { nullptr };
        return sl->getService(Gaudi::Utils::TypeNameString(name), svc, false).isSuccess() ? svc : nullptr;
  } );
  if (sc.isFailure()) return sc;
  sc = getDependencies( begin(m_topConfig), end(m_topConfig),
                        std::back_inserter(depRefs),
                        [&](const std::string &name) {
          IAlgorithm *alg { nullptr };
          return m_appMgr->getAlgorithm(name, alg).isSuccess() ? alg : nullptr;
  }) ;
  if (sc.isFailure()) return sc;

  ConfigTreeNode topConfig(ConfigTreeNode::digest_type::createInvalid(),
                           depRefs, m_label);
  auto topDigest = m_accessSvc->writeConfigTreeNode(topConfig);
  if (topDigest.invalid()) {
    error() << "problem writing config " << endmsg;
    return StatusCode::FAILURE;
  }
  info() << " top id for this config: " << topDigest << endmsg;

  // create top level alias for this id
  auto topAlias = m_accessSvc->writeConfigTreeNodeAlias(
          ConfigTreeNodeAlias::createTopLevel(m_release, m_hltType, topConfig));
  if (topAlias.invalid()) {
    error() << "problem writing alias " << endmsg;
    return StatusCode::FAILURE;
  }
  return sc;
}

void HltGenConfig::onCreate(const IAlgTool *tool) {
  assert(tool);
  auto name = tool->name();
  auto i = m_toolmap.emplace(name.substr(0, name.rfind('.')), tool);
  debug() << "adding " << i->first << " -> " << name << endmsg;
}
//=============================================================================
StatusCode HltGenConfig::execute() { return StatusCode::SUCCESS; }

//=============================================================================
StatusCode HltGenConfig::finalize() {
  static bool first(true);
  if (first) {
    first = false;
    try {
        auto sc = generateConfig();
        if (sc.isFailure()) return sc;
    }
    catch ( const std::runtime_error& err ) {
        error() << err.what() << endmsg;
        return StatusCode::FAILURE;
    }
  }
  return GaudiAlgorithm::finalize();
}
