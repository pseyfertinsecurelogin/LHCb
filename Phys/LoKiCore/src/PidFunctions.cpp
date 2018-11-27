/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
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
{}
// ============================================================================
LoKi::Pids::GetPids::GetPids ( const unsigned long id )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , m_ints  ( 1 , id )
{}
// ============================================================================
LoKi::Pids::GetPids::GetPids ( const LHCb::ParticleID& id )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , m_ints  ( 1 , id.pid()  )
{}
// ============================================================================
LoKi::Pids::GetPids::GetPids ( const std::string& id )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , m_names ( 1 , id )
{}
// ============================================================================
LoKi::Pids::GetPids::GetPids ( const std::vector<int>& id )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , m_ints  ( id.begin() , id.end() )
{}
// ============================================================================
LoKi::Pids::GetPids::GetPids ( const std::vector<unsigned int>& id )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , m_ints  ( id.begin() , id.end() )
{}
// ============================================================================
LoKi::Pids::GetPids::GetPids ( const std::vector<long>& id )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , m_ints  ( id.begin() , id.end() )
{}
// ============================================================================
LoKi::Pids::GetPids::GetPids ( const std::vector<unsigned long>& id )
  : LoKi::AuxFunBase ( std::tie ( id ) )
  , m_ints  ( id.begin() , id.end() )
{}
// ============================================================================
LoKi::Pids::GetPids::GetPids ( const std::vector<LHCb::ParticleID>& id )
  : LoKi::AuxFunBase ( std::tie ( id ) )
{
  m_ints.resize  ( id.size  () ) ;
  std::transform ( id.begin () , id.end   () ,
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
  std::transform ( pids.begin () , pids.end   () ,
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
  decltype(m_ints) ints; ints.reserve(pids.size());
  std::transform ( pids.begin () , pids.end   () ,
                   std::back_inserter(ints),
                   [](const LHCb::ParticleID& id) { return id.pid(); } );
  m_ints = std::move(ints); // minimize datarace window...
  //
  return StatusCode::SUCCESS ;
}
// ============================================================================
// The END
// ============================================================================
