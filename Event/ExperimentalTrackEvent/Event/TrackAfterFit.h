#pragma once
#include "Event/State.h"
#include "Kernel/LHCbID.h"
#include <boost/container/small_vector.hpp>
#include <gsl/span>

namespace experimental
{
  class TrackAfterFit
  {
  public:
    // FIXME: very initial
    // FIXME: separate c++ types for different track types?
    enum class Type {
      Velo,
      Long,
      Downstream,
      Upstream,
      TTrack,
    };

  private:
    LHCb::State m_physicsstate;
    Type        m_type;
  };
  namespace TrackAfterFitExtension
  {
    class Hits
    {
    private:
      // FIXME: separate containers for different containers?
      // FIXME: proper size
      boost::container::small_vector<LHCb::LHCbID, 30> m_hits;
    public:
      Hits(const std::vector<LHCb::LHCbID>&);
      Hits(const span<const LHCbID>);
    };

    class AdditionalStates
    {
      LHCb::State              m_BegRich1;
      LHCb::State              m_EndRich1;
      LHCb::State              m_BegRich2;
      std::vector<LHCb::State> m_moreStates;
    };
  } // namespace TrackAfterFitExtension
} // namespace experimental
