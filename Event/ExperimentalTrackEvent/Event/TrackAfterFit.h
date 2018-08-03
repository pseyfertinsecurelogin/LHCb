#pragma once
#include "Event/ExperimentalTags.h"
#include "Event/State.h"
#include "Event/Track.h"
#include "Kernel/LHCbID.h"
#include "Kernel/STLExtensions.h"
#include <boost/container/small_vector.hpp>
#include <vector>

#include "SOAContainer/SOAContainer.h"

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
    } // namespace TrackAfterFit
  }   // namespace Converters
} // namespace LHCb

namespace experimental
{
  class TrackAfterFit;
  using TrackAfterFits = std::vector<TrackAfterFit>; // CRJ : Grammatically, this should be TracksAfterFit

  class TrackAfterFit
  {
  public:
    // FIXME: very initial

    friend LHCb::Converters::TrackAfterFit::experimental::fromLHCbTrack;

    LHCb::State m_physicsstate;
    LHCb::Track::Types m_type;
    float m_chisqperdof{0};
    int m_dof{0};
  };
  namespace TrackAfterFitExtension
  {
    class Hits;
    class Rich1States;
    class Rich2States;
    class AdditionalStates;
    class BackCompat;

    using Rich1Statess      = std::vector<Rich1States>;
    using Rich2Statess      = std::vector<Rich2States>;
    using AdditionalStatess = std::vector<AdditionalStates>;
    using Hitss             = std::vector<Hits>;
    using BackCompats       = std::vector<BackCompat>;

    class Hits
    {
    private:
      // FIXME: separate containers for different containers?
      // FIXME: proper size
      boost::container::small_vector<LHCb::LHCbID, 30> m_hits; // CRJ : better than just std::array ?

    public:
      Hits( const std::vector<LHCb::LHCbID>& );
      Hits( const LHCb::span<const LHCb::LHCbID> );
      friend LHCb::Converters::TrackAfterFit::experimental::fromLHCbTrack;
    };

    class Rich1States
    {
    private:
      LHCb::State m_BegRich1;
      LHCb::State m_EndRich1;

    public:
      Rich1States( std::vector<LHCb::State>& states );
      Rich1States( LHCb::State&& Beg, LHCb::State&& End );
      friend LHCb::Converters::TrackAfterFit::experimental::fromLHCbTrack;
    };

    class Rich2States
    {
    private:
      LHCb::State m_BegRich2;

    public:
      Rich2States( std::vector<LHCb::State>& states );
      Rich2States( LHCb::State&& Beg );
      friend LHCb::Converters::TrackAfterFit::experimental::fromLHCbTrack;
    };

    class AdditionalStates
    {
    private:
      std::vector<LHCb::State> m_moreStates;

    public:
      AdditionalStates( std::vector<LHCb::State>&& states, experimental::Tag::State::AssumeUnfiltered_tag = {} );
      AdditionalStates( std::vector<LHCb::State>&& states, experimental::Tag::State::AssumeFiltered_tag );
      friend LHCb::Converters::TrackAfterFit::experimental::fromLHCbTrack;
    };

    class BackCompat
    {
    private:
      const LHCb::Track* m_origin = nullptr;

    public:
      BackCompat( const LHCb::Track* t ) : m_origin( t ) {}
    };
  } // namespace TrackAfterFitExtension

  namespace TrackData
  {
    SOAFIELD_TRIVIAL( f_core, core, TrackAfterFit);
    SOAFIELD_TRIVIAL( f_rich2, rich2, TrackAfterFitExtension::Rich2States);
    SOAFIELD_TRIVIAL( f_rich1, rich1, TrackAfterFitExtension::Rich1States);
    SOAFIELD_TRIVIAL( f_moreStates, moreStates, TrackAfterFitExtension::AdditionalStates);
    SOAFIELD_TRIVIAL( f_origin, origin, TrackAfterFitExtension::BackCompat);

    SOASKIN( VelotrackSkin, f_core, f_rich1, f_moreStates, f_origin )
    {
      SOASKIN_INHERIT_DEFAULT_METHODS( VelotrackSkin );
      //
      // TODO very very important constructors!!!
    };

    SOASKIN( UpstreamtrackSkin, f_core, f_rich1, f_moreStates, f_origin )
    {
      SOASKIN_INHERIT_DEFAULT_METHODS( UpstreamtrackSkin );

      // TODO very very important constructors!!!
    };

    SOASKIN( LongtrackSkin, f_core, f_rich1, f_rich2, f_moreStates, f_origin )
    {
      SOASKIN_INHERIT_DEFAULT_METHODS( LongtrackSkin );

        /// TODO: better to handle this by an external converter which avoids initializing the members
      LongtrackSkin( const LHCb::Track& t )
      {
        this->core().m_physicsstate = t.firstState();
        this->core().m_type         = t.type();
        this->core().m_chisqperdof  = t.chi2PerDoF();
        this->core().m_dof          = t.nDoF();

        std::vector<LHCb::State> states;
        states.reserve( t.states().size() );
        for ( auto s : t.states() ) {
          states.emplace_back( *s );
        }

        this->rich1()      = TrackAfterFitExtension::Rich1States( states );
        this->rich2()      = TrackAfterFitExtension::Rich2States( states );
        this->moreStates() = TrackAfterFitExtension::AdditionalStates( std::move( states ) );

        this->origin() = &t;
      }
      // TODO very very important constructors!!!
    };
    SOASKIN( DowntrackSkin, f_core, f_rich1, f_rich2, f_moreStates, f_origin )
    {
      SOASKIN_INHERIT_DEFAULT_METHODS( DowntrackSkin );

      // TODO very very important constructors!!!
    };
    SOASKIN( SeedtrackSkin, f_core, f_rich1, f_rich2, f_moreStates, f_origin )
    {
      SOASKIN_INHERIT_DEFAULT_METHODS( SeedtrackSkin );

      // TODO very very important constructors!!!
    };
  }
  typedef SOA::Container<std::vector, TrackData::LongtrackSkin> Longtracks;
  typedef typename Longtracks::proxy Longtrack;
  typedef SOA::Container<std::vector, TrackData::VelotrackSkin> Velotracks;
  typedef typename Velotracks::proxy Velotrack;
  typedef SOA::Container<std::vector, TrackData::DowntrackSkin> Downtracks;
  typedef typename Downtracks::proxy Downtrack;
  typedef SOA::Container<std::vector, TrackData::SeedtrackSkin> Seedtracks;
  typedef typename Seedtracks::proxy Seedtrack;
  typedef SOA::Container<std::vector, TrackData::UpstreamtrackSkin> Upstreamtracks;
  typedef typename Upstreamtracks::proxy Upstreamtrack;
} // namespace experimental
