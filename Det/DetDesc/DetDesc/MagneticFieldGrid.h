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

// STD
#include <array>
#include <cmath>
#include <vector>

// Gaudi
#include "GaudiKernel/GenericMatrixTypes.h"
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"

// VectorClass
#include "vectorclass.h"

class MagneticFieldGridReader;

namespace LHCb {

  /** @class MagneticFieldGrid MagneticFieldGrid.h
   *  Simple class to hold a magnetic field grid.
   *
   *  @author Marco Cattaneo, Wouter Hulsbergen
   *  @date   2008-07-26
   */
  class MagneticFieldGrid final {

  public:
    /// typedefs etc
    typedef Gaudi::XYZVector FieldVector;
    typedef Gaudi::Matrix3x3 FieldGradient;

    /// declare the field reader as friend so that we don't have to make a constructor
    friend class ::MagneticFieldGridReader;

    /// Return the field vector fvec at the point xyz by interpolation
    /// on the grid.
    FieldVector fieldVector( const Gaudi::XYZPoint& xyz ) const;

    /// Return the field vector fvec at the point xyz by interpolation
    /// on the grid.
    FieldGradient fieldGradient( const Gaudi::XYZPoint& xyz ) const;

    /// Return the field vector fvec at the point xyz by choosing the
    /// closest point on the grid.
    FieldVector fieldVectorClosestPoint( const Gaudi::XYZPoint& xyz ) const;

    /// Return the magnetic field scale factor
    float scaleFactor() const noexcept { return m_scaleFactor; }

    /// Update the scale factor
    void setScaleFactor( const float& s ) {
      m_scaleFactor   = s;
      m_scaleFactor_V = {s, s, s};
    }

  private:
  private:
    /// converts an array to Vec4f format. Note that this is a noop once compiled
    template <typename ARRAY>
    static Vec4f toVec4f( ARRAY const& a ) {
      return Vec4f().load( a.data() );
    }

    template <class vectype, size_t... Is>
    std::array<vectype, 3> fetchVectorQ_helper( const vectype& indices, std::index_sequence<Is...> ) const {
      std::array<Vec4f, sizeof...( Is )> Qs{toVec4f( m_Q_V[indices[Is]] )...};

      // TODO: This could be done more efficiently
      //       with templating magic and permute or similar
      return {vectype{Qs[Is][0]...}, vectype{Qs[Is][1]...}, vectype{Qs[Is][2]...}};
    }

    template <class vectype, size_t width>
    std::array<vectype, 3> fetchVectorQ( const vectype& indices ) const {
      return fetchVectorQ_helper( indices, std::make_index_sequence<width>{} );
    }

    float m_scaleFactor = 1; ///< The scale factor

    std::array<float, 4> m_scaleFactor_V = {1, 1, 1, 0}; ///< Vector version of the scale factor, padded to 4 floats
    std::vector<std::array<float, 4>> m_Q_V;             ///< Vectorised Field map, padded to 4 floats per field
    std::array<float, 4>              m_min_FL_V  = {0., 0., 0., 0.}; ///< Offset in x, y and z, padded to 4 floats
    std::array<float, 4>              m_Dxyz_V    = {0., 0., 0., 0.}; ///< Steps in x, y and z, padded to 4 floats
    std::array<float, 4>              m_invDxyz_V = {0., 0., 0.,
                                        0.}; ///< Inverse of steps in x, y and z (cached for speed), padded to 4 floats
    std::array<unsigned, 4>           m_Nxyz_V    = {0, 0, 0, 0}; ///< Number of steps in x, y and z, padded to 4 floats
  };

} // namespace LHCb
