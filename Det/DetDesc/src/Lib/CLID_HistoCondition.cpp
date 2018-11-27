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
// DetDesc 
// ============================================================================
#include "DetDesc/HistoCondition.h"
// ============================================================================
/** @file 
 *  Set the unique Class IDs for the Histogram conditions 
 *  @see CLID 
 *  @see DetDesc::Histo1DCond 
 *  @see DetDesc::Histo2DCond 
 *  @author Vanya BELYAEV Ivan.Bwlyaev@nikhef.nl
 *  @date   2009-10-22
 */
// ============================================================================
// The unique Class ID 
// ============================================================================
const CLID& DetDesc::Histo1DCond::classID () 
{
  static const CLID s_CLID = 101 ;
  return s_CLID ;
}
// ============================================================================
// The unique Class ID 
// ============================================================================
const CLID& DetDesc::Histo2DCond::classID () 
{
  static const CLID s_CLID = 102 ;
  return s_CLID ;
}
// ============================================================================
// The END 
// ============================================================================
