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
// LoKi
// ============================================================================
#include "LoKi/Report.h"
#include "LoKi/GenHybridLock.h"
#include "LoKi/GenHybridEngineActor.h"
// ============================================================================
/** @file
 *  Implementation file for class LoKi::Hybrid::GenLock
 *  @date 2007-06-10
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 */
// ============================================================================
// contructor
// ============================================================================
LoKi::Hybrid::GenLock::GenLock
( const IGenHybridTool* tool   , 
  const LoKi::Context&  context ) 
  : m_tool ( tool )
{
  LoKi::Hybrid::GenEngineActor& actor = LoKi::Hybrid::GenEngineActor::instance() ;
  // connect the tool to the actor
  StatusCode sc = actor.connect ( m_tool.getObject () , context ) ;
  if ( sc.isFailure () )
  { LoKi::Report::Error
      ( "LoKi::Hybrid::GenLock: error from connectTool", sc ) .ignore() ; }
}
// ============================================================================
// destructor
// ============================================================================
LoKi::Hybrid::GenLock::~GenLock()
{
  LoKi::Hybrid::GenEngineActor& actor = LoKi::Hybrid::GenEngineActor::instance() ;
  // connect the tool to the actor
  StatusCode sc = actor.disconnect ( m_tool.getObject () ) ;
  if ( sc.isFailure () ) 
  { LoKi::Report::Error
      ( "LoKi::Hybrid::GenLock: error from releaseTool", sc ) .ignore() ; }
}
// ============================================================================
// The END
// ============================================================================
