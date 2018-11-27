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
#include "LoKi/HltLock.h"
#include "LoKi/HltEngineActor.h"
// ============================================================================
/** @file
 *  Implementation file for class LoKi::Hybrid::HltLock
 *  @date 2008-09-18
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 */
// ============================================================================
// contructor
// ============================================================================
LoKi::Hybrid::HltLock::HltLock
( const LoKi::Hybrid::IHltAntiFactory* tool    , 
  const LoKi::Context&                 context ) 
  : m_tool ( tool )
{
  auto& actor = LoKi::Hybrid::HltEngineActor::instance() ;
  // connect the tool to the actor
  StatusCode sc = actor.connect ( m_tool.getObject () , context ) ;
  if ( sc.isFailure () ) 
  { LoKi::Report::Error
      ( "LoKi::Hybrid::HltLock: error from connectTool", sc ) .ignore() ; }
}
// ============================================================================
// destructor
// ============================================================================
LoKi::Hybrid::HltLock::~HltLock()
{
  auto& actor = LoKi::Hybrid::HltEngineActor::instance() ;
  // connect the tool to the actor
  StatusCode sc = actor.disconnect ( m_tool.getObject () ) ;
  if ( sc.isFailure () ) 
  { LoKi::Report::Error
      ( "LoKi::Hybrid::HltLock: error from releaseTool", sc ) .ignore() ; }
}
// ============================================================================
// The END
// ============================================================================
