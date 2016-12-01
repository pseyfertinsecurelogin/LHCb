#ifndef CONFIGCDBACCESSSVC_H
#define CONFIGCDBACCESSSVC_H 1

// Include files
#include <string>
#include <memory>

// boost
#include "boost/optional.hpp"

// from Gaudi
#include "GaudiKernel/Service.h"
#include "GaudiKernel/MsgStream.h"
#include <GaudiKernel/IIncidentListener.h>

#include "Kernel/IConfigAccessSvc.h"

/** @class ConfigFileAccessSvc ConfigFileAccessSvc.h
 *
 *  functionality:
 *        read/write configure information to files
 *
 *  @author Gerhard Raven
 *  @date   2009-06-04
 */

namespace ConfigCDBAccessSvc_details  {
   class CDB;
}

class ConfigCDBAccessSvc : public extends<Service,IConfigAccessSvc> {
public:
  ConfigCDBAccessSvc(const std::string& name, ISvcLocator* pSvcLocator);

  StatusCode initialize() override;    ///< Service initialization
  StatusCode finalize() override;      ///< Service initialization

  boost::optional<PropertyConfig>  readPropertyConfig(const PropertyConfig::digest_type& ref) override;
  PropertyConfig::digest_type      writePropertyConfig(const PropertyConfig& config) override;

  boost::optional<ConfigTreeNode>  readConfigTreeNode(const ConfigTreeNode::digest_type& ref) override;
  ConfigTreeNode::digest_type      writeConfigTreeNode(const ConfigTreeNode& config) override;

  boost::optional<ConfigTreeNode>  readConfigTreeNodeAlias(const ConfigTreeNodeAlias::alias_type&) override;
  ConfigTreeNodeAlias::alias_type writeConfigTreeNodeAlias(const ConfigTreeNodeAlias&) override;

  std::vector<ConfigTreeNodeAlias> configTreeNodeAliases(const ConfigTreeNodeAlias::alias_type&) override;

  StatusCode stop() override;

private:

  ConfigCDBAccessSvc_details::CDB*  file() const;

  mutable std::string                m_name;   ///< filename of tar file from which to read configurations
  std::string                        m_mode;   ///< which flags to specify when opening the tar file
  std::string                        m_incident;   ///< the incident to
  mutable std::mutex                                       m_file_mtx;
  mutable std::unique_ptr<ConfigCDBAccessSvc_details::CDB> m_file;
  std::unique_ptr<IIncidentListener> m_initListener;

  template <typename T> boost::optional<T> read(const std::string& path) const;
  template <typename T> bool write(const std::string& path,const T& object) const;
  std::string propertyConfigPath( const PropertyConfig::digest_type& digest ) const;
  std::string configTreeNodePath( const ConfigTreeNode::digest_type& digest ) const;
  std::string configTreeNodeAliasPath( const ConfigTreeNodeAlias::alias_type& alias ) const;

};
#endif // CONFIGTARFILEACCESSSVC_H
