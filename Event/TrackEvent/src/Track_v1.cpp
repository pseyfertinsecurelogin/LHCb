/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
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
#include "Event/TrackFunctor.h"
#include "Event/Track_v1.h"
#include "GaudiKernel/TaggedBool.h"

// ============================================================================

//-----------------------------------------------------------------------------
// Implementation file for class : Track
//
// 2004-12-14 : Jose Hernando, Eduardo Rodrigues
//-----------------------------------------------------------------------------
namespace {
  template <typename T1, typename T2, typename F>
  constexpr decltype( auto ) with_selection_invoke( bool b, T1&& t1, T2&& t2, F&& f ) {
    return b ? std::invoke( std::forward<F>( f ), std::forward<T1>( t1 ) )
             : std::invoke( std::forward<F>( f ), std::forward<T2>( t2 ) );
  }

  using useDecreasingOrder = Gaudi::tagged_bool<struct useDecreasingOrder_tag>;

  template <typename F>
  constexpr decltype( auto ) with_order( useDecreasingOrder decreasing, F&& f ) {
    return with_selection_invoke( UNLIKELY( static_cast<bool>( decreasing ) ), TrackFunctor::decreasingByZ(),
                                  TrackFunctor::increasingByZ(), std::forward<F>( f ) );
  }
} // namespace
namespace LHCb::Event {
  inline namespace v1 {
    //=============================================================================
    // Retrieve a pointer to the fit result
    //=============================================================================
    const ITrackFitResult* Track::fitResult() const { return m_fitResult.get(); }

    //=============================================================================
    // Retrieve a pointer to the fit result
    //=============================================================================
    ITrackFitResult* Track::fitResult() { return m_fitResult.get(); }

    //=============================================================================
    // Set the fit result. This takes ownership.
    //=============================================================================
    void Track::setFitResult( ITrackFitResult* absfit ) {
      if ( m_fitResult.get() != absfit ) m_fitResult.reset( absfit );
    }

    //=============================================================================
    // Move assignement operator
    //=============================================================================
    Track& Track::operator=( Track&& track ) {
      setChi2PerDoF( track.chi2PerDoF() );
      setNDoF( track.nDoF() );
      setLikelihood( track.likelihood() );
      setGhostProbability( track.ghostProbability() );
      setFlags( track.flags() );
      m_lhcbIDs = std::move( track.m_lhcbIDs );

      // "copy" the states, avoiding real copy
      swap( m_states, track.m_states );

      // copy the track fit info
      m_fitResult = std::move( track.m_fitResult );

      m_extraInfo = std::move( track.extraInfo() );

      // copy the ancestors
      m_ancestors = std::move( track.m_ancestors );

      return *this;
    }

    namespace {

      template <size_t N>
      std::array<double, N + 1> generate_chi2max( double limit ) {
        std::array<double, N + 1> c = {0};
        std::generate( std::next( std::begin( c ) ), std::end( c ),
                       [limit, i = 1u]() mutable { return gsl_cdf_chisq_Qinv( limit, i++ ); } );
        return c;
      }

      // could put this into probChi2, but then the table is generated at
      // first use of probChi2, i.e. during the event loop.
      // This way, it tends to be generated when libTrackEvent.so is
      // dynamically linked into the executable, i.e. very early.
      static const auto chi2max = generate_chi2max<256>( 1e-15 );
    } // namespace

    //=============================================================================
    // Retrieve the probability of chi^2
    //=============================================================================
    double Track::probChi2() const {
      double val( 0 );
      if ( nDoF() > 0 ) {
        // what to do if nDoF is bigger than the lookup table?
        // let's just do a range-checked acess, and have it throw
        // an exception... maybe not the most elegant solution...
        // alternative: chi2max[  std::min( nDoF(), int(chi2max.size() ) ]
        // in which case for unreasonable nDoF we don't go until 1e-15...
        val = ( chi2() < chi2max.at( nDoF() ) ? gsl_cdf_chisq_Q( chi2(), nDoF() ) : 0 );
      }
      return val;
    }

    //=============================================================================
    // Retrieve the reference to the state closest to the given z-position
    //=============================================================================
    State& Track::closestState( double z ) {
      auto iter = std::min_element( m_states.begin(), m_states.end(), TrackFunctor::distanceAlongZ( z ) );
      if ( iter == m_states.end() ) throw GaudiException( "No state closest to z", "Track.cpp", StatusCode::FAILURE );
      return *( *iter );
    }

    //=============================================================================
    // Retrieve the (const) reference to the state closest to the given z-position
    //=============================================================================
    const State& Track::closestState( double z ) const {
      auto iter = std::min_element( m_states.begin(), m_states.end(), TrackFunctor::distanceAlongZ( z ) );
      if ( iter == m_states.end() ) throw GaudiException( "No state closest to z", "Track.cpp", StatusCode::FAILURE );
      return **iter;
    }

    //=============================================================================
    // Retrieve the (const) reference to the state closest to the given plane
    //=============================================================================
    const State& Track::closestState( const Gaudi::Plane3D& plane ) const {
      auto iter = std::min_element( m_states.begin(), m_states.end(), TrackFunctor::distanceToPlane( plane ) );
      if ( iter == m_states.end() )
        throw GaudiException( "No state closest to plane", "Track.cpp", StatusCode::FAILURE );
      return **iter;
    }

    //=============================================================================
    // check the existence of a state at a certain predefined location
    //=============================================================================
    bool Track::hasStateAt( const State::Location& location ) const { return stateAt( location ) != nullptr; }

    //=============================================================================
    // Retrieve the pointer to the state closest to the given plane
    //=============================================================================
    State* Track::stateAt( const State::Location& location ) {
      auto iter = std::find_if( m_states.begin(), m_states.end(),
                                [&]( const State* s ) { return s->checkLocation( location ); } );
      return iter != m_states.end() ? *iter : nullptr;
    }

    //=============================================================================
    // Retrieve the (const) pointer to the state at a given location
    //=============================================================================
    const State* Track::stateAt( const State::Location& location ) const {
      auto iter = std::find_if( m_states.begin(), m_states.end(),
                                [&]( const State* s ) { return s->checkLocation( location ); } );
      return iter != m_states.end() ? *iter : nullptr;
    }

    //=============================================================================
    // Add a State to the list of States associated to the Track
    //=============================================================================
    void Track::addToStates( const State& state ) {
      auto      local = new State{state};
      const int order = ( checkFlag( Track::Flags::Backward ) ? -1 : 1 );
      auto      ipos  = std::upper_bound( m_states.begin(), m_states.end(), local, TrackFunctor::orderByZ( order ) );
      m_states.insert( ipos, local );
    }

    //=============================================================================
    // Add a list of states to the list associated to the Track.
    //=============================================================================
    void Track::addToStates( span<State* const> states, Tag::State::AssumeUnordered_tag ) {
      auto pivot = m_states.insert( m_states.end(), states.begin(), states.end() );
      // do not assumme that the incoming states are properly sorted.
      // it's already sorted in most cases
      with_order( useDecreasingOrder{checkFlag( Track::Flags::Backward )}, [&]( auto order ) {
        std::sort( pivot, m_states.end(), order );
        std::inplace_merge( m_states.begin(), pivot, m_states.end(), order );
      } );
    }

    //=============================================================================
    // Add a set of sorted states by increasing Z to the track.
    //=============================================================================
    void Track::addToStates( span<State* const> states, Tag::State::AssumeSorted_tag ) {
      // debug assert checking whether it's correctly sorted or not
      assert( with_order( useDecreasingOrder{checkFlag( Track::Flags::Backward )},
                          [&]( auto order ) { return std::is_sorted( states.begin(), states.end(), order ); } ) &&
              "states are not correctly sorted;"
              "hint: use the general addToStates function assuming unordered states" );

      auto pivot = m_states.insert( m_states.end(), states.begin(), states.end() );
      with_order( useDecreasingOrder{checkFlag( Track::Flags::Backward )},
                  [&]( auto order ) { std::inplace_merge( m_states.begin(), pivot, m_states.end(), order ); } );
    }

    //=============================================================================
    // Remove an LHCbID from the list of LHCbIDs associated to the Track
    //=============================================================================
    void Track::removeFromLhcbIDs( const LHCbID& value ) {
      auto pos = std::lower_bound( m_lhcbIDs.begin(), m_lhcbIDs.end(), value );
      if ( pos != m_lhcbIDs.end() && *pos == value ) m_lhcbIDs.erase( pos );
    }

    //=============================================================================
    // Remove a State from the list of States associated to the Track
    //=============================================================================
    void Track::removeFromStates( State* state ) { TrackFunctor::deleteFromList( m_states, state ); }

    //=============================================================================
    // Add LHCbIDs to track
    //=============================================================================
    bool Track::addToLhcbIDs( const LHCbID& value ) {
      auto       pos = std::lower_bound( m_lhcbIDs.begin(), m_lhcbIDs.end(), value );
      const bool rc  = ( pos == m_lhcbIDs.end() ) || !( *pos == value );
      if ( rc ) m_lhcbIDs.insert( pos, value );
      return rc;
    }

    //=============================================================================
    // Add LHCbIDs to track
    //=============================================================================
    bool Track::addSortedToLhcbIDs( span<const LHCbID> ids ) {
      LHCbIDContainer result;
      result.reserve( ids.size() + m_lhcbIDs.size() );
      std::set_union( ids.begin(), ids.end(), m_lhcbIDs.begin(), m_lhcbIDs.end(), std::back_inserter( result ) );
      auto rc   = ( result.size() == ids.size() + m_lhcbIDs.size() );
      m_lhcbIDs = std::move( result );
      return rc;
    }

    //=============================================================================
    // Count the number of LHCbIDs that two tracks have in common
    //=============================================================================
    namespace {

      // inserter which only counts how often something is inserted
      struct counting_inserter {
        size_t             count = 0;
        counting_inserter& operator++() { return *this; } // nop
        counting_inserter& operator*() { return *this; }  // redirect to self, so that our op= is called
        counting_inserter& operator=( const LHCbID& ) {
          ++count;
          return *this;
        } // raison d'etre
      };

    } // namespace

    size_t Track::nCommonLhcbIDs( const Track& rhs ) const {
      return std::set_intersection( begin( m_lhcbIDs ), end( m_lhcbIDs ), begin( rhs.m_lhcbIDs ), end( rhs.m_lhcbIDs ),
                                    counting_inserter{} )
          .count;
    }

    //=============================================================================
    // Check whether the given LHCbID is on the Track
    //=============================================================================
    bool Track::isOnTrack( const LHCbID& value ) const {
      auto pos = std::lower_bound( m_lhcbIDs.begin(), m_lhcbIDs.end(), value );
      return pos != m_lhcbIDs.end() && *pos == value;
    }

    //=============================================================================
    // reset the track
    //=============================================================================
    void Track::reset() {
      setChi2PerDoF( 0 );
      setNDoF( 0 );
      setFlags( 0 );
      setGhostProbability( 999 );
      setLikelihood( 999 );

      m_lhcbIDs.clear();
      std::for_each( m_states.begin(), m_states.end(), TrackFunctor::deleteObject() );
      m_states.clear();
      m_ancestors.clear();
      m_extraInfo.clear();
      m_fitResult.reset();
    }

    //=============================================================================
    // Copy the info from the argument track into this track
    //=============================================================================
    void Track::copy( const Track& track ) {
      setChi2PerDoF( track.chi2PerDoF() );
      setNDoF( track.nDoF() );
      setLikelihood( track.likelihood() );
      setGhostProbability( track.ghostProbability() );
      setFlags( track.flags() );
      m_lhcbIDs = track.lhcbIDs();

      // copy the states
      clearStates();
      m_states.reserve( track.states().size() );
      std::transform( track.states().begin(), track.states().end(), std::back_inserter( m_states ),
                      []( const State* s ) { return new State{*s}; } );

      // copy the track fit info
      m_fitResult = track.m_fitResult ? track.m_fitResult->clone() : std::unique_ptr<ITrackFitResult>{};

      setExtraInfo( track.extraInfo() );

      // copy the ancestors
      m_ancestors = track.m_ancestors;
    }

    //=============================================================================
    // Clear the state vector
    //=============================================================================
    void Track::clearStates() {
      std::for_each( m_states.begin(), m_states.end(), TrackFunctor::deleteObject() );
      m_states.clear();
    }

    //=============================================================================
    /** Check the presence of the information associated with
     *  a given key
     *
     *  @code
     *
     *  const Track* p = ... ;
     *
     *  Track::Key key = ... ;
     *  bool hasKey = p->hasInfo( key ) ;
     *
     *  @endcode
     *  @param    key key to be checked
     *  @return  'true' if there is informaiton with the 'key',
     *           'false' otherwise
     */
    //=============================================================================
    bool Track::hasInfo( const int key ) const { return m_extraInfo.end() != m_extraInfo.find( key ); }

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
    bool Track::addInfo( const int key, const double info ) { return m_extraInfo.insert( key, info ).second; }

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
    double Track::info( const int key, const double def ) const {
      auto i = m_extraInfo.find( key );
      return m_extraInfo.end() == i ? def : i->second;
    }

    //=============================================================================
    /** erase the information associated with the given key
     *
     *  @code
     *
     *  Track* p = ... ;
     *
     *  Track::Key  key   = ... ;
     *
     *  int erased = p->eraseInfo( key ) ;
     *
     *  @endcode
     *
     *  @param key key for the information
     *  @return return number of erased elements
     */
    //=============================================================================
    Track::ExtraInfo::size_type Track::eraseInfo( const int key ) { return m_extraInfo.erase( key ); }

    //=============================================================================
    // Fill stream
    //=============================================================================
    std::ostream& Track::fillStream( std::ostream& os ) const {
      os << "*** Track ***"
         << "\n key        : " << key() << "\n type       : " << type() << "\n history    : " << history()
         << "\n fitstatus  : " << fitStatus() << "\n # ids      : " << nLHCbIDs()
         << "\n chi2PerDoF : " << (float)m_chi2PerDoF << "\n nDoF       : " << m_nDoF
         << "\n GhostProb  : " << ghostProbability() << "\n Likelihood : " << likelihood() << "\n extraInfo : [";
      for ( const auto& i : extraInfo() ) {
        const auto info = static_cast<Track::AdditionalInfo>( i.first );
        os << " " << info << "=" << i.second << " ";
      }
      os << "]\n";

      if ( !m_states.empty() ) {
        os << " p  : " << (float)firstState().p() << "\n pt : " << (float)firstState().pt() << "\n " << nStates()
           << " states at z =";
        for ( const auto& s : states() ) {
          if ( s ) os << " " << s->z();
        }
        os << "  :-\n";
        for ( const auto& s : states() ) { os << " " << *s; }
        os << '\n';
      } else {
        os << " # states : " << nStates() << '\n';
      }

      return os;
    }

  } // namespace v1
} // namespace LHCb::Event
