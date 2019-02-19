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
#include <array>
#include <cstdint>
#include <type_traits>
#include <utility>
#include <vector>

// Vc
// simdize.h causes problems so include by hand the includes in Vc/Vc
// see https://github.com/VcDevel/Vc/issues/177
// Can be removed once Vc we use is updated to fix the above issue.
//#include <Vc/Vc>
#ifndef VC_VC_
#  define VC_VC_
#  include <Vc/Allocator>
#  include <Vc/IO>
#  include <Vc/Memory>
#  include <Vc/SimdArray>
#  include <Vc/Utils>
#  include <Vc/iterators>
#  include <Vc/vector.h>
#endif // VC_VC_

// geometry
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Transform3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"

namespace LHCb {

  /** @namespace LHCb::SIMD
   *
   *  Namespace for SIMD types and utilities
   *
   *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
   *  @date   17/10/2017
   */
  namespace SIMD {

    //------------------------------------------------------------------------------------------------
    // SIMD types
    //------------------------------------------------------------------------------------------------

    /// The SIMD floating point type
    template <typename FPTYPE>
    using FP = Vc::Vector<FPTYPE>;

    /// SIMD Point
    template <typename FPTYPE>
    using Point = ROOT::Math::PositionVector3D<ROOT::Math::Cartesian3D<FP<FPTYPE>>>;

    /// SIMD Vector
    template <typename FPTYPE>
    using Vector = ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<FP<FPTYPE>>>;

    /// SIMD Plane
    template <typename FPTYPE>
    using Plane = ROOT::Math::Impl::Plane3D<FP<FPTYPE>>;

    /// SIMD Transform3D
    template <typename FPTYPE>
    using Transform3D = ROOT::Math::Impl::Transform3D<FP<FPTYPE>>;

    //------------------------------------------------------------------------------------------------
    // Types for 64 bit types
    //------------------------------------------------------------------------------------------------

    /// SIMD double
    using FPD = FP<double>;

    /// SIMD double point
    using PointD = Point<double>;

    /// SIMD double vector
    using VectorD = Vector<double>;

    /// SIMD double plane
    using PlaneD = Plane<double>;

    //------------------------------------------------------------------------------------------------
    // Types for 32 bit types
    //------------------------------------------------------------------------------------------------

    /// SIMD float
    using FPF = FP<float>;

    /// SIMD float point
    using PointF = Point<float>;

    /// SIMD float vector
    using VectorF = Vector<float>;

    /// SIMD float plane
    using PlaneF = Plane<float>;

    //------------------------------------------------------------------------------------------------
    // Int types
    //------------------------------------------------------------------------------------------------

#if defined( Vc_IMPL_AVX ) && !defined( Vc_IMPL_AVX2 )

    // Despite what the Vc documentation claims, when only AVX is available float and int
    // Vc::Vector sizes are not the same (float 8, int 4).... This works around this.

    /// The SIMD int type with same size as that for floats
    template <typename ITYPE>
    using INT = Vc::SimdArray<ITYPE, FPF::Size>;

#else

    /// The SIMD int type with same size as that for floats
    template <typename ITYPE>
    using INT = Vc::Vector<ITYPE>;

#endif

    /// SIMD Int8
    using Int8 = INT<std::int8_t>;

    /// SIMD UInt8
    using UInt8 = INT<std::uint8_t>;

    /// SIMD Int16
    using Int16 = INT<std::int16_t>;

    /// SIMD UInt16
    using UInt16 = INT<std::uint16_t>;

    /// SIMD Int32
    using Int32 = INT<std::int32_t>;

    /// SIMD UInt32
    using UInt32 = INT<std::uint32_t>;

    //------------------------------------------------------------------------------------------------
    // Default scalar types
    //------------------------------------------------------------------------------------------------

    /// Default scalar float precision for SIMD types
    using DefaultScalarFP = float;

    /// Default scalar int type
    using DefaultScalarInt = Int32::EntryType;

    /// Default scalar unsigned int type
    using DefaultScalarUInt = UInt32::EntryType;

    //------------------------------------------------------------------------------------------------
    // Containers for SIMD types
    //------------------------------------------------------------------------------------------------

    /// SIMD Vector. std::vector with alignment respecting allocator.
    template <typename TYPE>
    using STDVector = std::vector<TYPE, Vc::Allocator<TYPE>>;

    /// SIMD 'Array' (same size as Vc vectors)
    template <typename TYPE, typename FPTYPE = FP<DefaultScalarFP>>
    using STDArray = std::array<TYPE, FPTYPE::Size>;

    //------------------------------------------------------------------------------------------------
    // Utility methods from Vc
    //------------------------------------------------------------------------------------------------

    /// SIMD cast
    using Vc::simd_cast;

  } // namespace SIMD
} // namespace LHCb
