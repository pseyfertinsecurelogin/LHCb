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
#ifndef LOKI_MCKINEMATICS_H
#define LOKI_MCKINEMATICS_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/Kernel.h"
// ============================================================================
// LoKiCore
// ============================================================================
#include "LoKi/Kinematics.h"
// ============================================================================
// forward declarations
// ============================================================================
namespace LHCb {
  class MCParticle;
}
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
 *  @date 2001-01-23
 */
// ============================================================================
namespace LoKi {
  // ==========================================================================
  /** @namespace LoKi::MCKinematics
   *  collection varios kinematic-related functions
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  namespace MCKinematics {
    // ========================================================================
    /** Trivial function to evaluate the mass MCParticle
     *  @param  p particle
     *  @return invariant mass
     *  @see LHCb::MCParticle
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-17
     */
    double GAUDI_API mass( const LHCb::MCParticle* p );
    // ========================================================================
    /** Trivial function to evaluate the mass MCParticle
     *  @param  p1 the first particle
     *  @param  p2 the second particle
     *  @return invariant mass
     *  @see LHCb::MCParticle
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-17
     */
    double GAUDI_API mass( const LHCb::MCParticle* p1, const LHCb::MCParticle* p2 );
    // ========================================================================
    /** Trivial function to evaluate the mass MCParticle
     *  @param  p1 the first particle
     *  @param  p2 the second particle
     *  @param  p3 the third particle
     *  @return invariant mass
     *  @see LHCb::MCParticle
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-17
     */
    double GAUDI_API mass( const LHCb::MCParticle* p1, const LHCb::MCParticle* p2, const LHCb::MCParticle* p3 );
    // ========================================================================
    /** Trivial function to evaluate the mass MCParticle
     *  @param  p1 the first particle
     *  @param  p2 the second particle
     *  @param  p3 the third particle
     *  @param  p4 the fourth particle
     *  @return invariant mass
     *  @see LHCb::MCParticle
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-17
     */
    double GAUDI_API mass( const LHCb::MCParticle* p1, const LHCb::MCParticle* p2, const LHCb::MCParticle* p3,
                           const LHCb::MCParticle* p4 );
    // ========================================================================
  } // namespace MCKinematics
  // ==========================================================================
  namespace Kinematics {
    // ========================================================================
    /// export the namespace into the proper scope
    using namespace LoKi::MCKinematics;
    // ========================================================================
  } // namespace Kinematics
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_MCKINEMATICS_H
