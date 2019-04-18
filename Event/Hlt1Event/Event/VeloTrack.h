/*****************************************************************************\
* (c) Copyright 2019 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef HLT1EVENTVELOTRACK
#define HLT1EVENTVELOTRACK

// FIXME RENAME THIS FILE!

#include "Event/State.h"
#include "Event/TrackHit.h"
#include "Kernel/LHCbID.h"
#include "Kernel/STLExtensions.h"
#include "SOAContainer/SOAField.h"
#include "SOAContainer/SOAView.h"
#include <boost/container/small_vector.hpp>
#include <cstddef>

/**
 * @brief Data classes for the Hlt1 with multiple usages
 *
 * There may be additional exchange classes defined in other projects to extend the classes defined here. The
 * distinction is made by persistency and visibility aspects. Hlt1 objects that are written to file must be defined in
 * LHCb. Objects that are exclusively used between two algorithms (ideally within the same "project", e.g. Velo->VeloUT)
 * can be added in downstream projects.
 *
 * The classes are intended to be used with the Zipping::ZipContainer (SOAContainer) and Zipping::ZipSelection
 * frameworks.
 */
namespace LHCb::Hlt1Event {
  inline namespace v1 {
    namespace VeloTESLocation {
      inline const std::string ForwardHits               = "Rec/Velo/ForwardHits";
      inline const std::string BackwardHits              = "Rec/Velo/BackwardHits";
      inline const std::string ForwardVertexingStates    = "Rec/Velo/ForwardVertexing";
      inline const std::string BackwardVertexingStates   = "Rec/Velo/BackwardVertexing";
      inline const std::string ForwardVertexingStateSel  = "Rec/Velo/ForwardVertexingSel";
      inline const std::string BackwardVertexingStateSel = "Rec/Velo/BackwardVertexingSel";
    } // namespace VeloTESLocation
    /**
     * @brief number of velo hits for track for which the class has been optimized
     *
     * It is implementation defined behaviour if exceeding this number will just be inefficient or raise an exception
     */
    static constexpr std::size_t maxNumberVeloHitsPerTrack() { return 20; }

    /** @class VeloHitBlock
     *
     * @brief Information which pixels make up a Velo segment.
     *
     * Anticipated usage:
     *  - Truth matching
     *  - Track fit
     *
     *  Up for immediate review:
     *   - Store pointers/indices of Hits rather than Hits.
     *
     *  Up for future validation:
     *   - Are Hits needed or LHCbIDs enough?
     *   - is the soft upper limit of hits per track good (should it be hard, higher/lower)?
     */
    struct VeloHitBlock final {
      using VeloLHCbIDContainer = boost::container::small_vector<LHCb::LHCbID, maxNumberVeloHitsPerTrack()>;
      using VeloHitContainer    = boost::container::small_vector<LHCb::TrackHit, maxNumberVeloHitsPerTrack()>;
      VeloLHCbIDContainer m_VeloLHCbIDContainer; ///< container of LHCb::LHCbID
      VeloHitContainer    m_VeloHitContainer;    ///< container of LHCb::TrackHit
      std::size_t         size() const { return m_VeloLHCbIDContainer.size(); }
      void                reserve( std::size_t s ) {
        m_VeloHitContainer.reserve( s );
        m_VeloLHCbIDContainer.reserve( s );
      }
      void push_back( LHCb::LHCbID id, LHCb::TrackHit hit ) {
        m_VeloHitContainer.push_back( hit );
        m_VeloLHCbIDContainer.push_back( id );
      }
      const auto& lhcbIDs() const { return m_VeloLHCbIDContainer; }
    };
    using ClosestToBeamState = LHCb::State;
  } // namespace v1
} // namespace LHCb::Hlt1Event

/*
 * and now the SOAContainer things
 *
 * probably almost unused by now
 */
