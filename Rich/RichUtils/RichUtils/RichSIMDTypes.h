
#pragma once

// STL
#include <vector>
#include <array>

// Vc
#include <Vc/Vc>

// geometry
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"
#include "GaudiKernel/Transform3DTypes.h"

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
    using STDArray = std::array< TYPE, FP<FPTYPE>::Size >;
   
  }
}
