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
#ifndef LOKI_MCALGSDICTS_H
#define LOKI_MCALGSDICTS_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Constants.h"
#include "LoKi/MCAlgs.h"
// ============================================================================
namespace LoKi {
  namespace Dicts {
    namespace MCAlgs {
      // ======================================================================
      /// count in the tree
      std::size_t count_if( const LHCb::MCParticle* particle, const LoKi::MCTypes::MCCuts& cut,
                            const bool decayOnly = false );
      // ======================================================================
      /// count in the tree
      std::size_t count_if( const LHCb::MCVertex* vertex, const LoKi::MCTypes::MCCuts& cut,
                            const bool decayOnly = false );
      // ======================================================================
      /// check in the tree
      bool found( const LHCb::MCParticle* particle, const LoKi::MCTypes::MCCuts& cut, const bool decayOnly = false );
      // ======================================================================
      /// check in the tree
      bool found( const LHCb::MCVertex* vertex, const LoKi::MCTypes::MCCuts& cut, const bool decayOnly = false );
      // ======================================================================
      /// find minimal value in the tree
      double min_value( const LHCb::MCParticle* particle, const LoKi::MCTypes::MCFunc& fun,
                        const LoKi::MCTypes::MCCuts& cut, const bool decayOnly = false,
                        double result = LoKi::Constants::PositiveInfinity );
      // ======================================================================
      /// find maximal value in the tree
      double max_value( const LHCb::MCParticle* particle, const LoKi::MCTypes::MCFunc& fun,
                        const LoKi::MCTypes::MCCuts& cut, const bool decayOnly = false,
                        double result = LoKi::Constants::NegativeInfinity );
      // ======================================================================
      /// find minimal value in the tree
      double min_value( const LHCb::MCVertex* vertex, const LoKi::MCTypes::MCFunc& fun,
                        const LoKi::MCTypes::MCCuts& cut, const bool decayOnly = false,
                        double result = LoKi::Constants::PositiveInfinity );
      // ======================================================================
      /// find maximal value in the tree
      double max_value( const LHCb::MCVertex* vertex, const LoKi::MCTypes::MCFunc& fun,
                        const LoKi::MCTypes::MCCuts& cut, const bool decayOnly = false,
                        double result = LoKi::Constants::NegativeInfinity );
      // ======================================================================
      /// accumulate over the tree
      double accumulate( const LHCb::MCParticle* particle, const LoKi::MCTypes::MCFunc& fun,
                         const LoKi::MCTypes::MCCuts& cut, const bool decayOnly = false, double result = 0.0 );
      // ======================================================================
      /// accumulate over the tree
      double accumulate( const LHCb::MCVertex* vertex, const LoKi::MCTypes::MCFunc& fun,
                         const LoKi::MCTypes::MCCuts& cut, const bool decayOnly = false, double result = 0.0 );
      // ======================================================================
      /// find a minimal element
      const LHCb::MCParticle* min_element( const LHCb::MCParticle* particle, const LoKi::MCTypes::MCFunc& fun,
                                           const LoKi::MCTypes::MCCuts& cut, const bool decayOnly = false );
      // ======================================================================
      /// find a minimal element
      const LHCb::MCParticle* min_element( const LHCb::MCVertex* vertex, const LoKi::MCTypes::MCFunc& fun,
                                           const LoKi::MCTypes::MCCuts& cut, const bool decayOnly = false );
      // ======================================================================
      /// find a maximal element
      const LHCb::MCParticle* max_element( const LHCb::MCParticle* particle, const LoKi::MCTypes::MCFunc& fun,
                                           const LoKi::MCTypes::MCCuts& cut, const bool decayOnly = false );
      // ======================================================================
      /// find a maximal element
      const LHCb::MCParticle* max_element( const LHCb::MCVertex* vertex, const LoKi::MCTypes::MCFunc& fun,
                                           const LoKi::MCTypes::MCCuts& cut, const bool decayOnly = false );
      // ======================================================================
      /// find a mimimal element
      const LHCb::MCParticle* min_element( const LHCb::MCParticle::Container* particles,
                                           const LoKi::MCTypes::MCFunc& fun, const LoKi::MCTypes::MCCuts& cut );
      // ======================================================================
      /// find a miximal element
      const LHCb::MCParticle* max_element( const LHCb::MCParticle::Container* particles,
                                           const LoKi::MCTypes::MCFunc& fun, const LoKi::MCTypes::MCCuts& cut );
      // ======================================================================
      /// find a mimimal element
      const LHCb::MCParticle* min_element( const LHCb::MCParticle::ConstVector& particles,
                                           const LoKi::MCTypes::MCFunc& fun, const LoKi::MCTypes::MCCuts& cut );
      // ======================================================================
      /// find a miximal element
      const LHCb::MCParticle* max_element( const LHCb::MCParticle::ConstVector& particles,
                                           const LoKi::MCTypes::MCFunc& fun, const LoKi::MCTypes::MCCuts& cut );
      // ======================================================================
    } // namespace MCAlgs
  }   // namespace Dicts
} // end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_MCALGSDICTS_H
