// Include files 

// local
#include "CompareProtoParticle.h"

#include <functional>

#include "Event/PackedProtoParticle.h"
#include "Event/ProtoParticle.h"
#include "Gaudi/Details/PluginServiceDetails.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"

class ISvcLocator;

//-----------------------------------------------------------------------------
// Implementation file for class : CompareProtoParticle
//
// 2008-11-14 : Olivier Callot
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( CompareProtoParticle )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
CompareProtoParticle::CompareProtoParticle( const std::string& name,
                                            ISvcLocator* pSvcLocator)
  : GaudiAlgorithm ( name , pSvcLocator )
{
  declareProperty( "InputName", m_inputName = LHCb::ProtoParticleLocation::Charged );
  declareProperty( "TestName" , m_testName  = LHCb::ProtoParticleLocation::Charged+"Test" );
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode CompareProtoParticle::execute() 
{
  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Execute" << endmsg;
 
  LHCb::ProtoParticles* old  = get<LHCb::ProtoParticles>( m_inputName  );
  LHCb::ProtoParticles* test = get<LHCb::ProtoParticles>( m_testName  );

  // check and return
  const LHCb::ProtoParticlePacker packer(this);
  return packer.check( *old, *test );
}

//=============================================================================
