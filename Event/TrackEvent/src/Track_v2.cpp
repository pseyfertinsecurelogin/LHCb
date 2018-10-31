/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// Include files

#include <algorithm>
#include <array>
#include <map>
#include <string>

// from gsl
#include "gsl/gsl_cdf.h"

// local
#include "Event/TrackFitResult.h"
#include "Event/TrackFunctor.h"
#include "Event/Track_v2.h"

// ============================================================================

//-----------------------------------------------------------------------------
// Implementation file for class : Track
//
// 2004-12-14 : Jose Hernando, Eduardo Rodrigues
//-----------------------------------------------------------------------------
namespace
{
      template <typename Order1, typename Order2, typename F >
      constexpr decltype(auto) select_order( bool b, Order1 o1, Order2 o2, F&& f )
      {
          return b ? std::invoke(f,o1) : std::invoke(f,o2);
      }

      template <size_t N>
      std::array<double, N + 1> generate_chi2max( double limit )
      {
        std::array<double, N + 1> c = {0};
        std::generate( std::next( std::begin( c ) ), std::end( c ),
                       [ limit, i = 1u ]() mutable { return gsl_cdf_chisq_Qinv( limit, i++ ); } );
        return c;
      }


      // could put this into probChi2, but then the table is generated at
      // first use of probChi2, i.e. during the event loop.
      // This way, it tends to be generated when libTrackEvent.so is
      // dynamically linked into the executable, i.e. very early.
      static const auto chi2max = generate_chi2max<256>( 1e-15 );



}

namespace LHCb::Event
{
  namespace v2
  {

    namespace details {

      template <typename Enum> struct enumMap {
          static Enum const unknown;
          static GaudiUtils::VectorMap<std::string, Enum> const map;
          static std::string const s_unknown;
      };

      template  std::string const& toStringHelper( Track::History const flag );
      template  Track::History toEnumHelper<Track::History>( std::string const& aName );
      template <> const Track::History enumMap<Track::History>::unknown = Track::History::HistoryUnknown;
      template <> const std::string enumMap<Track::History>::s_unknown = "HistoryError";
      template <> const GaudiUtils::VectorMap<std::string, Track::History> enumMap<Track::History>::map = {
              {"HistoryUnknown", Track::History::HistoryUnknown},
              {"TrackIdealPR", Track::History::TrackIdealPR},
              {"TrackSeeding", Track::History::TrackSeeding},
              {"PatVelo", Track::History::PatVelo},
              {"PatVeloTT", Track::History::PatVeloTT},
              {"TrackVeloTT", Track::History::TrackVeloTT},
              {"PatForward", Track::History::PatForward},
              {"TrackMatching", Track::History::TrackMatching},
              {"PatKShort", Track::History::PatKShort},
              {"TsaTrack", Track::History::TsaTrack},
              {"MuonID", Track::History::MuonID},
              {"PatVeloR", Track::History::PatVeloR},
              {"PatVeloGeneric", Track::History::PatVeloGeneric},
              {"PatVeloGeneral", Track::History::PatVeloGeneral},
              {"PatVeloOpen", Track::History::PatVeloOpen},
              {"PatSeeding", Track::History::PatSeeding},
              {"PatMatch", Track::History::PatMatch},
              {"PatDownstream", Track::History::PatDownstream},
              {"TTGeneric", Track::History::TTGeneric},
              {"PatVPCheated", Track::History::PatVPCheated},
              {"HLTImportedTrack", Track::History::HLTImportedTrack},
              {"PatFastVelo", Track::History::PatFastVelo},
              {"PatVP", Track::History::PatVP},
              {"PatVeloUT", Track::History::PatVeloUT},
              {"PrForward", Track::History::PrForward},
              {"PrSeeding", Track::History::PrSeeding},
              {"PrMatch", Track::History::PrMatch},
              {"PrDownstream", Track::History::PrDownstream},
              {"PrVeloUT", Track::History::PrVeloUT}};


