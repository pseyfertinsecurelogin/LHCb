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

// local
#include "DetDesc/MagneticFieldGrid.h"
// Gaudi
#include "GaudiKernel/Kernel.h"

//-----------------------------------------------------------------------------
// Implementation file for class : MagneticFieldGrid
//
// 2008-07-26 : Marco Cattaneo
// 2009-06-05 : Wouter Hulsbergen
//-----------------------------------------------------------------------------

//=============================================================================
// Return the field vector fvec at the point xyz
//=============================================================================

namespace LHCb {

  MagneticFieldGrid::FieldVector MagneticFieldGrid::fieldVectorClosestPoint( const Gaudi::XYZPoint& r ) const {
    uint32_t i = ( r.x() - m_min_FL_V[0] ) * m_invDxyz_V[0] + 0.5f;
    uint32_t j = ( r.y() - m_min_FL_V[1] ) * m_invDxyz_V[1] + 0.5f;
    uint32_t k = ( r.z() - m_min_FL_V[2] ) * m_invDxyz_V[2] + 0.5f;
    if ( i < m_Nxyz_V[0] && j < m_Nxyz_V[1] && k < m_Nxyz_V[2] ) {
      auto index = m_Nxyz_V[0] * ( m_Nxyz_V[1] * k + j ) + i;
      auto vx    = m_Q_V[index][0] * m_scaleFactor;
      auto vy    = m_Q_V[index][1] * m_scaleFactor;
      auto vz    = m_Q_V[index][2] * m_scaleFactor;
      return {vx, vy, vz};
    }
    return {0, 0, 0};
  }

  MagneticFieldGrid::FieldGradient MagneticFieldGrid::fieldGradient( const Gaudi::XYZPoint& r ) const {
    FieldGradient _rc;
    uint32_t      i = ( r.x() - m_min_FL_V[0] ) * m_invDxyz_V[0];
    uint32_t      j = ( r.y() - m_min_FL_V[1] ) * m_invDxyz_V[1];
    uint32_t      k = ( r.z() - m_min_FL_V[2] ) * m_invDxyz_V[2];
    if ( i < m_Nxyz_V[0] && j < m_Nxyz_V[1] && k < m_Nxyz_V[2] ) {
      const auto ijk000 = ( m_Nxyz_V[0] * ( m_Nxyz_V[1] * k + j ) + i );
      const auto dijk0  = 1;
      const auto dijk1  = m_Nxyz_V[0];
      const auto dijk2  = m_Nxyz_V[0] * m_Nxyz_V[1];
      for ( int row = 0; row < 3; ++row ) {
        _rc( row, 0 ) = ( m_Q_V[ijk000 + dijk0][row] - m_Q_V[ijk000][row] ) * m_scaleFactor * m_invDxyz_V[0];
        _rc( row, 1 ) = ( m_Q_V[ijk000 + dijk1][row] - m_Q_V[ijk000][row] ) * m_scaleFactor * m_invDxyz_V[1];
        _rc( row, 2 ) = ( m_Q_V[ijk000 + dijk2][row] - m_Q_V[ijk000][row] ) * m_scaleFactor * m_invDxyz_V[2];
      }
    }
    return _rc;
  }

  MagneticFieldGrid::FieldVector MagneticFieldGrid::fieldVector( const Gaudi::XYZPoint& r ) const {
    // Interpolate the field on a cube

    const Vec4f _r( r.x(), r.y(), r.z(), 0.0f );

    const auto   abc = ( _r - toVec4f( m_min_FL_V ) ) * toVec4f( m_invDxyz_V );
    const Vec4ui ijk( truncate_to_int( abc ) );
    const auto   _i( ijk[0] ), _j( ijk[1] ), _k( ijk[2] );

    if ( LIKELY( _i < m_Nxyz_V[0] - 1 && //
                 _j < m_Nxyz_V[1] - 1 && //
                 _k < m_Nxyz_V[2] - 1 ) ) {

      // auxiliary variables defined at the vertices of the cube that
      // contains the (x, y, z) point where the field is interpolated
      const auto ijk000 = m_Nxyz_V[0] * ( m_Nxyz_V[1] * _k + _j ) + _i;
      const auto ijk001 = m_Nxyz_V[0] * ( m_Nxyz_V[1] * ( _k + 1 ) + _j ) + _i;
      const auto ijk010 = m_Nxyz_V[0] * ( m_Nxyz_V[1] * _k + _j + 1 ) + _i;
      const auto ijk011 = m_Nxyz_V[0] * ( m_Nxyz_V[1] * ( _k + 1 ) + _j + 1 ) + _i;

      const auto h1 = abc - Vec4f( _i, _j, _k, 0.0f );
      const auto h0 = Vec4f( 1.0f, 1.0f, 1.0f, 0.0f ) - h1;

      const auto h1x( h1[0] ), h1y( h1[1] ), h1z( h1[2] );
      const auto h0x( h0[0] ), h0y( h0[1] ), h0z( h0[2] );

      const Vec4f h00 = h0x * h0y;
      const Vec4f h01 = h0x * h1y;
      const Vec4f h10 = h1x * h0y;
      const Vec4f h11 = h1x * h1y;

      const Vec4f bf_V = //
          toVec4f( m_scaleFactor_V ) *
          ( h0z * ( ( ( h00 * toVec4f( m_Q_V[ijk000] ) ) + ( h10 * toVec4f( m_Q_V[ijk000 + 1] ) ) ) +   //
                    ( ( h01 * toVec4f( m_Q_V[ijk010] ) ) + ( h11 * toVec4f( m_Q_V[ijk010 + 1] ) ) ) ) + //
            h1z * ( ( ( h00 * toVec4f( m_Q_V[ijk001] ) ) + ( h10 * toVec4f( m_Q_V[ijk001 + 1] ) ) ) +   //
                    ( ( h01 * toVec4f( m_Q_V[ijk011] ) ) + ( h11 * toVec4f( m_Q_V[ijk011 + 1] ) ) ) ) );
      return {bf_V[0], bf_V[1], bf_V[2]};

    } else {
      return {0, 0, 0};
    }
  }
} // namespace LHCb
