
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
#include <cmath>
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
      // Utility methods
      //------------------------------------------------------------------------------------------------

      /// SIMD cast
      using Vc::simd_cast;

      /// is negative
      using Vc::isnegative;

      /// inline if '( a ? x : y )'
      using Vc::iif;

      /// 1 / sqrt
      using Vc::rsqrt;

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
        static constexpr typename ve::simdv<TYPE, SIZE>::size_type Size = SIZE;
        using mask_type  = SIMDV<typename ve::simdv<TYPE, SIZE>::mask_type::value_type, SIZE>;
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

      template <typename V, typename = typename std::enable_if_t<std::is_base_of_v<VETAG, V>>>
      constexpr decltype( auto ) isnegative( const V& v ) {
        return ( v < V::Zero() );
      }

      /// SIMD cast
      template <typename TO, typename FROM,                                            //
                typename = typename std::enable_if_t<std::is_base_of_v<VETAG, FROM> && //
                                                     std::is_base_of_v<VETAG, TO>>>
      TO simd_cast( FROM x ) noexcept {
        static_assert( FROM::Size == TO::Size, "size mismatch" );
        return SIMDV<typename TO::value_type, FROM::Size>( x );
      }

      /// inline if '( m ? a : b )'
      template <typename MASK, typename VTRUE, typename VFALSE,                         //
                typename = typename std::enable_if_t<std::is_base_of_v<VETAG, VTRUE> && //
                                                     std::is_base_of_v<VETAG, VFALSE>>>
      decltype( auto ) iif( MASK m, VTRUE a, VFALSE b ) noexcept {
        return sel( m, a, b );
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
      using STDArray = std::array<TYPE, FPTYPE::Size>;

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

      /// local float 1/sqrt implementation
      inline FPF rsqrt( const FPF& x ) noexcept {
        return ve::invsqrt<1>( x ); // ~O(1e-6) precision
      }

      /// local double 1/sqrt implementation
      inline FPD rsqrt( const FPD& x ) noexcept {
        return ve::invsqrt<1>( x ); // ~O(1e-8) precision
      }

    } // namespace VE

    //------------------------------------------------------------------------------------------------
    // type traits
    //------------------------------------------------------------------------------------------------

    namespace details {

      /// Check if a given type is one of a given list.
      template <typename T, typename... Types>
      using _is_one_of = std::disjunction<std::is_base_of<T, Types>...>;

      /// Checks if the given type is a SIMD type.
      template <typename T>
      using _check_SIMD = _is_one_of<T,                      // test type
                                     VC::FPD, VC::FPF,       // VC floats
                                     VC::Int16, VC::Int32,   // VC ints
                                     VC::UInt16, VC::UInt32, // VC unsigned ints
                                     VE::FPD, VE::FPF,       // VE floats
                                     VE::Int16, VE::Int32,   // VE ints
                                     VE::UInt16, VE::UInt32, // VE unsigned ints
                                     VE::VETAG>;             // VE tag

    } // namespace details

    /// Checks if given TYPE is SIMD
    template <typename Type>
    using is_SIMD = details::_check_SIMD<Type>;
    template <typename Type>
    inline constexpr bool is_SIMD_v = is_SIMD<Type>::value;

    /// Checks if a given list of types are all SIMD types
    template <typename... Types>
    using all_SIMD = std::conjunction<details::_check_SIMD<Types>...>;
    template <typename... Types>
    inline constexpr bool all_SIMD_v = all_SIMD<Types...>::value;

    /// Checks if at least one type in a given list of types is SIMD
    template <typename... Types>
    using any_SIMD = std::disjunction<details::_check_SIMD<Types>...>;
    template <typename... Types>
    inline constexpr bool any_SIMD_v = any_SIMD<Types...>::value;

    /// Checks if a given list of types are all arithmetic types
    template <typename... Types>
    using all_arithmetic = std::conjunction<std::is_arithmetic<Types>...>;
    template <typename... Types>
    inline constexpr bool all_arithmetic_v = all_arithmetic<Types...>::value;

    /// Checks if at least one type in a given list of types is arithmetic
    template <typename... Types>
    using any_arithmetic = std::disjunction<std::is_arithmetic<Types>...>;
    template <typename... Types>
    inline constexpr bool any_arithmetic_v = any_arithmetic<Types...>::value;

    //------------------------------------------------------------------------------------------------
    // scalar/SIMD supporting cast
    //------------------------------------------------------------------------------------------------

    template <typename TO, typename FROM>
    TO lb_cast( FROM x ) noexcept {
      if constexpr ( all_arithmetic_v<TO, FROM> ) {
        return ( TO )( x );
      } else {
        // Note we are just assuming TO and FROM are SIMD types at
        // this point and call simd_cast on them. If they aren't, this will fail,
        // as simd_cast will not be defined for them.
        return simd_cast<TO>( x );
      }
    }

    //------------------------------------------------------------------------------------------------
    // scalar fallback implementations. Helps make templated scalar/SIMD code easier.
    //------------------------------------------------------------------------------------------------

    /// inline if '( m ? a : b )'
    template <typename VTRUE, typename VFALSE, //
              typename = typename std::enable_if_t<all_arithmetic_v<VTRUE, VFALSE>>>
    decltype( auto ) iif( const bool m, VTRUE a, VFALSE b ) noexcept {
      return ( m ? a : b );
    }

    /// isnegative
    template <typename TYPE, //
              typename = typename std::enable_if_t<all_arithmetic_v<TYPE>>>
    decltype( auto ) isnegative( const TYPE x ) noexcept {
      return std::signbit( x );
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