      template  std::string const& toStringHelper( Track::FitHistory const flag );
      template  Track::FitHistory toEnumHelper<Track::FitHistory>( std::string const& aName );
      template <> const Track::FitHistory enumMap<Track::FitHistory>::unknown = Track::FitHistory::FitUnknown;
      template <> const std::string enumMap<Track::FitHistory>::s_unknown = "FitUnknown";
      template <> const GaudiUtils::VectorMap<std::string, Track::FitHistory> enumMap<Track::FitHistory>::map = {
              {"FitUnknown", Track::FitHistory::FitUnknown},
              {"StdKalman", Track::FitHistory::StdKalman},
              {"BiKalman", Track::FitHistory::BiKalman}};


      template  std::string const& toStringHelper( Track::Types const flag );
      template  Track::Types toEnumHelper<Track::Types>( std::string const& aName );
      template <> const Track::Types enumMap<Track::Types>::unknown = Track::Types::TypeUnknown;
      template <> const std::string enumMap<Track::Types>::s_unknown = "TypeUnknown";
      template <> const GaudiUtils::VectorMap<std::string, Track::Types> enumMap<Track::Types>::map = {
              {"TypeUnknown", Track::Types::TypeUnknown},
              {"Velo", Track::Types::Velo},
              {"VeloR", Track::Types::VeloR},
              {"Long", Track::Types::Long},
              {"Upstream", Track::Types::Upstream},
              {"Downstream", Track::Types::Downstream},
              {"Ttrack", Track::Types::Ttrack},
              {"Muon", Track::Types::Muon},
              {"Calo", Track::Types::Calo},
              {"TT", Track::Types::TT},
              {"UT", Track::Types::UT}};


      template  std::string const& toStringHelper( Track::FitStatus const flag );
      template  Track::FitStatus toEnumHelper<Track::FitStatus>( std::string const& aName );
      template <> const Track::FitStatus enumMap<Track::FitStatus>::unknown = Track::FitStatus::FitStatusUnknown;
      template <> const std::string enumMap<Track::FitStatus>::s_unknown = "FitStatusUnknown";
      template <> const GaudiUtils::VectorMap<std::string, Track::FitStatus> enumMap<Track::FitStatus>::map = {
              {"FitStatusUnknown", Track::FitStatus::FitStatusUnknown},
              {"Fitted", Track::FitStatus::Fitted},
              {"FitFailed", Track::FitStatus::FitFailed}};

      template  std::string const& toStringHelper( Track::PatRecStatus const flag );
      template  Track::PatRecStatus toEnumHelper<Track::PatRecStatus>( std::string const& aName );
      template <> const Track::PatRecStatus enumMap<Track::PatRecStatus>::unknown = Track::PatRecStatus::PatRecStatusUnknown;
      template <> const std::string enumMap<Track::PatRecStatus>::s_unknown = "PatRecStatusUnknown";
      template <> const GaudiUtils::VectorMap<std::string, Track::PatRecStatus> enumMap<Track::PatRecStatus>::map = {
              {"PatRecStatusUnknown", Track::PatRecStatus::PatRecStatusUnknown},
              {"PatRecIDs", Track::PatRecStatus::PatRecIDs},
              {"PatRecMeas", Track::PatRecStatus::PatRecMeas}};


      template  std::string const& toStringHelper( Track::Flags const flag );
      template  Track::Flags toEnumHelper<Track::Flags>( std::string const& aName );
      template <> const Track::Flags enumMap<Track::Flags>::unknown = Track::Flags::FlagsUnknown;
      template <> std::string const enumMap<Track::Flags>::s_unknown = "FlagsUnknown";
      template <> const GaudiUtils::VectorMap<std::string, Track::Flags> enumMap<Track::Flags>::map = {
              {"FlagsUnknown", Track::Flags::FlagsUnknown},
              {"Backward", Track::Flags::Backward},
              {"Invalid", Track::Flags::Invalid},
              {"Clone", Track::Flags::Clone},
              {"Used", Track::Flags::Used},
              {"IPSelected", Track::Flags::IPSelected},
              {"PIDSelected", Track::Flags::PIDSelected},
              {"Selected", Track::Flags::Selected},
              {"L0Candidate", Track::Flags::L0Candidate}};


