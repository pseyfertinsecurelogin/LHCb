// $Id$
// ============================================================================
// Include files 
// ============================================================================
// HepMC
// ============================================================================
#ifdef __INTEL_COMPILER
  #pragma warning (disable:1572) // floating-point equality and inequality comparisons are unreliable
  #pragma warning(push)
#endif
#ifdef __INTEL_COMPILER
  #pragma warning(pop)
#endif
#include <iterator>
#include <vector>

// ============================================================================
// Event 
// ============================================================================
#include "Event/HepMCEvent.h"
#include "HepMC/IteratorRange.h"
#include "LoKi/GenExtract.h"
#include "LoKi/GenTypes.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Objects.h"
// IWYU issue 441
#include "LoKi/GenExtractDicts.h" // IWYU pragma: keep

namespace HepMC {
class GenEvent;
class GenParticle;
class GenVertex;
}  // namespace HepMC

// ============================================================================
/** @file 
 *  Implementation file for class LoKi::Dicts::GenExtract
 *  @date 2007-06-03 
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 */
// ============================================================================
// extract the particles from the event 
// ============================================================================
std::vector<const HepMC::GenParticle*>
LoKi::GenExtract::genParticles 
( const HepMC::GenEvent*       event , 
  const LoKi::GenTypes::GCuts& cuts  ) 
{
  std::vector<const HepMC::GenParticle*> result ;
  LoKi::Extract::genParticles ( event , std::back_inserter( result ) , cuts ) ;
  return result ;
}
// ============================================================================
// extract the particles from the event 
// ============================================================================
std::vector<const HepMC::GenParticle*>
LoKi::GenExtract::genParticles 
( const LHCb::HepMCEvent*      event , 
  const LoKi::GenTypes::GCuts& cuts  ) 
{
  std::vector<const HepMC::GenParticle*> result ;
  LoKi::Extract::genParticles ( event , std::back_inserter( result ) , cuts ) ;
  return result ;
}
// ============================================================================
// extract the particles from the event 
// ============================================================================
std::vector<const HepMC::GenParticle*>
LoKi::GenExtract::genParticles 
( const LHCb::HepMCEvents*     event , 
  const LoKi::GenTypes::GCuts& cuts  ) 
{
  std::vector<const HepMC::GenParticle*> result ;
  LoKi::Extract::genParticles ( event , std::back_inserter( result ) , cuts ) ;
  return result ;
}
// ============================================================================
// extract the particles from the vertex 
// ============================================================================
std::vector<const HepMC::GenParticle*>
LoKi::GenExtract::genParticles 
( const HepMC::GenVertex*      vertex , 
  const LoKi::GenTypes::GCuts& cuts   ,
  HepMC::IteratorRange         range  ) 
{
  std::vector<const HepMC::GenParticle*> result ;
  LoKi::Extract::genParticles 
    ( vertex , range , std::back_inserter( result ) , cuts ) ;
  return result ;
}
// ============================================================================
// extract the particles from the vertex 
// ============================================================================
std::vector<const HepMC::GenParticle*>
LoKi::GenExtract::genParticles 
( const HepMC::GenVertex*      vertex , 
  HepMC::IteratorRange         range  ) 
{
  std::vector<const HepMC::GenParticle*> result ;
  LoKi::Extract::genParticles 
    ( vertex , range , std::back_inserter( result ) , LoKi::Objects::_VALID_  ) ;
  return result ;
}
// ============================================================================
// extract the particles from the event 
// ============================================================================
std::vector<const HepMC::GenParticle*>
LoKi::GenExtract::genParticles 
( const HepMC::GenParticle*    particle  , 
  const LoKi::GenTypes::GCuts& cuts  ) 
{
  std::vector<const HepMC::GenParticle*> result ;
  LoKi::Extract::genParticles 
    ( particle , std::back_inserter( result ) , cuts ) ;
  return result ;
}
// ============================================================================
// The END 
// ============================================================================
