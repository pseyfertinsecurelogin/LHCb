/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <vector>
#include <string>
// ============================================================================
// GaudiKernrel
// ============================================================================
#include "GaudiKernel/IInterface.h"
#include "GaudiKernel/SmartIF.h"
// ============================================================================
// local
// ============================================================================
#include "LoKi/Listener.h"
#include "LoKi/ILoKiSvc.h"
// ============================================================================
/** @file Listener.cpp
 *  Implementation file for class : Listener
 *  Helepr class to listen incidents
 *  @see IIncidentSvc
 *  @see IIncidentListener
 *  @see  Incident
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date   2010-04-03
 */
// ============================================================================
/// local anonymous namespace to hide some technical details
namespace
{
  // ==========================================================================
  auto match_first = [](std::string val) {
    return [v=std::move(val)]
           (const std::pair<std::string,long>& x)
           { return x.first == v; };
  };
  // ==========================================================================
  const std::string s_ALL = "ALL" ;
  // ==========================================================================
} //                                           end of local anonymous namespace
// ============================================================================
// default constructor (empty)
// ============================================================================
LoKi::Listener::Listener()
  : LoKi::AuxFunBase(std::tie())
{}
// ============================================================================
// copy constructor
// ============================================================================
LoKi::Listener::Listener ( const LoKi::Listener& right )
  : LoKi::AuxFunBase  ( right          )
  , IInterface        ( right          )
  , IIncidentListener ( right          )
  , extend_interfaces1<IIncidentListener> ( right )
  , implements1<IIncidentListener>        ( right )
  , m_incSvc          ( right.m_incSvc )
{
  // subscribe to all incidents
  for ( auto& ii : right.m_incidents ) subscribe( ii.first, ii.second ).ignore();
}
// ============================================================================
// MANDATORY: virtual destructor
// ============================================================================
LoKi::Listener::~Listener()
{
  while ( !m_incidents.empty()  && m_incSvc.validPointer() ) {
      m_incSvc->removeListener ( this , m_incidents.back().first ) ;
      m_incidents.pop_back() ;
  }
  m_incSvc.release() ;
}
// ============================================================================
// assignement
// ============================================================================
LoKi::Listener& LoKi::Listener:: operator=( const LoKi::Listener& right )
{
  // avoid self-assignement
  if ( &right == this ) { return *this ; }

  // 0. unsubscribe all own incidents
  unsubscribe ().ignore() ;

  //
  // start the actual assignement:
  //

  // 1. assign the base
  LoKi::AuxFunBase::operator= ( right ) ;
  // 2. copy the service
  m_incSvc = right.m_incSvc ;
  // 3. subscribe to all incidents from the right:
  for ( auto& ii : right.m_incidents )
  { subscribe ( ii.first, ii.second ).ignore() ; }
  //
  return *this ;
}
// ============================================================================
// subscribe the incident
// ============================================================================
StatusCode LoKi::Listener::subscribe
( const std::string& incident ,
  const long         priority )
{
  // specific incident ?
  auto ifind = std::find_if
    ( m_incidents.begin() , m_incidents.end() , match_first ( incident ) ) ;
  //
  if ( m_incidents.end() != ifind )
  {
    return Warning ( "subscribe: Incident '" + incident + "' already in the list",
                     StatusCode::SUCCESS ) ;
  }
  //
  if ( !m_incSvc )
  {
    SmartIF<IIncidentSvc> iis ( lokiSvc().getObject() ) ;
    m_incSvc = iis ;
  }
  Assert ( !(!m_incSvc) , "Unable to get Incident Service" ) ;
  //
  m_incSvc->addListener ( this , incident, priority ) ;
  m_incidents.emplace_back( incident , priority ) ;
  //
  return StatusCode::SUCCESS ;
}
// ============================================================================
// unsubscribe the incident
// ============================================================================

StatusCode LoKi::Listener::unsubscribe ( const std::string& incident )
{
  // no incidents?
  if ( m_incidents.empty() ) {
    StatusCode sc ( 810 , true ) ;
    return Error ( "unsubscribe: Empty list of incidents!", sc ) ;
  }
  // all incidents?
  if ( incident.empty() || s_ALL == incident ) {
    //
    if ( !m_incSvc ) {
      SmartIF<IIncidentSvc> iis ( lokiSvc().getObject() ) ;
      m_incSvc = iis ;
    }
    Assert ( !(!m_incSvc) , "Unable to get IIncident Service" ) ;
    //
    while ( !m_incidents.empty() ) {
      m_incSvc->removeListener ( this , m_incidents.back().first ) ;
      m_incidents.pop_back() ;
    }
  }
  // specific incident ?
  auto ifind = std::find_if
    ( m_incidents.begin() , m_incidents.end() , match_first(incident) ) ;
  if ( m_incidents.end() == ifind ) {
    StatusCode sc ( 811 , true ) ;
    return Error ( "unsubscribe: Unknown incident type: '" + incident + "'", sc ) ;
  }
  //
  if ( !m_incSvc ) {
    SmartIF<IIncidentSvc> iis ( lokiSvc().getObject() ) ;
    m_incSvc = iis ;
  }
  Assert ( !(!m_incSvc) , "Unable to get IIncident Service" ) ;
  //
  m_incSvc->removeListener ( this , incident ) ;
  m_incidents.erase ( ifind ) ;
  //
  return StatusCode::SUCCESS ;
}
// ========================================================================

// ============================================================================
// The END
// ============================================================================
