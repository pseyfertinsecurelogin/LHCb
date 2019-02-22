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
#include "GaudiKernel/Property.h"
#include "GaudiKernel/Service.h"
#include "Kernel/ILinePersistenceSvc.h"
#include "Kernel/IPropertyConfigSvc.h"
#include "Kernel/TCK.h"
#include "LinePersistenceSvcCommon.h"
#include <vector>

class TCKLinePersistenceSvc : public extends<Service, ILinePersistenceSvc>, private LinePersistenceSvcCommon {
public:
  using extends::extends;

  StatusCode initialize() override;
  StatusCode finalize() override;

  Locations locationsToPersist( const LHCb::HltDecReports& hdr, const LineDecNames& lines ) const override;
  Locations turboPPLocationsToPersist( const LHCb::HltDecReports& hdr, const LineDecNames& lines ) const override;
  RawBanks  rawBanksToPersist( const LHCb::HltDecReports& hdr, const LineDecNames& lines ) const override;

private:
  using LineRequests = std::tuple<NameListPerLine, LineDecNames, RawBanksPerLine>;

  const PropertyConfig& hltSvcPropertyConfig( const TCK& tck ) const;
  template <typename T>
  T                   parseProperty( const PropertyConfig& config, const char* name ) const;
  const LineRequests& lineRequests( unsigned int tck ) const;

  Gaudi::Property<std::string> m_propertyConfigSvcName{this, "IPropertyConfigSvcInstance", "PropertyConfigSvc"};
  Gaudi::Property<std::string> m_instanceName{this, "InstanceName", "HltLinePersistenceSvc"};
  /// Property giving the mapping between packed containers and containers
  Gaudi::Property<std::map<std::string, std::string>> m_containerMap{this, "ContainerMap"};
  mutable SmartIF<IPropertyConfigSvc>                 m_propertyConfigSvc;
  mutable std::map<TCK, LineRequests>                 m_cache; // TODO: flush cache if m_instanceName changes
};

DECLARE_COMPONENT( TCKLinePersistenceSvc )

StatusCode TCKLinePersistenceSvc::initialize() {
  StatusCode sc = Service::initialize();
  if ( sc.isFailure() ) {
    fatal() << "Service::initialize() failed!" << endmsg;
    return sc;
  }

  m_propertyConfigSvc = service( m_propertyConfigSvcName );
  if ( !m_propertyConfigSvc ) {
    fatal() << "TCKLinePersistenceSvc failed to get the IConfigAccessSvc." << endmsg;
    return StatusCode::FAILURE;
  }

  return sc;
}

StatusCode TCKLinePersistenceSvc::finalize() {
  m_propertyConfigSvc.reset();
  return Service::finalize();
}

ILinePersistenceSvc::Locations
TCKLinePersistenceSvc::locationsToPersist( const LHCb::HltDecReports&               hdr,
                                           const ILinePersistenceSvc::LineDecNames& lines ) const {
  auto requests = lineRequests( hdr.configuredTCK() );
  return locationsToPersistImpl( hdr, lines, std::get<0>( requests ) );
}

ILinePersistenceSvc::Locations
TCKLinePersistenceSvc::turboPPLocationsToPersist( const LHCb::HltDecReports&               hdr,
                                                  const ILinePersistenceSvc::LineDecNames& lines ) const {
  auto requests = lineRequests( hdr.configuredTCK() );
  return turboPPLocationsToPersistImpl( hdr, lines, std::get<1>( requests ), std::get<0>( requests ) );
}

ILinePersistenceSvc::RawBanks
TCKLinePersistenceSvc::rawBanksToPersist( const LHCb::HltDecReports&               hdr,
                                          const ILinePersistenceSvc::LineDecNames& lines ) const {
  auto requests = lineRequests( hdr.configuredTCK() );
  return rawBanksToPersistImpl( hdr, lines, std::get<2>( requests ) );
}

const PropertyConfig& TCKLinePersistenceSvc::hltSvcPropertyConfig( const TCK& tck ) const {
  // grab properties of child from config database...
  const ConfigTreeNode* tree =
      m_propertyConfigSvc->resolveConfigTreeNode( ConfigTreeNodeAlias::alias_type{std::string( "TCK/" ) + tck.str()} );
  if ( !tree ) {
    // If we could not resolve the (non-zero) TCK we have a problem
    error() << "Requested TCK " << tck << " could not resolved." << endmsg;
    throw GaudiException( "FATAL error", this->name(), StatusCode::FAILURE );
  }
  PropertyConfig::digest_type child = m_propertyConfigSvc->findInTree( tree->digest(), m_instanceName );
  if ( child.invalid() ) {
    error() << "Error finding configuration of " << m_instanceName << " for TCK " << tck << endmsg;
    throw GaudiException( "FATAL error", this->name(), StatusCode::FAILURE );
  }
  const PropertyConfig* config = m_propertyConfigSvc->resolvePropertyConfig( child );
  if ( !config ) {
    error() << "Error reading configuration of " << m_instanceName << " for TCK " << tck << endmsg;
    throw GaudiException( "FATAL error", this->name(), StatusCode::FAILURE );
  }

  return *config;
}

template <typename T>
T TCKLinePersistenceSvc::parseProperty( const PropertyConfig& config, const char* name ) const {
  auto prop = std::find_if( std::begin( config.properties() ), std::end( config.properties() ),
                            [&]( const std::pair<std::string, std::string>& p ) { return p.first == name; } );
  if ( prop == std::end( config.properties() ) ) {
    error() << "Error finding requested property " << name << " in  configuration of " << m_instanceName << endmsg;
    throw GaudiException( "FATAL error", this->name(), StatusCode::FAILURE );
  }

  T    result;
  auto status = Gaudi::Parsers::parse( result, prop->second );
  if ( status.isFailure() ) {
    error() << "Error interpreting requested property " << name << " in  configuration of " << m_instanceName << endmsg;
    throw GaudiException( "FATAL error", this->name(), StatusCode::FAILURE );
  }
  return result;
}

const TCKLinePersistenceSvc::LineRequests& TCKLinePersistenceSvc::lineRequests( unsigned int tck ) const {
  TCK _tck( tck );
  _tck.normalize();
  auto entry = m_cache.find( _tck );
  if ( entry == std::end( m_cache ) ) {
    // Fetch and parse/convert the properties from the TCK
    auto config          = hltSvcPropertyConfig( _tck );
    auto locationsMap    = parseProperty<NameListPerLine>( config, "Locations" );
    auto turboPPLines    = parseProperty<std::vector<std::string>>( config, "TurboPPLines" );
    auto turboPPLinesMap = ILinePersistenceSvc::LineDecNames{turboPPLines.begin(), turboPPLines.end()};
    auto rawBankTypesMap = parseProperty<NameListPerLine>( config, "RawBankTypes" );
    auto rawBanksMap     = typeNamesToBitset( rawBankTypesMap );

    // Apply the ContainerMap
    for ( auto& pair : locationsMap ) {
      for ( auto& location : pair.second ) {
        auto mappedLocation = m_containerMap.find( location );
        if ( mappedLocation != std::end( m_containerMap ) ) { location = mappedLocation->second; }
      }
    }

    auto status = m_cache.emplace( _tck, LineRequests{locationsMap, turboPPLinesMap, rawBanksMap} );
    if ( !status.second ) {
      error() << "Error updating cache for TCK " << _tck << " Returning an empty map... " << endmsg;
      throw GaudiException( "FATAL error", this->name(), StatusCode::FAILURE );
    }
    entry = status.first;
  }
  return entry->second;
}
