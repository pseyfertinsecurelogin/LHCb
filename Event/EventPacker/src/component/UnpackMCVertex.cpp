
// STL
#include <algorithm>

#include "Event/MCVertex.h"
#include "Event/StandardPacker.h"
#include "Event/PackedMCVertex.h"
// local
#include "UnpackMCVertex.h"

//-----------------------------------------------------------------------------
// Implementation file for class : UnpackMCVertex
//
// 2005-03-18 : Olivier Callot
//-----------------------------------------------------------------------------

DECLARE_ALGORITHM_FACTORY( UnpackMCVertex )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
UnpackMCVertex::UnpackMCVertex( const std::string& name,
                                ISvcLocator* pSvcLocator)
  : GaudiAlgorithm ( name , pSvcLocator )
{
  declareProperty( "InputName" , m_inputName  = LHCb::PackedMCVertexLocation::Default );
  declareProperty( "OutputName", m_outputName = LHCb::MCVertexLocation::Default );
  declareProperty( "AlwaysCreateOutput",         m_alwaysOutput = false     );
  //setProperty( "OutputLevel", 1 );
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode UnpackMCVertex::execute()
{

  // CRJ : If packed data does not exist just return (by default). Needed for packing of 
  //     : spillover which is not neccessarily available for each event
  if ( !m_alwaysOutput && !exist<LHCb::PackedMCVertices>(m_inputName) ) return StatusCode::SUCCESS;

  const auto* dst = 
    getOrCreate<LHCb::PackedMCVertices,LHCb::PackedMCVertices>( m_inputName );

  if ( msgLevel(MSG::DEBUG) )
    debug() << "Size of PackedMCVertices = " << dst->mcVerts().size() << endmsg;

  auto* newMCVertices = new LHCb::MCVertices();
  put( newMCVertices, m_outputName );

  StandardPacker pack(this);

  // Packing version
  const char pVer = dst->packingVersion();

  newMCVertices->reserve( dst->mcVerts().size() );
  for ( const auto & src : dst->mcVerts() )
  {

    auto* vert = new LHCb::MCVertex( );
    newMCVertices->insert( vert, src.key );
    vert->setPosition( Gaudi::XYZPoint( pack.position( src.x ), 
                                        pack.position( src.y ), 
                                        pack.position( src.z ) ) );
    vert->setTime( src.tof );
    vert->setType( (LHCb::MCVertex::MCVertexType) src.type );

    int hintID(0), key(0);
    if ( -1 != src.mother )
    {
      if ( ( 0==pVer && pack.hintAndKey32( src.mother, dst, newMCVertices, hintID, key ) ) ||
           ( 0!=pVer && pack.hintAndKey64( src.mother, dst, newMCVertices, hintID, key ) ) )
      {
        SmartRef<LHCb::MCParticle> ref( newMCVertices, hintID, key );
        vert->setMother( ref );
      }
      else { Error( "Corrupt MCVertex Mother MCParticle SmartRef detected" ).ignore(); }
    }

    // List of processed refs, to check for duplicates
    std::vector<long long> processedRefs;
    processedRefs.reserve( src.products.size() );

    // loop over refs and process
    for ( const auto& I : src.products )
    {
      // Check for duplicates ...
      if ( std::none_of( processedRefs.begin(), processedRefs.end(),
                         [&I]( const auto& J ) { return I == J; } ) )
      {
        // save this packed ref to the list of those already processed.
        processedRefs.push_back(I);
        // Unpack the ref and save to the vertex
        hintID = key = 0;
        if ( ( 0==pVer && pack.hintAndKey32( I, dst, newMCVertices, hintID, key ) ) ||
             ( 0!=pVer && pack.hintAndKey64( I, dst, newMCVertices, hintID, key ) ) )
        {
          // Construct the smart ref
          SmartRef<LHCb::MCParticle> ref( newMCVertices, hintID, key );
          // save
          vert->addToProducts( ref );
        }
        else { Error( "Corrupt MCVertex Daughter MCParticle SmartRef detected" ).ignore(); }
      }
      else
      {
        Warning( "Found duplicate in packed MCVertex products", 
                 StatusCode::SUCCESS ).ignore();
      }
    }
  }
  
  return StatusCode::SUCCESS;
}

//=============================================================================
