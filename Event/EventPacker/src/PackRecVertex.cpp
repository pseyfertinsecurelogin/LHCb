// $Id: PackRecVertex.cpp,v 1.6 2009-11-07 12:20:39 jonrob Exp $
// Include files

// from Gaudi
#include "GaudiKernel/AlgFactory.h"
#include "Event/RecVertex.h"
#include "Event/PackedRecVertex.h"
#include "Kernel/StandardPacker.h"
// local
#include "PackRecVertex.h"

//-----------------------------------------------------------------------------
// Implementation file for class : PackRecVertex
//
// 2008-11-14 : Olivier Callot
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( PackRecVertex )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  PackRecVertex::PackRecVertex( const std::string& name,
                                ISvcLocator* pSvcLocator)
    : GaudiAlgorithm ( name , pSvcLocator )
{
  declareProperty( "InputName" , m_inputName  = LHCb::RecVertexLocation::Primary );
  declareProperty( "OutputName", m_outputName = LHCb::PackedRecVertexLocation::Primary );
  declareProperty( "AlwaysCreateOutput",         m_alwaysOutput = false     );
  declareProperty( "DeleteInput",                m_deleteInput  = false     );
  declareProperty( "Version",                    m_version = 2              );
}
//=============================================================================
// Destructor
//=============================================================================
PackRecVertex::~PackRecVertex() {}

//=============================================================================
// Main execution
//=============================================================================
StatusCode PackRecVertex::execute() 
{
  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Execute" << endmsg;

  // If input does not exist, and we aren't making the output regardless, just return
  if ( !m_alwaysOutput && !exist<LHCb::RecVertices>(m_inputName) ) 
    return StatusCode::SUCCESS;

  LHCb::RecVertices* verts = 
    getOrCreate<LHCb::RecVertices,LHCb::RecVertices>( m_inputName );

  LHCb::PackedRecVertices* out = new LHCb::PackedRecVertices();
  out->vertices().reserve(verts->size());
  put( out, m_outputName );
  out->setVersion( (unsigned char)m_version ); 

  static const LHCb::RecVertexPacker rvPacker;

  for ( LHCb::RecVertices::const_iterator itV = verts->begin(); 
        verts->end() != itV; ++itV ) 
  {
    const LHCb::RecVertex* vert = *itV;

    // Make and save a new packed object
    out->vertices().push_back( LHCb::PackedRecVertex() );
    LHCb::PackedRecVertex & pVert = out->vertices().back();

    // Key
    pVert.key = vert->key();

    // Physics info
    rvPacker.pack( *vert, pVert, *verts, *out );
  }

  // If requested, remove the input data from the TES and delete
  if ( m_deleteInput )
  {
    evtSvc()->unregisterObject( verts );
    delete verts;
    verts = NULL;
  }
  else
  {
    // Clear the registry address of the unpacked container, to prevent reloading
    verts->registry()->setAddress( 0 );
  }

  return StatusCode::SUCCESS;
}
//=============================================================================
