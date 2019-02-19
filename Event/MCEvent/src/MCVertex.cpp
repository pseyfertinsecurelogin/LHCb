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

// Event
#include "Event/MCVertex.h"

//-----------------------------------------------------------------------------
// Implementation file for class : MCVertex
//
// 2012-11-20 : Chris Jones
//-----------------------------------------------------------------------------

std::ostream& LHCb::MCVertex::fillStream( std::ostream& s ) const {
  s << "{ Position = " << position() << " Time = " << time() << " Type = " << type() << " Mother = " << mother()
    << " Products : #=" << products().size() << " [";
  for ( const auto& p : products() ) { s << " " << p.target(); }
  s << " ] }";
  return s;
}
