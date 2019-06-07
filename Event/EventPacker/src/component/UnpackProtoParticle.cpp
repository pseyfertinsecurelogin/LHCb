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

// local
#include "UnpackProtoParticle.h"

//-----------------------------------------------------------------------------
// Implementation file for class : UnpackProtoParticle
//
// 2008-11-14 : Olivier Callot
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( UnpackProtoParticle )

StatusCode UnpackProtoParticle::execute() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;

  // If input does not exist, and we aren't making the output regardless, just return
  if ( !m_alwaysOutput.value() && !m_packedProtos.exist() ) return StatusCode::SUCCESS;

  const auto* dst = m_packedProtos.getOrCreate();
  if ( msgLevel( MSG::DEBUG ) )
    debug() << "Found " << dst->protos().size() << " PackedProtoParticles at " << m_packedProtos.fullKey() << endmsg;

  auto* newProtoParticles = m_protos.put( std::make_unique<LHCb::ProtoParticles>() );

  // unpack
  const LHCb::ProtoParticlePacker packer( this );
  packer.unpack( *dst, *newProtoParticles );

  if ( msgLevel( MSG::DEBUG ) )
    debug() << "Created " << newProtoParticles->size() << " ProtoParticles at " << m_protos.fullKey() << endmsg;

  return StatusCode::SUCCESS;
}

//=============================================================================
