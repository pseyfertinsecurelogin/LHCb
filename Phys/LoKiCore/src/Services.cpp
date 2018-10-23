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
// ===========================================================================
// Include files
// ===========================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/ISvcLocator.h"
// ============================================================================
// PartProp
// ============================================================================
#include "Kernel/IParticlePropertySvc.h"
// ============================================================================
// LoKi
// ===========================================================================
#include "LoKi/Services.h"
#include "LoKi/Welcome.h"
#include "LoKi/Report.h"
// ============================================================================
/** @file
 *
 *  Implementation file for class LoKi::Services
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23
 */
// ===========================================================================
// accessor to unique instance ("Meyer's singleton" pattern)
// ===========================================================================
LoKi::Services& LoKi::Services::instance()
{
  static LoKi::Services s_instance;
  return s_instance ;
}
// ===========================================================================
// standard (default) constructor
// ===========================================================================
LoKi::Services::Services()
{
  LoKi::Welcome::instance() ;
}
// ===========================================================================
// release all services
// ===========================================================================
StatusCode LoKi::Services::releaseAll()
{
  // release services
  m_updateSvc.reset();
  // release services
  m_chronoSvc.reset();
  // release services
  m_statSvc.reset();
  // release services
  m_histoSvc.reset();
  // 'release' the service
  m_evtSvc.reset();
  // release services
  m_ppSvc.reset();
  // release services
  m_randSvc.reset();
  // 'release' the service
  m_contextSvc.reset();
  // 'release' the service
  if (m_lokiSvc) { m_lokiSvc->release(); m_lokiSvc = nullptr; }
  //
  return StatusCode::SUCCESS ;
}
// ===========================================================================
/*  Print the error  message, return status code
 *  @param msg    error message
 *  @param st     status code
 *  @return       status code
 */
// ===========================================================================
StatusCode LoKi::Services::Error
( const std::string& msg ,
  const StatusCode   st  ,
  const size_t       mx  ) const
{
  return LoKi::Report::Error(" LoKi::Services " + msg , st , mx );
}
// ===========================================================================
/*  Print the warning  message, return status code
 *  @param msg    warning message
 *  @param st     status code
 *  @return       status code
 */
// ===========================================================================
StatusCode LoKi::Services::Warning
( const std::string& msg  ,
  const StatusCode   st   ,
  const size_t       mx   ) const
{
  return LoKi::Report::Warning(" LoKi::Services " + msg , st , mx ) ;
}
// ===========================================================================
// accessor to main LoKi algorithm
// ===========================================================================
LoKi::ILoKiSvc* LoKi::Services::lokiSvc () const { return m_lokiSvc ; }
// ===========================================================================
// set new main LoKi algorithms
// ===========================================================================
LoKi::ILoKiSvc* LoKi::Services::setLoKi( LoKi::ILoKiSvc* svc )
{
  // add reference to the new algo
  if ( svc ) { svc -> addRef() ; }
  // release all  previously allocated  services
  releaseAll().ignore() ;
  // set new algorithm
  m_lokiSvc = svc ;
  //
  return lokiSvc();
}
// ===========================================================================
// accessor to particle properties service
// ===========================================================================
LHCb::IParticlePropertySvc* LoKi::Services::ppSvc     () const
{
  return svc(m_ppSvc, m_lokiSvc," LHCb::IParticlePropertySvc* points to NULL, return NULL");
}
// ===========================================================================
// accessor to context service
// ===========================================================================
IAlgContextSvc* LoKi::Services::contextSvc () const
{
  return svc(m_contextSvc,m_lokiSvc," IAlgContextSvc* points to NULL, return NULL");
}
// ===========================================================================
// accessor to histogram service
// ===========================================================================
IHistogramSvc* LoKi::Services::histoSvc () const
{
  return svc(m_histoSvc, m_lokiSvc, " IHistogramSvc* points to NULL, return NULL" );
}
// ===========================================================================
// accessor to Random Numbers Service
// ===========================================================================
IRndmGenSvc* LoKi::Services::randSvc () const
{
  return svc(m_randSvc, m_lokiSvc, "IRndmGenSvc* points to NULL, return NULL" );
}
// ===========================================================================
// accessor to Event Data Service
// ===========================================================================
IDataProviderSvc* LoKi::Services::evtSvc     () const
{
  return svc(m_evtSvc,m_lokiSvc, "IDataProviderSvc* points to NULL, return NULL" );
}
// ===========================================================================
// accessor to Statistical Service
// ===========================================================================
IStatSvc* LoKi::Services::statSvc     () const
{
  return svc(m_statSvc,  m_lokiSvc,"IStatSvc* points to NULL, return NULL" );
}
// ===========================================================================
// accessor to Chrono Service
// ===========================================================================
IChronoSvc* LoKi::Services::chronoSvc     () const
{
  return svc( m_chronoSvc, m_lokiSvc, "IChronoSvc* points to NULL, return NULL" );
}
// ===========================================================================
// accessor to Chrono Service
// ===========================================================================
IUpdateManagerSvc* LoKi::Services::updateSvc     () const
{
  return svc( m_updateSvc, m_lokiSvc, "IUpdateManagerSvc* points to NULL, return NULL");
}
// ===========================================================================
// The END
// ===========================================================================