      template  std::string const& toStringHelper( Track::AdditionalInfo const flag );
      template  Track::AdditionalInfo toEnumHelper<Track::AdditionalInfo>( std::string const& aName );
      template <> const Track::AdditionalInfo enumMap<Track::AdditionalInfo>::unknown = Track::AdditionalInfo::AdditionalInfoUnknown;
      template <> const std::string enumMap<Track::AdditionalInfo>::s_unknown = "AdditionalInfoUnknown";
      template <> const GaudiUtils::VectorMap<std::string, Track::AdditionalInfo> enumMap<Track::AdditionalInfo>::map = {
              {"AdditionalInfoUnknown", Track::AdditionalInfo::AdditionalInfoUnknown},
              {"DC06Likelihood", Track::AdditionalInfo::DC06Likelihood},
              {"PatQuality", Track::AdditionalInfo::PatQuality},
              {"Cand1stQPat", Track::AdditionalInfo::Cand1stQPat},
              {"Cand2ndQPat", Track::AdditionalInfo::Cand2ndQPat},
              {"NCandCommonHits", Track::AdditionalInfo::NCandCommonHits},
              {"Cand1stChi2Mat", Track::AdditionalInfo::Cand1stChi2Mat},
              {"Cand2ndChi2Mat", Track::AdditionalInfo::Cand2ndChi2Mat},
              {"DC06nExpectedVelo", Track::AdditionalInfo::DC06nExpectedVelo},
              {"DC06nExpectedTT", Track::AdditionalInfo::DC06nExpectedTT},
              {"DC06nExpectedIT", Track::AdditionalInfo::DC06nExpectedIT},
              {"DC06nExpectedOT", Track::AdditionalInfo::DC06nExpectedOT},
              {"MatchChi2", Track::AdditionalInfo::MatchChi2},
              {"FitVeloChi2", Track::AdditionalInfo::FitVeloChi2},
              {"FitVeloNDoF", Track::AdditionalInfo::FitVeloNDoF},
              {"FitTChi2", Track::AdditionalInfo::FitTChi2},
              {"FitTNDoF", Track::AdditionalInfo::FitTNDoF},
              {"FitMatchChi2", Track::AdditionalInfo::FitMatchChi2},
              {"FitFracUsedOTTimes", Track::AdditionalInfo::FitFracUsedOTTimes},
              {"TsaLikelihood", Track::AdditionalInfo::TsaLikelihood},
              {"CloneDist", Track::AdditionalInfo::CloneDist},
              {"DC06GhostProbability", Track::AdditionalInfo::DC06GhostProbability},
              {"nPRVeloRZExpect", Track::AdditionalInfo::nPRVeloRZExpect},
              {"nPRVelo3DExpect", Track::AdditionalInfo::nPRVelo3DExpect},
              {"AdditionalInfo201", Track::AdditionalInfo::AdditionalInfo201},
              {"AdditionalInfo202", Track::AdditionalInfo::AdditionalInfo202},
              {"MuonChi2perDoF", Track::AdditionalInfo::MuonChi2perDoF},
              {"MuonMomentumPreSel", Track::AdditionalInfo::MuonMomentumPreSel},
              {"MuonInAcceptance", Track::AdditionalInfo::MuonInAcceptance},
              {"IsMuonLoose", Track::AdditionalInfo::IsMuonLoose},
              {"IsMuon", Track::AdditionalInfo::IsMuon},
              {"MuonDist2", Track::AdditionalInfo::MuonDist2},
              {"MuonDLL", Track::AdditionalInfo::MuonDLL},
              {"MuonNShared", Track::AdditionalInfo::MuonNShared},
              {"MuonCLQuality", Track::AdditionalInfo::MuonCLQuality},
              {"MuonCLArrival", Track::AdditionalInfo::MuonCLArrival},
              {"IsMuonTight", Track::AdditionalInfo::IsMuonTight}};


      template <typename Flag>
      std::string const& toStringHelper( Flag const flag )
      {
        auto iter = std::find_if( enumMap<Flag>::map.begin(), enumMap<Flag>::map.end(),
                                  [&]( const std::pair<std::string const, Flag>& i ) { return i.second == flag; } );
        assert( iter != enumMap<Flag>::map.end() );
        return iter != enumMap<Flag>::map.end() ? iter->first : enumMap<Flag>::s_unknown;
      }

