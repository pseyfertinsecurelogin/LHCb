
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
#include <cassert>
#include <cstdint>
#include <iostream>
#include <limits>
#include <string>
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

// geometry (ROOT GenVector)
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Transform3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"

// geometry (LHCbMath)
#include "LHCbMath/Geom/Plane.h"
#include "LHCbMath/Geom/Point.h"
#include "LHCbMath/Geom/Transform3D.h"
#include "LHCbMath/Geom/Vector.h"

// ve
#include "LHCbMath/ve/ve.h"
#include "LHCbMath/ve/vemath.h"

namespace LHCb {

  /** @namespace LHCb::SIMD
   *
   *  Namespace for SIMD types and utilities
   *
   *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
   *  @date   17/10/2017
   */
  namespace SIMD {

    /** @namespace LHCb::SIMD::VC
     *
     *  Namespace for types based on Vc
     *
     *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
     *  @date   12/03/2019
     */
    namespace VC {

      //------------------------------------------------------------------------------------------------
      // SIMD types
      //------------------------------------------------------------------------------------------------

      /// The SIMD floating point type
      template <typename FPTYPE>
      using FP = Vc::Vector<FPTYPE>;

      /// SIMD Point
      template <typename FPTYPE>
      using Point = LHCb::Geom::Point<FP<FPTYPE>>;
      // using Point = ROOT::Math::PositionVector3D<ROOT::Math::Cartesian3D<FP<FPTYPE>>>;

      /// SIMD Vector
      template <typename FPTYPE>
      using Vector = LHCb::Geom::Vector<FP<FPTYPE>>;
      // using Vector = ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<FP<FPTYPE>>>;

      /// SIMD Plane
      template <typename FPTYPE>
      using Plane = LHCb::Geom::Plane<FP<FPTYPE>>;
      // using Plane = ROOT::Math::Impl::Plane3D<FP<FPTYPE>>;

      /// SIMD Transform3D
      template <typename FPTYPE>
      using Transform3D = LHCb::Geom::Transform3D<FP<FPTYPE>>;
      // using Transform3D = ROOT::Math::Impl::Transform3D<FP<FPTYPE>>;

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

      /// SIMD Int64
      using Int64 = INT<std::int64_t>;

      /// SIMD UInt64
      using UInt64 = INT<std::uint64_t>;

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
      // Utility methods
      //------------------------------------------------------------------------------------------------

      /// SIMD cast
      using Vc::simd_cast;
      /// is negative
      using Vc::isnegative;

    } // namespace VC

    /** @namespace LHCb::SIMD::VE
     *
     *  Namespace for types based on ve
     *
     *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
     *  @date   12/03/2019
     */
    namespace VE {

      //------------------------------------------------------------------------------------------------
      // SIMD types
      //------------------------------------------------------------------------------------------------

      // Optimal SIMD size for current compilation options.
      // for the moment just piggy back on Vc for this number

      //------------------------------------------------------------------------------------------------

      /// tag class
      class VETAG {};

      /// minimal wrapper class around underlying ve type.
      template <typename TYPE,
// what list to use here ... ?
#if defined( __AVX512CD__ ) || defined( __AVX512ER__ ) || defined( __AVX512BW__ ) || defined( __AVX512F__ )
                std::size_t SIZE = 2 * LHCb::SIMD::VC::FP<TYPE>::Size // Vc only goes up to AVX2, so double it ;)
#else
                // for the reset use Vc
                std::size_t SIZE = LHCb::SIMD::VC::FP<TYPE>::Size
#endif
                >
      class SIMDV : public VETAG, public ve::simdv<TYPE, SIZE> {

      public:
        using mask_type = SIMDV<typename ve::simdv<TYPE, SIZE>::mask_type::value_type, SIZE>;
        static constexpr typename ve::simdv<TYPE, SIZE>::size_type Size = SIZE;

        // using index_type = typename ve::simdv<TYPE, SIZE>::mask_type;
        using index_type = SIMDV<std::uint32_t, SIZE>;
        using IndexType  = index_type;

      private:
        template <typename SC>
        using is_scalar_t = typename std::enable_if<std::is_same<SC, TYPE>::value>::type;

        template <typename SC>
        using is_int_t = typename std::enable_if<std::is_integral<SC>::value>::type;

      public:
        using ve::simdv<TYPE, SIZE>::simdv;
        using ve::simdv<TYPE, SIZE>::operator=;
        constexpr SIMDV( const ve::simdv<TYPE, SIZE>& other ) noexcept : ve::simdv<TYPE, SIZE>( other ) {}
        constexpr SIMDV( ve::simdv<TYPE, SIZE>&& other ) noexcept : ve::simdv<TYPE, SIZE>( std::move( other ) ) {}

        SIMDV() = default;

      public:
        static constexpr SIMDV One() noexcept { return SIMDV( 1 ); }
        static constexpr SIMDV Zero() noexcept { return SIMDV( 0 ); }

        static constexpr index_type IndexesFromZero() noexcept { return {typename SIMDV::iota(), 0}; }

        constexpr void setZero() noexcept { *this &= typename SIMDV::mask_type( 0 ); }
        constexpr void setZero( const mask_type m ) noexcept { *this = sel( 0 == m, *this, 0 ); }
        constexpr void setZeroInverted( const mask_type m ) noexcept { *this = sel( 0 != m, *this, 0 ); }

      public:
        constexpr decltype( auto ) sum() const noexcept { return hadd( *this ); }

      public:
        class MaskedWriteProxy {
        private:
          SIMDV&          v;
          const mask_type m;

