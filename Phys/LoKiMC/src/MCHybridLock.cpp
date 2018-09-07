// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Report.h"
#include "LoKi/MCHybridLock.h"
#include "LoKi/MCHybridEngineActor.h"
#include "LoKi/Context.h"
// ============================================================================
/** @file
 *  Implementation file for class LoKi::MCHybridLock
 *  @date 2007-06-10
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 */
// ============================================================================
// contructor
// ============================================================================
LoKi::Hybrid::MCLock::MCLock
( const LoKi::IMCHybridTool* tool   ,
  const LoKi::Context&       context ) 
  : m_tool ( tool )
{
  LoKi::Hybrid::MCEngineActor& actor = LoKi::Hybrid::MCEngineActor::instance() ;
  // connect the tool to the actor
  StatusCode sc = actor.connect( m_tool.getObject () , context ) ;
  if ( sc.isFailure () )
  {  LoKi::Report::Error
      ( "LoKi::Hybrid::MCLock: error from connect", sc ) .ignore() ;
  }
}
// ============================================================================
// destructor
// ============================================================================
LoKi::Hybrid::MCLock::~MCLock()
{
  LoKi::Hybrid::MCEngineActor& actor = LoKi::Hybrid::MCEngineActor::instance() ;
  // connect the tool to the actor
  StatusCode sc = actor.disconnect ( m_tool.getObject () ) ;
  if ( sc.isFailure () )
  { LoKi::Report::Error
      ( "LoKi::Hybrid::MCLock: error from disconnect", sc ) .ignore() ;
  }
}
// ============================================================================
// The END
// ============================================================================