      template <typename Enum>
      Enum toEnumHelper( std::string const& aName )
      {
        auto iter = enumMap<Enum>::map.find( aName );
        return iter != enumMap<Enum>::map.end() ? iter->second : enumMap<Enum>::unknown;
      }


    }
    //=============================================================================
    // Set the fit result. This takes ownership.
    //=============================================================================
    void Track::setFitResult( std::unique_ptr<TrackFitResult> absfit )
    {
      m_fitResult = LHCb::cxx::PolymorphicValue<TrackFitResult>{std::move( absfit )};
    }

    //=============================================================================
    // Get a range of nodes in this track
    //=============================================================================
    Track::ConstNodeRange Track::nodes() const
    {
      if ( !m_fitResult ) return {};
      const auto& nodes_ = m_fitResult->nodes();
      return {nodes_.data(), std::next( nodes_.data(), nodes_.size() )};
    }

    //=============================================================================
    // Get the measurements on the track. Note that it does not return a
    // reference. This is because I want to remove this container from
    // fitresult.
    //=============================================================================
    Track::MeasurementContainer Track::measurements() const
    {
      return m_fitResult ? MeasurementContainer{m_fitResult->measurements().begin(), m_fitResult->measurements().end()}
                         : MeasurementContainer{};
    }


    //=============================================================================
    // Retrieve the probability of chi^2
    //=============================================================================
    double Track::probChi2() const
    {
      // what to do if nDoF is bigger than the lookup table?
      // let's just do a range-checked acess, and have it throw
      // an exception... maybe not the most elegant solution...
      // alternative: chi2max[  std::min( nDoF(), int(chi2max.size() ) ]
      // in which case for unreasonable nDoF we don't go until 1e-15...
      return ( nDoF()>0 && chi2() < chi2max.at( nDoF() ) ) ? gsl_cdf_chisq_Q( chi2(), nDoF() ) : 0 ;
    }

    //=============================================================================
    // Retrieve the reference to the state closest to the given z-position
    //=============================================================================
    State& Track::closestState( double z )
    {
      auto iter = std::min_element( m_states.begin(), m_states.end(), TrackFunctor::distanceAlongZ( z ) );
      if ( iter == m_states.end() ) throw GaudiException( "No state closest to z", "Track.cpp", StatusCode::FAILURE );
      return *iter;
    }

    //=============================================================================
    // Retrieve the (const) reference to the state closest to the given z-position
    //=============================================================================
    const State& Track::closestState( double const z ) const
    {
      if ( m_fitResult && !m_fitResult->nodes().empty() ) {
        auto iter = std::min_element( m_fitResult->nodes().begin(), m_fitResult->nodes().end(),
                                      TrackFunctor::distanceAlongZ( z ) );
        if ( iter == m_fitResult->nodes().end() )
          throw GaudiException( "No state closest to z", "Track.cpp", StatusCode::FAILURE );
        return ( *iter )->state();
      } else {
        auto iter = std::min_element( m_states.begin(), m_states.end(), TrackFunctor::distanceAlongZ( z ) );
        if ( iter == m_states.end() ) throw GaudiException( "No state closest to z", "Track.cpp", StatusCode::FAILURE );
        return *iter;
      }
    }

    //=============================================================================
    // Retrieve the (const) reference to the state closest to the given plane
    //=============================================================================
    const State& Track::closestState( const Gaudi::Plane3D& plane ) const
    {
      if ( m_fitResult && !m_fitResult->nodes().empty() ) {
        auto iter = std::min_element( m_fitResult->nodes().begin(), m_fitResult->nodes().end(),
                                      TrackFunctor::distanceToPlane( plane ) );
        if ( iter == m_fitResult->nodes().end() )
          throw GaudiException( "No state closest to z", "Track.cpp", StatusCode::FAILURE );
        return ( *iter )->state();
      } else {
        auto iter = std::min_element( m_states.begin(), m_states.end(), TrackFunctor::distanceToPlane( plane ) );
        if ( iter == m_states.end() )
          throw GaudiException( "No state closest to plane", "Track.cpp", StatusCode::FAILURE );
        return *iter;
      }
    }

    //=============================================================================
    // Retrieve the pointer to the state closest to the given plane
    //=============================================================================
    State* Track::stateAt( const State::Location& location )
    {
      auto iter = std::find_if( m_states.begin(), m_states.end(),
                                [&]( const State& s ) { return s.checkLocation( location ); } );
      return iter != m_states.end() ? &( *iter ) : nullptr;
    }

