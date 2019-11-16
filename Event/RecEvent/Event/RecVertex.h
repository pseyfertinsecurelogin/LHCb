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
#include "Event/Track.h"
#include "Event/VertexBase.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/NamedRange.h"
#include "GaudiKernel/SharedObjectsContainer.h"
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_RecVertex = 10030;

  // Namespace for locations in TDS
  namespace RecVertexLocation {
    inline const std::string Velo2D          = "Hlt/Vertex/PV2D";
    inline const std::string Velo3D          = "Hlt/Vertex/PV3D";
    inline const std::string Primary         = "Rec/Vertex/Primary";
    inline const std::string FilteredPrimary = "Rec/Vertex/FilteredPrimary";
    inline const std::string V0              = "Rec/Vertex/V0";
  } // namespace RecVertexLocation

  /** @class RecVertex RecVertex.h
   *
   * Reconstructed Vertices
   *
   * @author Juan Palacios
   *
   */

  class RecVertex : public VertexBase {
  public:
    /// typedef for std::vector of RecVertex
    using Vector      = std::vector<RecVertex*>;
    using ConstVector = std::vector<const RecVertex*>;

    /// typedef for KeyedContainer of RecVertex
    typedef KeyedContainer<RecVertex, Containers::HashMap> Container;

    /// Container type for shared PVs (without ownership)
    using Selection = SharedObjectsContainer<LHCb::RecVertex>;
    /// For uniform access to containers in TES (KeyedContainer,SharedContainer)
    using Range = Gaudi::NamedRange_<ConstVector>;
    /// Type for storing a Track pointer with its associated weight
    typedef std::pair<const LHCb::Track*, float> TrackWithWeight;
    /// Type for a vector of TrackWithWeight objects
    using TrackWithWeightVector = std::vector<TrackWithWeight>;

    /// Describe how the vertex was made (NEED MORE)
    enum RecVertexType {
      Unknown = 0,
      Vertex2D,
      Vertex3D,
      V0,
      Primary = LHCb::VertexBase::VertexType::Primary,
      LastRec = 10000
    };

    /// create a vertex with a selected key
    explicit RecVertex( int key ) : VertexBase( key ) {}

    /// Constructor from a point
    RecVertex( const Gaudi::XYZPoint& point ) : VertexBase( point ) {}

    /// Default Constructor
    RecVertex() = default;

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Clone vertex
    RecVertex* clone() const override;

    /// Is the vertex a primary?
    bool isPrimary() const override;

    /// Set the tracks for this PV. Weights are implicitly set to 1 for each
    void setTracks( const SmartRefVector<LHCb::Track>& tracks );

    /// Add a track to the track list, with an implicit weight of 1.0
    void addToTracks( const LHCb::Track* track );

    /// Add a track to the track list, with the given weight
    void addToTracks( const LHCb::Track* track, float weight );

    /// Remove the given track from the list of tracks and its associated weight
    void removeFromTracks( const LHCb::Track* track );

    /// Remove all tracks, and their associated weights, from this vertex
    void clearTracks();

    /// Access the tracks with their associated weights
    TrackWithWeightVector tracksWithWeights() const;

    /// Set the tracks with their associated weights
    void setTracksWithWeights( const TrackWithWeightVector& tracksAndWeights );

    /// Set the weight for the given Track. Returns true if weight is successfuly set, false if the track is not part of
    /// this vertex
    bool setTrackWeight( const LHCb::Track* track, float weight );

    /// Returns a pair containing a bool, indicating if the track was part of this vertex or not, and its associated
    /// weight (0 if track not included)
    std::pair<bool, float> trackWeight( const LHCb::Track* track ) const;

    /// Print this RecVertex in a human readable way
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  How the vertex was made
    const RecVertexType& technique() const;

    /// Update  How the vertex was made
    void setTechnique( const RecVertexType& value );

    /// Retrieve const  Tracks this vertex was made from
    const SmartRefVector<LHCb::Track>& tracks() const;

    /// Retrieve const  vector of weights for each track in this PV
    const std::vector<float>& weights() const;

    friend std::ostream& operator<<( std::ostream& str, const RecVertex& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    RecVertexType               m_technique{RecVertex::RecVertexType::Unknown}; ///< How the vertex was made
    SmartRefVector<LHCb::Track> m_tracks;                                       ///< Tracks this vertex was made from
    std::vector<float>          m_weights; ///< vector of weights for each track in this PV

  }; // class RecVertex

  /// Definition of Keyed Container for RecVertex
  typedef KeyedContainer<RecVertex, Containers::HashMap> RecVertices;

  inline std::ostream& operator<<( std::ostream& s, LHCb::RecVertex::RecVertexType e ) {
    switch ( e ) {
    case LHCb::RecVertex::Unknown:
      return s << "Unknown";
    case LHCb::RecVertex::Vertex2D:
      return s << "Vertex2D";
    case LHCb::RecVertex::Vertex3D:
      return s << "Vertex3D";
    case LHCb::RecVertex::V0:
      return s << "V0";
    case LHCb::RecVertex::Primary:
      return s << "Primary";
    case LHCb::RecVertex::LastRec:
      return s << "LastRec";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::RecVertex::RecVertexType";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::RecVertex::clID() const { return LHCb::RecVertex::classID(); }

inline const CLID& LHCb::RecVertex::classID() { return CLID_RecVertex; }

inline const LHCb::RecVertex::RecVertexType& LHCb::RecVertex::technique() const { return m_technique; }

inline void LHCb::RecVertex::setTechnique( const RecVertexType& value ) { m_technique = value; }

inline const SmartRefVector<LHCb::Track>& LHCb::RecVertex::tracks() const { return m_tracks; }

inline const std::vector<float>& LHCb::RecVertex::weights() const { return m_weights; }

inline void LHCb::RecVertex::setTracks( const SmartRefVector<LHCb::Track>& tracks ) {

  m_tracks  = tracks;
  m_weights = std::vector<float>( tracks.size(), 1.0 );
}

inline void LHCb::RecVertex::addToTracks( const LHCb::Track* track ) { addToTracks( track, 1.0 ); }

inline void LHCb::RecVertex::addToTracks( const LHCb::Track* track, const float weight ) {

  m_tracks.push_back( track );
  m_weights.push_back( weight );
}

inline void LHCb::RecVertex::clearTracks() {

  m_tracks.clear();
  m_weights.clear();
}
