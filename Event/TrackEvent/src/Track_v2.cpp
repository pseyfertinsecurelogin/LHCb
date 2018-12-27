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
#include "Event/Track_v2.h"
#include "GaudiKernel/TaggedBool.h"

// ============================================================================
namespace
{
      template <typename T1, typename T2, typename F >
      constexpr decltype(auto) with_selection_invoke( bool b, T1 t1, T2 t2, F&& f )
      {
          return b ? std::invoke(f,t1) : std::invoke(f,t2);
      }

      using useDecreasingOrder = Gaudi::tagged_bool<struct useDecreasingOrder_tag>;

      template < typename F >
      constexpr decltype(auto) with_order( useDecreasingOrder decreasing, F&& f )
      {
          return with_selection_invoke( UNLIKELY(static_cast<bool>(decreasing)),
                                                TrackFunctor::decreasingByZ(),
                                                TrackFunctor::increasingByZ(),
                                                std::forward<F>(f) );
      }

      // inserter which only counts how often something is inserted
      struct counting_inserter {
        size_t count = 0;
        counting_inserter& operator++() { return *this; } // nop
        counting_inserter& operator*() { return *this; }  // redirect to self, so that our op= is called
        template <typename T>
        counting_inserter& operator=( T&& )
        {
          ++count; // raison d'etre
          return *this;
        }
      };

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

namespace LHCb::Event::v2
{
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
      auto iter = std::min_element( m_states.begin(), m_states.end(), TrackFunctor::distanceAlongZ( z ) );
      if ( iter == m_states.end() ) throw GaudiException( "No state closest to z", "Track.cpp", StatusCode::FAILURE );
      return *iter;
    }

    //=============================================================================
    // Retrieve the (const) reference to the state closest to the given plane
    //=============================================================================
    const State& Track::closestState( const Gaudi::Plane3D& plane ) const
    {
      auto iter = std::min_element( m_states.begin(), m_states.end(), TrackFunctor::distanceToPlane( plane ) );
      if ( iter == m_states.end() )
        throw GaudiException( "No state closest to plane", "Track.cpp", StatusCode::FAILURE );
      return *iter;
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
    Track& Track::addToStates( const State& state )
    {
      auto ipos = with_order( useDecreasingOrder{ checkFlag(Flag::Backward) },
                              [&](auto order) { return std::upper_bound( m_states.begin(), m_states.end(), state, order); } );
      m_states.emplace( ipos, state );
      return *this;
    }

    //=============================================================================
    // Add a list of states to the list associated to the Track.
    //=============================================================================
    Track& Track::addToStates( span<const State> states, Tag::Unordered_tag )
    {
      auto pivot = m_states.insert( m_states.end(), states.begin(), states.end() );
      // do not assumme that the incoming states are properly sorted.
      with_order( useDecreasingOrder{ checkFlag(Flag::Backward) }, [&](auto order) {
        std::sort( pivot, m_states.end(), order );
        std::inplace_merge( m_states.begin(), pivot, m_states.end(), order );
      });
      return *this;
    }

    //=============================================================================
    // Add a set of sorted states by increasing Z to the track.
    //=============================================================================
    Track& Track::addToStates( span<const State> states, Tag::Sorted_tag )
    {
      // debug assert checking whether it's correctly sorted or not
      assert( with_order( useDecreasingOrder{ checkFlag( Flag::Backward ) },
                          [&states](auto order) { return std::is_sorted( states.begin(), states.end(), order ); } ) &&
              "states are not correctly sorted;"
              "hint: use the general addToStates function assuming unordered states" );

      auto pivot = m_states.insert( m_states.end(), states.begin(), states.end() );
      with_order( useDecreasingOrder{ checkFlag( Flag::Backward ) },
                  [&](auto order) { std::inplace_merge( m_states.begin(), pivot, m_states.end(), order ); });
      return *this;
    }

    //=============================================================================
    // Remove an LHCbID from the list of LHCbIDs associated to the Track
    //=============================================================================
    Track& Track::removeFromLhcbIDs( const LHCbID& value )
    {
      auto pos = std::lower_bound( m_lhcbIDs.begin(), m_lhcbIDs.end(), value );
      if ( pos != m_lhcbIDs.end() && *pos == value ) m_lhcbIDs.erase( pos );
      return *this;
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
    size_t Track::nCommonLhcbIDs( const Track& rhs ) const
    {
      return std::set_intersection( begin( m_lhcbIDs ), end( m_lhcbIDs ),
                                    begin( rhs.m_lhcbIDs ), end( rhs.m_lhcbIDs ),
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

     int Track::charge() const
    {
      double qP = firstState().qOverP();
      return UNLIKELY( std::abs( qP ) < TrackParameters::lowTolerance ) ? 0 : qP < 0 ? -1 : +1;
    }

    Track& Track::setQOverPInAllStates( float const qop )
    {
      for ( auto& state : m_states ) state.setQOverP( qop );
      return *this;
    }

    Track& Track::setQOverPAndErrInAllStates( float const qop, float const err )
    {
      for ( auto& state : m_states ) {
        state.setQOverP( qop );
        state.setErrQOverP2( err );
      }
      return *this;
    }

    Track& Track::setLhcbIDs( LHCbIDContainer&& value, Tag::Sorted_tag )
    {
      m_lhcbIDs = std::move( value );
      assert( std::is_sorted( m_lhcbIDs.begin(), m_lhcbIDs.end() ) );
      assert( std::adjacent_find( m_lhcbIDs.begin(), m_lhcbIDs.end() ) == m_lhcbIDs.end() );
      return *this;
    }

    bool Track::containsLhcbIDs( Track const& track ) const { return containsLhcbIDs( track.lhcbIDs() ); }

    bool Track::containsLhcbIDs( span<LHCbID const> ids ) const
    {
      return std::includes( m_lhcbIDs.begin(), m_lhcbIDs.end(), ids.begin(), ids.end() );
    }

    Track& Track::setFlag( Track::Flag const flag, bool const ok )
    {
      uint32_t val = ( ( static_cast<uint32_t>( flag ) ) << details::trailing_zeros( flagsMasks::flagMask ) ) &
                     flagsMasks::flagMask;
      if ( ok )
        m_flags |= val;
      else
        m_flags &= ~val;
      return *this;
    }

    bool Track::checkFlag( Track::Flag const flag ) const
    {
      uint32_t val = ( static_cast<uint32_t>( flag ) << details::trailing_zeros( flagsMasks::flagMask ) );
      return ( 0 != ( m_flags & flagsMasks::flagMask & val ) );
    }

    bool Track::hasT() const
    {
      auto const t = type();
      return t == Type::Ttrack || t == Type::Downstream || t == Type::Long;
    }

    bool Track::hasVelo() const
    {
      auto const t = type();
      return t == Type::Velo || t == Type::VeloR || t == Type::Upstream || t == Type::Long;
    }

    bool Track::hasTT() const
    {
      auto const t = type();
      return t == Type::Downstream || t == Type::Upstream || t == Type::Long;
    }

    bool Track::hasUT() const
    {
      auto const t = type();
      return t == Type::Downstream || t == Type::Upstream || t == Type::Long;
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
         << "\n chi2PerDoF : " << (float)chi2PerDoF()
         << "\n nDoF       : " << nDoF();

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
