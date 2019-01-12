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
#ifndef LOKI_GENEXTRACTDICTS_H
#define LOKI_GENEXTRACTDICTS_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/GenExtract.h"
#include "LoKi/GenTypes.h"
// ============================================================================
namespace LoKi
{
  namespace GenExtract
  {
    // ======================================================================
    /// extract the particles from the event
    std::vector<const HepMC::GenParticle*>
    genParticles
    ( const HepMC::GenEvent*       event ,
      const LoKi::GenTypes::GCuts& cuts  ) ;
    // ======================================================================
    /// extract the particles from the event
    std::vector<const HepMC::GenParticle*>
    genParticles
    ( const LHCb::HepMCEvent*      event ,
      const LoKi::GenTypes::GCuts& cuts  ) ;
    // ======================================================================
    /// extract the particles from the event
    std::vector<const HepMC::GenParticle*>
    genParticles
    ( const LHCb::HepMCEvents*     event ,
      const LoKi::GenTypes::GCuts& cuts  ) ;
    // ======================================================================
    /// extract the particles from the vertex
    std::vector<const HepMC::GenParticle*>
    genParticles
    ( const HepMC::GenVertex*      veretx ,
      const LoKi::GenTypes::GCuts& cuts   ,
      HepMC::IteratorRange         range = HepMC::descendants ) ;
    // ======================================================================
    /// extract the particles from the vertex
    std::vector<const HepMC::GenParticle*>
    genParticles
    ( const HepMC::GenVertex*      veretx ,
      HepMC::IteratorRange         range = HepMC::descendants ) ;
    // ======================================================================
    /// extract the particles from the particle
    std::vector<const HepMC::GenParticle*>
    genParticles
    ( const HepMC::GenParticle*    particle ,
      const LoKi::GenTypes::GCuts& cuts     ) ;
    // ======================================================================
  }
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_GENEXTRACTDICTS_H
