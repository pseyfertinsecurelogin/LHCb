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
#pragma once
#include "Event/MuonPID_v2.h"

#include "GaudiKernel/GaudiException.h"
#include "LHCbMath/SIMDWrapper.h"
#include "LHCbMath/TypeMapping.h"
#include "SOAExtensions/ZipUtils.h"

// Include files
#include <functional>
#include <utility>
#include <vector>

/** @file  PrMuonPIDs.h
 *  @brief Definition of object for MuonPID.
 */

namespace LHCb::Pr::Muon {
  enum StatusMasks { IsMuon = 0x1, InAcceptance = 0x2, PreSelMomentum = 0x4, IsMuonLoose = 0x8, IsMuonTight = 0x10 };

  namespace details {
    // Copied from SelUtils
    constexpr int popcount( bool x ) { return x; }

    template <StatusMasks Mask>
    constexpr int setFlag( int input, bool value ) {
      return input ^ ( ( -value ^ input ) & Mask );
    }
  } // namespace details

  /** @class LHCb::Pr::Muon::PID
   *  @brief Helper struct to add pid object to pid container.
   *
   */
  struct PID {
    bool  isMuon{false};
    float chi2Corr{0.f};
  };

  /** @class LHCb::Pr::Muon::PIDs
   *  @brief SOA container of Muon PIDs.
   *
   */
  class PIDs {
    std::vector<int>         m_statuses;
    std::vector<float>       m_chi2Corrs;
    Zipping::ZipFamilyNumber m_zipIdentifier;

  public:
    /** Constructor which takes tracks and an operation acting on tracks, similar to transform in ZipAlgorithms.h */
    template <typename Tracks, typename Operation>
    PIDs( Tracks const& tracks, Operation&& operation ) : m_zipIdentifier{tracks.zipIdentifier()} {
      reserve( tracks.size() );

      // Assume the container is iterable -- for LHCb::Pr::*::Tracks this means
      // that you need to use LHCb::Pr::make_zip( ... )
      for ( auto const track : tracks ) { emplace_back( std::invoke( operation, track ) ); }

      if ( tracks.size() != size() ) {
        throw GaudiException{"Asked to zip containers that are not the same size", "LHCb::Pr::Zip",
                             StatusCode::FAILURE};
      }
    }

    /** Default constructor, requires to be given a family id. */
    PIDs( Zipping::ZipFamilyNumber family ) : m_zipIdentifier{std::move( family )} {}

    /** Special constructor for zipping and selections */
    PIDs( Zipping::ZipFamilyNumber family, PIDs const& /*old*/ ) : m_zipIdentifier{std::move( family )} {}

    /** Force use of move constructor and forbid copying */
    PIDs( PIDs const& ) = delete;

    /** Move constructor */
    PIDs( PIDs&& other ) = default;

    /** Return the size of our containers. */
    std::size_t size() const { return m_statuses.size(); }

    /** Make sure our containers have at least this capacity, rounded up to be a
     *  multiple of the vector unit size so that compressstore() will never write
     *  to out-of-bounds memory.
     */
    void reserve( std::size_t capacity ) {
      // Make sure that we round up to vector-unit-sized boundary
      m_statuses.reserve( align_size( capacity ) );
      m_chi2Corrs.reserve( align_size( capacity ) );
    }

    /** Resize the containers to the given size, making sure that the capacity is
     *  rounded up appropriately
     */
    void resize( std::size_t new_size ) {
      // Our special reserve() makes sure that the capacity is rounded up
      reserve( new_size );
      m_statuses.resize( new_size );
      m_chi2Corrs.resize( new_size );
    }

    /** Identifier showing which family of containers these columns can be zipped
     *  into.
     */
    Zipping::ZipFamilyNumber zipIdentifier() const { return m_zipIdentifier; }

    template <typename dType, typename Mask>
    void copy_back( PIDs const& from, int at, Mask mask ) {
      using I = typename dType::int_v;
      using F = typename dType::float_v;
      // How many elements are we adding?
      using details::popcount;
      auto to_add = popcount( mask );

      // How many elements do we already have?
      auto old_size = size();

      // Make sure there's space for the incoming values
      resize( old_size + to_add );

      // Do the copy
      I( &from.m_statuses[at] ).compressstore( mask, m_statuses.data() + old_size );
      F( &from.m_chi2Corrs[at] ).compressstore( mask, m_chi2Corrs.data() + old_size );
    }

    auto const& statuses() const { return m_statuses; }
    auto const& chi2Corrs() const { return m_chi2Corrs; }

    // TODO remove, helps in transition
    void push_back( LHCb::Event::v2::MuonPID const& pid ) {
      m_statuses.push_back( pid.Status() );
      m_chi2Corrs.push_back( pid.chi2Corr() );
    }

    // TODO remove, helps in transition
    void emplace_back( LHCb::Event::v2::MuonPID&& pid ) { push_back( std::move( pid ) ); }

    template <typename IsMuon, typename Chi2Corr>
    void emplace_back( IsMuon&& isMuon, Chi2Corr&& chi2Corr ) {
      m_statuses.emplace_back( details::setFlag<StatusMasks::IsMuon>( 0x0, isMuon ) );
      m_chi2Corrs.emplace_back( chi2Corr );
    }

    template <typename _PID>
    void emplace_back( _PID&& pid ) {
      emplace_back( pid.isMuon, pid.chi2Corr );
    }

    template <typename dType, bool unwrap>
    auto IsMuon( size_t offset ) const {
      if constexpr ( unwrap ) {
        return ( m_statuses[offset] & StatusMasks::IsMuon ) != 0;
      } else {
        return !( ( typename dType::int_v( &m_statuses[offset] ) & StatusMasks::IsMuon ) == 0 );
      }
    }

    template <typename dType, bool unwrap>
    auto Chi2Corr( size_t offset ) const {
      if constexpr ( unwrap ) {
        return m_chi2Corrs[offset];
      } else {
        return typename dType::float_v( &m_chi2Corrs[offset] );
      }
    }
  };
} // namespace LHCb::Pr::Muon
