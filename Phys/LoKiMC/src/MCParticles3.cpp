// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/MCParticles.h"
#include "LoKi/MCParticles1.h"
#include "LoKi/MCParticles3.h"
// ============================================================================
/** @file Implementation file for classsed from file LoKi/MCParticles3.h
 *  @date 2017-02-07
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 */
// ============================================================================
LoKi::MCParticles::FromInclusive::FromInclusive
( const LoKi::Types::MCCuts& mother    ,
  const LoKi::Types::MCCuts& daughter  ,
  const bool                 decayOnly )
  : LoKi::AuxFunBase ( std::tie ( mother , daughter , decayOnly ) )
  , m_cut ( daughter && LoKi::MCParticles::InAncestors ( mother , decayOnly ) )
{}
// ============================================================================
LoKi::MCParticles::FromInclusive::FromInclusive
( const Decays::iNode& mother    ,
  const Decays::iNode& daughter  ,
  const bool           decayOnly )
  : LoKi::AuxFunBase ( std::tie ( mother , daughter , decayOnly ) )
  , m_cut
    ( LoKi::MCParticles::DecNode ( daughter ) &&
      LoKi::MCParticles::InAncestors
      ( LoKi::MCParticles::DecNode ( mother ) , decayOnly ) )
{}
// ============================================================================
LoKi::MCParticles::FromInclusive::FromInclusive
( const std::string& mother    ,
  const std::string& daughter  ,
  const bool         decayOnly )
  : LoKi::AuxFunBase ( std::tie ( mother , daughter , decayOnly ) )
  , m_cut
    ( LoKi::MCParticles::DecNode ( daughter ) &&
      LoKi::MCParticles::InAncestors
      ( LoKi::MCParticles::DecNode ( mother ) , decayOnly ) )
{}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::MCParticles::FromInclusive*
LoKi::MCParticles::FromInclusive::clone() const
{ return new LoKi::MCParticles::FromInclusive(*this) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool
LoKi::MCParticles::FromInclusive::operator() ( const LHCb::MCParticle* p ) const
{ return m_cut.fun ( p ) ; }
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream&
LoKi::MCParticles::FromInclusive::fillStream ( std::ostream& s ) const
{ return s << m_cut ; }
// ============================================================================




// ============================================================================
// The END
// ============================================================================
