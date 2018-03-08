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

   enum class Mode { ReadOnly, ReadWrite };
   StatusCode parse(Mode& result, const std::string& input);
   const std::string& toString(const Mode&);
   std::ostream& toStream(const Mode& mode, std::ostream& os) { return os << std::quoted(toString(mode),'\''); }
   std::ostream& operator<<(std::ostream& os, const Mode& mode) { return toStream(mode,os); }

}

class ConfigCDBAccessSvc : public extends<Service,IConfigAccessSvc> {
public:
  using extends::extends;

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

  Gaudi::Property<std::string> m_name{ this, "File" } ;   ///< filename of file from which to read configurations
  // todo: use custom type instead of std::string
  Gaudi::Property<ConfigCDBAccessSvc_details::Mode>   m_mode{ this, "Mode", ConfigCDBAccessSvc_details::Mode::ReadOnly };   ///< which flags to specify when opening the tar file
  Gaudi::Property<std::string>         m_incident { this, "CloseIncident" };   ///< the incident to
  mutable std::mutex                                       m_file_mtx;
  mutable std::unique_ptr<ConfigCDBAccessSvc_details::CDB> m_file;
  std::unique_ptr<IIncidentListener> m_initListener;

  template <typename T> boost::optional<T> read(boost::string_ref path) const;
  template <typename T> bool write(boost::string_ref path,const T& object) const;

};
#endif // CONFIGTARFILEACCESSSVC_H
