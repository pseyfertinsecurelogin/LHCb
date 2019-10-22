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
#include "GaudiKernel/GenericVectorTypes.h"
#include "GaudiKernel/SymmetricMatrixTypes.h"
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  /** @class PrimaryVertexTrack PrimaryVertexTrack.h
   *
   * Compact information for track in primary vertex
   *
   * @author Wouter Hulsbergen
   *
   */

  class PrimaryVertexTrack {
  public:
    /// Vector of size 2
    using Vector2 = Gaudi::Vector2;
    /// Vector of size 3
    using Vector3 = Gaudi::Vector3;
    /// Vector of size 4
    using Vector4 = Gaudi::Vector4;
    /// Symmetric matrix of size 2
    using SymMatrix2 = Gaudi::SymMatrix2x2;
    /// Symmetric matrix of size 3
    using SymMatrix3 = Gaudi::SymMatrix3x3;

    /// Constructor
    PrimaryVertexTrack( const LHCb::Track& track, double weight, double refz );

    /// Default Constructor
    PrimaryVertexTrack() = default;

    /// Default Destructor
    ~PrimaryVertexTrack() = default;

    /// Read data from buffer (for unpacking)
    template <class Buffer>
    void read( Buffer& buffer );

    /// Write data to buffer (for packing)
    template <class Buffer>
    void write( Buffer& buffer ) const;

    /// compute chi2 contribution for given vertex pos. note: refz must be subtracted by caller
    [[nodiscard]] double chi2( const Vector3& vertexpos ) const;

    /// initialize the cache
    void initCache();

    /// less than operator
    [[nodiscard]] friend bool operator<( const PrimaryVertexTrack& lhs, const PrimaryVertexTrack& rhs ) {
      return lhs.m_id < rhs.m_id;
    }

    /// equality operator
    [[nodiscard]] friend bool operator==( const PrimaryVertexTrack& lhs, const PrimaryVertexTrack& rhs ) {
      return lhs.m_id == rhs.m_id;
    }

    /// less than operator
    bool operator<( const uint32_t& rhsid ) const { return m_id < rhsid; }

    /// equality operator
    bool operator==( const uint32_t& rhsid ) const { return m_id == rhsid; }

    /// Retrieve const  unique ID for velo segment
    [[nodiscard]] const uint32_t& id() const { return m_id; }

    /// Retrieve const  (x,y,tx,ty) at position refz (refz stored in mother vertex)
    [[nodiscard]] const Vector4& state() const { return m_state; };

    /// Retrieve const  inverse of cov(x,y)
    [[nodiscard]] const SymMatrix2& invcov() const { return m_invcov; }

    /// Retrieve const  Tukey weight from PV fit
    [[nodiscard]] float weight() const { return m_weight; }

    /// Update  Tukey weight from PV fit
    void setWeight( float value ) { m_weight = value; }

    /// Retrieve const  cached chi2 contribution of track relative to (0,0,zref) [not persisted]
    [[nodiscard]] float chi2() const { return m_chi2; };

    /// Retrieve const  0.5 * first derivative of chi2 to vertex parameters [not persisted]
    [[nodiscard]] const Vector3& halfDChi2DX() const { return m_halfDChi2DX; }

    /// Retrieve const  0.5 * second derivative of chi2 to vertex parameters [not persisted]
    [[nodiscard]] const SymMatrix3& halfD2Chi2DX2() const { return m_halfD2Chi2DX2; }

  private:
    uint32_t   m_id{};          ///< unique ID for velo segment
    Vector4    m_state;         ///< (x,y,tx,ty) at position refz (refz stored in mother vertex)
    SymMatrix2 m_invcov;        ///< inverse of cov(x,y)
    float      m_weight{0.0};   ///< Tukey weight from PV fit
    float      m_chi2{0.0};     ///< cached chi2 contribution of track relative to (0,0,zref) [not persisted]
    Vector3    m_halfDChi2DX;   ///< 0.5 * first derivative of chi2 to vertex parameters [not persisted]
    SymMatrix3 m_halfD2Chi2DX2; ///< 0.5 * second derivative of chi2 to vertex parameters [not persisted]

  }; // class PrimaryVertexTrack

  // Including forward declarations

  inline double PrimaryVertexTrack::chi2( const Vector3& vertexpos ) const {

    Vector2 res;
    res( 0 ) = m_state( 0 ) + m_state( 2 ) * vertexpos( 2 ) - vertexpos( 0 );
    res( 1 ) = m_state( 1 ) + m_state( 2 ) * vertexpos( 2 ) - vertexpos( 1 );
    return ROOT::Math::Similarity( res, m_invcov );
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------
