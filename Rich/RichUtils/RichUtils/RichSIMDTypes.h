
#pragma once

// STL
#include <vector>
#include <array>
#include <type_traits>
#include <cstdint>

// Vc
// simdize.h causes problems so include by hand the includes in Vc/Vc
// see https://github.com/VcDevel/Vc/issues/177
//#include <Vc/Vc> 
#ifndef VC_VC_
#define VC_VC_
#include <Vc/vector.h>
#include <Vc/IO>
#include <Vc/Memory>
#include <Vc/Utils>
#include <Vc/Allocator>
#include <Vc/iterators>
#include <Vc/SimdArray>
#endif // VC_VC_

// geometry
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"
#include "GaudiKernel/Transform3DTypes.h"

// Kernel
#include "Kernel/RichDetectorType.h"
#include "Kernel/RichSide.h"

namespace Rich
{
  
  /** @namespace Rich::SIMD
   *
   *  Namespace for RICH SIMD types and utilities
   *
   *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
   *  @date   17/10/2017
   */
  namespace SIMD
  {
    
    //------------------------------------------------------------------------------------------------
    // SIMD types
    //------------------------------------------------------------------------------------------------
    
    /// The SIMD floating point type
    template < typename FPTYPE >
    using FP     = Vc::Vector<FPTYPE>;

    /// The SIMD floating point type
    template < typename ITYPE >
    using INT    = Vc::Vector<ITYPE>;
    
    /// SIMD Point
    template < typename FPTYPE >
    using Point  = ROOT::Math::PositionVector3D< ROOT::Math::Cartesian3D< FP<FPTYPE> > >;
    
    /// SIMD Vector
    template < typename FPTYPE >
    using Vector = ROOT::Math::DisplacementVector3D< ROOT::Math::Cartesian3D< FP<FPTYPE> > >;
    
    /// SIMD Plane
    template < typename FPTYPE >
    using Plane  = ROOT::Math::Impl::Plane3D< FP<FPTYPE> >;

    /// SIMD Transform3D
    template < typename FPTYPE >
    using Transform3D = ROOT::Math::Impl::Transform3D< FP<FPTYPE> >;
    
    //------------------------------------------------------------------------------------------------
    // Types for 64 bit types
    //------------------------------------------------------------------------------------------------
    
    /// SIMD double
    using FPD     = FP<double>;
    
    /// SIMD double point
    using PointD  = Point<double>;
    
    /// SIMD double vector
    using VectorD = Vector<double>;
    
    /// SIMD double plane
    using PlaneD  = Plane<double>;

    //------------------------------------------------------------------------------------------------
    // Types for 32 bit types
    //------------------------------------------------------------------------------------------------
    
    /// SIMD float
    using FPF     = FP<float>;
    
    /// SIMD float point
    using PointF  = Point<float>;
    
    /// SIMD float vector
    using VectorF = Vector<float>;
    
    /// SIMD float plane
    using PlaneF  = Plane<float>;

    //------------------------------------------------------------------------------------------------
    // Int types
    //------------------------------------------------------------------------------------------------

#if defined(Vc_IMPL_AVX) && !defined(Vc_IMPL_AVX2) 

    // Despite what the Vc documentation claims, when only AVX is available float and int
    // Vc::Vector sizes are not the same (float 8, int 4).... This works around this.

    /// SIMD Int8
    using Int8    = Vc::SimdArray<std::int8_t,FPF::Size>;

    /// SIMD UInt8
    using UInt8   = Vc::SimdArray<std::uint8_t,FPF::Size>;

    /// SIMD Int16
    using Int16   = Vc::SimdArray<std::int16_t,FPF::Size>;

    /// SIMD UInt32
    using UInt16  = Vc::SimdArray<std::uint16_t,FPF::Size>;

    /// SIMD Int32
    using Int32   = Vc::SimdArray<std::int32_t,FPF::Size>;

    /// SIMD UInt32
    using UInt32  = Vc::SimdArray<std::uint32_t,FPF::Size>;

#else

    /// SIMD Int8
    using Int8    = INT<std::int8_t>;

    /// SIMD UInt8
    using UInt8   = INT<std::uint8_t>;

    /// SIMD Int16
    using Int16   = INT<std::int16_t>;

    /// SIMD UInt16
    using UInt16  = INT<std::uint16_t>;

    /// SIMD Int32
    using Int32   = INT<std::int32_t>;

    /// SIMD UInt32
    using UInt32  = INT<std::uint32_t>;

#endif

    //------------------------------------------------------------------------------------------------
    // Default scalar types
    //------------------------------------------------------------------------------------------------

    /// Default scalar float precision for SIMD types
    using DefaultScalarFP   = float;

    /// Default scalar int type
    using DefaultScalarInt  = Int32::EntryType;

    /// Default scalar unsigned int type
    using DefaultScalarUInt = UInt32::EntryType;

   //------------------------------------------------------------------------------------------------
    // Containers for SIMD types
    //------------------------------------------------------------------------------------------------
    
    /// SIMD Vector
    template < typename TYPE >
    using STDVector = std::vector< TYPE, Vc::Allocator<TYPE> >;

    /// SIMD 'Array' (same size as Vc vectors
    template < typename TYPE, typename FPTYPE = FP<DefaultScalarFP> >
    using STDArray = std::array< TYPE, FPTYPE::Size >;

    //------------------------------------------------------------------------------------------------
    // SIMD Representations of various RICH enums
    //------------------------------------------------------------------------------------------------

    /// Container for detector sides
    using Sides = Int32;
    
  }
}
