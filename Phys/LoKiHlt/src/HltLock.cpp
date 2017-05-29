// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "GaudiKernel/StatusCode.h"
#include "LoKi/HltEngineActor.h"
#include "LoKi/HltLock.h"
#include "LoKi/IHltAntiFactory.h"
#include "LoKi/Interface.h"
#include "LoKi/Report.h"

// ============================================================================
/** @file
 *  Implementation file for class LoKi::Hybrid::HltLock
 *  @date 2008-09-18
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 */
// ============================================================================
// contructor
// ============================================================================
LoKi::Hybrid::HltLock::HltLock
( LoKi::Hybrid::IHltAntiFactory* tool )
  : m_tool ( tool )
{
  auto& actor = LoKi::Hybrid::HltEngineActor::instance() ;
  // connect the tool to the actor
  StatusCode sc = actor.connectTool ( m_tool.getObject () ) ;
  if ( sc.isFailure () ) {
    LoKi::Report::Error
      ( "LoKi::Hybrid::HltLock: error from connectTool", sc ) .ignore() ;
  }
}
// ============================================================================
// destructor
// ============================================================================
LoKi::Hybrid::HltLock::~HltLock()
{
  auto& actor = LoKi::Hybrid::HltEngineActor::instance() ;
  // connect the tool to the actor
  StatusCode sc = actor.releaseTool ( m_tool.getObject () ) ;
  if ( sc.isFailure () ) {
    LoKi::Report::Error
      ( "LoKi::Hybrid::HltLock: error from releaseTool", sc ) .ignore() ;
  }
}
// ============================================================================
// The END
// ============================================================================
