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
// Event
// ============================================================================
#include "Event/MCParticle.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/MCPIDOperators.h"
#include "LoKi/MCParticles.h"
#include "LoKi/ParticleProperties.h"
// ============================================================================
/** @file
 *
 * Implementation file for functions from the file LoKi/MCPIDOperators.h
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
namespace LoKi {
  // ==========================================================================
  namespace MCParticles {
    // ========================================================================
    // MCID ==
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator==( const LoKi::MCParticles::Identifier& /* id  */, const LHCb::ParticleID& pid ) {
      return LoKi::MCParticles::IsID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator==( const LoKi::MCParticles::Identifier& /* id */, const long pid ) {
      return LoKi::MCParticles::IsID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator==( const LoKi::MCParticles::Identifier& /* id */, const unsigned long pid ) {
      return LoKi::MCParticles::IsID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator==( const LoKi::MCParticles::Identifier& /* id */, const std::string& pid ) {
      return LoKi::MCParticles::IsID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator==( const LoKi::MCParticles::Identifier& /* id  */, const std::vector<LHCb::ParticleID>& pid ) {
      return LoKi::MCParticles::IsID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator==( const LoKi::MCParticles::Identifier& /* id */, const std::vector<int>& pid ) {
      return LoKi::MCParticles::IsID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator==( const LoKi::MCParticles::Identifier& /* id */, const std::vector<unsigned int>& pid ) {
      return LoKi::MCParticles::IsID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator==( const LoKi::MCParticles::Identifier& /* id */, const std::vector<long>& pid ) {
      return LoKi::MCParticles::IsID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator==( const LoKi::MCParticles::Identifier& /* id */, const std::vector<unsigned long>& pid ) {
      return LoKi::MCParticles::IsID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator==( const LoKi::MCParticles::Identifier& /* id */, const std::vector<std::string>& pid ) {
      return LoKi::MCParticles::IsID( pid );
    }
    // ========================================================================
    // MCABSID ==
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator==( const LoKi::MCParticles::AbsIdentifier& /* id  */, const LHCb::ParticleID& pid ) {
      return LoKi::MCParticles::IsAbsID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator==( const LoKi::MCParticles::AbsIdentifier& /* id */, const long pid ) {
      return LoKi::MCParticles::IsAbsID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator==( const LoKi::MCParticles::AbsIdentifier& /* id */, const unsigned long pid ) {
      return LoKi::MCParticles::IsAbsID( pid );
    }
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    // ========================================================================
    operator==( const LoKi::MCParticles::AbsIdentifier& /* id */, const std::string& pid ) {
      return LoKi::MCParticles::IsAbsID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator==( const LoKi::MCParticles::AbsIdentifier& /* id  */, const std::vector<LHCb::ParticleID>& pid ) {
      return LoKi::MCParticles::IsAbsID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator==( const LoKi::MCParticles::AbsIdentifier& /* id */, const std::vector<int>& pid ) {
      return LoKi::MCParticles::IsAbsID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator==( const LoKi::MCParticles::AbsIdentifier& /* id */, const std::vector<unsigned int>& pid ) {
      return LoKi::MCParticles::IsAbsID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator==( const LoKi::MCParticles::AbsIdentifier& /* id */, const std::vector<long>& pid ) {
      return LoKi::MCParticles::IsAbsID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator==( const LoKi::MCParticles::AbsIdentifier& /* id */, const std::vector<unsigned long>& pid ) {
      return LoKi::MCParticles::IsAbsID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator==( const LoKi::MCParticles::AbsIdentifier& /* id */, const std::vector<std::string>& pid ) {
      return LoKi::MCParticles::IsAbsID( pid );
    }
    // ========================================================================

    // ========================================================================
    // MCID !=
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator!=( const LoKi::MCParticles::Identifier& /* id  */, const LHCb::ParticleID& pid ) {
      return LoKi::MCParticles::IsNotID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator!=( const LoKi::MCParticles::Identifier& /* id */, const long pid ) {
      return LoKi::MCParticles::IsNotID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator!=( const LoKi::MCParticles::Identifier& /* id */, const unsigned long pid ) {
      return LoKi::MCParticles::IsNotID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator!=( const LoKi::MCParticles::Identifier& /* id */, const std::string& pid ) {
      return LoKi::MCParticles::IsNotID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator!=( const LoKi::MCParticles::Identifier& /* id  */, const std::vector<LHCb::ParticleID>& pid ) {
      return LoKi::MCParticles::IsNotID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator!=( const LoKi::MCParticles::Identifier& /* id */, const std::vector<int>& pid ) {
      return LoKi::MCParticles::IsNotID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator!=( const LoKi::MCParticles::Identifier& /* id */, const std::vector<unsigned int>& pid ) {
      return LoKi::MCParticles::IsNotID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator!=( const LoKi::MCParticles::Identifier& /* id */, const std::vector<long>& pid ) {
      return LoKi::MCParticles::IsNotID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator!=( const LoKi::MCParticles::Identifier& /* id */, const std::vector<unsigned long>& pid ) {
      return LoKi::MCParticles::IsNotID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator!=( const LoKi::MCParticles::Identifier& /* id */, const std::vector<std::string>& pid ) {
      return LoKi::MCParticles::IsNotID( pid );
    }
    // ========================================================================
    // MCABSID !=
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator!=( const LoKi::MCParticles::AbsIdentifier& /* id  */, const LHCb::ParticleID& pid ) {
      return LoKi::MCParticles::IsNotAbsID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator!=( const LoKi::MCParticles::AbsIdentifier& /* id */, const long pid ) {
      return LoKi::MCParticles::IsNotAbsID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator!=( const LoKi::MCParticles::AbsIdentifier& /* id */, const unsigned long pid ) {
      return LoKi::MCParticles::IsNotAbsID( pid );
    }
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    // ========================================================================
    operator!=( const LoKi::MCParticles::AbsIdentifier& /* id */, const std::string& pid ) {
      return LoKi::MCParticles::IsNotAbsID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator!=( const LoKi::MCParticles::AbsIdentifier& /* id  */, const std::vector<LHCb::ParticleID>& pid ) {
      return LoKi::MCParticles::IsNotAbsID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator!=( const LoKi::MCParticles::AbsIdentifier& /* id */, const std::vector<int>& pid ) {
      return LoKi::MCParticles::IsNotAbsID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator!=( const LoKi::MCParticles::AbsIdentifier& /* id */, const std::vector<unsigned int>& pid ) {
      return LoKi::MCParticles::IsNotAbsID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator!=( const LoKi::MCParticles::AbsIdentifier& /* id */, const std::vector<long>& pid ) {
      return LoKi::MCParticles::IsNotAbsID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator!=( const LoKi::MCParticles::AbsIdentifier& /* id */, const std::vector<unsigned long>& pid ) {
      return LoKi::MCParticles::IsNotAbsID( pid );
    }
    // ========================================================================
    LoKi::BasicFunctors<const LHCb::MCParticle*>::PredicateFromPredicate
    operator!=( const LoKi::MCParticles::AbsIdentifier& /* id */, const std::vector<std::string>& pid ) {
      return LoKi::MCParticles::IsNotAbsID( pid );
    }
    // ========================================================================

    // ========================================================================
  } // namespace MCParticles
  // ==========================================================================
} // namespace LoKi
// ============================================================================
// The END
// ============================================================================
