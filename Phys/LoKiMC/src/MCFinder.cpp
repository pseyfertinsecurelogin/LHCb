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
// GaudiAlg
// ============================================================================
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiAlg/GaudiTool.h"
// ============================================================================
// LoKiMC
// ============================================================================
#include "LoKi/MCFinder.h"
#include "LoKi/MCFinderObj.h"
// ============================================================================
/** @file
 *
 *  Implementation file for class LoKi::MCFinder
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
 *  @date 2006-03-11
 */
// ============================================================================
//  Standard constructor
// ============================================================================
LoKi::MCFinder::MCFinder( const LoKi::MCFinderObj* obj ) : LoKi::Interface<LoKi::MCFinderObj>( obj ) {}
// ============================================================================
//  implicit conversion to the pointer
// ============================================================================
LoKi::MCFinder::operator const LoKi::MCFinderObj*() const { return getObject(); }
// ============================================================================

// ============================================================================
// The END
// ============================================================================
