
#pragma once

#include <functional>
#include <string>
#include <tuple>

#include "Event/MCParticle.h"
// Event
#include "Event/Track.h"
// Gaudi Functional
#include "GaudiAlg/Transformer.h"
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/System.h"
// Linkers
#include "Linker/LinkedTo.h"
// Rich (Future) Kernel
#include "RichFutureKernel/RichAlgBase.h"
// Relations
#include "RichFutureMCUtils/RichMCRelations.h"

class ISvcLocator;
namespace Gaudi {
namespace Functional {
namespace Traits {
template <typename Base> struct BaseClass_t;
}  // namespace Traits
}  // namespace Functional
}  // namespace Gaudi
namespace Rich {
namespace Future {
class AlgBase;
}  // namespace Future
}  // namespace Rich

namespace Rich
{
  namespace Future
  {
    namespace MC
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
  }
}

