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
    typedef Gaudi::Vector2 Vector2;
    /// Vector of size 3
    typedef Gaudi::Vector3 Vector3;
    /// Vector of size 4
    typedef Gaudi::Vector4 Vector4;
    /// Symmetric matrix of size 2
    typedef Gaudi::SymMatrix2x2 SymMatrix2;
    /// Symmetric matrix of size 3
    typedef Gaudi::SymMatrix3x3 SymMatrix3;

    /// Constructor
    PrimaryVertexTrack( const LHCb::Track& track, double weight, double refz );

    /// Default Constructor
    PrimaryVertexTrack()
        : m_id(), m_state(), m_invcov(), m_weight( 0.0 ), m_chi2( 0.0 ), m_halfDChi2DX(), m_halfD2Chi2DX2() {}

    /// Default Destructor
    ~PrimaryVertexTrack() {}

    /// Read data from buffer (for unpacking)
    template <class Buffer>
    void read( Buffer& buffer );

    /// Write data to buffer (for packing)
    template <class Buffer>
    void write( Buffer& buffer ) const;

    /// compute chi2 contribution for given vertex pos. note: refz must be subtracted by caller
    double chi2( const Vector3& vertexpos ) const;

    /// initialize the cache
    void initCache();

    /// less than operator
    bool operator<( const PrimaryVertexTrack& rhs ) const;

    /// equality operator
    bool operator==( const PrimaryVertexTrack& rhs ) const;

    /// less than operator
    bool operator<( const uint32_t& rhsid ) const;

    /// equality operator
    bool operator==( const uint32_t& rhsid ) const;

    /// Retrieve const  unique ID for velo segment
    const uint32_t& id() const;

    /// Retrieve const  (x,y,tx,ty) at position refz (refz stored in mother vertex)
    const Vector4& state() const;

    /// Retrieve const  inverse of cov(x,y)
    const SymMatrix2& invcov() const;

    /// Retrieve const  Tukey weight from PV fit
    float weight() const;

    /// Update  Tukey weight from PV fit
    void setWeight( float value );

    /// Retrieve const  cached chi2 contribution of track relative to (0,0,zref) [not persisted]
    float chi2() const;

    /// Retrieve const  0.5 * first derivative of chi2 to vertex parameters [not persisted]
    const Vector3& halfDChi2DX() const;

    /// Retrieve const  0.5 * second derivative of chi2 to vertex parameters [not persisted]
    const SymMatrix3& halfD2Chi2DX2() const;

  protected:
  private:
    uint32_t   m_id;            ///< unique ID for velo segment
    Vector4    m_state;         ///< (x,y,tx,ty) at position refz (refz stored in mother vertex)
    SymMatrix2 m_invcov;        ///< inverse of cov(x,y)
    float      m_weight;        ///< Tukey weight from PV fit
    float      m_chi2;          ///< cached chi2 contribution of track relative to (0,0,zref) [not persisted]
    Vector3    m_halfDChi2DX;   ///< 0.5 * first derivative of chi2 to vertex parameters [not persisted]
    SymMatrix3 m_halfD2Chi2DX2; ///< 0.5 * second derivative of chi2 to vertex parameters [not persisted]

  }; // class PrimaryVertexTrack

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const uint32_t& LHCb::PrimaryVertexTrack::id() const { return m_id; }

inline const LHCb::PrimaryVertexTrack::Vector4& LHCb::PrimaryVertexTrack::state() const { return m_state; }

inline const LHCb::PrimaryVertexTrack::SymMatrix2& LHCb::PrimaryVertexTrack::invcov() const { return m_invcov; }

inline float LHCb::PrimaryVertexTrack::weight() const { return m_weight; }

inline void LHCb::PrimaryVertexTrack::setWeight( float value ) { m_weight = value; }

inline float LHCb::PrimaryVertexTrack::chi2() const { return m_chi2; }

inline const LHCb::PrimaryVertexTrack::Vector3& LHCb::PrimaryVertexTrack::halfDChi2DX() const { return m_halfDChi2DX; }

inline const LHCb::PrimaryVertexTrack::SymMatrix3& LHCb::PrimaryVertexTrack::halfD2Chi2DX2() const {
  return m_halfD2Chi2DX2;
}

inline double LHCb::PrimaryVertexTrack::chi2( const Vector3& vertexpos ) const {

  Vector2 res;
  res( 0 ) = m_state( 0 ) + m_state( 2 ) * vertexpos( 2 ) - vertexpos( 0 );
  res( 1 ) = m_state( 1 ) + m_state( 2 ) * vertexpos( 2 ) - vertexpos( 1 );
  return ROOT::Math::Similarity( res, m_invcov );
}

inline bool LHCb::PrimaryVertexTrack::operator<( const PrimaryVertexTrack& rhs ) const { return m_id < rhs.m_id; }

inline bool LHCb::PrimaryVertexTrack::operator==( const PrimaryVertexTrack& rhs ) const { return m_id == rhs.m_id; }

inline bool LHCb::PrimaryVertexTrack::operator<( const uint32_t& rhsid ) const { return m_id < rhsid; }

inline bool LHCb::PrimaryVertexTrack::operator==( const uint32_t& rhsid ) const { return m_id == rhsid; }
