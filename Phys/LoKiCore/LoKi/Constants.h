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
#ifndef LOKI_CONSTANTS_H
#define LOKI_CONSTANTS_H 1
// ============================================================================
// include files
// ============================================================================
// STD & STL
// ============================================================================
#include <limits>
// ============================================================================
// Gaudi Units (previously CLHEP)
// ============================================================================
#include "GaudiKernel/SystemOfUnits.h"
#include "GaudiKernel/PhysicalConstants.h"
// ============================================================================
/** @file LoKi/Constants.h
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
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  /** @namespace LoKi::Constants LoKi/Constants.h
   *
   *  Namespace  with the collection of useful constants
   *
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date   2003-07-10
   */
  namespace Constants
  {
    // ========================================================================
    /// representation of invalid time
    constexpr double InvalidTime         = -1.e+10 * Gaudi::Units::ns ;
    /// representation of invalid/infinite time
    constexpr double InfiniteTime        =  1.e+10 * Gaudi::Units::ns ;
    /// representation of invalid distance
    constexpr double InvalidDistance     = -1.e+10 * Gaudi::Units::km ;
    /// representation of invalid chi2
    constexpr double InvalidChi2         = -1.e+10     ;
    /// representation of invalid particle ID
    constexpr double InvalidID           =      0      ;
    /// representation of invalid particle charge
    constexpr double InvalidCharge       =  -1000      ;
    /// representation of invalid PID
    constexpr double InvalidPID          =      0      ;
    /// representation of invalid confedence level
    constexpr double InvalidConfLevel    =  -1000      ;
    /// representation of invalid energy
    constexpr double InvalidEnergy       =  -1.e+10 * Gaudi::Units::TeV ;
    /// representation of invalid momentum
    constexpr double InvalidMomentum     =  -1.e+10 * Gaudi::Units::TeV ;
    /// representation of invalid mass
    constexpr double InvalidMass         =  -1.e+10 * Gaudi::Units::TeV ;
    /// representation of invalid sine/cosine/angle, etc
    constexpr double InvalidAngle        =  -1.e+10    ;
    /// representation of HUGE distance
    constexpr double HugeDistance        =   1.e+10 * Gaudi::Units::km  ;
    /// representation of HUGE chi2
    constexpr double HugeChi2            =   1.e+10    ;
    /// representation of positive INFINITY
    constexpr double PositiveInfinity    = std::numeric_limits<float>::max() ;
    /// represetnation of Negative INFINITY
    constexpr double NegativeInfinity    = -PositiveInfinity ;
    // ========================================================================
  } //                                         end of namespace LoKi::Constants
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_CONSTANTS_H
// ============================================================================