    //=============================================================================
    // Retrieve the (const) pointer to the state at a given location
    //=============================================================================
    const State* Track::stateAt( const State::Location& location ) const
    {
      auto iter = std::find_if( m_states.begin(), m_states.end(),
                                [&]( const State& s ) { return s.checkLocation( location ); } );
      return iter != m_states.end() ? &( *iter ) : nullptr;
    }

    //=============================================================================
    // Add a State to the list of States associated to the Track
    //=============================================================================
    void Track::addToStates( const State& state )
    {
      auto ipos = select_order( checkFlag(Flags::Backward), TrackFunctor::decreasingByZ(), TrackFunctor::increasingByZ(),
                                [&](auto order) { return std::upper_bound( m_states.begin(), m_states.end(), state, order); } );
      m_states.emplace( ipos, state );
    }

    //=============================================================================
    // Add a list of states to the list associated to the Track.
    //=============================================================================
    void Track::addToStates( span<const State> states, Tag::Unordered_tag )
    {
      auto pivot = m_states.insert( m_states.end(), states.begin(), states.end() );
      // do not assumme that the incoming states are properly sorted.
      select_order( checkFlag(Flags::Backward), TrackFunctor::decreasingByZ(), TrackFunctor::increasingByZ(),
                    [&](auto order) {
        std::sort( pivot, m_states.end(), order );
        std::inplace_merge( m_states.begin(), pivot, m_states.end(), order );
      });
    }

    //=============================================================================
    // Add a set of sorted states by increasing Z to the track.
    //=============================================================================
    void Track::addToStates( span<const State> states, Tag::Sorted_tag )
    {
      // debug assert checking whether it's correctly sorted or not
      assert( select_order( checkFlag( Track::Flags::Backward ), TrackFunctor::decreasingByZ(),  TrackFunctor::increasingByZ(),
                            [&states](auto order) { return std::is_sorted( begin(states), end(states), order ) } ) &&
              "states are not correctly sorted;"
              "hint: use the general addToStates function assuming unordered states" );

      auto pivot = m_states.insert( m_states.end(), states.begin(), states.end() );
      select_order(  UNLIKELY( checkFlag( Track::Flags::Backward ) ), TrackFunctor::decreasingByZ(),
                                                                      TrackFunctor::increasingByZ(),
                     [&](auto order) { std::inplace_merge( m_states.begin(), pivot, m_states.end(), order ); });
    }

    //=============================================================================
    // Remove an LHCbID from the list of LHCbIDs associated to the Track
    //=============================================================================
    void Track::removeFromLhcbIDs( const LHCbID& value )
    {
      auto pos = std::lower_bound( m_lhcbIDs.begin(), m_lhcbIDs.end(), value );
      if ( pos != m_lhcbIDs.end() && *pos == value ) m_lhcbIDs.erase( pos );
    }

    //=============================================================================
    // Add LHCbIDs to track
    //=============================================================================
    bool Track::addToLhcbIDs( const LHCbID& value )
    {
      auto       pos = std::lower_bound( m_lhcbIDs.begin(), m_lhcbIDs.end(), value );
      const bool rc  = ( pos == m_lhcbIDs.end() ) || !( *pos == value );
      if ( rc ) m_lhcbIDs.insert( pos, value );
      return rc;
    }

    //=============================================================================
    // Add LHCbIDs to track, specify Track::IDsSorted{} or Track::IDsUnordered{}
    //=============================================================================
    template <typename S,
              typename = std::enable_if_t<std::is_same_v<S, Tag::Sorted_tag> || std::is_same_v<S, Tag::Unordered_tag>>>
    bool Track::addToLhcbIDs( span<LHCbID const> ids, S /**/ )
    {
      // check that original list is sorted and contained only unique elements
      if constexpr( std::is_same_v<S, Tag::Sorted_tag> )
        {
          assert( std::is_sorted( ids.begin(), ids.end() ) );
          assert( std::adjacent_find( ids.begin(), ids.end() ) == ids.end() );
        }

      auto pivot    = m_lhcbIDs.insert( end( m_lhcbIDs ), std::begin( ids ), std::end( ids ) );
      auto org_size = m_lhcbIDs.size();

      if constexpr( std::is_same_v<S, Tag::Unordered_tag> )
        {
          std::sort( pivot, end( m_lhcbIDs ) );
          // check that original list contained only unique elements
          assert( std::adjacent_find( pivot, end( m_lhcbIDs ) ) == end( m_lhcbIDs ) );
        }

      std::inplace_merge( begin( m_lhcbIDs ), pivot, end( m_lhcbIDs ) );
      m_lhcbIDs.erase( unique( begin( m_lhcbIDs ), end( m_lhcbIDs ) ), end( m_lhcbIDs ) );
      auto rc = ( m_lhcbIDs.size() == ids.size() + org_size );
      return rc;
    }

