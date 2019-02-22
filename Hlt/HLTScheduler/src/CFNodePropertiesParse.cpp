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
#include "CFNodePropertiesParse.h"
namespace std {
  ostream& operator<<( ostream& s, nodeType const& m ) {
    Gaudi::Utils::toStream( nodeTypeNames.at( m ), s );
    return s;
  }

  ostream& operator<<( ostream& s, NodeDefinition const& m ) {
    s << "[";
    Gaudi::Utils::toStream( m.name, s ) << ", ";
    Gaudi::Utils::toStream( m.type, s ) << ", ";
    if ( m.children.empty() ) {
      s << "[]";
    } else {
      s << "[" << m.children.front();
      std::for_each( next( begin( m.children ) ), end( m.children ), [&]( auto i ) { s << ", " << i; } );
      s << "]";
    }
    s << ", ";
    s << m.ordered;
    s << "]";
    return s;
  }
} // namespace std
