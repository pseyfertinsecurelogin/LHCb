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
#ifndef LOKI_GENPIDOPERATORS_H
#define LOKI_GENPIDOPERATORS_H 1
// ============================================================================
// Include files
// ============================================================================
// LCbKernel
// ============================================================================
#include "Kernel/ParticleID.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/GenParticles.h"
#include "LoKi/Operators.h"
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
  namespace GenParticles {
    // ========================================================================
    struct Identifier;
    struct AbsIdentifier;
    // ========================================================================

    // ========================================================================
    // ID ==
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const LoKi::GenParticles::Identifier& /* id  */, const LHCb::ParticleID& pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const LoKi::GenParticles::Identifier& /* id */, const long pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const LoKi::GenParticles::Identifier& /* id */, const unsigned long pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const LoKi::GenParticles::Identifier& /* id */, const std::string& pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const LoKi::GenParticles::Identifier& /* id  */, const std::vector<LHCb::ParticleID>& pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const LoKi::GenParticles::Identifier& /* id */, const std::vector<int>& pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const LoKi::GenParticles::Identifier& /* id */, const std::vector<unsigned int>& pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const LoKi::GenParticles::Identifier& /* id */, const std::vector<long>& pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const LoKi::GenParticles::Identifier& /* id */, const std::vector<unsigned long>& pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const LoKi::GenParticles::Identifier& /* id */, const std::vector<std::string>& pid );
    // ========================================================================

    // ========================================================================
    // ABSID ==
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const LoKi::GenParticles::AbsIdentifier& /* id  */, const LHCb::ParticleID& pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const LoKi::GenParticles::AbsIdentifier& /* id */, const long pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const LoKi::GenParticles::AbsIdentifier& /* id */, const unsigned long pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const LoKi::GenParticles::AbsIdentifier& /* id */, const std::string& pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const LoKi::GenParticles::AbsIdentifier& /* id  */, const std::vector<LHCb::ParticleID>& pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const LoKi::GenParticles::AbsIdentifier& /* id */, const std::vector<int>& pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const LoKi::GenParticles::AbsIdentifier& /* id */, const std::vector<unsigned int>& pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const LoKi::GenParticles::AbsIdentifier& /* id */, const std::vector<long>& pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const LoKi::GenParticles::AbsIdentifier& /* id */, const std::vector<unsigned long>& pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const LoKi::GenParticles::AbsIdentifier& /* id */, const std::vector<std::string>& pid );
    // ========================================================================

    // ========================================================================
    // ID !=
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const LoKi::GenParticles::Identifier& /* id  */, const LHCb::ParticleID& pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const LoKi::GenParticles::Identifier& /* id */, const long pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const LoKi::GenParticles::Identifier& /* id */, const unsigned long pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const LoKi::GenParticles::Identifier& /* id */, const std::string& pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const LoKi::GenParticles::Identifier& /* id  */, const std::vector<LHCb::ParticleID>& pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const LoKi::GenParticles::Identifier& /* id */, const std::vector<int>& pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const LoKi::GenParticles::Identifier& /* id */, const std::vector<unsigned int>& pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const LoKi::GenParticles::Identifier& /* id */, const std::vector<long>& pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const LoKi::GenParticles::Identifier& /* id */, const std::vector<unsigned long>& pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const LoKi::GenParticles::Identifier& /* id */, const std::vector<std::string>& pid );
    // ========================================================================

    // ========================================================================
    // ABSID !=
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const LoKi::GenParticles::AbsIdentifier& /* id  */, const LHCb::ParticleID& pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const LoKi::GenParticles::AbsIdentifier& /* id */, const long pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const LoKi::GenParticles::AbsIdentifier& /* id */, const unsigned long pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const LoKi::GenParticles::AbsIdentifier& /* id */, const std::string& pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const LoKi::GenParticles::AbsIdentifier& /* id  */, const std::vector<LHCb::ParticleID>& pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const LoKi::GenParticles::AbsIdentifier& /* id */, const std::vector<int>& pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const LoKi::GenParticles::AbsIdentifier& /* id */, const std::vector<unsigned int>& pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const LoKi::GenParticles::AbsIdentifier& /* id */, const std::vector<long>& pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const LoKi::GenParticles::AbsIdentifier& /* id */, const std::vector<unsigned long>& pid );
    // ========================================================================
    LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const LoKi::GenParticles::AbsIdentifier& /* id */, const std::vector<std::string>& pid );
    // ========================================================================

    // ========================================================================
    // ID ==
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const LHCb::ParticleID& pid, const LoKi::GenParticles::Identifier& id ) {
      return id == pid;
    }

    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const long pid, const LoKi::GenParticles::Identifier& id ) {
      return id == pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const unsigned long pid, const LoKi::GenParticles::Identifier& id ) {
      return id == pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const std::string& pid, const LoKi::GenParticles::Identifier& id ) {
      return id == pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const std::vector<LHCb::ParticleID>& pid, const LoKi::GenParticles::Identifier& id ) {
      return id == pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const std::vector<int>& pid, const LoKi::GenParticles::Identifier& id ) {
      return id == pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const std::vector<unsigned int>& pid, const LoKi::GenParticles::Identifier& id ) {
      return id == pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const std::vector<long>& pid, const LoKi::GenParticles::Identifier& id ) {
      return id == pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const std::vector<unsigned long>& pid, const LoKi::GenParticles::Identifier& id ) {
      return id == pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const std::vector<std::string>& pid, const LoKi::GenParticles::Identifier& id ) {
      return id == pid;
    }
    // ========================================================================

    // ========================================================================
    // ABSID ==
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const LHCb::ParticleID& pid, const LoKi::GenParticles::AbsIdentifier& id ) {
      return id == pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const long pid, const LoKi::GenParticles::AbsIdentifier& id ) {
      return id == pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const unsigned long pid, const LoKi::GenParticles::AbsIdentifier& id ) {
      return id == pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const std::string& pid, const LoKi::GenParticles::AbsIdentifier& id ) {
      return id == pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const std::vector<LHCb::ParticleID>& pid, const LoKi::GenParticles::AbsIdentifier& id ) {
      return id == pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const std::vector<int>& pid, const LoKi::GenParticles::AbsIdentifier& id ) {
      return id == pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const std::vector<unsigned int>& pid, const LoKi::GenParticles::AbsIdentifier& id ) {
      return id == pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const std::vector<long>& pid, const LoKi::GenParticles::AbsIdentifier& id ) {
      return id == pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const std::vector<unsigned long>& pid, const LoKi::GenParticles::AbsIdentifier& id ) {
      return id == pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const std::vector<std::string>& pid, const LoKi::GenParticles::AbsIdentifier& id ) {
      return id == pid;
    }
    // ========================================================================

    // ========================================================================
    // ID !=
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const LHCb::ParticleID& pid, const LoKi::GenParticles::Identifier& id ) {
      return id != pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const long pid, const LoKi::GenParticles::Identifier& id ) {
      return id != pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const unsigned long pid, const LoKi::GenParticles::Identifier& id ) {
      return id != pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const std::string& pid, const LoKi::GenParticles::Identifier& id ) {
      return id != pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const std::vector<LHCb::ParticleID>& pid, const LoKi::GenParticles::Identifier& id ) {
      return id != pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const std::vector<int>& pid, const LoKi::GenParticles::Identifier& id ) {
      return id != pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const std::vector<unsigned int>& pid, const LoKi::GenParticles::Identifier& id ) {
      return id != pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const std::vector<long>& pid, const LoKi::GenParticles::Identifier& id ) {
      return id != pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const std::vector<unsigned long>& pid, const LoKi::GenParticles::Identifier& id ) {
      return id != pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const std::vector<std::string>& pid, const LoKi::GenParticles::Identifier& id ) {
      return id != pid;
    }
    // ========================================================================

    // ========================================================================
    // ABSID !=
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const LHCb::ParticleID& pid, const LoKi::GenParticles::AbsIdentifier& id ) {
      return id != pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const long pid, const LoKi::GenParticles::AbsIdentifier& id ) {
      return id != pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const unsigned long pid, const LoKi::GenParticles::AbsIdentifier& id ) {
      return id != pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const std::string& pid, const LoKi::GenParticles::AbsIdentifier& id ) {
      return id != pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const std::vector<LHCb::ParticleID>& pid, const LoKi::GenParticles::AbsIdentifier& id ) {
      return id != pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const std::vector<int>& pid, const LoKi::GenParticles::AbsIdentifier& id ) {
      return id != pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const std::vector<unsigned int>& pid, const LoKi::GenParticles::AbsIdentifier& id ) {
      return id != pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const std::vector<long>& pid, const LoKi::GenParticles::AbsIdentifier& id ) {
      return id != pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const std::vector<unsigned long>& pid, const LoKi::GenParticles::AbsIdentifier& id ) {
      return id != pid;
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const std::vector<std::string>& pid, const LoKi::GenParticles::AbsIdentifier& id ) {
      return id != pid;
    }
    // ========================================================================

    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const LoKi::GenParticles::Identifier& id, const int pid ) {
      return id == LHCb::ParticleID( pid );
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const LoKi::GenParticles::AbsIdentifier& id, const int pid ) {
      return id == LHCb::ParticleID( pid );
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const LoKi::GenParticles::Identifier& id, const int pid ) {
      return id != LHCb::ParticleID( pid );
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const LoKi::GenParticles::AbsIdentifier& id, const int pid ) {
      return id != LHCb::ParticleID( pid );
    }
    // ========================================================================

    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const int pid, const LoKi::GenParticles::Identifier& id ) {
      return id == LHCb::ParticleID( pid );
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator==( const int pid, const LoKi::GenParticles::AbsIdentifier& id ) {
      return id == LHCb::ParticleID( pid );
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const int pid, const LoKi::GenParticles::Identifier& id ) {
      return id != LHCb::ParticleID( pid );
    }
    // ========================================================================
    inline LoKi::BasicFunctors<const HepMC::GenParticle*>::PredicateFromPredicate
    operator!=( const int pid, const LoKi::GenParticles::AbsIdentifier& id ) {
      return id != LHCb::ParticleID( pid );
    }
    // ========================================================================

    // ========================================================================
  } // namespace GenParticles
  // ==========================================================================
} // namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_GENPIDOPERATORS_H
