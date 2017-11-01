
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
    // Types for doubles
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
    // Types for floats
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
    // Containers for SIMD types
    //------------------------------------------------------------------------------------------------
    
    /// SIMD Vector
    template < typename TYPE >
    using STDVector = std::vector< TYPE, Vc::Allocator<TYPE> >;

    /// SIMD 'Array' (same size as Vc vectors
    template < typename TYPE, typename FPTYPE >
    using STDArray = std::array< TYPE, FPTYPE::Size >;

    /// Default float precision for SIMD types
    using DefaultFP = float;

    //------------------------------------------------------------------------------------------------
    // SIMD Representations of various RICH enums
    //------------------------------------------------------------------------------------------------

    /// Container for detector sides
    using Sides = Vc::Vector<int32_t>;
    
  }
}