    template bool Track::addToLhcbIDs( span<LHCbID const> ids, Tag::Sorted_tag /**/ );
    template bool Track::addToLhcbIDs( span<LHCbID const> ids, Tag::Unordered_tag /**/ );

    //=============================================================================
    // Count the number of LHCbIDs that two tracks have in common
    //=============================================================================
    namespace
    {

      // inserter which only counts how often something is inserted
      struct counting_inserter {
        size_t count = 0;
        counting_inserter& operator++() { return *this; } // nop
        counting_inserter& operator*() { return *this; }  // redirect to self, so that our op= is called
        counting_inserter& operator=( const LHCbID& )
        {
          ++count;
          return *this;
        } // raison d'etre
      };
    }

    size_t Track::nCommonLhcbIDs( const Track& rhs ) const
    {
      return std::set_intersection( begin( m_lhcbIDs ), end( m_lhcbIDs ), begin( rhs.m_lhcbIDs ), end( rhs.m_lhcbIDs ),
                                    counting_inserter{} )
          .count;
    }

    //=============================================================================
    // Check whether the given LHCbID is on the Track
    //=============================================================================
    bool Track::isOnTrack( LHCbID const value ) const
    {
      auto pos = std::lower_bound( m_lhcbIDs.begin(), m_lhcbIDs.end(), value );
      return pos != m_lhcbIDs.end() && *pos == value;
    }

    //=============================================================================
    /** Add new information, associated with the specified key.
     *  This method cannot be used to modify information for an already existing key
     *
     *  @code
     *
     *  Track* p = ... ;
     *
     *  Track::Key  key   = ... ;
     *  Track::Info info  = ... ;
     *
     *  bool inserted = p->addInfo( key , info ) ;
     *
     *  @endcode
     *
     *  @param key key for the information
     *  @param info information to be associated with the key
     *  @return 'true' if information is inserted,
     *         'false' if information was not inserted, due to already existing key
     */
    //=============================================================================
    bool Track::addInfo( AdditionalInfo const key, double const info )
    {
      return m_extraInfo.insert( static_cast<int>( key ), info ).second;
    }

    //=============================================================================
    /** extract the information associated with the given key
     *  If there is no such infomration the default value will
     *  be returned
     *
     *  @code
     *
     *  const Track* p = ... ;
     *
     *  Track::Key  key   = ... ;
     *
     *  // extract the information
     *  Track::Info info = p->info( key, -999 ) ;
     *
     *  @endcode
     *
     *  @param key key for the information
     *  @param def the default value to be returned
     *         in the case of missing info
     *  @return information associated with the key if there
     *          is such information, the default value otherwise
     */
    //=============================================================================
    double Track::info( AdditionalInfo const key, double const def ) const
    {
      auto i = m_extraInfo.find( static_cast<int>( key ) );
      return m_extraInfo.end() == i ? def : i->second;
    }

    void Track::removeFromAncestors( const Track* value )
    {
      auto i = std::remove( m_ancestors.begin(), m_ancestors.end(), value );
      m_ancestors.erase( i, m_ancestors.end() );
    }

    int Track::charge() const
    {
      double qP = firstState().qOverP();
      return UNLIKELY( std::abs( qP ) < TrackParameters::lowTolerance ) ? 0 : qP < 0 ? -1 : +1;
    }

    void Track::setQOverPInAllStates( float const qop )
    {
      for ( auto& state : m_states ) state.setQOverP( qop );
    }