        public:
          MaskedWriteProxy() = delete;
          constexpr MaskedWriteProxy( SIMDV& _v, const mask_type _m ) : v( _v ), m( _m ) {}

        public:
          template <typename S>
          constexpr SIMDV& operator=( const SIMDV<S, SIZE>& x ) {
            v = sel( 0 == m, v, x );
            return v;
          }
          template <typename S>
          constexpr SIMDV& operator+=( const SIMDV<S, SIZE>& x ) {
            v += sel( 0 != m, x, 0 );
            return v;
          }
          template <typename S>
          constexpr SIMDV& operator-=( const SIMDV<S, SIZE>& x ) {
            v -= sel( 0 != m, x, 0 );
            return v;
          }
          template <typename S>
          constexpr SIMDV& operator*=( const SIMDV<S, SIZE>& x ) {
            v *= sel( 0 != m, x, 1 );
            return v;
          }
          template <typename S>
          constexpr SIMDV& operator/=( const SIMDV<S, SIZE>& x ) {
            v /= sel( 0 != m, x, 1 );
            return v;
          }
        };

        template <typename MASK>
        constexpr decltype( auto ) operator()( const MASK m ) noexcept {
          return MaskedWriteProxy{*this, m};
        }

        template <typename MASK>
        constexpr void setQnan( const MASK& m ) noexcept {
          ( *this ) = sel( m, SIMDV( std::numeric_limits<TYPE>::quiet_NaN() ), *this );
        }

        template <typename S = TYPE, typename = is_int_t<S>>
        constexpr decltype( auto ) count() const noexcept {
          std::size_t c{0};
          for ( std::size_t i = 0; i < SIZE; ++i ) {
            if ( ( *this )[i] > 0 ) { ++c; }
          }
          return c;
        }
      };

      template <typename V, typename = typename std::enable_if<std::is_base_of<VETAG, V>::value>::type>
      constexpr decltype( auto ) isnegative( const V& v ) {
        return ( v < V::Zero() );
      }

      /// SIMD cast
      template <typename TO, typename FROM, std::size_t SIZE = TO::size(),
                typename = typename std::enable_if<std::is_base_of<VETAG, FROM>::value &&
                                                   std::is_base_of<VETAG, TO>::value>::type>
      TO simd_cast( FROM x ) noexcept {
        static_assert( FROM::size() == TO::size(), "size mismatch" );
        return SIMDV<typename TO::value_type, SIZE>( x.begin(), x.end() );
      }

      //------------------------------------------------------------------------------------------------

      /// The SIMD floating point type
      template <typename FPTYPE>
      using FP = SIMDV<FPTYPE>;

      /// SIMD Point
      template <typename FPTYPE>
      using Point = LHCb::Geom::Point<FP<FPTYPE>>;

      /// SIMD Vector
      template <typename FPTYPE>
      using Vector = LHCb::Geom::Vector<FP<FPTYPE>>;

      /// SIMD Plane
      template <typename FPTYPE>
      using Plane = LHCb::Geom::Plane<FP<FPTYPE>>;

      /// SIMD Transform3D
      template <typename FPTYPE>
      using Transform3D = LHCb::Geom::Transform3D<FP<FPTYPE>>;

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

      /// The SIMD int type with same size as that for floats
      template <typename ITYPE>
      using INT = SIMDV<ITYPE, VE::FPF::Size>;

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

      /// SIMD Int32
      using Int64 = INT<std::int64_t>;

      /// SIMD UInt32
      using UInt64 = INT<std::uint64_t>;

      //------------------------------------------------------------------------------------------------
      // Default scalar types
      //------------------------------------------------------------------------------------------------

      /// Default scalar float precision for SIMD types
      using DefaultScalarFP = float;

      /// Default scalar int type
      using DefaultScalarInt = Int32::value_type;

      /// Default scalar unsigned int type
      using DefaultScalarUInt = UInt32::value_type;

      //------------------------------------------------------------------------------------------------
      // Containers for SIMD types
      //------------------------------------------------------------------------------------------------

      /// SIMD Vector. std::vector with alignment respecting allocator.
      template <typename TYPE>
      using STDVector = std::vector<TYPE, Vc::Allocator<TYPE>>;

      /// SIMD 'Array' (always same size as VE vectors)
      template <typename TYPE, typename FPTYPE = FP<DefaultScalarFP>>
      using STDArray = std::array<TYPE, FPF::Size>;

      //------------------------------------------------------------------------------------------------
      // Utility methods
      //------------------------------------------------------------------------------------------------

      /// local float sqrt implementation
      inline FPF sqrt( const FPF& x ) noexcept {
        return ve::sqrt<1>( x ); // ~O(1e-6) precision
      }

      /// local double sqrt implementation
      inline FPD sqrt( const FPD& x ) noexcept {
        return ve::sqrt<1>( x ); // ~O(1e-8) precision
      }

    } // namespace VE

    // scalar/SIMD supporting cast
    template <typename TO, typename FROM>
    TO lb_cast( FROM x ) noexcept {
      if constexpr ( std::is_arithmetic<FROM>::value && std::is_arithmetic<TO>::value ) {
        return ( TO )( x );
      } else {
        return simd_cast<TO>( x );
      }
    }

    //------------------------------------------------------------------------------------------------

    /// The default underlying SIMD abstraction layer to use
    using namespace LHCb::SIMD::VC;
    // using namespace LHCb::SIMD::VE;

    //------------------------------------------------------------------------------------------------

  } // namespace SIMD
} // namespace LHCb

// make some methods available as `std` versions
namespace std {
  using LHCb::SIMD::VE::sqrt;
}
