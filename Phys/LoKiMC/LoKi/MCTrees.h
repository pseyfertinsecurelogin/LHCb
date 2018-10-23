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
// ===========================================================================
#ifndef LOKI_MCTREES_H
#define LOKI_MCTREES_H 1
#include "GaudiKernel/Kernel.h" // for GAUDI_API
// ===========================================================================
// forward declarations
// ===========================================================================
namespace LHCb
{
  class MCParticle ;
  class MCVertex   ;
}
// ===========================================================================
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
namespace LoKi
{
  // ==========================================================================
  /** @namespace LoKi::MCTrees
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2006-11-25
   */
  namespace MCTrees
  {
    // ========================================================================
    /** simple function with evaluates to true
     *  if MC particle  originates in the decay
     *  tree of parent particle
     *  @param particle pointer to MC particle
     *  @param parent pointer to the mother particle
     *  @return true if particle belongd to decay tree of parent
     */
    GAUDI_API
    bool fromMCTree
    ( const LHCb::MCParticle* particle  ,
      const LHCb::MCParticle* parent    ) ;
    // ========================================================================
    /** simple function with evaluates to true
     *  if MC particle  originates in the decay
     *  tree of parent veretx
     *  @param particle pointer to MC particle
     *  @param parent pointer to the mother veretx
     *  @return true if particle belongd to decay tree of parent
     */
    GAUDI_API
    bool fromMCTree
    ( const LHCb::MCParticle* particle  ,
      const LHCb::MCVertex*   parent    ) ;
    // ========================================================================
  } //                                           end of namespace LoKi::MCTrees
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_MCTREES_H
// ============================================================================
