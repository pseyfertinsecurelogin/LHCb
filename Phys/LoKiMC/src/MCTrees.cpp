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
#include "Event/MCParticle.h"
#include "Event/MCVertex.h"
// ============================================================================
// local
// ============================================================================
#include "LoKi/MCTrees.h"
// ============================================================================

// ============================================================================
/** @file
 *
 *  Implementation file for functions from namespace LoKi::MCTrees
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

// ============================================================================
/** simple function with evaluates to true
 *  if MC particle  originates in the decay
 *  tree of parent veretx
 *  @param particle pointer to MC particle
 *  @param parent pointer to the mother veretx
 *  @return true if particle belongd to decay tree of parent
 */
// ============================================================================
bool LoKi::MCTrees::fromMCTree( const LHCb::MCParticle* particle, const LHCb::MCVertex* parent ) {
  if ( 0 == particle || 0 == parent ) { return false; }
  typedef SmartRefVector<LHCb::MCParticle> Products;
  const Products&                          products = parent->products();
  for ( Products::const_iterator it = products.begin(); products.end() != it; ++it ) {
    const LHCb::MCParticle* mcp = *it;
    if ( 0 == mcp ) { continue; }
    if ( fromMCTree( particle, mcp ) ) { return true; } // RETURN
  }
  return false;
}
// ============================================================================

// ============================================================================
/** simple function with evaluates to true
 *  if MC particle  originates in the decay
 *  tree of parent particle
 *  @param particle pointer to MC particle
 *  @param parent pointer to the mother particle
 *  @return true if particle belongd to decay tree of parent
 */
// ============================================================================
bool LoKi::MCTrees::fromMCTree( const LHCb::MCParticle* particle, const LHCb::MCParticle* parent ) {
  if ( 0 == particle || 0 == parent ) { return false; } // RETURN

  if ( particle == parent ) { return true; } // RETURN

  typedef SmartRefVector<LHCb::MCVertex> EndVertices;
  const EndVertices&                     endVertices = parent->endVertices();
  for ( EndVertices::const_iterator it = endVertices.begin(); endVertices.end() != it; ++it ) {
    const LHCb::MCVertex* mcv = *it;
    if ( 0 == mcv ) { continue; }
    if ( fromMCTree( particle, mcv ) ) { return true; } // RETURN
  }
  return false;
}
// ============================================================================

// ============================================================================
// The END
// ============================================================================
