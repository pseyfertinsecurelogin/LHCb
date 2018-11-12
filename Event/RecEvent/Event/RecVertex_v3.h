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
#pragma once

#include "Event/PrPixelOutput.h"
#include "Event/Track_v2.h"
#include "GaudiKernel/SystemOfUnits.h"
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/SymmetricMatrixTypes.h"
#include <vector>
#include <utility>
#include <ostream>
#include <optional>

namespace LHCb::Event::v3 {

  // Namespace for locations in TDS
  namespace RecVertexLocation {
    static const std::string Velo2D = "Hlt/Vertex/PV2D";
    static const std::string Velo3D = "Hlt/Vertex/PV3D";
    static const std::string Primary = "Rec/Vertex/Primary";
    static const std::string FilteredPrimary = "Rec/Vertex/FilteredPrimary";
    static const std::string V0 = "Rec/Vertex/V0";
  }

  /// Reconstructed Vertices class
  class RecVertex final {
  public:

    /// local Track type
    using Track = foo::PrPixelOutputForVertexing;
    /// typedef for std::vector of RecVertex
    using Vector = std::vector<RecVertex*>;
    using ConstVector = std::vector<const RecVertex*>;
  
    /// Describe how the vertex was made (NEED MORE)
    enum RecVertexType{ Unknown=0,
                        Vertex2D,
                        Vertex3D,
                        V0,
                        Primary,
                        LastRec=10000
    };
  
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
  
    /// Retrieve const  Chi square of vertex fit
    auto chi2() const { return m_chi2PerDoF.chi2(); }
  
    /// Retrieve const  Number of degree of freedom
    auto nDoF() const { return m_chi2PerDoF.nDoF; }
  
    /// Retrieve the Chi^2/DoF of vertex
    auto chi2PerDoF() const { return m_chi2PerDoF.chi2PerDoF; }
  
    /// Set the Chi^2 and the DoF of the vertex (fit)
    void setChi2PerDoF( LHCb::Event::v2::Track::Chi2PerDoF const chi2PerDof ) { m_chi2PerDoF = chi2PerDof; };

    /// Set the tracks for this PV. Weights are implicitly set to 1 for each
    void setTracks(const std::vector<const Track*>& tracks) {
      m_tracks = tracks;
      m_weights = std::vector<float>( tracks.size(), 1.0 );
    };

    /// Add a track to the track list, with the given weight
    void addToTracks(const Track* track,
                     const float weight=1.0) {
      m_tracks.push_back(track);
      m_weights.push_back(weight);
    }
  
    /// Remove the given track from the list of tracks and its associated weight
    void removeFromTracks(const Track* track);
  
    /// Remove all tracks, and their associated weights, from this vertex
    void clearTracks() {
      m_tracks.clear();
      m_weights.clear();
    }

    /// Set the weight for the given Track. Returns true if weight is successfuly set, false if the track is not part of this vertex
    bool setTrackWeight(const Track* track,
                        const float weight);
  
    /// Returns a pair containing a bool, indicating if the track was part of this vertex or not, and its associated weight (0 if track not included)
    std::optional<float> trackWeight(const Track* track) const;

      
    /// Print this RecVertex in a human readable way
    std::ostream& fillStream(std::ostream& s) const;
  
    /// Retrieve const  How the vertex was made
    const RecVertexType& technique() const { return m_technique; }
  
    /// Update  How the vertex was made
    void setTechnique(const RecVertexType& value) { m_technique = value; }
  
    /// Retrieve const  Tracks this vertex was made from
    const std::vector<const Track*>& tracks() const { return m_tracks; }
  
    /// Retrieve const  vector of weights for each track in this PV
    const std::vector<float>& weights() const { return m_weights; }

  private:
    
    /// Position in LHCb reference system
    Gaudi::XYZPoint       m_position{0.0, 0.0, -100*Gaudi::Units::m};
    /// Covariance matrix containing errors on vertex position
    Gaudi::SymMatrix3x3   m_covMatrix{};
    /// Chi square and number of degree of freedom
    LHCb::Event::v2::Track::Chi2PerDoF     m_chi2PerDoF{};
    /// How the vertex was made
    RecVertexType         m_technique{RecVertexType::Unknown};
    /// Tracks this vertex was made from
    std::vector<const Track*> m_tracks;
    /// vector of weights for each track in this PV
    std::vector<float>    m_weights;

  }; // class RecVertex
  
  inline std::ostream& operator<< (std::ostream& str, const RecVertex& obj) {
    return obj.fillStream(str);
  }
  
  inline std::ostream & operator << (std::ostream & s, RecVertex::RecVertexType e) {
    switch (e) {
      case RecVertex::Unknown  : return s << "Unknown";
      case RecVertex::Vertex2D : return s << "Vertex2D";
      case RecVertex::Vertex3D : return s << "Vertex3D";
      case RecVertex::V0       : return s << "V0";
      case RecVertex::Primary  : return s << "Primary";
      case RecVertex::LastRec  : return s << "LastRec";
      default : return s << "ERROR wrong value " << int(e) << " for enum LHCb::RecVertex::RecVertexType";
    }
  }  
  
} // namespace LHCb::Event::v3
