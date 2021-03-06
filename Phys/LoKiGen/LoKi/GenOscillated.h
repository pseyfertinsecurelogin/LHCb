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
#ifndef LOKI_GENOSCILLATED_H
#define LOKI_GENOSCILLATED_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiKerne
// ============================================================================
#include "GaudiKernel/Kernel.h"
// ============================================================================
namespace HepMC {
  class GenParticle;
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
 *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
 *
 */
// ===========================================================================
namespace LoKi {
  // ==========================================================================
  namespace GenParticles {
    // ========================================================================
    /** check the oscillation for the particle
     *  @param p the particle
     *  @return the oscillated mother particle
     *  @author Vanya BELYAEV  Ivan.Belyaev@nikhef.nl
     *  @date 2008-06-03
     */
    GAUDI_API
    const HepMC::GenParticle* oscillated1( const HepMC::GenParticle* p );
    // ========================================================================
    /** check the oscillation for the particle
     *  @param p the particle
     *  @return the oscillated daughter particle
     *  @author Vanya BELYAEV  Ivan.Belyaev@nikhef.nl
     *  @date 2008-06-03
     */
    GAUDI_API
    const HepMC::GenParticle* oscillated2( const HepMC::GenParticle* p );
    // ========================================================================
  } // namespace GenParticles
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_GENOSCILLATED_H