    void Track::setQOverPAndErrInAllStates( float const qop, float const err )
    {
      for ( auto& state : m_states ) {
        state.setQOverP( qop );
        state.setErrQOverP2( err );
      }
    }

    void Track::setLhcbIDs( span<LHCbID const> ids, Tag::Unordered_tag )
    {
      m_lhcbIDs.assign( ids.begin(), ids.end() );
      std::sort( m_lhcbIDs.begin(), m_lhcbIDs.end() );
      assert( std::adjacent_find( m_lhcbIDs.begin(), m_lhcbIDs.end() ) == m_lhcbIDs.end() );
    }

    void Track::setLhcbIDs( span<LHCbID const> ids, Tag::Sorted_tag )
    {
      assert( std::is_sorted( ids.begin(), ids.end() ) );
      assert( std::adjacent_find( ids.begin(), ids.end() ) == ids.end() );

      m_lhcbIDs.assign( ids.begin(), ids.end() );
    }

    void Track::setLhcbIDs( LHCbIDContainer&& value, Tag::Sorted_tag )
    {
      m_lhcbIDs = std::move( value );
      assert( std::is_sorted( begin( m_lhcbIDs ), end( m_lhcbIDs ) ) );
      assert( std::adjacent_find( m_lhcbIDs.begin(), m_lhcbIDs.end() ) == m_lhcbIDs.end() );
    }

    bool Track::containsLhcbIDs( Track const& track ) const { return containsLhcbIDs( track.lhcbIDs() ); }

    bool Track::containsLhcbIDs( span<LHCbID const> ids ) const
    {
      return std::includes( m_lhcbIDs.begin(), m_lhcbIDs.end(), ids.begin(), ids.end() );
    }

    void Track::setFlag( Flags const flag, bool const ok )
    {
      uint32_t val = ( ( static_cast<uint32_t>( flag ) ) << details::trailing_zeros( flagsMasks::flagMask ) ) &
                     flagsMasks::flagMask;
      if ( ok )
        m_flags |= val;
      else
        m_flags &= ~val;
    }

    bool Track::checkFlag( Flags const flag ) const
    {
      uint32_t val = ( static_cast<uint32_t>( flag ) << details::trailing_zeros( flagsMasks::flagMask ) );
      return ( 0 != ( m_flags & flagsMasks::flagMask & val ) );
    }

    bool Track::hasT() const
    {
      auto const t = type();
      return t == Types::Ttrack || t == Types::Downstream || t == Types::Long;
    }

    bool Track::hasVelo() const
    {
      auto const t = type();
      return t == Types::Velo || t == Types::VeloR || t == Types::Upstream || t == Types::Long;
    }

    bool Track::hasTT() const
    {
      auto const t = type();
      return t == Types::Downstream || t == Types::Upstream || t == Types::Long;
    }

    bool Track::hasUT() const
    {
      auto const t = type();
      return t == Types::Downstream || t == Types::Upstream || t == Types::Long;
    }

    //=============================================================================
    // Fill stream
    //=============================================================================
    std::ostream& Track::fillStream( std::ostream& os ) const
    {
      os << "*** Track ***"
         << "\n type       : " << type()
         << "\n history    : " << history()
         << "\n fitstatus  : " << fitStatus()
         << "\n # ids      : " << nLHCbIDs()
         << "\n # meas     : " << nMeasurements()
         << "\n chi2PerDoF : " << (float)chi2PerDoF()
         << "\n nDoF       : " << nDoF()
         << "\n GhostProb  : " << ghostProbability()
         << "\n Likelihood : " << likelihood();

      os << "\n extraInfo : [";
      for ( const auto& i : extraInfo() ) {
        const Track::AdditionalInfo info = static_cast<Track::AdditionalInfo>( i.first );
        os << " " << info << "=" << i.second << " ";
      }
      os << "]\n" ;

      if ( !m_states.empty() ) {
        os << " p  : " << (float)firstState().p()
           << "\n pt : " << (float)firstState().pt()
           << "\n " << nStates() << " states at z =";
        for ( const auto& s : states() ) {
          os << " " << s.z();
        }
        os << "  :-\n" ;
        for ( const auto& s : states() ) {
          os << " " << s;
        }
        os << '\n';
      } else {
        os << " # states : " << nStates() << '\n';
      }

      return os;
    }

  }
}
