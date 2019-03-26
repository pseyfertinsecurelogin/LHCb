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
#ifndef LOKI_MCEXTRACTDICTS_H
#define LOKI_MCEXTRACTDICTS_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/MCTypes.h"
// ============================================================================
namespace LoKi {
  // ==========================================================================
  namespace MCExtract {
    // ========================================================================
    /// get MC-particles form decay tree
    LHCb::MCParticle::ConstVector mcParticles( const LHCb::MCParticle* particle, const LoKi::MCTypes::MCCuts& cuts,
                                               const bool decayOnly = true );
    // ========================================================================
    /// get MC-particles from decay tree
    LHCb::MCParticle::ConstVector mcParticles( const LHCb::MCVertex* vertex, const LoKi::MCTypes::MCCuts& cuts,
                                               const bool decayOnly = true );
    // ========================================================================
  } // namespace MCExtract
  // ==========================================================================
} // namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_MCEXTRACTDICTS_H
