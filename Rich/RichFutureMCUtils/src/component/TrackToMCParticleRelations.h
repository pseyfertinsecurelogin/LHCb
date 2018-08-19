
#pragma once

// Rich (Future) Kernel
#include "RichFutureKernel/RichAlgBase.h"

// Gaudi Functional
#include "GaudiAlg/Transformer.h"

// Event
#include "Event/Track.h"
#include "Event/MCParticle.h"

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
  class TrackToMCParticleRelations final :
    public Transformer< Relations::TkToMCPRels( const LHCb::Tracks&,
                                                const LHCb::MCParticles& ),
    Traits::BaseClass_t<AlgBase> >
  {

  public:

    /// Standard constructor
    TrackToMCParticleRelations( const std::string& name, ISvcLocator* pSvcLocator );

    /// Algorithm execution via transform
    Rich::Future::MC::Relations::TkToMCPRels
      operator()( const LHCb::Tracks& tks,
                  const LHCb::MCParticles& mcps ) const override;

  private:

    /// Allow more than one MCParticle per track ?
    Gaudi::Property<bool> m_allowMultMPs { this, "AllowMultipleMCPsPerTrack", true };

  };

}

