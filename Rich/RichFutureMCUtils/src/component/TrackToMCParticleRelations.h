
#pragma once

// Rich (Future) Kernel
#include "RichFutureKernel/RichAlgBase.h"

// Gaudi Functional
#include "GaudiAlg/Transformer.h"

// Event
#include "Event/Track.h"
#include "Event/MCParticle.h"

// Relations 
//#include "Relations/IRelationWeighted.h" 
//#include "Relations/RelationWeighted.h" 
#include "Relations/IRelationWeighted2D.h" 
#include "Relations/RelationWeighted2D.h"

// Linkers
#include "Linker/LinkedTo.h"

// Relations locations
#include "RichFutureMCUtils/RichMCRelationsLocations.h"

namespace Rich
{
  namespace Future
  {
    namespace MC
    {
      
      // Use the functional framework
      using namespace Gaudi::Functional;

      namespace
      {
        /// The relations type
        using TkToMCPRels = LHCb::RelationWeighted2D<LHCb::Track,LHCb::MCParticle,double>;
      }

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
        public Transformer< TkToMCPRels( const LHCb::Tracks&, 
                                         const LHCb::MCParticles& ),
                            Traits::BaseClass_t<AlgBase> >
      {
        
      public:

        /// Standard constructor
        TrackToMCParticleRelations( const std::string& name, ISvcLocator* pSvcLocator );
        
        /// Algorithm execution via transform
        TkToMCPRels operator()( const LHCb::Tracks& tks, 
                                const LHCb::MCParticles& mcps ) const override;
        
      };

    }
  }
}

