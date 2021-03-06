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
#include "ConfigStackAccessSvc.h"

// Factory implementation
DECLARE_COMPONENT( ConfigStackAccessSvc )

//=============================================================================
// Initialization
//=============================================================================
StatusCode ConfigStackAccessSvc::initialize() {
  debug() << "Initialize" << endmsg;
  StatusCode status = Service::initialize();
  if ( !status.isSuccess() ) return status;
  for ( const auto& i : s_svcs ) {
    IConfigAccessSvc* svc{nullptr};
    info() << " requesting service " << i << endmsg;
    status = service( i, svc );
    info() << " requested service " << i << " : " << status << endmsg;
    if ( !status.isSuccess() ) return status;
    assert( svc );
    m_svcs.push_back( svc );
  }
  if ( m_svcs.empty() ) {
    error() << "must specify at least one service in ConfigAccessSvcs..." << endmsg;
    return StatusCode::FAILURE;
  }
  return StatusCode::SUCCESS;
}
//=============================================================================
// Finalization
//=============================================================================
StatusCode ConfigStackAccessSvc::finalize() {
  for ( auto& i : m_svcs ) i.release();
  return Service::finalize();
}

std::optional<PropertyConfig> ConfigStackAccessSvc::readPropertyConfig( const PropertyConfig::digest_type& ref ) {
  std::optional<PropertyConfig> x;
  for ( auto& i : m_svcs ) {
    x = i.readPropertyConfig( ref );
    debug() << "read of " << ref << " from " << i.name() << ": " << ( !x ? "failed" : "OK" ) << endmsg;
    if ( x ) break;
  }
  return x;
}

PropertyConfig::digest_type ConfigStackAccessSvc::writePropertyConfig( const PropertyConfig& config ) {
  PropertyConfig::digest_type id = m_svcs.front().writePropertyConfig( config );
  debug() << "write of " << config.name() << " to " << m_svcs.front().name() << ": "
          << ( id.invalid() ? "failed" : "OK" ) << endmsg;
  return id;
}

std::optional<ConfigTreeNode> ConfigStackAccessSvc::readConfigTreeNode( const ConfigTreeNode::digest_type& ref ) {
  std::optional<ConfigTreeNode> x;
  for ( auto& i : m_svcs ) {
    x = i.readConfigTreeNode( ref );
    debug() << "read of " << ref << " from " << i.name() << ": " << ( !x ? "failed" : "OK" ) << endmsg;
    if ( x ) break;
  }
  return x;
}

std::vector<ConfigTreeNodeAlias>
ConfigStackAccessSvc::configTreeNodeAliases( const ConfigTreeNodeAlias::alias_type& alias ) {
  std::vector<ConfigTreeNodeAlias> vec;
  for ( auto& i : m_svcs ) {
    for ( const auto& j : i.configTreeNodeAliases( alias ) ) {
      auto k = std::find_if( std::begin( vec ), std::end( vec ),
                             [&]( const ConfigTreeNodeAlias& z ) { return z.alias() == j.alias(); } );
      if ( k == vec.end() ) { // this is a new one!
        vec.push_back( j );
      } else if ( *k != j ) { // check for consistency if duplication
        warning() << "inconsistent alias in stack (" << j << ") -- continuing to use earlier definition: " << *k
                  << endmsg;
      }
    }
  }
  return vec;
}

ConfigTreeNode::digest_type ConfigStackAccessSvc::writeConfigTreeNode( const ConfigTreeNode& config ) {
  ConfigTreeNode::digest_type id = m_svcs.front().writeConfigTreeNode( config );
  debug() << "write of " << config << " to " << m_svcs.front().name() << ": " << ( id.invalid() ? "failed" : "OK" )
          << endmsg;
  return id;
}

std::optional<ConfigTreeNode>
ConfigStackAccessSvc::readConfigTreeNodeAlias( const ConfigTreeNodeAlias::alias_type& alias ) {
  std::optional<ConfigTreeNode> x;
  for ( auto& i : m_svcs ) {
    x = i.readConfigTreeNodeAlias( alias );
    debug() << "read of " << alias << " from " << i.name() << ": " << ( !x ? "failed" : "OK" ) << endmsg;
  }
  return x;
}

ConfigTreeNodeAlias::alias_type ConfigStackAccessSvc::writeConfigTreeNodeAlias( const ConfigTreeNodeAlias& alias ) {
  ConfigTreeNodeAlias::alias_type id = m_svcs.begin()->writeConfigTreeNodeAlias( alias );
  debug() << "write of " << alias << " to " << m_svcs.front().name() << ": " << ( id.invalid() ? "failed" : "OK" )
          << endmsg;
  return id;
}
