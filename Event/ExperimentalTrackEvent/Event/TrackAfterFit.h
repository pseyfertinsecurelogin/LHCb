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

  namespace Velotrack
  {
    SOAFIELD( core, TrackAfterFit, SOAFIELD_ACCESSORS( core )
              // core methods
    SOAFIELD( rich1, TrackAfterFitExtension::Rich1States, SOAFIELD_ACCESSORS( rich1 )
              // rich1 access methods
    );
    SOAFIELD( moreStates, TrackAfterFitExtension::AdditionalStates, SOAFIELD_ACCESSORS( moreStates )
              // moreStates access methods
    );
    SOAFIELD( origin, TrackAfterFitExtension::BackCompat, SOAFIELD_ACCESSORS( origin )
              // backward compatibility methods
    );
    SOASKIN(VelotrackSkin, core, rich1, moreStates, origin) {
      SOASKIN_INHERIT_DEFAULT_METHODS( VelotrackSkin );

      // TODO very very important constructors!!!

    };
  }

  namespace Upstreamtrack
  {
    SOAFIELD( core, TrackAfterFit, SOAFIELD_ACCESSORS( core )
              // core methods
    SOAFIELD( rich1, TrackAfterFitExtension::Rich1States, SOAFIELD_ACCESSORS( rich1 )
              // rich1 access methods
    );
    SOAFIELD( moreStates, TrackAfterFitExtension::AdditionalStates, SOAFIELD_ACCESSORS( moreStates )
              // moreStates access methods
    );
    SOAFIELD( origin, TrackAfterFitExtension::BackCompat, SOAFIELD_ACCESSORS( origin )
              // backward compatibility methods
    );
    SOASKIN(UpstreamtrackSkin, core, rich1, moreStates, origin) {
      SOASKIN_INHERIT_DEFAULT_METHODS( UpstreamtrackSkin );

      // TODO very very important constructors!!!

    };
  }

  namespace LongtrackData
  {
    SOAFIELD( core, TrackAfterFit, SOAFIELD_ACCESSORS( core )
              // core methods
    SOAFIELD( rich1, TrackAfterFitExtension::Rich1States, SOAFIELD_ACCESSORS( rich1 )
              // rich1 access methods
    );
    SOAFIELD( rich2, TrackAfterFitExtension::Rich2States, SOAFIELD_ACCESSORS( rich2 )
              // rich2 access methods
    );
    SOAFIELD( moreStates, TrackAfterFitExtension::AdditionalStates, SOAFIELD_ACCESSORS( moreStates )
              // moreStates access methods
    );
    SOAFIELD( origin, TrackAfterFitExtension::BackCompat, SOAFIELD_ACCESSORS( origin )
              // backward compatibility methods
    );
    SOASKIN(LongtrackSkin, core, rich1, rich2, moreStates, origin) {
      SOASKIN_INHERIT_DEFAULT_METHODS( LongtrackSkin );

      LongtrackSkin( const LHCb::Track& t )
      {
        core.m_physicsstate = t.firstState();
        core.m_type         = t.type();
        core.m_chisqperdof  = t.chi2PerDoF();
        core.m_dof          = t.nDoF();

        std::vector<LHCb::State> states;
        states.reserve( t.states().size() );
        for ( auto s : t.states() ) {
          states.emplace_back( *s );
        }

        rich1      = TrackAfterFitExtension::Rich1States( states );
        rich2      = TrackAfterFitExtension::Rich12tates( states );
        moreStates = TrackAfterFitExtension::AdditionalStates( std::move( states ) );

        origin = &t;
      }
      // TODO very very important constructors!!!

    };
  }
  typedef SOA::Container<std::vector, LongtrackData::LongtrackSkin> Longtracks;
  typedef typename Longtracks::proxy Longtrack;

  namespace Downtrack
  {
    SOAFIELD( core, TrackAfterFit, SOAFIELD_ACCESSORS( core )
              // core methods
    SOAFIELD( rich1, TrackAfterFitExtension::Rich1States, SOAFIELD_ACCESSORS( rich1 )
              // rich1 access methods
    );
    SOAFIELD( rich2, TrackAfterFitExtension::Rich2States, SOAFIELD_ACCESSORS( rich2 )
              // rich2 access methods
    );
    SOAFIELD( moreStates, TrackAfterFitExtension::AdditionalStates, SOAFIELD_ACCESSORS( moreStates )
              // moreStates access methods
    );
    SOAFIELD( origin, TrackAfterFitExtension::BackCompat, SOAFIELD_ACCESSORS( origin )
              // backward compatibility methods
    );
    SOASKIN(DowntrackSkin, core, rich1, rich2, moreStates, origin) {
      SOASKIN_INHERIT_DEFAULT_METHODS( DowntrackSkin );

      // TODO very very important constructors!!!

    };
  }

  namespace Seedtrack
  {
    SOAFIELD( core, TrackAfterFit, SOAFIELD_ACCESSORS( core )
              // core methods
    SOAFIELD( rich1, TrackAfterFitExtension::Rich1States, SOAFIELD_ACCESSORS( rich1 )
              // rich1 access methods
    );
    SOAFIELD( rich2, TrackAfterFitExtension::Rich2States, SOAFIELD_ACCESSORS( rich2 )
              // rich2 access methods
    );
    SOAFIELD( moreStates, TrackAfterFitExtension::AdditionalStates, SOAFIELD_ACCESSORS( moreStates )
              // moreStates access methods
    );
    SOAFIELD( origin, TrackAfterFitExtension::BackCompat, SOAFIELD_ACCESSORS( origin )
              // backward compatibility methods
    );
    SOASKIN(SeedtrackSkin, core, rich1, rich2, moreStates, origin) {
      SOASKIN_INHERIT_DEFAULT_METHODS( SeedtrackSkin );

      // TODO very very important constructors!!!

    };
  }
} // namespace experimental
