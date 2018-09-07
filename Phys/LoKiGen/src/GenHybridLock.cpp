// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Report.h"
#include "LoKi/GenHybridLock.h"
#include "LoKi/GenHybridEngineActor.h"
// ============================================================================
/** @file
 *  Implementation file for class LoKi::Hybrid::GenLock
 *  @date 2007-06-10
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 */
// ============================================================================
// contructor
// ============================================================================
LoKi::Hybrid::GenLock::GenLock
( const IGenHybridTool* tool   , 
  const LoKi::Context&  context ) 
  : m_tool ( tool )
{
  LoKi::Hybrid::GenEngineActor& actor = LoKi::Hybrid::GenEngineActor::instance() ;
  // connect the tool to the actor
  StatusCode sc = actor.connect ( m_tool.getObject () , context ) ;
  if ( sc.isFailure () )
  { LoKi::Report::Error
      ( "LoKi::Hybrid::GenLock: error from connectTool", sc ) .ignore() ; }
}
// ============================================================================
// destructor
// ============================================================================
LoKi::Hybrid::GenLock::~GenLock()
{
  LoKi::Hybrid::GenEngineActor& actor = LoKi::Hybrid::GenEngineActor::instance() ;
  // connect the tool to the actor
  StatusCode sc = actor.disconnect ( m_tool.getObject () ) ;
  if ( sc.isFailure () ) 
  { LoKi::Report::Error
      ( "LoKi::Hybrid::GenLock: error from releaseTool", sc ) .ignore() ; }
}
// ============================================================================
// The END
// ============================================================================
