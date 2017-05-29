// Include files

// local
#include "UnpackProtoParticle.h"

#include <functional>
#include <vector>

#include "Event/PackedProtoParticle.h"
#include "Event/ProtoParticle.h"
#include "Gaudi/Details/PluginServiceDetails.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/SmartDataStorePtr.h"

class ISvcLocator;

//-----------------------------------------------------------------------------
// Implementation file for class : UnpackProtoParticle
//
// 2008-11-14 : Olivier Callot
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( UnpackProtoParticle )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  UnpackProtoParticle::UnpackProtoParticle( const std::string& name,
                                            ISvcLocator* pSvcLocator)
    : GaudiAlgorithm ( name , pSvcLocator )
{
  declareProperty( "InputName" , m_inputName  = LHCb::PackedProtoParticleLocation::Charged );
  declareProperty( "OutputName", m_outputName = LHCb::ProtoParticleLocation::Charged );
  declareProperty( "AlwaysCreateOutput", m_alwaysOutput = false );
  //setProperty( "OutputLevel", 1 );
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode UnpackProtoParticle::execute()
{
  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Execute" << endmsg;

  // If input does not exist, and we aren't making the output regardless, just return
  if ( !m_alwaysOutput && !exist<LHCb::PackedProtoParticles>(m_inputName) )
    return StatusCode::SUCCESS;

  const auto * dst =
    getOrCreate<LHCb::PackedProtoParticles,LHCb::PackedProtoParticles>( m_inputName );
  if ( msgLevel(MSG::DEBUG) )
    debug() << "Found " << dst->protos().size() << " PackedProtoParticles at '"
            << m_inputName << "'" << endmsg;

  auto * newProtoParticles = new LHCb::ProtoParticles();
  put( newProtoParticles, m_outputName );

  // unpack
  const LHCb::ProtoParticlePacker packer(this);
  packer.unpack( *dst, *newProtoParticles );

  if ( msgLevel(MSG::DEBUG) )
    debug() << "Created " << newProtoParticles->size() << " ProtoParticles at '"
            << m_outputName << "'" << endmsg;

  return StatusCode::SUCCESS;
}

//=============================================================================
