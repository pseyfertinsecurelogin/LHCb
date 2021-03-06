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
#ifndef LOKI_GENALGSDICTS_H
#define LOKI_GENALGSDICTS_H
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Constants.h"
#include "LoKi/GenTypes.h"
// ============================================================================
namespace LoKi {
  namespace Dicts {
    namespace GenAlgs {
      // ======================================================================
      /// count the particles in the event
      std::size_t count_if( const HepMC::GenEvent* event, const LoKi::GenTypes::GCuts& cuts );
      // ======================================================================
      /// count the particels in the event
      std::size_t count_if( const LHCb::HepMCEvent* event, const LoKi::GenTypes::GCuts& cuts );
      // ======================================================================
      /// count the particels in the event
      std::size_t count_if( const LHCb::HepMCEvent::Container* event, const LoKi::GenTypes::GCuts& cuts );
      // ======================================================================
      /// count the particles in the tree
      std::size_t count_if( const HepMC::GenParticle* particle, const LoKi::GenTypes::GCuts& cuts );
      // ======================================================================
      /// count the particles in the tree
      std::size_t count_if( const HepMC::GenVertex* vertex, const LoKi::GenTypes::GCuts& cuts,
                            HepMC::IteratorRange range = HepMC::descendants );
      // ======================================================================
      /// check the presence in the event
      bool found( const HepMC::GenEvent* event, const LoKi::GenTypes::GCuts& cuts );
      // ======================================================================
      /// check the presence in the event
      bool found( const LHCb::HepMCEvent* event, const LoKi::GenTypes::GCuts& cuts );
      // ======================================================================
      /// check the presence in the event
      bool found( const LHCb::HepMCEvent::Container* event, const LoKi::GenTypes::GCuts& cuts );
      // ======================================================================
      /// check the presence in the tree
      bool found( const HepMC::GenParticle* particle, const LoKi::GenTypes::GCuts& cuts );
      // ======================================================================
      /// check the presence in the tree
      bool found( const HepMC::GenVertex* vertex, const LoKi::GenTypes::GCuts& cuts,
                  HepMC::IteratorRange range = HepMC::descendants );
      // ======================================================================
      /// accumulate through the addition
      double accumulate( const HepMC::GenEvent* event, const LoKi::GenTypes::GFunc& fun,
                         const LoKi::GenTypes::GCuts& cut, double res = 0.0 );
      // ======================================================================
      /// accumulate through the addition
      double accumulate( const LHCb::HepMCEvent* event, const LoKi::GenTypes::GFunc& fun,
                         const LoKi::GenTypes::GCuts& cut, double res = 0.0 );
      // ======================================================================
      /// accumulate through the addition
      double accumulate( const LHCb::HepMCEvent::Container* event, const LoKi::GenTypes::GFunc& fun,
                         const LoKi::GenTypes::GCuts& cut, double res = 0.0 );
      // ======================================================================
      /// accumulate through the addition
      double accumulate( const HepMC::GenVertex* vertex, const LoKi::GenTypes::GFunc& fun,
                         const LoKi::GenTypes::GCuts& cut, HepMC::IteratorRange range = HepMC::descendants,
                         double res = 0.0 );
      // ======================================================================
      /// accumulate through the addition
      double accumulate( const HepMC::GenParticle* particle, const LoKi::GenTypes::GFunc& fun,
                         const LoKi::GenTypes::GCuts& cut, double res = 0.0 );
      // ======================================================================
      /// find minimal value over the event
      double min_value( const HepMC::GenEvent* event, const LoKi::GenTypes::GFunc& fun,
                        const LoKi::GenTypes::GCuts& cut, double res = LoKi::Constants::PositiveInfinity );
      // ======================================================================
      /// find minimal value over the event
      double min_value( const LHCb::HepMCEvent* event, const LoKi::GenTypes::GFunc& fun,
                        const LoKi::GenTypes::GCuts& cut, double res = LoKi::Constants::PositiveInfinity );
      // ======================================================================
      /// find minimal value over the event
      double min_value( const LHCb::HepMCEvent::Container* event, const LoKi::GenTypes::GFunc& fun,
                        const LoKi::GenTypes::GCuts& cut, double res = LoKi::Constants::PositiveInfinity );
      // ======================================================================
      /// find minimal value over the tree:
      double min_value( const HepMC::GenParticle* particle, const LoKi::GenTypes::GFunc& fun,
                        const LoKi::GenTypes::GCuts& cut, double res = LoKi::Constants::PositiveInfinity );
      // ======================================================================
      /// find minimal value over the tree:
      double min_value( const HepMC::GenVertex* veretx, const LoKi::GenTypes::GFunc& fun,
                        const LoKi::GenTypes::GCuts& cut, HepMC::IteratorRange range = HepMC::descendants,
                        double res = LoKi::Constants::PositiveInfinity );
      // ======================================================================
      /// find maximum value over the event
      double max_value( const HepMC::GenEvent* event, const LoKi::GenTypes::GFunc& fun,
                        const LoKi::GenTypes::GCuts& cut, double res = LoKi::Constants::NegativeInfinity );
      // ======================================================================
      /// find maximum value over the event
      double max_value( const LHCb::HepMCEvent* event, const LoKi::GenTypes::GFunc& fun,
                        const LoKi::GenTypes::GCuts& cut, double res = LoKi::Constants::NegativeInfinity );
      // ======================================================================
      /// find maximum value over the event
      double max_value( const LHCb::HepMCEvent::Container* event, const LoKi::GenTypes::GFunc& fun,
                        const LoKi::GenTypes::GCuts& cut, double res = LoKi::Constants::NegativeInfinity );
      // ======================================================================
      /// find maximum value over the tree
      double max_value( const HepMC::GenParticle* particle, const LoKi::GenTypes::GFunc& fun,
                        const LoKi::GenTypes::GCuts& cut, double res = LoKi::Constants::NegativeInfinity );
      // ======================================================================
      /// find maximum value over the tree
      double max_value( const HepMC::GenVertex* vertex, const LoKi::GenTypes::GFunc& fun,
                        const LoKi::GenTypes::GCuts& cut, HepMC::IteratorRange range = HepMC::descendants,
                        double res = LoKi::Constants::NegativeInfinity );
      // ======================================================================
      /// find the minimum element through the event
      const HepMC::GenParticle* min_element( const HepMC::GenEvent* event, const LoKi::GenTypes::GFunc& fun,
                                             const LoKi::GenTypes::GCuts& cut );
      // ======================================================================
      /// find the minimum element through the event
      const HepMC::GenParticle* min_element( const LHCb::HepMCEvent* event, const LoKi::GenTypes::GFunc& fun,
                                             const LoKi::GenTypes::GCuts& cut );
      // ======================================================================
      /// find the minimum element through the event
      const HepMC::GenParticle* min_element( const LHCb::HepMCEvent::Container* event, const LoKi::GenTypes::GFunc& fun,
                                             const LoKi::GenTypes::GCuts& cut );
      // ======================================================================
      /// find the minimum element through the tree
      const HepMC::GenParticle* min_element( const HepMC::GenParticle* particle, const LoKi::GenTypes::GFunc& fun,
                                             const LoKi::GenTypes::GCuts& cut );
      // ======================================================================
      /// find the minimum element through the tree
      const HepMC::GenParticle* min_element( const HepMC::GenVertex* veretx, const LoKi::GenTypes::GFunc& fun,
                                             const LoKi::GenTypes::GCuts& cut,
                                             HepMC::IteratorRange         range = HepMC::descendants );
      // ======================================================================
      /// find the maximal element through the event
      const HepMC::GenParticle* max_element( const HepMC::GenEvent* event, const LoKi::GenTypes::GFunc& fun,
                                             const LoKi::GenTypes::GCuts& cut );
      // ======================================================================
      /// find the minimum element through the event
      const HepMC::GenParticle* max_element( const LHCb::HepMCEvent* event, const LoKi::GenTypes::GFunc& fun,
                                             const LoKi::GenTypes::GCuts& cut );
      // ======================================================================
      /// find the minimum element through the event
      const HepMC::GenParticle* max_element( const LHCb::HepMCEvent::Container* event, const LoKi::GenTypes::GFunc& fun,
                                             const LoKi::GenTypes::GCuts& cut );
      // ======================================================================
      /// find the minimum element through the tree
      const HepMC::GenParticle* max_element( const HepMC::GenParticle* particle, const LoKi::GenTypes::GFunc& fun,
                                             const LoKi::GenTypes::GCuts& cut );
      // ======================================================================
      /// find the minimum element through the tree
      const HepMC::GenParticle* max_element( const HepMC::GenVertex* veretx, const LoKi::GenTypes::GFunc& fun,
                                             const LoKi::GenTypes::GCuts& cut,
                                             HepMC::IteratorRange         range = HepMC::descendants );
      // ======================================================================
    } // namespace GenAlgs
  }   // namespace Dicts
} // end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_GENALGSDICTS_H
