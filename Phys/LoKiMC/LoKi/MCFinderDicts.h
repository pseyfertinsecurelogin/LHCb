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
#ifndef LOKI_MCFINDERDICTS_H
#define LOKI_MCFINDERDICTS_H 1
// ============================================================================
// Include Files
// ============================================================================
#include "LoKi/MCFinder.h"
namespace LoKi {
  namespace Dicts {
    namespace MCFinderDicts {
      // ======================================================================
      /// valid object ?
      bool valid( const LoKi::MCFinder& f );
      // ======================================================================
      /// find MC-decays according to decay descriptor
      LoKi::Types::MCRange findDecays( const LoKi::MCFinder& f, const std::string& decay );
      // ======================================================================
      /// find MC-decays according to decay descriptor
      LoKi::Types::MCRange findDecays( const LoKi::MCFinder& f, const std::string& decay, const std::string& address );
      // ======================================================================
      /// find MC-decays according to decay descriptor
      LoKi::Types::MCRange findDecays( const LoKi::MCFinder& f, const std::string& decay,
                                       const LHCb::MCParticle::Container* mcps );
      // ======================================================================
      /// find MC-decays according to decay descriptor
      LoKi::Types::MCRange findDecays( const LoKi::MCFinder& f, const std::string& decay,
                                       const LoKi::Types::MCRange& mcps );
      // ======================================================================
      /// find MC-decays according to decay descriptor
      LoKi::Types::MCRange findDecays( const LoKi::MCFinder& f, const std::string& decay,
                                       const LHCb::MCParticle::Vector& mcps );
      // ======================================================================
      /// find MC-decays according to decay descriptor
      LoKi::Types::MCRange findDecays( const LoKi::MCFinder& f, const std::string& decay,
                                       const LHCb::MCParticle::ConstVector& mcps );
      // ======================================================================
      /// find MC-decays according to decay descriptor
      LoKi::Types::MCRange findDecays( const LoKi::MCFinder& f, const std::string& decay,
                                       const LoKi::Keeper<LHCb::MCParticle>& mcps );
      // ======================================================================
      /// find MC-decays according to decay descriptor
      LoKi::Types::MCRange findDecays( const LoKi::MCFinder& f, const std::string& decay,
                                       const LoKi::UniqueKeeper<LHCb::MCParticle>& mcps );
    } // namespace MCFinderDicts
  }   // namespace Dicts
} // end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_MCFINDERDICTS_H
