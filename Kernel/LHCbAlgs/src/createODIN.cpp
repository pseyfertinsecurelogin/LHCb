// Include files 

#include <functional>

#include "Gaudi/Details/PluginServiceDetails.h"
#include "GaudiAlg/GaudiCommonImp.h"
#include "GaudiKernel/Algorithm.h"
// from Gaudi
#include "GaudiKernel/IEventTimeDecoder.h"
#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"
// local
#include "createODIN.h"

class ISvcLocator;

//-----------------------------------------------------------------------------
// Implementation file for class : createODIN
//
// 2008-01-15 : Marco Cattaneo
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( createODIN )


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
createODIN::createODIN( const std::string& name,
                        ISvcLocator* pSvcLocator )
  : GaudiAlgorithm ( name , pSvcLocator ) { }

//=============================================================================
// Initialization
//=============================================================================
StatusCode createODIN::initialize() 
{
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm

  m_decoder = tool<IEventTimeDecoder>( "OdinTimeDecoder" );

  return sc;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode createODIN::execute()
{
  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug() << "==> Execute" << endmsg;

  // OdinTimeDecoder internally calls getODIN() when getTime() is invoked
  m_decoder->getTime();
  
  return StatusCode::SUCCESS;
}
//=============================================================================
