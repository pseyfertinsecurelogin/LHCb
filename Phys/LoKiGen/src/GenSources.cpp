// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/SmartDataPtr.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/GenSources.h"
#include "LoKi/GenAlgs.h"
#include "LoKi/GenExtract.h"
#include "LoKi/Services.h"
// ============================================================================
/** @file
 *  Implementation file for various sources
 *  @author Vanya BELYAEV ibelyav@physics.syr.edu
 *  @date 2006-12-07
 */
// ============================================================================
// constructor from the service, TES location and cuts
// ============================================================================
LoKi::GenParticles::SourceTES::SourceTES
( const IDataProviderSvc*      svc  ,
  const LoKi::GenTypes::GCuts& cuts ,
  const std::string&           path )
  : LoKi::AuxFunBase   ( std::tie ( svc , cuts , path ) ) 
  , SourceTES::_Base   ( svc , path ) 
  , m_cut              ( cuts )
{}
// ============================================================================
// constructor from the service, TES location and cuts
// ============================================================================
LoKi::GenParticles::SourceTES::SourceTES
( const IDataProviderSvc*      svc  ,
  const std::string&           path )
  : LoKi::AuxFunBase   ( std::tie ( svc , path ) ) 
  , SourceTES::_Base   ( svc , path ) 
  , m_cut              ( LoKi::BasicFunctors<const HepMC::GenParticle*>::BooleanConstant ( true ) )
{}
// ============================================================================
// constructor from the service, TES location and cuts
// ============================================================================
LoKi::GenParticles::SourceTES::SourceTES
( const GaudiAlgorithm*        svc          ,
  const LoKi::GenTypes::GCuts& cuts         ,
  const std::string&           path         ,
  const bool                   useRootInTES ) 
  : LoKi::AuxFunBase   ( std::tie ( svc , cuts , path , useRootInTES ) ) 
  , SourceTES::_Base   ( svc , path , useRootInTES ) 
  , m_cut              ( cuts )
{}
// ============================================================================
// constructor from the service, TES location and cuts
// ============================================================================
LoKi::GenParticles::SourceTES::SourceTES
( const GaudiAlgorithm*        svc          ,
  const std::string&           path         , 
  const bool                   useRootInTES ) 
  : LoKi::AuxFunBase   ( std::tie ( svc , path , useRootInTES ) ) 
  , SourceTES::_Base   ( svc , path , useRootInTES ) 
  , m_cut              ( LoKi::BasicFunctors<const HepMC::GenParticle*>::BooleanConstant ( true ) )
{}
// ============================================================================
// MANDATORY: the only essential method:
std::vector<const HepMC::GenParticle*>
LoKi::GenParticles::SourceTES::operator() () const
{
  
  const LHCb::HepMCEvent::Container* events = 
    LoKi::TES::get_<LHCb::HepMCEvent::Container>( *this ) ;
  Assert ( events , "No valid data is found at location '"+ path() +"'") ;
  std::vector<const HepMC::GenParticle*> particles ;
  particles.reserve ( 2000 ) ;
  //
  LoKi::Extract::getGenParticles
    ( events , std::back_inserter ( particles ) , m_cut.func () ) ;
  //
  return particles ;
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream&
LoKi::GenParticles::SourceTES::fillStream ( std::ostream& o ) const
{ return o << "GSOURCE(" << path () << "," << m_cut << ")" ; }
// ============================================================================
//                                                                      The END
// ============================================================================
