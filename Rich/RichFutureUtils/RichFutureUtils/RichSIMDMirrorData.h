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

// STL
#include <iostream>
#include <string>

// Gaudi
#include "GaudiKernel/SerializeSTL.h"
#include "GaudiKernel/ToStream.h"

// RichUtils
#include "RichUtils/RichSIMDTypes.h"

// RichDet
#include "RichDet/DeRichSphMirror.h"

namespace Rich::SIMD
{

  /** @class MirrorData RichSIMDMirrorData.h
   *
   *  Utility class to cache SIMD data associated to a set of mirrors
   *
   *  @author Chris Jones
   *  @date   07/02/2018
   */
  class MirrorData final
  {

  public:

    /// Basic precision (float)
    using FP = Rich::SIMD::DefaultScalarFP;
    /// SIMD version of FP
    using SIMDFP = Rich::SIMD::FP< FP >;
    /// Type for container of mirror pointers. Same size as SIMDFP.
    using Mirrors = Rich::SIMD::STDArray< const DeRichSphMirror *, SIMDFP >;

  public:

    /** @class XYZ
     *  Basic representation of a (X,Y,Z) type
     */
    template < typename TYPE >
    class XYZ
    {
    public:

      /// Basic type alias
      using Scalar = TYPE;

    public:

      /// Coordinates
      TYPE _X {}, _Y {}, _Z {};

    public:

      /// X coordinate
      inline TYPE X() const noexcept { return _X; }
      /// Y coordinate
      inline TYPE Y() const noexcept { return _Y; }
      /// Z coordinate
      inline TYPE Z() const noexcept { return _Z; }

    public:

      /// X coordinate
      inline TYPE x() const noexcept { return X(); }
      /// Y coordinate
      inline TYPE y() const noexcept { return Y(); }
      /// Z coordinate
      inline TYPE z() const noexcept { return Z(); }

    public:

      /// send to std::ostream
      friend std::ostream &operator<<( std::ostream &s, const XYZ< TYPE > &xyz )
      {
        return s << "{ " << xyz.X() << ", " << xyz.Y() << ", " << xyz.Z() << " }";
      }
    };

    /** @class Vector
     *  Basic representation of a (X,Y,Z) displacement vector
     */
    template < typename TYPE >
    class Vector : public XYZ< TYPE >
    {};

    /** @class Point
     *  Basic representation of a (X,Y,Z) position vector
     */
    template < typename TYPE >
    class Point : public XYZ< TYPE >
    {};

    // Non member operator - for points
    template < typename POINTA, typename POINTB >
    friend SIMD::Vector< FP > operator-( const POINTA &pa, const POINTB &pb ) noexcept
    {
      return { pa.X() - pb.X(), pa.Y() - pb.Y(), pa.Z() - pb.Z() };
    }

    /** @class Plane
     *  Basic representation of a SIMD vectorised plane.
     *  Described by the form A.x + B.y + C.z + D = 0 where (x,y,z) is
     *  any point in the plane.
     */
    template < typename TYPE >
    class Plane
    {
    public:

      /// Basic type alias
      using Scalar = TYPE;

    public:

      /// Plane Normal (X,Y,Z) == plane parameters (A,B,C)
      Vector< TYPE > normal;
      /// Parameter D
      TYPE D {};

    public:

      /// Normal Vector
      inline const Vector< TYPE > &Normal() const noexcept { return normal; }
      /// Distance to a point
      template < typename POINT >
      inline decltype( auto ) Distance( const POINT &p ) const noexcept
      {
        return ( ( normal.X() * p.X() ) + ( normal.Y() * p.Y() ) + ( normal.Z() * p.Z() ) + D );
      }

    public:

      /// send to std::ostream
      friend std::ostream &operator<<( std::ostream &s, const Plane< TYPE > &p )
      {
        return s << "{ Normal=" << p.normal << " D=" << p.D << " }";
      }
    };

  public:

    /// Default constructor
    MirrorData() = default;

  private: // Mirror data

    /// The mirrors for which the caches are valid
    Mirrors cache_mirrors { {} };
    /// RoC values
    SIMDFP m_RoCs {};
    /// CoC point
    Point< SIMDFP > m_CoCs;
    /// Normal Plane
    Plane< SIMDFP > m_NormPs;

  public:

    /// Get the SIMD RoC values for the current mirrors
    inline const SIMDFP &getRoCs() const noexcept { return m_RoCs; }
    /// Get the SIMD centreNormalPlane for the current mirrors
    inline const Plane< SIMDFP > &getNormalPlane() const noexcept { return m_NormPs; }
    /// Get the SIM CoC for the current mirrors
    inline const Point< SIMDFP > &getCoCs() const noexcept { return m_CoCs; }

  public:

    /// Update the cached information for the given mirrors
    void update( const Mirrors &mirrors ) noexcept;

  public:

    /// send to std::ostream
    inline friend std::ostream &operator<<( std::ostream &s, const MirrorData &data )
    {
      return s << "{ Mirrors "
               //<< data.mirrors // ToDo FixMe
               << " RoCs " << data.getRoCs() << " CoCs " << data.getCoCs() << " Plane "
               << data.getNormalPlane() << " }";
    }
  };

} // namespace Rich::SIMD
