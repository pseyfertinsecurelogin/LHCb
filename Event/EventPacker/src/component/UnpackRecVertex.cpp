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
// Include files

// from Gaudi
#include "Event/StandardPacker.h"
// local
#include "UnpackRecVertex.h"

//-----------------------------------------------------------------------------
// Implementation file for class : UnpackRecVertex
//
// 2008-11-17 : Olivier Callot
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( UnpackRecVertex )

StatusCode UnpackRecVertex::execute() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;

  // If input does not exist, and we aren't making the output regardless, just return
  if ( !m_alwaysOutput.value() && !m_packedVertices.exist() ) return StatusCode::SUCCESS;

  const auto* dst = m_packedVertices.getOrCreate();

  if ( (int)dst->version() < 2 ) {
    // We can't unpack the weights vector because it lives in a seperate
    // location for < v2 packed vertices; this means fully supporting all
    // vertices would require an 'optional' data dependency which the Gaudi
    // HiveDataBroker doesn't support
    Warning( "PackedRecVertex version < 2 not fully supported; associated weights vector will not be unpacked" )
        .ignore();
  }

  if ( msgLevel( MSG::DEBUG ) ) debug() << "Size of PackedRecVertices = " << dst->vertices().size() << endmsg;

  auto* newRecVertices = m_vertices.put( std::make_unique<LHCb::RecVertices>() );

  const LHCb::RecVertexPacker rvPacker( this );

  for ( const auto& src : dst->vertices() ) {
    auto* vert = new LHCb::RecVertex();
    newRecVertices->insert( vert, src.key );
    rvPacker.unpack( src, *vert, *dst, *newRecVertices );
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
