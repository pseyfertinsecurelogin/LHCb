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
#ifndef LOKI_IGENDECAY_H
#define LOKI_IGENDECAY_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/DecayFinder.h"
#include "LoKi/IDecayNode.h"
#include "LoKi/iTree.h"
// ============================================================================
// forward declaration
// ============================================================================
namespace HepMC {
  class GenParticle;
}
// ============================================================================
/** @file LoKi/IGenDecay.h
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
 *
 */
// ============================================================================
namespace Decays {
  // ==========================================================================
  /** @class IGenDecay LoKi/IGenDecay.h
   *  New "Decay Finder"
   *  @author Ivan BELYAEV
   *  @date   2009-05-22
   */
  struct GAUDI_API IGenDecay : extend_interfaces<IDecayNode> {
    // ========================================================================
    /// the actual type of the tree
    typedef Decays::Tree_<const HepMC::GenParticle*> Tree;
    /// the actual type of the tree
    typedef Decays::iTree_<const HepMC::GenParticle*> iTree;
    /// the actual type of decay finder:
    typedef Decays::Finder_<const HepMC::GenParticle*> Finder;
    // ========================================================================
    /** create the decay tree from the descriptor
     *  @param descriptor (INPUT)  the decay descriptor
     *  @param tree       (OUTPUT) the decay tree
     */
    virtual Tree tree( const std::string& decay = "" ) const = 0;
    // ========================================================================
    DeclareInterfaceID( IGenDecay, 2, 0 );
    // ========================================================================
  };
  // ==========================================================================
} //                                                    end of namespace Decays
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_IGENDECAY_H
