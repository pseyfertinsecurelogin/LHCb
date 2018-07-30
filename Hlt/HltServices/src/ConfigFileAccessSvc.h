#ifndef CONFIGFILEACCESSSVC_H
#define CONFIGFILEACCESSSVC_H 1

// Include files
#include <string>
#include <memory>
#include "boost/filesystem/path.hpp"
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
 *  @date   2007-12-14
 */
class ConfigFileAccessSvc : public extends<Service, IConfigAccessSvc> {
public:
  using extends::extends;

  StatusCode initialize() override;    ///< Service initialization

  std::optional<PropertyConfig>  readPropertyConfig(const PropertyConfig::digest_type& ref) override;
  PropertyConfig::digest_type      writePropertyConfig(const PropertyConfig& config) override;

  std::optional<ConfigTreeNode>  readConfigTreeNode(const ConfigTreeNode::digest_type& ref) override;
  ConfigTreeNode::digest_type      writeConfigTreeNode(const ConfigTreeNode& config) override;

  std::optional<ConfigTreeNode>  readConfigTreeNodeAlias(const ConfigTreeNodeAlias::alias_type&) override;
  ConfigTreeNodeAlias::alias_type writeConfigTreeNodeAlias(const ConfigTreeNodeAlias&) override;

  std::vector<ConfigTreeNodeAlias> configTreeNodeAliases(const ConfigTreeNodeAlias::alias_type&) override;
private:

  boost::filesystem::path dir() const;

  mutable Gaudi::Property<std::string> m_dir { this, "Directory" };   ///< where to read/write configurations from/to?

  // TODO: replace fs::path with a concrete config...
  bool isCompatible(const ConfigTreeNodeAlias& alias, const boost::filesystem::path& dirName ) const;

  boost::filesystem::path propertyConfigPath( const PropertyConfig::digest_type& digest ) const;
  boost::filesystem::path configTreeNodePath( const ConfigTreeNode::digest_type& digest ) const;
  boost::filesystem::path configTreeNodeAliasPath( const ConfigTreeNodeAlias::alias_type& alias ) const;
  bool create_directories( boost::filesystem::path dir ) const;

};
#endif // CONFIGFILEACCESSSVC_H