namespace LHCb::Hlt1Event {
  inline namespace v1 {

    /** @class VeloHitBlockField
     *
     * @brief SOA::Field for a VeloHitBlock.
     *
     * This is assumed to be unique in every container family, thus no disambiguation.
     */
    SOAFIELD_TRIVIAL( VeloHitBlockField, veloHitBlock, VeloHitBlock );

    /** @class VeloTrackHits
     *
     * @brief SOA::Skin for a VeloHitBlock.
     */
    SOASKIN( VeloTrackHits, VeloHitBlockField ) {
      SOASKIN_INHERIT_DEFAULT_METHODS( VeloTrackHits );

      void push_back( LHCb::LHCbID id, LHCb::TrackHit hit ) {
        this->veloHitBlock().m_VeloHitContainer.push_back( hit );
        this->veloHitBlock().m_VeloLHCbIDContainer.push_back( id );
      }

      void reserve( std::size_t s ) {
        this->veloHitBlock().m_VeloHitContainer.reserve( s );
        this->veloHitBlock().m_VeloLHCbIDContainer.reserve( s );
      }

      /**
       * @brief Number of Velo hits on the track
       */
      auto size() const { return this->veloHitBlock().m_VeloHitContainer.size(); }
      /**
       * @brief access a track's Velo "hits" as TrackHit
       *
       * @return a gsl::span<LHCb::TrackHit>
       */
      auto trackHits() const { return LHCb::span<const LHCb::TrackHit>( this->veloHitBlock().m_VeloHitContainer ); };
      /**
       * @brief access a track's Velo "hits" as LHCbIDs
       *
       * @return a gsl::span<LHCb::LHCbID>
       */
      auto lhcbIDs() const { return LHCb::span<const LHCb::LHCbID>( this->veloHitBlock().m_VeloLHCbIDContainer ); };
    };

    // /**
    //  * @brief Replacement of the Run1 ClosestToBeam state of a track
    //  *
    //  * Anticipated usage:
    //  *  - Physics selections
    //  *  - output of multiple tracking algorithms
    //  *  - PV reconstruction
    //  *  - IP filter
    //  *
    //  * Up for immediate redesign:
    //  *  - Use a native SOA::Container architecture instead
    //  */
    // struct ClosestToBeamState final {
    //   LHCb::State m_state; ///< actual state
    //   /// "copy construct" from an existing LHCb::State
    //   ClosestToBeamState( const LHCb::State& s ) : m_state( s ) {
    //     assert( m_state.checkLocation( LHCb::State::ClosestToBeam ) );
    //   }
    //   /// "move construct" from an existing LHCb::State
    //   ClosestToBeamState( LHCb::State&& s ) : m_state( std::move( s ) ) {
    //     assert( m_state.checkLocation( LHCb::State::ClosestToBeam ) );
    //   }

    //   [[deprecated]] auto slopes() const { return m_state.slopes(); }
    //   [[deprecated]] auto position() const { return m_state.position(); }
    //   auto errX2() const { return m_state.errX2(); }
    //   auto errY2() const { return m_state.errY2(); }
    //   auto x() const { return m_state.x(); }
    //   auto y() const { return m_state.y(); }
    //   auto z() const { return m_state.z(); }
    //   auto tx() const { return m_state.tx(); }
    //   auto ty() const { return m_state.ty(); }

    //   /// access as if it was a real LHCb::State (non-const)
    //   operator LHCb::State&() { return m_state; }
    //   /// access as if it was a real const LHCb::State
    //   operator const LHCb::State&() const { return m_state; }
    // };

    SOAFIELD_TRIVIAL( ClosestToBeamStateField, closestToBeamState, ClosestToBeamState );
    SOASKIN( AtVertexState, ClosestToBeamStateField ) {
      SOASKIN_INHERIT_DEFAULT_METHODS( AtVertexState );

      operator const LHCb::State&() const { return this->closestToBeamState(); }
      operator LHCb::State&() { return this->closestToBeamState(); }
    };

    SOASKIN_TRIVIAL( VeloTrack, ClosestToBeamStateField, VeloHitBlockField );

  } // namespace v1
} // namespace LHCb::Hlt1Event

#endif
