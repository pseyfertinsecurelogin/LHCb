
#pragma once

// STL
#include <ostream>
#include <type_traits>
#include <vector>

// local
#include "RichUtils/RichSIMDTypes.h"

// Vc
#include <Vc/vector>

namespace Rich
{

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

  template< typename TYPE, size_t NBINS = 1000 >
  class LookupTableInterpolator final
  {

  private:

    /// class for each interpolator bin
    class Bin 
    {
    public:
      TYPE m{0}; ///< Slope    in y = mx +c
      TYPE c{0}; ///< Constant in y = mx +c
    public:
      /// Constructor from bin min and max, and value functor
      template< typename FUNC >
      Bin( const TYPE lowX, const TYPE highX, const FUNC func )
      {
        const auto lowY  = func(lowX);
        const auto highY = func(highX);
        m = ( lowY - highY ) / ( lowX - highX );
        c = lowY - ( lowX * m );
      }
    };

  private:

    /// Data storage 
    Vc::vector<Bin> m_data;
    
    /// Minimum valid x
    TYPE m_minX{0};

    /// Maximum valid x
    TYPE m_maxX{0};

    /// 1 / the bin increment
    TYPE m_incXinv{0};

  private:

    /// Get the low x value for a given bin index
    inline TYPE binLowX( const std::size_t i ) const noexcept
    {
      return m_minX + (i/m_incXinv);
    }

    /// Get the high x value for a given bin index
    inline TYPE binHighX( const std::size_t i ) const noexcept
    {
      return binLowX(i) + (1.0/m_incXinv);
    }

    /// Get the look up index for a given scalar x (no range checks)
    template < typename PARAM,
               typename std::enable_if<std::is_arithmetic<PARAM>::value>::type * = nullptr >
    inline decltype(auto) xIndex( const PARAM x ) const noexcept
    {
      return (std::size_t)( ( x - m_minX ) * m_incXinv );
    }

    /// Get the look up index for a given SIMD x (no range checks)
    template < typename PARAM,
               typename std::enable_if<!std::is_arithmetic<PARAM>::value>::type * = nullptr >
    inline decltype(auto) xIndex( const PARAM & x ) const noexcept
    {
      return Vc::simd_cast< typename PARAM::IndexType >( ( x - PARAM(m_minX) ) * PARAM(m_incXinv) );
    }

  public:

    /// Default constructor
    LookupTableInterpolator() = default;

    /// Constructor from data
    template< typename... Args >
    LookupTableInterpolator( Args&&... args ) { init( std::forward<Args>(args)... ); }

  public:

    /// Initialise
    template< typename FUNC >
    inline void init( const TYPE minX, 
                      const TYPE maxX,
                      const FUNC func )
    {
      // clear and reserve storage
      m_data.clear();
      m_data.reserve( NBINS );

      // Initalise the range data
      m_minX    = minX;
      m_maxX    = maxX;
      m_incXinv = (TYPE)NBINS / ( maxX - minX );
      
      // Fill the data storage for each bin
      for ( std::size_t i = 0; i < NBINS; ++i )
      {
        m_data.emplace_back( binLowX(i), binHighX(i), func );
      }
    }

  public:

    /// Access the computed value for the given scalar parameter
    template < typename PARAM,
               typename std::enable_if<std::is_arithmetic<PARAM>::value>::type * = nullptr >
    inline PARAM operator() ( const PARAM x ) const
    {
      // Note for speed no range checking....
      // User must garantee x is in the valid range

      // get the data bin for the index for the given x
      const auto & bin = m_data[xIndex(x)];

      // return the function value in this bin
      return ( bin.m * x ) + bin.c;
    }

    /// Access the computed value for the given SIMD parameter
    template < typename PARAM,
               typename std::enable_if<!std::is_arithmetic<PARAM>::value>::type * = nullptr >
    inline PARAM operator() ( const PARAM & x ) const
    {
      // Note for speed no range checking....
      // User must garantee x is in the valid range

      // get the indices
      const auto ix = xIndex(x);
      
      // gather the m and c parameters for x
      const auto m = m_data[ix][&Bin::m];
      const auto c = m_data[ix][&Bin::c];
      
      // return the function values
      return ( m * x ) + c;
    }

  };

}
