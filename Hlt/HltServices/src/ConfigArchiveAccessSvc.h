#ifndef CONFIGARCHIVEACCESSSVC_H 
#define CONFIGARCHIVEACCESSSVC_H 

// Include files
#include <string>
#include <memory>
#include "boost/optional.hpp"
// from Gaudi
#include "GaudiKernel/Service.h"
#include "GaudiKernel/MsgStream.h"

#include "Kernel/IConfigAccessSvc.h"

/** @class ConfigFileAccessSvc ConfigFileAccessSvc.h
 *  
 *  functionality:
 *        read/write configure information to files
 *
 *  @author Gerhard Raven
 *  @date   2009-06-04
 */


class IArchive;

class ConfigArchiveAccessSvc : public extends1<Service,IConfigAccessSvc> {
public:
  ConfigArchiveAccessSvc(const std::string& name, ISvcLocator* pSvcLocator);
  ~ConfigArchiveAccessSvc( ) override = default;     ///< Destructor

  StatusCode initialize() override;    ///< Service initialization

  boost::optional<PropertyConfig>  readPropertyConfig(const PropertyConfig::digest_type& ref) override;
  PropertyConfig::digest_type      writePropertyConfig(const PropertyConfig& config) override;

  boost::optional<ConfigTreeNode>  readConfigTreeNode(const ConfigTreeNode::digest_type& ref) override;
  ConfigTreeNode::digest_type      writeConfigTreeNode(const ConfigTreeNode& config) override;

  boost::optional<ConfigTreeNode>  readConfigTreeNodeAlias(const ConfigTreeNodeAlias::alias_type&) override;
  ConfigTreeNodeAlias::alias_type writeConfigTreeNodeAlias(const ConfigTreeNodeAlias&) override;

  std::vector<ConfigTreeNodeAlias> configTreeNodeAliases(const ConfigTreeNodeAlias::alias_type&) override;

private:
  virtual IArchive*  file() const = 0;

  template <typename T> boost::optional<T> read(const std::string& path) const;
  template <typename T> bool write(const std::string& path,const T& object) const;
  std::string propertyConfigPath( const PropertyConfig::digest_type& digest ) const;
  std::string configTreeNodePath( const ConfigTreeNode::digest_type& digest ) const;
  std::string configTreeNodeAliasPath( const ConfigTreeNodeAlias::alias_type& alias ) const;

};
#endif // CONFIGTARFILEACCESSSVC_H
