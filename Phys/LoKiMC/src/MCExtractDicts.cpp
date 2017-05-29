// $Id$
// ============================================================================
// Include files 
// ============================================================================
// LoKi
// ============================================================================
#include <iterator>

#include "Event/MCParticle.h"
#include "LoKi/MCExtract.h"
#include "LoKi/MCTypes.h"

namespace LHCb {
class MCVertex;
}  // namespace LHCb

// ============================================================================
/** @file 
 *  Implementation file for class for functions LoKi::Dicts::Exract::MCExtractDicts
 *  @author Vanya BELYAEV ibelyev@physics.syr.edu
 *  @date 2007-08-11 
 */
// ============================================================================
// get MC-particles form decay tree 
// ============================================================================
LHCb::MCParticle::ConstVector
LoKi::MCExtract::mcParticles
( const LHCb::MCParticle*      particle  ,
  const LoKi::MCTypes::MCCuts& cuts      ,
  const bool                   decayOnly ) 
{
  LHCb::MCParticle::ConstVector result ;
  result.reserve ( 15 ) ;
  LoKi::Extract::getMCParticles 
    ( particle , std::back_inserter ( result ) , cuts , decayOnly ) ;
  return result ;
}
// ============================================================================
// get MC-particles form decay tree 
// ============================================================================
LHCb::MCParticle::ConstVector
LoKi::MCExtract::mcParticles
( const LHCb::MCVertex*        vertex    ,
  const LoKi::MCTypes::MCCuts& cuts      ,
  const bool                   decayOnly ) 
{
  LHCb::MCParticle::ConstVector result ;
  result.reserve ( 15 ) ;
  LoKi::Extract::getMCParticles 
    ( vertex , std::back_inserter ( result ) , cuts , decayOnly ) ;
  return result ;
}
// ============================================================================
// The END 
// ============================================================================
