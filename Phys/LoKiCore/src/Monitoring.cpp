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
// Include files
// ============================================================================
// GaudioKernel
// ============================================================================
#include "GaudiKernel/IAlgContextSvc.h"
#include "GaudiKernel/IStatSvc.h"
#include "GaudiKernel/Stat.h"
#include "GaudiKernel/StatEntity.h"
// ============================================================================
// GaudiAlg
// ============================================================================
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiAlg/GaudiTool.h"
#include "GaudiAlg/GetAlgs.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Monitor.h"
#include "LoKi/Monitoring.h"
#include "LoKi/Report.h"
#include "LoKi/Services.h"
// ============================================================================
/*  get the (global) counter by name using IStatSvc
 *  @param IStatSvc
 *  @param ssvc service of statistics
 *  @param name the counter name
 *  @return the counter
 */
// ============================================================================
StatEntity* LoKi::Monitoring::getCounter( IStatSvc* csvc, const std::string& name ) {
  //
  if ( 0 == csvc ) // try to get it from LoKi
  {
    const LoKi::Services& svcs = LoKi::Services::instance();
    csvc                       = svcs.statSvc();
  }
  //
  if ( 0 == csvc ) {
    LoKi::Report::Error( "LoKi::Monitoring::getCounter(" + Gaudi::Utils::toString( name ) + "): invalid IStatSvc" );
    return 0; // RETURN
  }
  // get the counter
  Stat stat( csvc, name );
  //
  const StatEntity* counter = stat.entity();
  //
  if ( 0 == counter ) {
    LoKi::Report::Error( "LoKi::Monitoring::getCounter(" + Gaudi::Utils::toString( name ) + "): invalid StatEntity" );
    return 0; // RETURN
  }
  //
  LoKi::Report::Print( "LoKi::Monitoring::getCounter(" + Gaudi::Utils::toString( name ) + ")" +
                           " success with IStatSvc",
                       StatusCode::SUCCESS, MSG::DEBUG );
  //
  //
  return const_cast<StatEntity*>( counter ); // RETURN
}
// ============================================================================
/*  get the counter by name using GaudiAlgorithm
 *  @param alg the algorithm
 *  @param name the counter name
 *  @return the counter
 */
// ============================================================================
StatEntity* LoKi::Monitoring::getCounter( GaudiAlgorithm* alg, const std::string& name ) {
  if ( 0 == alg ) {
    LoKi::Report::Error( "LoKi::Monitoring::getCounter(" + Gaudi::Utils::toString( name ) +
                         "): invalid GaudiAlgorithm" );
    return 0;
  }
  // get the counter
  StatEntity* counter = &( alg->counter( name ) );
  //
  LoKi::Report::Print( "LoKi::Monitoring::getCounter(" + Gaudi::Utils::toString( name ) + ")" +
                           " success with GaudiAlgorithm: '" + alg->name() + "'",
                       StatusCode::SUCCESS, MSG::DEBUG );
  //
  return counter;
}
// ============================================================================
/*  get the counter by name using GaudiTool
 *  @param tool the tool
 *  @param name the counter name
 *  @return the counter
 */
// ============================================================================
StatEntity* LoKi::Monitoring::getCounter( GaudiTool* tool, const std::string& name ) {
  if ( 0 == tool ) {
    LoKi::Report::Error( "LoKi::Monitoring::getCounter(" + Gaudi::Utils::toString( name ) + "): invalid GaudiTool" );
    return 0;
  }
  // get the counter
  StatEntity* counter = &( tool->counter( name ) );
  //
  LoKi::Report::Print( "LoKi::Monitoring::getCounter(" + Gaudi::Utils::toString( name ) + ")" +
                           " success with GaudiTool: '" + tool->name() + "'",
                       StatusCode::SUCCESS, MSG::DEBUG );
  //
  return counter;
}
// ============================================================================
/*  get the counter by name using IAlgContextSvc
 *  @param name the counter name
 *  @param csvc context service
 *  @return the counter
 */
// ============================================================================
StatEntity* LoKi::Monitoring::getCounter( const IAlgContextSvc* csvc, const std::string& name ) {
  if ( 0 == csvc ) // try to get it for LoKi
  {
    const LoKi::Services& svcs = LoKi::Services::instance();
    csvc                       = svcs.contextSvc();
  }
  //
  if ( 0 == csvc ) {
    LoKi::Report::Error( "LoKi::Monitoring::getCounter(" + Gaudi::Utils::toString( name ) +
                         "): invalid IAlgContextSvc" );
    return 0; // RETURN
  }
  // get GaudiAlgorithm for the context:
  GaudiAlgorithm* alg = Gaudi::Utils::getGaudiAlg( csvc );
  //
  return getCounter( alg, name );
}
// ============================================================================
/*  get the counter by name using IStatSvc or IAlgContextSvc
 *  @param flag  local/global flag
 *  @param name  the counter name
 *  @return the counter
 */
// ============================================================================
StatEntity* LoKi::Monitoring::getCounter( const Flag flag, const std::string& name ) {
  return getCounter( flag, "", name );
}
// ============================================================================
/*  get the counter by name using IStatSvc or IAlgContextSvc
 *  @param flag  local/global flag
 *  @param name  the counter name
 *  @param group the counter grop
 *  @return the counter
 */
// ============================================================================
StatEntity* LoKi::Monitoring::getCounter( const Flag flag, const std::string& group, const std::string& name ) {
  IStatSvc* const       stat    = nullptr;
  IAlgContextSvc* const context = nullptr;
  //
  switch ( flag ) {
  case StatSvc:
    return group.empty() ? getCounter( stat, name ) : getCounter( stat, name + "::" + group );
  case ContextSvc:
    return group.empty() ? getCounter( context, name ) : getCounter( context, name + "::" + group );
  default:
    return group.empty() ? getCounter( stat, name ) : getCounter( stat, name + "::" + group );
  }
}
// ============================================================================
StatEntity* LoKi::Monitoring::getCounter( const LoKi::CounterDef& cnt ) {
  return getCounter( cnt.flag(), cnt.group(), cnt.name() );
}
// ============================================================================
// The END
// ============================================================================
