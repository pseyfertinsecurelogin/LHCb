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
#include <algorithm>
#include <ostream>
#include <type_traits>
#include <utility>
#include <vector>

// local
#include "RichUtils/RichSIMDTypes.h"

// Vc
// Note clang 5.0 has problems with Vc::vector.
// See https://bugs.llvm.org/show_bug.cgi?id=26764
#ifndef __clang__
#include <Vc/vector>
#endif

namespace Rich
{

  //------------------------------------------------------------------------
  /** @class BaseLookupTableInterpolator RichUtils/LookupTableInterpolator.h
   *
   *  Base class for the look up table interpolators
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   2017-11-03
   */
  //------------------------------------------------------------------------
  template < typename TYPE, size_t NBINS >
  class BaseLookupTableInterpolator
  {

  protected:

    /// SIMD floating point type
    using SIMDFP = SIMD::FP< Rich::SIMD::DefaultScalarFP >;

  protected:

    /// class for each interpolator bin
    class Bin
    {
    public:

      TYPE m { 0 }; ///< Slope    in y = mx +c
      TYPE c { 0 }; ///< Constant in y = mx +c
    public:

      /// Constructor from bin min and max, and value functor
      template < typename FUNC >
      Bin( const TYPE lowX, const TYPE highX, const FUNC &func )
      {
        const auto lowY  = func( lowX );
        const auto highY = func( highX );
        m                = ( lowY - highY ) / ( lowX - highX );
        c                = lowY - ( lowX * m );
      }

    public:

#ifndef __clang__
      using Vector = Vc::vector< Bin >;
#else
      using Vector = std::vector< Bin >;
#endif
    };

  public:

    /// Default constructor
    BaseLookupTableInterpolator() = default;

    /// Constructor from data
    template < typename... Args >
    BaseLookupTableInterpolator( Args &&... args )
    {
      init( std::forward< Args >( args )... );
    }

  protected:

    /// Initialise
    template < typename FUNC >
    inline void init( const TYPE minX, const TYPE maxX, FUNC &&func )
    {
      // clear and reserve storage
      m_data.clear();
      m_data.reserve( NBINS );

      // Initalise the range data
      m_minXSIMD = m_minX = minX;
      m_maxX              = maxX;
      m_incXinvSIMD = m_incXinv = (TYPE)NBINS / ( maxX - minX );

      // Fill the data storage for each bin
      for ( std::size_t i = 0; i < NBINS; ++i )
      { m_data.emplace_back( binLowX( i ), binHighX( i ), func ); }
    }

  protected:

    /// Get the low x value for a given bin index
    inline TYPE binLowX( const std::size_t i ) const noexcept
    {
      return m_minX + ( i / this->m_incXinv );
    }

    /// Get the high x value for a given bin index
    inline TYPE binHighX( const std::size_t i ) const noexcept
    {
      return m_minX + ( ( i + 1 ) / this->m_incXinv );
    }

  protected:

    /** Returns the scalar value for the given scalar index and x value.
     *  Assumes range checking if required is already done. */
    inline TYPE value( const std::size_t index, const TYPE x ) const noexcept
    {
      // get the data bin for the index for the given x
      const auto &bin = m_data[index];
      // return the function value in this bin
      return ( bin.m * x ) + bin.c;
    }

    /** Returns the SIMD value for the given SIMD index and x value.
     *  Assumes range checking if required is already done. */
    inline SIMDFP value( const SIMDFP::IndexType &index, const SIMDFP &x ) const noexcept
    {
#ifndef __clang__
      // gather the m and c parameters for x
      const auto m = m_data[index][&Bin::m];
      const auto c = m_data[index][&Bin::c];
#else
      // clang 5.0 has issues with Vc::vector. So fallback to scalar lookup.
      SIMDFP m( SIMDFP::Zero() ), c( SIMDFP::Zero() );
      for ( std::size_t i = 0; i < SIMDFP::Size; ++i )
      {
        const auto &bin = m_data[index[i]];
        m[i]            = bin.m;
        c[i]            = bin.c;
      }
#endif
      // return the function values
      return ( m * x ) + c;
    }

  protected:

    /// Data storage
    typename Bin::Vector m_data;

    /// Minimum valid x (scalar)
    TYPE m_minX { 0 };

    /// Maximum valid x (scalar)
    TYPE m_maxX { 0 };

    /// 1 / the bin increment (scalar)
    TYPE m_incXinv { 0 };

  protected: // cache SIMD variables

    /// Minimum valid x (SIMD)
    SIMDFP m_minXSIMD = SIMDFP::Zero();

    /// 1 / the bin increment (SIMD)
    SIMDFP m_incXinvSIMD = SIMDFP::Zero();
  };

  //------------------------------------------------------------------------
  /** @class LookupTableInterpolator RichUtils/LookupTableInterpolator.h
   *
   *  A utility class providing a look up table interpolator for a
   *  given fuction. Uses a lookup table with the given number of bins
   *  and within each bin performs a linear interpolation.
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   2017-11-03
   */
  //------------------------------------------------------------------------

  template < typename TYPE, size_t NBINS = 1000 >
  class LookupTableInterpolator final : public BaseLookupTableInterpolator< TYPE, NBINS >
  {

  public:

