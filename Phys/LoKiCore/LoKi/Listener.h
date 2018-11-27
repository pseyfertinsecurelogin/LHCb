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
#ifndef LOKI_LISTENER_H
#define LOKI_LISTENER_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/IIncidentListener.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Interface.h"
#include "LoKi/AuxFunBase.h"
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  /** @class Listener  LoKi/Listener.h
   *  Helper class to listen incidents
   *  @see IIncidentSvc
   *  @see IIncidentListener
   *  @see  Incident
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date   2010-04-03
   */
  class Listener
    : public virtual LoKi::AuxFunBase
    , public virtual implements1<IIncidentListener>
  {
  protected:
    // ========================================================================
    /// the actual type of incidents and their priority
    typedef std::vector<std::pair<std::string,long> >               Incidents ;
    // ========================================================================
  public:
    // ========================================================================
    /// default constructor
    Listener () ;                              // default constructor
    /// copy  constructor
    Listener ( const Listener& ) ;             // copy constructor
    /// MANDATORY: virtual destructor
    virtual ~Listener () ;                     // MANDATORY: virtual destrcutor
    // ========================================================================
  public:
    // ========================================================================
    /** subscribe the incident
     *  @param incident (IN) the incident to be subscribed
     *  @reuturn status code
     */
    StatusCode subscribe   ( const std::string& incident     ,
                             const long         priority = 0 ) ;
    /** unsubscribe the incident
     *  @param incident (IN) the incident to be subscribed
     *                      (empty string - for all incidents)
     *  @reuturn status code
     */
    StatusCode unsubscribe ( const std::string& incident = "" ) ;
    /// get the list of subscribed incidents
    const Incidents& incidents() const { return m_incidents ; }
    // ========================================================================
  public:
    // ========================================================================
    /// assignement
    Listener& operator=( const Listener& ) ;                    // assignement
    // ========================================================================
  private:
    // ========================================================================
    /// Incident service
    LoKi::Interface<IIncidentSvc>  m_incSvc    ;            // Incident service
    /// the list of incidents
    Incidents                      m_incidents ;       // the list of incidents
    // ========================================================================
  } ;
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_LISTENER_H
// ============================================================================
