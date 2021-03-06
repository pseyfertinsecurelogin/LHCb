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
#ifndef CONFIGCDBACCESSSVC_H
#define CONFIGCDBACCESSSVC_H 1

// Include files
#include <memory>
#include <string>
#include <string_view>

// from Gaudi
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Service.h"
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

namespace ConfigCDBAccessSvc_details {
  class CDB;

  enum class Mode { ReadOnly, ReadWrite };
  StatusCode         parse( Mode& result, const std::string& input );
  const std::string& toString( const Mode& );
  std::ostream& toStream( const Mode& mode, std::ostream& os ) { return os << std::quoted( toString( mode ), '\'' ); }
  std::ostream& operator<<( std::ostream& os, const Mode& mode ) { return toStream( mode, os ); }

} // namespace ConfigCDBAccessSvc_details

class ConfigCDBAccessSvc final : public extends<Service, IConfigAccessSvc> {
public:
  using extends::extends;

  StatusCode initialize() override; ///< Service initialization
  StatusCode finalize() override;   ///< Service initialization

  std::optional<PropertyConfig> readPropertyConfig( const PropertyConfig::digest_type& ref ) override;
  PropertyConfig::digest_type   writePropertyConfig( const PropertyConfig& config ) override;

  std::optional<ConfigTreeNode> readConfigTreeNode( const ConfigTreeNode::digest_type& ref ) override;
  ConfigTreeNode::digest_type   writeConfigTreeNode( const ConfigTreeNode& config ) override;

  std::optional<ConfigTreeNode>   readConfigTreeNodeAlias( const ConfigTreeNodeAlias::alias_type& ) override;
  ConfigTreeNodeAlias::alias_type writeConfigTreeNodeAlias( const ConfigTreeNodeAlias& ) override;

  std::vector<ConfigTreeNodeAlias> configTreeNodeAliases( const ConfigTreeNodeAlias::alias_type& ) override;

  StatusCode stop() override;

private:
  ConfigCDBAccessSvc_details::CDB* file() const;

  Gaudi::Property<std::string> m_name{this, "File"}; ///< filename of file from which to read configurations
  // todo: use custom type instead of std::string
  Gaudi::Property<ConfigCDBAccessSvc_details::Mode> m_mode{
      this, "Mode", ConfigCDBAccessSvc_details::Mode::ReadOnly}; ///< which flags to specify when opening the tar file
  Gaudi::Property<std::string>                             m_incident{this, "CloseIncident"}; ///< the incident to
  mutable std::mutex                                       m_file_mtx;
  mutable std::unique_ptr<ConfigCDBAccessSvc_details::CDB> m_file;
  std::unique_ptr<IIncidentListener>                       m_initListener;

  template <typename T>
  std::optional<T> read( std::string_view path ) const;
  template <typename T>
  bool write( std::string_view path, const T& object ) const;
};
#endif // CONFIGTARFILEACCESSSVC_H
