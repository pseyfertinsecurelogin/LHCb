/*****************************************************************************\
* (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// ============================================================================
// local
#include "GaudiGSL/GaudiGSL.h"

// ============================================================================
/** @file
 *
 *  Implementation file for class : GaudiGSL
 *
 *  @date 29/04/2002
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 */
// ============================================================================

// ============================================================================
/** define the initial value for static variable
 */
// ============================================================================
namespace {
  static const IGslSvc* s_gslSvc = nullptr;
}
// ============================================================================

// ============================================================================
/** static accessor to Gaudi GSL Service
 *  @return (const) pointer to Gaudi GSL Service
 */
// ============================================================================
const IGslSvc* GaudiGSL::gslSvc() { return s_gslSvc; }
// ============================================================================

// ============================================================================
/**set new value for static Gaudi GSL Service
 *  @return (const) pointer to Gaudi GSL Service
 */
// ============================================================================
const IGslSvc* GaudiGSL::setGslSvc( const IGslSvc* value ) {
  s_gslSvc = value;
  return gslSvc();
}
// ============================================================================

// ============================================================================
// The END
// ============================================================================
