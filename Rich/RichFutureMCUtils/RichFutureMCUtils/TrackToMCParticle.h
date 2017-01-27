
#pragma once

// local
#include "RichFutureMCUtils/RichMCRelations.h"

// Kernel
#include "Kernel/RichParticleIDType.h"

// Event model
#include "Event/Track.h"
#include "Event/MCParticle.h"

namespace Rich
{
  namespace Future
  {
    namespace MC
    {
      namespace Relations
      {

        /// Helper for Track->MCParticle relations
        class TrackToMCParticle
        {

        public:

          /// Constructor from a relations table
          TrackToMCParticle( const TkToMCPRels & rels ) : m_rels(rels) { }

        public:
          
          /// Access the best MCParticle match for the given track
          const LHCb::MCParticle *
          mcParticle( const LHCb::Track & track, const double minWeight = 0.5 ) const;

          /// Get the MC RICH PID type for a given track
          Rich::ParticleIDType
          mcParticleType( const LHCb::Track & track,
                          const double minWeight = 0.5 ) const;
          
        private:
        
          /// reference to the relations
          const TkToMCPRels & m_rels;
        
        };

      }
    }
  }
}
