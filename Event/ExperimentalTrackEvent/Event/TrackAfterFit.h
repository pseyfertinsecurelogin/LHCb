#pragma once
#include "Event/State.h"
#include "Event/Track.h"
#include "Kernel/LHCbID.h"
#include "Kernel/STLExtensions.h"
#include <boost/container/small_vector.hpp>
#include <vector>

namespace LHCb
{
  namespace Converters
  {
    namespace TrackAfterFit
    {
      namespace experimental
      {
        struct fromLHCbTrack;
      }
    }
  }
}

namespace experimental
{
  class TrackAfterFit;
  using TrackAfterFits = std::vector<TrackAfterFit>;

  class TrackAfterFit
  {
  public:
    // FIXME: very initial

    TrackAfterFit();

    friend LHCb::Converters::TrackAfterFit::experimental::fromLHCbTrack;

    LHCb::State m_physicsstate;
    LHCb::Track::Types m_type;
    float m_chisqperdof;
    int m_dof;
  };
  namespace TrackAfterFitExtension
  {
    class Hits;
    class AdditionalStates;

    using AdditionalStatess = std::vector<AdditionalStates>;
    using Hitss             = std::vector<Hits>;

    class Hits
    {
    private:
      // FIXME: separate containers for different containers?
      // FIXME: proper size
      boost::container::small_vector<LHCb::LHCbID, 30> m_hits;

    public:
      Hits( const std::vector<LHCb::LHCbID>& );
      Hits( const LHCb::span<const LHCb::LHCbID> );
      friend LHCb::Converters::TrackAfterFit::experimental::fromLHCbTrack;
    };

    class AdditionalStates
    {
    private:
      LHCb::State m_BegRich1;
      LHCb::State m_EndRich1;
      LHCb::State m_BegRich2;
      std::vector<LHCb::State> m_moreStates;

    public:
      AdditionalStates( decltype( std::declval<LHCb::Track>().states().begin() ) it1,
                        decltype( std::declval<LHCb::Track>().states().begin() ) it2 );
      friend LHCb::Converters::TrackAfterFit::experimental::fromLHCbTrack;
    };
  } // namespace TrackAfterFitExtension
} // namespace experimental
