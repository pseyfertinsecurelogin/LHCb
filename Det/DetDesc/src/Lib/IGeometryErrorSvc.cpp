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
#include "DetDesc/IGeometryErrorSvc.h"
// ============================================================================
/** @file
 *  Implementation file for class DetDesc::IGeometryErrorSvc
 *  @date 2007-12-14
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 */
// ============================================================================
// Retrieve the unique interface ID
// ============================================================================
const InterfaceID& DetDesc::IGeometryErrorSvc::interfaceID() {
  static const InterfaceID s_IID( "DetDesc::IGeometryErrorSvc", 1, 0 );
  return s_IID;
}

// ============================================================================
// The END
// ============================================================================
