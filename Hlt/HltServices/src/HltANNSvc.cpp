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
#include "ANNSvc.h"
#include <functional>
#include <string>

class HltANNSvc : public ANNSvc {
public:
  HltANNSvc( const std::string& name, ISvcLocator* svcLocator )
      : ANNSvc( name, svcLocator,
                {{"Hlt1SelectionID"},
                 {"Hlt2SelectionID"},
                 {"InfoID"},
                 {"RoutingBits"},
                 {"RelInfoLocations"},
                 {"PackedObjectLocations"}} ) {}
  using ANNSvc::handleUndefined;
  std::optional<minor_value_type> handleUndefined( const major_key_type& major,
                                                   const std::string&    minor ) const override;

private:
  Gaudi::Property<bool> m_allowUndefined{this, "allowUndefined", true,
                                         "do we allow undefined, on-demand generated, key/value pairs?"};
};

DECLARE_COMPONENT( HltANNSvc )

std::optional<IANNSvc::minor_value_type> HltANNSvc::handleUndefined( const major_key_type& major,
                                                                     const std::string&    minor ) const {
  if ( !m_allowUndefined ) return {};

  static const std::hash<std::string> hasher;
  debug() << "handleUndefined called for " << major << " : " << minor << " -->  " << hasher( minor ) << endmsg;
  return minor_value_type( minor, hasher( minor ) );
}