    /// SIMD FP type
    using SIMDFP = typename BaseLookupTableInterpolator< TYPE, NBINS >::SIMDFP;
    /// SIMD Index type
    using SIMDIn = typename SIMDFP::IndexType;

  private:

    /// Get the look up index for a given scalar x (no range checks)
    template < typename PARAM,
               typename std::enable_if< std::is_arithmetic< PARAM >::value >::type * = nullptr >
    inline decltype( auto ) xIndex( const PARAM x ) const noexcept
    {
      return ( std::size_t )( ( x - this->m_minX ) * this->m_incXinv );
    }

    /// Get the look up index for a given SIMD x (no range checks)
    inline decltype( auto ) xIndex( const SIMDFP &x ) const noexcept
    {
      return LHCb::SIMD::simd_cast< SIMDIn >( ( x - this->m_minXSIMD ) * this->m_incXinvSIMD );
    }

  public:

    /// Inherit base class constructors
    using BaseLookupTableInterpolator< TYPE, NBINS >::BaseLookupTableInterpolator;

    /// Initialise
    template < typename FUNC >
    inline void init( const TYPE minX, const TYPE maxX, FUNC &&func )
    {
      BaseLookupTableInterpolator< TYPE, NBINS >::init( minX, maxX, std::forward< FUNC >( func ) );
    }

  public:

    /// Access the computed value for the given scalar parameter
    inline TYPE operator()( const TYPE x ) const noexcept
    {
      return this->value( std::min( xIndex( x ), NBINS - 1 ), x );
    }

    /// Access the computed value for the given SIMD parameter
    inline SIMDFP operator()( const SIMDFP &x ) const noexcept
    {
      // get the indices
      auto ix = xIndex( x );
      // range check
      ix( ix >= SIMDIn( NBINS ) ) = SIMDIn( NBINS - 1 );
      // return
      return this->value( ix, x );
    }

  public:

    /** Access the computed value for the given scalar parameter.
     *  WARNING NO Range checking is performed */
    inline TYPE fast_value( const TYPE x ) const noexcept { return this->value( xIndex( x ), x ); }

    /** Access the computed value for the given SIMD parameter.
     *  WARNING NO Range checking is performed */
    inline SIMDFP fast_value( const SIMDFP &x ) const noexcept
    {
      // return
      return this->value( xIndex( x ), x );
    }
  };

  //------------------------------------------------------------------------
  /** @class LookupTableInterpolatorFromZero RichUtils/LookupTableInterpolator.h
   *
   *  A utility class providing a look up table interpolator for a
   *  given fuction. Uses a lookup table with the given number of bins
   *  and within each bin performs a linear interpolation.
   *
   *  A variation on LookupTableInterpolator where the minimum of the valid
   *  range is known to be zero. Allows for certain optimisations.
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   2017-11-03
   */
  //------------------------------------------------------------------------

  template < typename TYPE, size_t NBINS = 1000 >
  class LookupTableInterpolatorFromZero final : public BaseLookupTableInterpolator< TYPE, NBINS >
  {

  public:

    /// SIMD FP type
    using SIMDFP = typename BaseLookupTableInterpolator< TYPE, NBINS >::SIMDFP;
    /// SIMD Index type
    using SIMDIn = typename SIMDFP::IndexType;

  private:

    /// Get the look up index for a given scalar x (no range checks)
    template < typename PARAM,
               typename std::enable_if< std::is_arithmetic< PARAM >::value >::type * = nullptr >
    inline decltype( auto ) xIndex( const PARAM x ) const noexcept
    {
      return ( std::size_t )( x * this->m_incXinv );
    }

    /// Get the look up index for a given SIMD x (no range checks)
    inline decltype( auto ) xIndex( const SIMDFP &x ) const noexcept
    {
      return LHCb::SIMD::simd_cast< SIMDIn >( x * this->m_incXinvSIMD );
    }

  public:

    /// Inherit base class constructors
    using BaseLookupTableInterpolator< TYPE, NBINS >::BaseLookupTableInterpolator;

    /// Initialise
    template < typename FUNC >
    inline void init( const TYPE maxX, FUNC &&func )
    {
      BaseLookupTableInterpolator< TYPE, NBINS >::init(
        TYPE( 0 ), maxX, std::forward< FUNC >( func ) );
    }

  public:

    /** Access the computed value for the given scalar parameter.
     *  Range checking is performed */
    inline TYPE operator()( const TYPE x ) const noexcept
    {
      return this->value( std::min( xIndex( x ), NBINS - 1 ), x );
    }

    /** Access the computed value for the given SIMD parameter.
     *  Range checking is performed */
    inline SIMDFP operator()( const SIMDFP &x ) const noexcept
    {
      // get the indices
      auto ix = xIndex( x );
      // range check
      ix( ix >= SIMDIn( NBINS ) ) = SIMDIn( NBINS - 1 );
      // return
      return this->value( ix, x );
    }

  public:

    /** Access the computed value for the given scalar parameter.
     *  WARNING NO Range checking is performed */
    inline TYPE fast_value( const TYPE x ) const noexcept { return this->value( xIndex( x ), x ); }

    /** Access the computed value for the given SIMD parameter.
     *  WARNING NO Range checking is performed */
    inline SIMDFP fast_value( const SIMDFP &x ) const noexcept
    {
      // return
      return this->value( xIndex( x ), x );
    }
  };

} // namespace Rich
