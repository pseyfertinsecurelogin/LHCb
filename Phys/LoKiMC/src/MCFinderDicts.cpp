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
// LoKi
// ============================================================================
#include "LoKi/MCFinderDicts.h"
#include "LoKi/MCFinder.h"
#include "LoKi/MCFinderObj.h"
#include "LoKi/MCTypes.h"
// ============================================================================
/** @file
 *  Implementation file for class LoKi::Dicts::MCFinder
 *  @date 2007-06-06
 *  @author Vanya BELYAEV
 */
// ============================================================================
namespace {
  /// empty range
  const LoKi::Types::MCRange s_EMPTY = LoKi::Types::MCRange();
} // namespace
// ===========================================================================
// valid object?
// ===========================================================================
bool LoKi::Dicts::MCFinderDicts::valid( const LoKi::MCFinder& m ) { return m.validPointer(); }
// ===========================================================================
// find MC-decays according to decay descriptor
// ===========================================================================
LoKi::Types::MCRange LoKi::Dicts::MCFinderDicts::findDecays( const LoKi::MCFinder& f, const std::string& decay ) {
  return f.validPointer() ? f->findDecays( decay ) : s_EMPTY;
}
// ===========================================================================
// find MC-decays according to decay descriptor
// ===========================================================================
LoKi::Types::MCRange LoKi::Dicts::MCFinderDicts::findDecays( const LoKi::MCFinder& f, const std::string& decay,
                                                             const std::string& address ) {
  return f.validPointer() ? f->findDecays( decay, address ) : s_EMPTY;
}
// ===========================================================================
// find MC-decays according to decay descriptor
// ===========================================================================
LoKi::Types::MCRange LoKi::Dicts::MCFinderDicts::findDecays( const LoKi::MCFinder& f, const std::string& decay,
                                                             const LHCb::MCParticle::Container* mcps ) {
  return f.validPointer() ? f->findDecays( decay, mcps ) : s_EMPTY;
}
// ===========================================================================
// find MC-decays according to decay descriptor
// ===========================================================================
LoKi::Types::MCRange LoKi::Dicts::MCFinderDicts::findDecays( const LoKi::MCFinder& f, const std::string& decay,
                                                             const LHCb::MCParticle::Vector& mcps ) {
  return f.validPointer() ? f->findDecays( decay, mcps ) : s_EMPTY;
}
// ===========================================================================
// find MC-decays according to decay descriptor
// ===========================================================================
LoKi::Types::MCRange LoKi::Dicts::MCFinderDicts::findDecays( const LoKi::MCFinder& f, const std::string& decay,
                                                             const LHCb::MCParticle::ConstVector& mcps ) {
  return f.validPointer() ? f->findDecays( decay, mcps ) : s_EMPTY;
}
// ===========================================================================
// find MC-decays according to decay descriptor
// ===========================================================================
LoKi::Types::MCRange LoKi::Dicts::MCFinderDicts::findDecays( const LoKi::MCFinder& f, const std::string& decay,
                                                             const LoKi::Types::MCRange& mcps ) {
  return f.validPointer() ? f->findDecays( decay, mcps ) : s_EMPTY;
}
// ===========================================================================
// find MC-decays according to decay descriptor
// ===========================================================================
LoKi::Types::MCRange LoKi::Dicts::MCFinderDicts::findDecays( const LoKi::MCFinder& f, const std::string& decay,
                                                             const LoKi::Keeper<LHCb::MCParticle>& mcps ) {
  return f.validPointer() ? f->findDecays( decay, mcps ) : s_EMPTY;
}
// ===========================================================================
// find MC-decays according to decay descriptor
// ===========================================================================
LoKi::Types::MCRange LoKi::Dicts::MCFinderDicts::findDecays( const LoKi::MCFinder& f, const std::string& decay,
                                                             const LoKi::UniqueKeeper<LHCb::MCParticle>& mcps ) {
  return f.validPointer() ? f->findDecays( decay, mcps ) : s_EMPTY;
}
// ===========================================================================

// ============================================================================
// The END
// ============================================================================
