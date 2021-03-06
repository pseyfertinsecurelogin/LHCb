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
#ifndef LOKI_MCFINDER_H
#define LOKI_MCFINDER_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
// ============================================================================
// LoKiCore
// ============================================================================
#include "LoKi/Interface.h"
#include "LoKi/MCFinderObj.h"
// ============================================================================
/** @file
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
namespace LoKi {
  // ==========================================================================
  /// forward declaration
  class MCFinderObj;
  // ==========================================================================
  /** @class MCFinder MCFinder.h LoKi/MCFinder.h
   *
   *  Helper class whcih simplify the manipulation with
   *  the class LoKi::IMCFinderObj
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date   2006-03-11
   */
  class GAUDI_API MCFinder : public LoKi::Interface<LoKi::MCFinderObj> {
  public:
    // ========================================================================
    /// Standard constructor
    MCFinder( const LoKi::MCFinderObj* );
    /// implicit conversion to the pointer
    operator const LoKi::MCFinderObj*() const;
    /// check for pointer validity:
    bool operator!() const { return validPointer(); }
    // ========================================================================
  };
  // ==========================================================================
} // namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_MCFINDER_H
