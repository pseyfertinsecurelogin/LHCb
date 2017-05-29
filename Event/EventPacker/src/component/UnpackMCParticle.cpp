
// STL
#include <algorithm>
#include <cmath>
#include <functional>
#include <vector>

// Event model
#include "Event/MCParticle.h"
#include "Event/MCVertex.h"
#include "Event/PackedMCParticle.h"
#include "Event/StandardPacker.h"
#include "Gaudi/Details/PluginServiceDetails.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/SmartDataStorePtr.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/Vector4DTypes.h"
#include "Kernel/ParticleID.h"
// local
#include "UnpackMCParticle.h"

class ISvcLocator;

//-----------------------------------------------------------------------------
// Implementation file for class : UnpackMCParticle
//
// 2005-03-18 : Olivier Callot
//-----------------------------------------------------------------------------

DECLARE_ALGORITHM_FACTORY( UnpackMCParticle )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  UnpackMCParticle::UnpackMCParticle( const std::string& name,
                                      ISvcLocator* pSvcLocator)
    : GaudiAlgorithm ( name , pSvcLocator )
{
  declareProperty( "InputName" , m_inputName  = LHCb::PackedMCParticleLocation::Default );
  declareProperty( "OutputName", m_outputName = LHCb::MCParticleLocation::Default );
  declareProperty( "AlwaysCreateOutput",         m_alwaysOutput = false     );
  //setProperty( "OutputLevel", 1 );
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode UnpackMCParticle::execute() 
{

  // CRJ : If packed data does not exist just return. Needed for packing of
  //     : spillover which is not neccessarily available for each event
  if ( !m_alwaysOutput && !exist<LHCb::PackedMCParticles>(m_inputName) )
    return StatusCode::SUCCESS;

  LHCb::PackedMCParticles* dst =
    getOrCreate<LHCb::PackedMCParticles,LHCb::PackedMCParticles>( m_inputName );

  if ( msgLevel(MSG::DEBUG) )
    debug() << "Size of PackedMCParticles = " << dst->mcParts().size() << endmsg;

  LHCb::MCParticles* newMCParticles = new LHCb::MCParticles();
  put( newMCParticles, m_outputName );

  StandardPacker pack(this);

  // Packing version
  const char pVer = dst->packingVersion();

  // random generator for private tests of flags. 
  //static std::default_random_engine gen;
  //static std::uniform_real_distribution<float> uniform(0,1);

  newMCParticles->reserve( dst->mcParts().size() );
  for ( const auto & src : dst->mcParts() )
  {

    LHCb::MCParticle * part = new LHCb::MCParticle( );
    newMCParticles->insert( part, src.key );

    const auto px = pack.energy( src.px );
    const auto py = pack.energy( src.py );
    const auto pz = pack.energy( src.pz );
    const auto mass = src.mass;
    const auto E = std::sqrt( (px*px) + (py*py) + (pz*pz) + (mass*mass) );
    part->setMomentum( Gaudi::LorentzVector( px, py, pz , E ) );

    part->setParticleID( LHCb::ParticleID(src.PID) );

    part->setFlags( src.flags );
    // for testing, randomly set 'fromSignal' to true 5% of the time.
    //part->setFromSignal( uniform(gen) > 0.95 );

    int hintID(0), key(0);
    if ( ( 0==pVer && pack.hintAndKey32( src.originVertex, dst, newMCParticles, hintID, key ) ) ||
         ( 0!=pVer && pack.hintAndKey64( src.originVertex, dst, newMCParticles, hintID, key ) ) )
    {
      SmartRef<LHCb::MCVertex> ref( newMCParticles, hintID, key );
      part->setOriginVertex( ref );
    }
    else { Error( "Corrupt MCParticle Origin MCVertex SmartRef detected" ).ignore(); }

    // List of processed refs, to check for duplicates
    std::vector<long long> processedRefs;
    processedRefs.reserve( src.endVertices.size() );

    // loop over refs and process
    for ( const auto& I : src.endVertices )
    {
      // Check for duplicates ...
      if ( std::none_of( processedRefs.begin(), processedRefs.end(),
                         [&I]( const auto& J ) { return I == J; } ) )
      {
        // save this packed ref to the list of those already processed.
        processedRefs.push_back(I);
        // Unpack the ref and save to the vertex
        hintID = key = 0;
        if ( ( 0==pVer && pack.hintAndKey32( I, dst, newMCParticles, hintID, key ) ) ||
             ( 0!=pVer && pack.hintAndKey64( I, dst, newMCParticles, hintID, key ) ) )
        {
          // Construct the smart ref
          SmartRef<LHCb::MCVertex> ref( newMCParticles, hintID, key );
          // save
          part->addToEndVertices( ref );
        }
        else { Error( "Corrupt MCParticle End MCVertex SmartRef detected" ).ignore(); }
      }
      else
      {
        Warning( "Found duplicate in packed MCParticle end vertices", 
                 StatusCode::SUCCESS ).ignore(); }
    }
  }
  
  return StatusCode::SUCCESS;
}

//=============================================================================
