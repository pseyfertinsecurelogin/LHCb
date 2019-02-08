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

#include "Event/Hlt1Track.h"
#include "Event/Hlt1TesLocations.h"
#include "GaudiKernel/SystemOfUnits.h"
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/SymmetricMatrixTypes.h"
#include "GaudiKernel/StatusCode.h"
#include "Kernel/meta_enum.h"
#include <vector>
#include <utility>
#include <ostream>
#include <optional>
#include "Event/Chi2PerDoF.h"

namespace LHCb::Event::v2 {

  // Namespace for locations in TDS
  namespace RecVertexLocation {
    inline const std::string Velo2D = "Hlt/Vertex/PV2D";
    inline const std::string Velo3D = "Hlt/Vertex/PV3D";
    inline const std::string Primary = "Rec/Vertex/Primary";
    inline const std::string FilteredPrimary = "Rec/Vertex/FilteredPrimary";
    inline const std::string V0 = "Rec/Vertex/V0";
  }

  namespace Enum::RecVertex {

    /// Enumeration to describe how the vertex was made
    meta_enum_class(RecVertexType, int,
                    Unknown=0,
                    Vertex2D,
                    Vertex3D,
                    V0,
                    Primary,
                    LastRec=10000)

    } // namespace RecVertexEnums

  /// helper class to bundle a Track and its weight
  struct WeightedTrack {
    WeightedTrack(const LHCb::HLT1::Track* t, float w) : track(t), weight(w){};
    const LHCb::HLT1::Track* track;
    float weight;
  };

  /// Reconstructed Vertices class
  class RecVertex final {
  public:

    /// local Track type
    using Track = LHCb::HLT1::Track;
    using RecVertexType = Enum::RecVertex::RecVertexType;
    /// typedef for std::vector of RecVertex
    using Vector = std::vector<RecVertex*>;
    using ConstVector = std::vector<const RecVertex*>;

    /// constructor
    RecVertex(const Gaudi::XYZPoint& position,
              const Gaudi::SymMatrix3x3& covMatrix,
              Chi2PerDoF chi2PerDoF) :
    m_position(position), m_covMatrix(covMatrix), m_chi2PerDoF(chi2PerDoF) {}

    /// Is the vertex a primary?
    bool isPrimary() const { return RecVertexType::Primary == technique(); }
  
    /// Retrieve const Position in LHCb reference system
    const Gaudi::XYZPoint& position() const { return m_position; }
  
    /// Update  Position in LHCb reference system
    void setPosition(const Gaudi::XYZPoint& value) { m_position = value; }
  
    /// Retrieve const  Covariance matrix containing errors on vertex position
    const Gaudi::SymMatrix3x3& covMatrix() const { return m_covMatrix; };
  
    /// Update  Covariance matrix containing errors on vertex position
    void setCovMatrix(const Gaudi::SymMatrix3x3& value) { m_covMatrix = value; }
  
    /// reserve space for n tracks
    void reserve(unsigned int n) { m_tracks.reserve(n); }

    /// Add a track to the track list, with the given weight
    void addToTracks(const Track* track,
                     const float weight=1.0) {
      m_tracks.emplace_back(track, weight);
    }
  
    /**
     * Remove the given track from the list of tracks and its associated weight
     * returns whether the track was found (and erased) or not
     */
    bool removeFromTracks(const Track* track);
  
    /// Remove all tracks, and their associated weights, from this vertex
    void clearTracks() {
      m_tracks.clear();
    }
  
    /// Returns a pair containing a bool, indicating if the track was part of this vertex or not, and its associated weight (0 if track not included)
    std::optional<float> trackWeight(const Track* track) const;
      
    /// Print this RecVertex in a human readable way
    std::ostream& fillStream(std::ostream& s) const;
  
    /// Retrieve const  How the vertex was made
    const RecVertexType& technique() const { return m_technique; }
  
    /// Update  How the vertex was made
    void setTechnique(const RecVertexType& value) { m_technique = value; }
  
    /// Retrieve const Tracks this vertex was made from
    const std::vector<WeightedTrack>& tracks() const { return m_tracks; }

    float chi2PerDoF() const { return m_chi2PerDoF.chi2PerDoF; }
    int nDoF() const { return m_chi2PerDoF.nDoF; }
  private:
    
    /// Position in LHCb reference system
    Gaudi::XYZPoint       m_position{0.0, 0.0, -100*Gaudi::Units::m};
    /// Covariance matrix containing errors on vertex position
    Gaudi::SymMatrix3x3   m_covMatrix{};
    /// How the vertex was made
    RecVertexType m_technique{};
    /// Tracks this vertex was made from
    std::vector<WeightedTrack> m_tracks;

    Chi2PerDoF m_chi2PerDoF;

  }; // class RecVertex
  
  inline std::ostream& operator<< (std::ostream& str, const RecVertex& obj) {
    return obj.fillStream(str);
  }
  
} // namespace LHCb::Event::v2
