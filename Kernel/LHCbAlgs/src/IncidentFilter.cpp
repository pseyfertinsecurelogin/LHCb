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
#include <set>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/Incident.h"
// ============================================================================
// GaudiAlg
// ============================================================================
#include "GaudiAlg/GaudiAlgorithm.h"
// ============================================================================
/** file
 *  Simple algorithm to filter events according occurance of
 *  the certains incidents
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
 *  @date 2011-12-16
 */
// ============================================================================
namespace Gaudi
{
  // ==========================================================================
  /** @class IncidentFilter
   *
   *  Trivial algorithm to filter events according to occurance
   *  of some incidents
   *
   *  @code
   *
   *  from Configurables import Gaudi__IncidentFilter and IncFltr
   *
   *  fltr = IncFltr ( Incidents = [ 'GoodEvent' ] )
   *
   *  @endcode
   *
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-10-05
   */
  class IncidentFilter : public extends<GaudiAlgorithm, IIncidentListener>
  {
  public:
    // ========================================================================
    /// standard constructor
    using extends::extends;
    /// standard initialization
    StatusCode initialize ()  override;
    /// standard finalization
    StatusCode finalize   ()  override;
    /// the only one essential method
    StatusCode execute ()  override; // the only one essential method
    // handle the incident
    void handle ( const Incident& inc ) override;
    // ========================================================================
  protected:
    // ========================================================================
    void setVeto ( bool value ) { m_veto = value ; }
    // ========================================================================
  private:
    // ========================================================================
    void   subscribe () ;
    void unsubscribe () ;
    // ========================================================================
  private:
    // ========================================================================
    bool                     m_veto     = false;
    bool                     m_decision = false;
    //
    typedef std::vector<std::string> LIST    ;
    Gaudi::Property<LIST> m_incidents
      { this, "Incidents" , {}, [=](auto&) {
          if ( Gaudi::StateMachine::INITIALIZED > FSMState() ) { return ; }
          if ( m_old_incidents == m_incidents.value()        ) { return ; }
          // unsubscribe old incidents
          IIncidentSvc* isvc = svc<IIncidentSvc>( "IncidentSvc" ) ;
          for ( const auto & item :  m_old_incidents )
          { isvc->removeListener ( this , item ) ; }
          // subscribe new incidents
          subscribe() ;
        }, "Incidents to be handled" };
    LIST m_old_incidents ;
    // ========================================================================
  } ; //                                            end of class IncidentFilter
  // ==========================================================================
  /** @class IncidentVeto
   *
   *  Trivial algorithm to veto events according to occurance
   *  of some incidents
   *
   *  @code
   *
   *  from Configurables import Gaudi__IncidentVeto and IncVeto
   *
   *  fltr = IncVeto ( Incidents = [ 'BadEvent' ] )
   *
   *  @endcode
   *
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-10-05
   */
  struct IncidentVeto : IncidentFilter
  {
    // ========================================================================
    /// standard constructor
    IncidentVeto
    ( const std::string& name ,
      ISvcLocator*       pSvc ) ;                       // standard constructor
    // ========================================================================
  } ; //                                            end of class IncidentFilter
  // ==========================================================================
} //                                                     end of namespace Gaudi
// ============================================================================
// Filter
// ============================================================================
// ============================================================================
// standard initialization
// ============================================================================
StatusCode Gaudi::IncidentFilter::initialize ()
{
  //
  StatusCode sc = extends::initialize() ;
  if ( sc.isFailure() ) { return sc ; }
  //
  subscribe () ;
  //
  return sc;
}
// ============================================================================
// finalize
// ============================================================================
StatusCode Gaudi::IncidentFilter::finalize ()
{
  unsubscribe () ;
  return extends::finalize () ;
}
// ============================================================================
// subscribe incidents
// ============================================================================
void Gaudi::IncidentFilter::subscribe ()
{
  //
  m_old_incidents.clear () ;
  //
  if ( m_incidents.empty() )
  { Warning ( "Empty list of incidents", StatusCode::SUCCESS ).ignore() ; return ; }
  //
  IIncidentSvc* isvc = svc<IIncidentSvc>( "IncidentSvc" ) ;
  //
  for ( const auto & item : m_incidents ) { isvc->addListener ( this , item ) ; }
  //
  if ( m_incidents.end() == std::find ( m_incidents.begin       () ,
                                        m_incidents.end         () ,
                                        IncidentType::BeginEvent   ) )
  { isvc->addListener ( this ,  IncidentType::BeginEvent  ) ; }
  //
  m_old_incidents = m_incidents ;
  //
}
// ============================================================================
// subscribe incidents
// ============================================================================
void Gaudi::IncidentFilter::unsubscribe ()
{
  m_old_incidents.clear () ;
  //
  IIncidentSvc* isvc = svc<IIncidentSvc>( "IncidentSvc" ) ;
  //
  for ( const auto & item : m_old_incidents )
  { isvc->removeListener ( this , item ) ; }
  //
  m_old_incidents = m_incidents ;
}
// ============================================================================
// handle the incident
// ============================================================================
void Gaudi::IncidentFilter::handle ( const Incident& inc )
{
  //
  // reset at the begin-event
  if ( IncidentType::BeginEvent == inc.type() ) { m_decision = m_veto ; }
  //
  // look in the list of incidents:
  if ( m_incidents.end() != std::find ( m_incidents.begin () ,
                                        m_incidents.end   () ,
                                        inc.type          () ) )
  { m_decision = !m_veto ; }
  //
}
// ============================================================================
// execute
// ============================================================================
StatusCode Gaudi::IncidentFilter::execute ()
{
  setFilterPassed ( m_decision ) ;
  return StatusCode::SUCCESS ;
}
// ============================================================================


// ============================================================================
// Veto
// ============================================================================
// standard constructor
// ============================================================================
Gaudi::IncidentVeto::IncidentVeto
( const std::string& name ,
  ISvcLocator*       pSvc )                        // standard constructor
  : Gaudi::IncidentFilter ( name , pSvc )
{
  setVeto ( true ) ;
}
// ============================================================================


// ============================================================================
// Factories
// ============================================================================
DECLARE_COMPONENT( Gaudi::IncidentFilter )
DECLARE_COMPONENT( Gaudi::IncidentVeto )

// ============================================================================
//                                                                      The END
// ============================================================================
