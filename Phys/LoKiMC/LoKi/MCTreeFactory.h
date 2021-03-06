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
#ifndef LOKI_MCTREEFACTORY_H
#define LOKI_MCTREEFACTORY_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <iosfwd>
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Trees.h"
#include "LoKi/iTree.h"
// ============================================================================
/// forward declarations
namespace LHCb {
  class MCParticle;
} // namespace LHCb
// ============================================================================
namespace Decays {
  // ==========================================================================
  namespace Parsers {
    class Tree;
  }
  // ==========================================================================
  namespace Trees {
    // ========================================================================
    /** "Factory" to create the proper Tree from the full description
     *   @param tree       (OUTPUT) the constructed tree
     *   @param mother     (INPUT)  the mother particle
     *   @param oscillated (INPUT)  the oscillation flag
     *   @param arrow      (INPUT)  the type of arrow
     *   @param daughters  (INPUT)  the list of daughter trees
     *   @param inclusive  (INPUT)  the flag for inclusive
     *   @param optional   (INPUT)  the list of "optional" particles
     *   @param stream     (OUTPUT) the stream to report errors
     *   @return StatusCode
     *   @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *   @date   2009-05-06
     */
    GAUDI_API
    StatusCode factory( Decays::Trees::Types_<const LHCb::MCParticle*>::Tree& tree, const Decays::iNode& mother,
                        const Decays::Trees::Oscillation& oscillated, const Decays::Trees::Arrow& arrow,
                        const Decays::Trees::Types_<const LHCb::MCParticle*>::TreeList& daughters, const bool inclusive,
                        const Decays::Trees::Types_<const LHCb::MCParticle*>::TreeList& optional,
                        std::ostream&                                                   stream );
    // ========================================================================
    /** "Factory" to create the proper Tree from "short" descriptor
     *   @param tree       (OUTPUT) the constructed tree
     *   @param mother     (INPUT)  the mother particle
     *   @param oscillated (INPUT)  the oscillation flag
     *   @return status code
     *   @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *   @date   2009-05-06
     */
    GAUDI_API
    StatusCode factory( Decays::Trees::Types_<const LHCb::MCParticle*>::Tree& tree, const Decays::iNode& mother,
                        const Decays::Trees::Oscillation& oscillated );
    // ========================================================================
    /** "Factory" to create the proper Tree from the parsed tree
     *   @param tree       (OUTPUT) the constructed tree
     *   @param parsed     (INPUT)  the parsed tree
     *   @param stream     (OUTPUT) the stream to report errors
     *   @return status code
     *   @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *   @date   2009-05-22
     */
    GAUDI_API
    StatusCode factory( Decays::Trees::Types_<const LHCb::MCParticle*>::Tree& tree, const Decays::Parsers::Tree& parsed,
                        std::ostream& stream );
    // ========================================================================
  } // namespace Trees
  // ==========================================================================
} //                                                    end of namespace Decays
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_MCTREEFACTORY_H
