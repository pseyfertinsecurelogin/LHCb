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
// Include files 
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Report.h"
#include "LoKi/CoreLock.h"
#include "LoKi/CoreEngineActor.h"
// ============================================================================
/** @file 
 *  Implementation file for class LoKi::Hybrid::CoreLock
 *  @date 2008-09-18 
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 */
// ============================================================================
// contructor 
// ============================================================================
LoKi::Hybrid::CoreLock::CoreLock 
( const LoKi::Hybrid::ICoreAntiFactory* tool    ,   
  const LoKi::Context&                  context ) 
  : m_tool ( tool ) 
{
  LoKi::Hybrid::CoreEngineActor& actor = 
    LoKi::Hybrid::CoreEngineActor::instance() ;
  // connect the tool to the actor 
  StatusCode sc = actor.connect ( m_tool.getObject () , context ) ;  
  if ( sc.isFailure () ) 
  {
    LoKi::Report::Error
      ( "LoKi::Hybrid::CoreLock: error from connect", sc ) .ignore() ;
  } 
}
// ============================================================================
// destructor 
// ============================================================================
LoKi::Hybrid::CoreLock::~CoreLock()
{
  LoKi::Hybrid::CoreEngineActor& actor = 
    LoKi::Hybrid::CoreEngineActor::instance() ;
  // connect the tool to the actor 
  StatusCode sc = actor.disconnect ( m_tool.getObject () ) ;  
  if ( sc.isFailure () ) 
  {
    LoKi::Report::Error
      ( "LoKi::Hybrid::CoreLock: error from disconnect", sc ) .ignore() ;
  } 
}
// ============================================================================
// The END 
// ============================================================================
