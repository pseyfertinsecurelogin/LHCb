/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/

#pragma once

// Rich (Future) Kernel
#include "RichFutureKernel/RichAlgBase.h"

// Gaudi Functional
#include "GaudiAlg/Transformer.h"

// Event
#include "Event/MCParticle.h"
#include "Event/Track.h"

// Linkers
#include "Linker/LinkedTo.h"

// Relations
#include "RichFutureMCUtils/RichMCRelations.h"

namespace Rich::Future::MC
{

  // Use the functional framework
  using namespace Gaudi::Functional;

  /** @class TrackToMCParticleRelations TrackToMCParticleRelations.h
   *
   *  (Temporary) algorithm that  takes the Track<->MCParticle linker and forms
   *  a relations object from it. Temporary in that the linker object as it stands
   *  is not compatible with the functional framework, given the way it internal
   *  interacts with the TES. Longer term the usage of the linkers in the future
   *  algorithms needs reviewing, and most probably removing.
   *
   *  @author Chris Jones
   *  @date   2016-12-07
   */
  class TrackToMCParticleRelations final
    : public Transformer< Relations::TkToMCPRels( const LHCb::Tracks &, const LHCb::MCParticles & ),
                          Traits::BaseClass_t< AlgBase > >
  {

  public:

    /// Standard constructor
    TrackToMCParticleRelations( const std::string &name, ISvcLocator *pSvcLocator );

    /// Algorithm execution via transform
    Rich::Future::MC::Relations::TkToMCPRels
    operator()( const LHCb::Tracks &tks, const LHCb::MCParticles &mcps ) const override;

  private:

    /// Allow more than one MCParticle per track ?
    Gaudi::Property< bool > m_allowMultMPs { this, "AllowMultipleMCPsPerTrack", true };
  };

} // namespace Rich::Future::MC
