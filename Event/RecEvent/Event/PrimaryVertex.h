/*****************************************************************************\
* (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#pragma once

// Include files
#include "Event/PrimaryVertexTrack.h"
#include "Event/VertexBase.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/NamedRange.h"
#include "GaudiKernel/SharedObjectsContainer.h"
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations
  class RecVertex;

  // Namespace for locations in TDS
  namespace PrimaryVertexLocation {
    inline const std::string Default = "Rec/Vertex/PrimaryVertices";
    inline const std::string Packed  = "pRec/Vertex/PrimaryVertices";
  } // namespace PrimaryVertexLocation

  /** @class PrimaryVertex PrimaryVertex.h
   *
   * Primary vertex class
   *
   * @author Wouter Hulsbergen
   *
   */

  class PrimaryVertex : public VertexBase {
  public:
    /// typedef for std::vector of PrimaryVertex
    typedef std::vector<PrimaryVertex*>       Vector;
    typedef std::vector<const PrimaryVertex*> ConstVector;

    /// typedef for KeyedContainer of PrimaryVertex
    typedef KeyedContainer<PrimaryVertex, Containers::HashMap> Container;

    /// Unique identifier for velo segment (hash of Velo lhcbids)
    typedef uint32_t VeloSegmentID;
    /// For defining SharedObjectContainer
    typedef SharedObjectsContainer<LHCb::PrimaryVertex> Selection;
    /// For accessing a list of vertices which is either a SharedObjectContainer, a KeyedContainer or a ConstVector
    typedef Gaudi::Range_<ConstVector> Range;
    /// Track container type
    typedef std::vector<LHCb::PrimaryVertexTrack> TrackContainer;

    /// Constructor
    PrimaryVertex( const LHCb::RecVertex& recvtx, bool refit = false );

    /// Default Constructor
    PrimaryVertex() : m_refZ( 0.0 ), m_tracks(), m_sumhalfD2Chi2DX2(), m_sumhalfDChi2DX(), m_sumchi2( 0.0 ) {}

    /// Default Destructor
    virtual ~PrimaryVertex() {}

    /// clone PrimaryVertex
    PrimaryVertex* clone() const override;

    /// Read data from buffer (for unpacking)
    template <class Buffer>
    void read( Buffer& buffer );

    /// Write data to buffer (for packing)
    template <class Buffer>
    void write( Buffer& buffer ) const;

    /// See VertexBase
    bool isPrimary() const override;

    /// Compute unique identifier for velo segment of track (hash from LHCbIds)
    static VeloSegmentID uniqueVeloSegmentID( const LHCb::Track& trk );

    /// return weight of track in vertex
    double weight( const VeloSegmentID& id ) const;

    /// return weight of track in vertex
    double weight( const LHCb::Track& track ) const;

    /// tell whether given track is in the vertex
    bool contains( const VeloSegmentID& id ) const;

    /// tell whether given track is in the vertex
    bool contains( const LHCb::Track& track ) const;

    /// Remove this set of tracks from the vertex. Returns the  number of tracks that was removed. Remove -1 in case of
    /// fit failure.
    int unbias( const std::vector<const LHCb::Track*>& tracksToRemove );

    /// Remove this set of tracks from the vertex. Returns the  number of tracks that was removed. Returns -1 in case of
    /// fit failure.
    int unbias( const std::vector<const LHCb::Track*>& tracksToRemove, LHCb::VertexBase& target ) const;

    /// Remove this set of tracks from the vertex. Returns the  number of tracks that was removed. Returns -1 in case of
    /// fit failure.
    int unbiasedPosition( const std::vector<const LHCb::Track*>& tracksToRemove, Gaudi::XYZPoint& position,
                          Gaudi::SymMatrix3x3& cov ) const;

    /// Remove set of tracks identified by VELO id from the vertex. Returns the  number of tracks that was removed.
    /// Returns -1 in case of fit failure.
    int unbiasedPosition( const std::vector<VeloSegmentID>& tracksToRemove, Gaudi::XYZPoint& position,
                          Gaudi::SymMatrix3x3& cov ) const;

    /// Refit the vertex position. Returns true in case of success.
    bool fit();

    /// Refit the vertex position
    void updateTukeyWeights( double maxchi2 );

    /// Retrieve const  Reference position for track states
    float refZ() const;

    /// Retrieve const  Tracks in PV
    const TrackContainer& tracks() const;

  protected:
  private:
    /// Vector of size 3
    typedef Gaudi::Vector3 Vector3;
    /// Symmetric matrix of size 3
    typedef Gaudi::SymMatrix3x3 SymMatrix3;

    /// find segment in segment container
    PrimaryVertex::TrackContainer::const_iterator find( const VeloSegmentID& id ) const;

    /// find segment in segment container
    PrimaryVertex::TrackContainer::iterator find( const VeloSegmentID& id );

    /// initialize the cache
    void initCache();

    float          m_refZ;             ///< Reference position for track states
    TrackContainer m_tracks;           ///< Tracks in PV
    SymMatrix3     m_sumhalfD2Chi2DX2; ///< Cached sum of second derivatives of tracks
    Vector3        m_sumhalfDChi2DX;   ///< Cached sum of first derivatives of tracks
    double         m_sumchi2;          ///< Cached sum of chi2 contributions of tracks

  }; // class PrimaryVertex

  /// Definition of Keyed Container for PrimaryVertex
  typedef KeyedContainer<PrimaryVertex, Containers::HashMap> PrimaryVertices;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline float LHCb::PrimaryVertex::refZ() const { return m_refZ; }

inline const LHCb::PrimaryVertex::TrackContainer& LHCb::PrimaryVertex::tracks() const { return m_tracks; }

inline LHCb::PrimaryVertex* LHCb::PrimaryVertex::clone() const { return new LHCb::PrimaryVertex( *this ); }

inline bool LHCb::PrimaryVertex::isPrimary() const { return true; }

inline double LHCb::PrimaryVertex::weight( const VeloSegmentID& id ) const {

  auto it = find( id );
  return it != m_tracks.end() ? it->weight() : 0;
}

inline double LHCb::PrimaryVertex::weight( const LHCb::Track& track ) const {

  auto it = find( uniqueVeloSegmentID( track ) );
  return it != m_tracks.end() ? it->weight() : 0;
}

inline bool LHCb::PrimaryVertex::contains( const VeloSegmentID& id ) const { return weight( id ) > 0; }

inline bool LHCb::PrimaryVertex::contains( const LHCb::Track& track ) const { return weight( track ) > 0; }

inline LHCb::PrimaryVertex::TrackContainer::const_iterator LHCb::PrimaryVertex::find( const VeloSegmentID& id ) const {

  return std::find( begin( m_tracks ), end( m_tracks ), id );
}

inline LHCb::PrimaryVertex::TrackContainer::iterator LHCb::PrimaryVertex::find( const VeloSegmentID& id ) {

  return std::find( begin( m_tracks ), end( m_tracks ), id );
}
