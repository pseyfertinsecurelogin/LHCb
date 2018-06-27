#pragma once
#include "Event/State.h"
#include "Kernel/LHCbID.h"
#include "Kernel/STLExtensions.h"
#include <boost/container/small_vector.hpp>
#include <vector>

namespace experimental
{
  class TrackAfterFit;
  using TrackAfterFits = std::vector<TrackAfterFit>;

  class TrackAfterFit
  {
  public:
    // FIXME: very initial

    TrackAfterFit();

    friend LHCb::Converters::TrackAfterFit::experimental;

    struct fromVectorLHCbTrack private : LHCb::State m_physicsstate;
    LHCb::Track::Type                                m_type;
    float                                            m_chisqperdof;
    int                                              m_dof;
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
    };

    class AdditionalStates
    {
    private:
      LHCb::State              m_BegRich1;
      LHCb::State              m_EndRich1;
      LHCb::State              m_BegRich2;
      std::vector<LHCb::State> m_moreStates;

    public:
      AdditionalStates( LHCb::States::const_iterator it1, LHCb::States::const_iterator it2 );
    };
  } // namespace TrackAfterFitExtension
} // namespace experimental
