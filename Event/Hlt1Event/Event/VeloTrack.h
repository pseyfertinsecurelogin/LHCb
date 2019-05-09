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
      inline const std::string ForwardVeloTracks    = "Rec/Track/VeloForward";
      inline const std::string BackwardVeloTracks   = "Rec/Track/VeloBackward";
      inline const std::string ForwardVeloTracksSel = "Rec/Track/VeloForwardSel";
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

    // FIXME would be good to tag these somehow to stop them being convertible to each other
    using ClosestToBeamState = LHCb::State;
    using EndOfVeloState     = LHCb::State;
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

    /** @class ClosestToBeamStateField
     *
     * @brief SOA::Field for a ClosestToBeamState.
     *
     */
    SOAFIELD_TRIVIAL( ClosestToBeamStateField, closestToBeamState, ClosestToBeamState );

    /** @class EndOfVeloStateField
     *
     * @brief SOA::Field for a EndOfVeloState.
     *
     */
    SOAFIELD_TRIVIAL( EndOfVeloStateField, endOfVeloState, EndOfVeloState );

    /** @class VeloTrack
     *
     * @brief SOA::Skin for a backwards velo track.
     *
     * This is a simple skin for the minimal backwards velo track object we can exchange
     * between the VP tracking and the PV reconstruction.
     *
     * NOTE: this additionally includes the hits, which are not really needed for the PV
     *       reconstruction, but *are* needed if we want to configure the MC checking.
     */
    SOASKIN_TRIVIAL( VeloTrack, ClosestToBeamStateField, VeloHitBlockField );

    /** @class ForwardVeloTrack
     *
     * @brief SOA::Skin for a forward velo track.
     *
     * This is similar to VeloTrack, but also includes a second state at the end of the velo,
     * which is used by the VeloUT tracking. Note also that the comment about the hits for
     * VeloTrack does not apply here.
     */
    SOASKIN( ForwardVeloTrack, ClosestToBeamStateField, EndOfVeloStateField, VeloHitBlockField ) {
      SOASKIN_INHERIT_DEFAULT_METHODS( ForwardVeloTrack );

      /**
       * @brief access the number of Velo hits/LHCbIDs
       */
      std::size_t nLHCbIDs() const { return this->veloHitBlock().size(); }

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
  } // namespace v1
} // namespace LHCb::Hlt1Event

#endif
