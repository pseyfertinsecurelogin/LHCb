// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include <iterator>
#include <ostream>
#include <string>
#include <vector>

#include "Event/HepMCEvent.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/SmartDataStorePtr.h"
#include "LoKi/AuxFunBase.h"
#include "LoKi/Functor.h"
#include "LoKi/GenExtract.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/GenSources.h"
#include "LoKi/GenTypes.h"
#include "LoKi/Interface.h"
#include "LoKi/Services.h"

namespace HepMC {
class GenParticle;
}  // namespace HepMC

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
( const std::string&           path ,
  IDataProviderSvc*            svc  ,
  const LoKi::GenTypes::GCuts& cuts )
  : m_path    ( path )
  , m_dataSvc ( svc  )
  , m_cut     ( cuts )
{}
// ============================================================================
// constructor from the service, TES location and cuts
// ============================================================================
LoKi::GenParticles::SourceTES::SourceTES
( const std::string&           path ,
  const LoKi::GenTypes::GCuts& cuts ,
  IDataProviderSvc*            svc  )
  : m_path    ( path )
  , m_dataSvc ( svc  )
  , m_cut     ( cuts )
{}
// ============================================================================
// constructor from the service, TES location and cuts
// ============================================================================
LoKi::GenParticles::SourceTES::SourceTES
( const LoKi::GenTypes::GCuts& cuts ,
  const std::string&           path ,
  IDataProviderSvc*            svc  )
  : m_path    ( path )
  , m_dataSvc ( svc  )
  , m_cut     ( cuts )
{}
// ============================================================================
// constructor from the service, TES location and cuts
// ============================================================================
LoKi::GenParticles::SourceTES::SourceTES
( const LoKi::GenTypes::GCuts& cuts ,
  IDataProviderSvc*            svc  ,
  const std::string&           path )
  : m_path    ( path )
  , m_dataSvc ( svc  )
  , m_cut     ( cuts )
{}
// ============================================================================
// MANDATORY: the only essential method:
LoKi::GenParticles::SourceTES::result_type
LoKi::GenParticles::SourceTES::operator() () const
{
  if ( !m_dataSvc )
  {
    const LoKi::Services& svcs = LoKi::Services::instance() ;
    m_dataSvc = svcs.evtSvc() ;
    Assert ( m_dataSvc.validPointer ( )               ,
             "Could not locate valid IDataProvidrSvc" ) ;
  }
  SmartDataPtr<LHCb::HepMCEvent::Container> events
    ( m_dataSvc , m_path ) ;
  const LHCb::HepMCEvent::Container* evts = events ;
  Assert ( 0 != evts , "No valid data is found at location '"+m_path+"'") ;
  std::vector<const HepMC::GenParticle*> particles ;
  particles.reserve ( 2000 ) ;
  //
  LoKi::Extract::getGenParticles
    ( evts , std::back_inserter ( particles ) , m_cut.func () ) ;
  //
  return particles ;
}
// ============================================================================
// OPTIONAL: the nice printout
// ============================================================================
std::ostream&
LoKi::GenParticles::SourceTES::fillStream ( std::ostream& o ) const
{ return o << "GSOURCE(" << m_path << "," << m_cut << ")" ; }
// ============================================================================
// The END
// ============================================================================
