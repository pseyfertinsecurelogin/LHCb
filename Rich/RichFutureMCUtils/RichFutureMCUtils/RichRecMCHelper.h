
#pragma once

// local
#include "RichFutureMCUtils/TrackToMCParticle.h"
#include "RichFutureMCUtils/RichSmartIDMCUtils.h"

// Rich Utils
#include "RichUtils/RichPixelCluster.h"
#include "RichUtils/RichTrackSegment.h"

namespace Rich
{
  namespace Future
  {
    namespace Rec
    {
      namespace MC
      {

        using namespace Rich::Future::MC::Relations;
        
        /// Utility class to provide various Rec MC utilities
        class Helper : public SmartIDUtils,
                       public TrackToMCParticle
        {

        public:

          /// Contructor from the underlying MC relations
          Helper( const TkToMCPRels & rels,
                  const LHCb::MCRichDigitSummarys & histories )
            : SmartIDUtils(histories), TrackToMCParticle(rels) { }
          
        public:

          using TrackToMCParticle::mcParticles;
          using SmartIDUtils::mcParticles;

        public:
          
          /// Access the common MCParticle(s) for the given track and RICH hit
          LHCb::MCParticle::ConstVector 
          trueRecPhoton( const LHCb::Track & track,
                         const Rich::PDPixelCluster& cluster ) const;
          
          /// Returns a valid pointer if the given MCParticle produced (part) of the given cluster
          const LHCb::MCParticle * 
          trueRecPhoton( const LHCb::MCParticle & mcPart,
                         const Rich::PDPixelCluster& cluster ) const;
          
          /// Returns a valid pointer if the given MCParticle produced the given channel ID
          const LHCb::MCParticle * 
          trueRecPhoton( const LHCb::MCParticle & mcPart,
                         const LHCb::RichSmartID id ) const;
          
          /// If a true Cherenkov photon, returns the parent MCParticle(s)
          LHCb::MCParticle::ConstVector 
          trueCherenkovPhoton( const LHCb::Track & track,
                               const Rich::RadiatorType rad,
                               const Rich::PDPixelCluster& cluster ) const;
          
        };

      }
    }
  }
}
