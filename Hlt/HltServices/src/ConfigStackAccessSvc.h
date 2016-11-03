#ifndef CONFIGFILEACCESSSVC_H 
#define CONFIGFILEACCESSSVC_H 1

// Include files
#include <string>
#include <memory>
#include "boost/optional.hpp"
#include "boost/ptr_container/ptr_vector.hpp"
// from Gaudi
#include "Kernel/IConfigAccessSvc.h"
#include "GaudiKernel/Service.h"
#include "GaudiKernel/MsgStream.h"
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
class ConfigStackAccessSvc : public extends1<Service, IConfigAccessSvc> {
public:
  ConfigStackAccessSvc(const std::string& name, ISvcLocator* pSvcLocator);

  StatusCode initialize() override;    ///< Service initialization
  StatusCode finalize() override;    ///< Service finalization

  boost::optional<PropertyConfig> readPropertyConfig(const PropertyConfig::digest_type& ref) override;
  PropertyConfig::digest_type    writePropertyConfig(const PropertyConfig& config) override;

  boost::optional<ConfigTreeNode> readConfigTreeNode(const ConfigTreeNode::digest_type& ref) override;
  ConfigTreeNode::digest_type    writeConfigTreeNode(const ConfigTreeNode& edge) override;

  boost::optional<ConfigTreeNode>  readConfigTreeNodeAlias(const ConfigTreeNodeAlias::alias_type&) override;
  ConfigTreeNodeAlias::alias_type writeConfigTreeNodeAlias(const ConfigTreeNodeAlias&) override;

  std::vector<ConfigTreeNodeAlias> configTreeNodeAliases(const ConfigTreeNodeAlias::alias_type& alias) override;

private:
  std::vector<std::string>             s_svcs;
  boost::ptr_vector<IConfigAccessSvc>  m_svcs;
};
#endif // CONFIGFILEACCESSSVC_H
