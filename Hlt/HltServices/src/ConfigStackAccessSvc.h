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
#ifndef CONFIGFILEACCESSSVC_H
#define CONFIGFILEACCESSSVC_H 1

// Include files
#include "boost/ptr_container/ptr_vector.hpp"
#include <memory>
#include <string>
// from Gaudi
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Service.h"
#include "Kernel/IConfigAccessSvc.h"
#include "Kernel/PropertyConfig.h"

/** @class ConfigStackAccessSvc ConfigStackAccessSvc.h
 *
 *  functionality:
 *        read/write configure information from a
 *        'stack' of IConfigAccessSvc implementations
 *        writes always go to the 'top' layer of the stack,
 *        reads first go to the 'top' layer, and if it
 *        isn't found, go to the next layer, and if it
 *        isn't found there, it goes to the next layer, and so on...
 *
 *  @author Gerhard Raven
 *  @date   2008-01-04
 */
class ConfigStackAccessSvc : public extends<Service, IConfigAccessSvc> {
public:
  using extends::extends;

  StatusCode initialize() override; ///< Service initialization
  StatusCode finalize() override;   ///< Service finalization

  std::optional<PropertyConfig> readPropertyConfig( const PropertyConfig::digest_type& ref ) override;
  PropertyConfig::digest_type   writePropertyConfig( const PropertyConfig& config ) override;

  std::optional<ConfigTreeNode> readConfigTreeNode( const ConfigTreeNode::digest_type& ref ) override;
  ConfigTreeNode::digest_type   writeConfigTreeNode( const ConfigTreeNode& edge ) override;

  std::optional<ConfigTreeNode>   readConfigTreeNodeAlias( const ConfigTreeNodeAlias::alias_type& ) override;
  ConfigTreeNodeAlias::alias_type writeConfigTreeNodeAlias( const ConfigTreeNodeAlias& ) override;

  std::vector<ConfigTreeNodeAlias> configTreeNodeAliases( const ConfigTreeNodeAlias::alias_type& alias ) override;

private:
  Gaudi::Property<std::vector<std::string>> s_svcs{this, "ConfigAccessSvcs", {{"ConfigCDBAccessSvc"}}};
  boost::ptr_vector<IConfigAccessSvc>       m_svcs;
};
#endif // CONFIGFILEACCESSSVC_H
