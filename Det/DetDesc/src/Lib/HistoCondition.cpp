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
// local
// ============================================================================
#include "DetDesc/HistoCondition.h"
// ============================================================================
/** @file 
 *
 *  Implementation file for class DetDesc::Histo1DCond
 *                     and  class DetDesc::Histo2DCond
 *
 *  @see DetDesc::Histo1DCond 
 *  @see DetDesc::Histo2DCond 
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date   2009-10-22
 */
// ============================================================================
// 1D  
// ============================================================================
// the constructor 
// ============================================================================
DetDesc::Histo1DCond::Histo1DCond ()
  : m_histo ("unspecified","unspecified",1,0,1)
{}
// ============================================================================
// update from other data object: 
// ============================================================================
void DetDesc::Histo1DCond::update ( DetDesc::Histo1DCond& right ) 
{
  m_histo = right.m_histo ;
  Condition::update ( right ) ;
}
// ============================================================================
// update from other data object: 
// ============================================================================
void DetDesc::Histo1DCond::update ( ValidDataObject& right ) 
{
  ValidDataObject* tmp = &right ;
  DetDesc::Histo1DCond* aux = dynamic_cast<DetDesc::Histo1DCond*> ( tmp ) ;
  if ( 0 != aux ) { return update ( *aux ) ; }                        // RETURN 
  Condition::update ( right ) ;
}
// ============================================================================
// perform the specific initialization (==creation of the histogram)
// ============================================================================
StatusCode DetDesc::Histo1DCond::initialize () 
{
  StatusCode sc =  Condition::initialize() ; 
  if ( sc.isFailure() ) { return sc ; }  // RETURN 
  // set the histogram here , later ...
  return StatusCode::SUCCESS ;
}
// ============================================================================
// 2D 
// ============================================================================
// the constructor 
// ============================================================================
DetDesc::Histo2DCond::Histo2DCond ()
  : m_histo ("unspecified","unspecified",1,0,1,1,0,1)
{}
// ============================================================================
// update from other data object: 
// ============================================================================
void DetDesc::Histo2DCond::update ( DetDesc::Histo2DCond& right ) 
{
  m_histo = right.m_histo ;
  Condition::update ( right ) ;
}
// ============================================================================
// update from other data object: 
// ============================================================================
void DetDesc::Histo2DCond::update ( ValidDataObject& right ) 
{
  ValidDataObject* tmp = &right ;
  DetDesc::Histo2DCond* aux = dynamic_cast<DetDesc::Histo2DCond*> ( tmp ) ;
  if ( 0 != aux ) { return update ( *aux ) ; }                        // RETURN 
  Condition::update ( right ) ;
}
// ============================================================================
// perform the specific initialization (==creation of the histogram)
// ============================================================================
StatusCode DetDesc::Histo2DCond::initialize () 
{ 
  StatusCode sc =  Condition::initialize() ; 
  if ( sc.isFailure() ) { return sc ; }  // RETURN 
  // set the histogram here , later ...
  return StatusCode::SUCCESS ;
}
// ============================================================================


// ============================================================================
// The END 
// ============================================================================
