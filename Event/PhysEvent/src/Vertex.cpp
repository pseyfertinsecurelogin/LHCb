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

#include "Event/Vertex.h"

// Gaudi
#include "GaudiKernel/IRegistry.h"

//-----------------------------------------------------------------------------
// Implementation file for class : Vertex
//
// 2012-04-26 : Chris Jones
//-----------------------------------------------------------------------------

std::ostream& LHCb::Vertex::fillStream( std::ostream& s ) const {
  VertexBase::fillStream( s );
  s << "{ "
    << "Technique         :	" << m_technique << std::endl
    << "OutgoingParticles : " << m_outgoingParticles;
  const std::string testLocation = ( parent() && parent()->registry() ? parent()->registry()->identifier() : "" );
  if ( !testLocation.empty() ) { s << std::endl << "TES=" << testLocation; }
  return s << " }";
}
