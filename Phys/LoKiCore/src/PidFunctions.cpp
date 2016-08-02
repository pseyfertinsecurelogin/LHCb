// $Id$ 
// ============================================================================
// PartProp
// ============================================================================
#include "Kernel/ParticleID.h"
#include "Kernel/ParticleProperty.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/PidFunctions.h"
#include "LoKi/ParticleProperties.h"
#include "LoKi/Math.h"
#include "LoKi/ToCpp.h"
// ============================================================================
/** @file 
 *  Implementation file for functions from namespace LoKi::Pids 
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2015-01-18 
 */
// ============================================================================
LoKi::Pids::GetPids::GetPids ( const long id )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , m_ints  ( 1 , id ) 
  , m_names ()
{}
// ============================================================================
LoKi::Pids::GetPids::GetPids ( const unsigned long id )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , m_ints  ( 1 , id ) 
  , m_names ()
{}
// ============================================================================
LoKi::Pids::GetPids::GetPids ( const LHCb::ParticleID& id )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , m_ints  ( 1 , id.pid()  ) 
  , m_names ()
{}
// ============================================================================
LoKi::Pids::GetPids::GetPids ( const std::string& id )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , m_ints  () 
  , m_names ( 1 , id )
{}
// ============================================================================
LoKi::Pids::GetPids::GetPids ( const std::vector<int>& id )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , m_ints  ( id.begin() , id.end() ) 
  , m_names ()
{}
// ============================================================================
LoKi::Pids::GetPids::GetPids ( const std::vector<unsigned int>& id )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , m_ints  ( id.begin() , id.end() ) 
  , m_names ()
{}
// ============================================================================
LoKi::Pids::GetPids::GetPids ( const std::vector<long>& id )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , m_ints  ( id.begin() , id.end() ) 
  , m_names ()
{}
// ============================================================================
LoKi::Pids::GetPids::GetPids ( const std::vector<unsigned long>& id )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , m_ints  ( id.begin() , id.end() ) 
  , m_names ()
{}
// ============================================================================
LoKi::Pids::GetPids::GetPids ( const std::vector<LHCb::ParticleID>& id )
  : LoKi::AuxFunBase ( std::tie ( id ) )
{
  m_ints.resize  ( id.size  () ) ;
  std::transform ( id.begin () , 
                   id.end   () , 
                   m_ints.begin () , 
                   [](const LHCb::ParticleID& id) { return id.pid(); } );
}
// ============================================================================
LoKi::Pids::GetPids::GetPids ( const std::vector<std::string>& id )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , m_names ( id )
{
  std::vector<LHCb::ParticleID> pids ;
  if ( gaudi() && !id.empty() ) 
  { pids = LoKi::Particles::pidsFromNames ( m_names ) ; }
  m_ints.resize  ( pids.size  () ) ;
  std::transform ( pids.begin () , 
                   pids.end   () , 
                   m_ints.begin () , 
                   [](const LHCb::ParticleID& id) { return id.pid(); } );
}
// ============================================================================
bool LoKi::Pids::GetPids::in_list ( const long id ) const 
{
  if ( m_ints.empty() && !m_names.empty() ) { getData() ; }
  return m_ints.end() != std::find ( m_ints.begin() , m_ints.end  () , id ) ;  
}
// ============================================================================
bool LoKi::Pids::GetPids::in_abs_list ( const long id ) const 
{
  //
  if ( m_ints.empty() && !m_names.empty() ) { getData() ; }
  //
  return std::any_of ( m_ints.begin(), m_ints.end(),
                       [absid=std::abs(id)] (const long i) 
                       { return std::abs(i) == absid; } );
}
// ============================================================================
// load data (if needed) 
// ============================================================================
StatusCode LoKi::Pids::GetPids::getData()  const 
{
  //
  if ( !m_ints.empty() && m_names.empty() ) { return StatusCode::SUCCESS ; }
  if (  m_ints.size () == m_names.size () ) { return StatusCode::SUCCESS ; }
  //
  std::vector<LHCb::ParticleID> pids ( LoKi::Particles::pidsFromNames ( m_names ) ) ;
  //
  m_ints.resize  ( pids.size  () ) ;
  std::transform ( pids.begin () , 
                   pids.end   () , 
                   m_ints .begin () , 
                   [](const LHCb::ParticleID& id) { return id.pid(); } );
  //
  return StatusCode::SUCCESS ;
}
// ============================================================================
// The END 
// ============================================================================
