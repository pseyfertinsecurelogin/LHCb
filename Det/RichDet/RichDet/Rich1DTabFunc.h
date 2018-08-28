
//============================================================================
/** @file Rich1DTabFunc.h
 *
 *  Header file for utility class : Rich::TabulatedFunction1D
 *
 *  @author Chris Jones       Christopher.Rob.Jones@cern.ch
 *  @date   2003-08-13
 */
//============================================================================

#pragma once

// STL
#include <cmath>
#include <iostream>
#include <map>
#include <memory>
#include <type_traits>
#include <vector>

// GSL interpolation
#include "gsl/gsl_errno.h"
#include "gsl/gsl_spline.h"

namespace Rich
{

  //============================================================================
  /** @class Rich::TabulatedFunction1D RichDet/Rich1DTabFunc.h
   *
   *  A class describing a function based on the interpolation of data points.
   *
   *  Uses a home made fixed binned linear interpolator, initialised from the input
   *  (x,y) data points using a GSL interpolator.
   *  Default GSL interpolator type is gsl_interp_linear.
   *
   *  For other possibilities see
   *  http://www.gnu.org/software/gsl/manual/html_node/Interpolation.html
   *
   *  @author Chris Jones       Christopher.Rob.Jones@cern.ch
   *  @date   2003-08-13
   */
  //============================================================================

  class TabulatedFunction1D
  {

  public:

    /// Typedef for a vector of pointers to TabulatedFunction1D
    using ConstVector = std::vector< const TabulatedFunction1D * >;

  protected: // definitions

    /// Internal data container type
    using Data = std::map< double, double >;

  private:

    /** Callable class to handle the GSL interpolator free call.
     *  Used in conjugation with std::unique_ptr. */
    class GSLSpline final
    {
    public:

      /// the deletion operator
      void operator()( gsl_spline *s ) { gsl_spline_free( s ); }
      /// The type for memory management
      using Ptr = std::unique_ptr< gsl_spline, GSLSpline >;
    };

    /** Callable class to handle the GSL accelerator free call.
     *  Used in conjugation with std::unique_ptr. */
    class GSLAccelerator final
    {
    public:

      /// the deletion operator
      void operator()( gsl_interp_accel *a ) { gsl_interp_accel_free( a ); }
      /// The type for memory management
      using Ptr = std::unique_ptr< gsl_interp_accel, GSLAccelerator >;
    };

    /// Fast linear interpolator using fixed sized bins
    template < typename TYPE,
               typename = typename std::enable_if< std::is_floating_point< TYPE >::value >::type >
    class LinearInterp final
    {

    public:

      /// A single bin in the interpolator
      class Bin final
      {
      public:

        /// Drault constructor
        Bin() = default;
        /// Constructor from bin low/high edges
        Bin( const TYPE lowX, const TYPE lowY, const TYPE highX, const TYPE highY )
        {
          m_minX  = lowX;
          m_maxX  = highX;
          m_slope = ( lowY - highY ) / ( lowX - highX );
          m_const = lowY - ( lowX * m_slope );
        }
        /// Get the y value for a given x for this bin
        inline TYPE getY( const TYPE x ) const noexcept { return ( m_const + ( x * m_slope ) ); }
        /// get the slope for this given bin
        inline TYPE slope() const noexcept { return m_slope; }
        /// Get the bin min x
        inline TYPE minX() const noexcept { return m_minX; }
        /// Get the bin max x
        inline TYPE maxX() const noexcept { return m_maxX; }
        /// Get the bin y value at min x
        inline TYPE minY() const noexcept { return getY( m_minX ); }
        /// Get the bin y value at max x
        inline TYPE maxY() const noexcept { return getY( m_maxX ); }

      private:

        /// The slope parameter
        TYPE m_slope { 0 };
        /// The constant parameter
        TYPE m_const { 0 };
        /// The bin min x
        TYPE m_minX { 0 };
        /// The bin max x
        TYPE m_maxX { 0 };

      public:

        /// type for storage of data points
        typedef std::vector< Bin > Storage;
      };

    public:

      /// Clear the interpolator
      void clear()
      {
        m_data.clear();
        m_incXinv = m_minX = m_maxX = 0;
      }

      /// Initialise
      void init( const TYPE minX, const TYPE maxX, gsl_spline *gslS, const unsigned int nsamples )
      {
        // reset container
        clear();
        // set min and max X
        m_minX = minX;
        m_maxX = maxX;
        // set increment and 1/increment
        const auto incX = ( maxX - minX ) / ( TYPE )( nsamples );
        m_incXinv       = 1.0 / incX;
        // accelerator for te GSL calls
        GSLAccelerator::Ptr acc( gsl_interp_accel_alloc() );
        // Fill the data storage
        m_data.reserve( nsamples );
        for ( unsigned int i = 0; i < nsamples; ++i )
        {
          const TYPE binXmin = std::max( minX, minX + ( i * incX ) );
          const TYPE binXmax = std::min( maxX, binXmin + ( incX ) );
          const TYPE binYmin = gsl_spline_eval( gslS, binXmin, acc.get() );
          const TYPE binYmax = gsl_spline_eval( gslS, binXmax, acc.get() );
          m_data.emplace_back( binXmin, binYmin, binXmax, binYmax );
        }
      }

    public:

      /// Access the value for a given X
      inline TYPE value( const TYPE x ) const noexcept { return m_data[xIndex( x )].getY( x ); }

      /// Access the first derivative (slope) at given x
      inline TYPE firstDerivative( const TYPE x ) const noexcept
      {
        return m_data[xIndex( x )].slope();
      }

      /// Access the min X value
      inline TYPE minX() const noexcept { return m_minX; }

      /// Access the max X value
      inline TYPE maxX() const noexcept { return m_maxX; }

      /// Access the Y value at the minimum X value
      inline TYPE minY() const noexcept { return m_data.front().minY(); }

      /// Access the Y value at the maximum X value
      inline TYPE maxY() const noexcept { return m_data.back().maxY(); }

      /// Compute the intergral for the given range
      inline TYPE integral( const TYPE from, const TYPE to, const bool weightByX = false ) const
      {
        TYPE sum = 0;
        // Loop over bins and sum as required
        for ( const auto &bin : m_data )
        {
          // Does this bin span the range at all
          if ( !( bin.maxX() < from || bin.minX() > to ) )
          {
            // get the range for this bin
            const auto bfrom = std::max( from, bin.minX() );
            const auto bto   = std::min( to, bin.maxX() );
            // average height for this trapezoid
            const auto h =
              ( !weightByX ? ( bin.getY( bto ) + bin.getY( bfrom ) ) * 0.5 :
                             ( ( bto * bin.getY( bto ) ) + ( bfrom * bin.getY( bfrom ) ) ) * 0.5 );
            // sum the area (trapezoid)
            sum += ( ( bto - bfrom ) * h );
          }
        }
        return sum;
      }

      /// Compute the mean value of x in the given range
      inline TYPE meanX( const TYPE from, const TYPE to ) const
      {
        const auto bot = integral( from, to );
        return ( fabs( bot ) > 0 ? integral( from, to, true ) / bot : 0 );
      }

      /// Access the number of data points
      inline std::size_t nDataPoints() const noexcept { return m_data.size(); }

    private:

      /// Get the look up index for a given x
      inline unsigned int xIndex( const TYPE x ) const noexcept
      {
        return ( x <= m_minX ?
                   0u :
                   x >= m_maxX ? m_data.size() - 1 : (unsigned int)( ( x - m_minX ) * m_incXinv ) );
        // return (unsigned int)( (x-m_minX) * m_incXinv );
      }

    private:

      /// The look up storage of data points
      typename Bin::Storage m_data;

      /// 1 / the bin increment
      TYPE m_incXinv { 0 };

      /// The minimum valid x
      TYPE m_minX { 0 };

      /// The maximum valid x
      TYPE m_maxX { 0 };
    };

  public:

    /// Default constructor
    TabulatedFunction1D() = default;

    /** Constructor from arrays containing x and y values
     *
     *  Arrays must be of the same size and ordered such that entry i in each
     *  correspond to each other.
     *
     *  @param x         Array of x values
     *  @param y         Array of y values
     *  @param size      Number of data points
     *  @param interType GSL Interpolator type
     */
    TabulatedFunction1D( const double           x[],
                         const double           y[],
                         const unsigned int     size,
                         const gsl_interp_type *interType = gsl_interp_linear )
    {
      initInterpolator( x, y, size, interType );
    }

    /** Constructor from std::vectors containing x and y values
     *
     *  Vectors must be of the same size and ordered such that entry i in each
     *  correspond to each other.
     *
     *  @param x         Vector of x values
     *  @param y         Vector of y values
     *  @param interType GSL Interpolator type
     */
    TabulatedFunction1D( const std::vector< double > &x,
                         const std::vector< double > &y,
                         const gsl_interp_type *      interType = gsl_interp_linear )
    {
      initInterpolator( x, y, interType );
    }

    /** Constructor from map of x,y values
     *
     *  @param data      map containing x(key) and y(data) values
     *  @param interType GSL Interpolator type
     */
    TabulatedFunction1D( const std::map< double, double > &data,
                         const gsl_interp_type *           interType = gsl_interp_linear )
    {
      initInterpolator( data, interType );
    }

    /** Constructor from vector of a pair of x,y values
     *
     *  @param data      std::vector containing and pair of x(first) and y(second) values
     *  @param interType GSL Interpolator type
     */
    TabulatedFunction1D( const std::vector< std::pair< double, double > > &data,
                         const gsl_interp_type *interType = gsl_interp_linear )
    {
      initInterpolator( data, interType );
    }

    /// Destructor
    virtual ~TabulatedFunction1D() = default;

  public:

    /** Create an interpolator that is the product of a list of interpolators
     *
     *  @param funcs   A vector containing pointers to the functions to merge
     *  @param samples Number of sample points to use
     *
     *  @return Pointer to an interpolator that represents the product of all functions
     *  @retval Non-nullptr Interpolator was successfully created
     *  @retval nullptr     Interpolator could not be created
     */
    static std::unique_ptr< TabulatedFunction1D >
    combine( const ConstVector &    funcs,
             const unsigned int     samples   = 100,
             const gsl_interp_type *interType = gsl_interp_linear );

  private:

    /// Check lower bound
    inline bool checkLowerBound( const double x ) const noexcept { return ( minX() <= x ); }

    /// Check upper bound
    inline bool checkUpperBound( const double x ) const noexcept { return ( x <= maxX() ); }

    /** Issue an out of range warning
     *  @param x    The requested x value
     *  @param retx The x value to use (corrected to be in range)
     *  @return x value to use
     */
    virtual double rangeWarning( const double x, const double retx ) const;

    /** Sanitise the input x value to enforce being inside the min max range.
     *  @param x The x value to check
     *  @return The x value to use
     */
    inline double sanitiseRange( const double x ) const noexcept
    {
      return ( !checkLowerBound( x ) ? rangeWarning( x, minX() ) :
                                       !checkUpperBound( x ) ? rangeWarning( x, maxX() ) : x );
    }

  public:

    /** Computes the function value (y) for the given parameter (x) value
     *
     *  @param x The parameter value
     *
     *  @return The value of the function at the given parameter value
     */
    inline double value( const double x ) const noexcept
    {
      // range checking is now performed by the fast interpolator
      // return ( !checkLowerBound(x) ? rangeWarning(x,minY()) :
      //         !checkUpperBound(x) ? rangeWarning(x,maxY()) :
      //         m_fastInterp.value( x )                      );
      return m_fastInterp.value( x );
    }

    /**  Returns the function value (y) for the given parameter (x) value
     *
     *  @param x The parameter value
     *
     *   @return The value of the function at the given parameter value
     */
    inline double operator[]( const double x ) const noexcept { return value( x ); }

    /** Computes the mean function value between the given parameter limits
     *
     *  @param from       The lower parameter limit
     *  @param to         The upper parameter limit
     *
     *  @return the mean function value
     */
    inline double meanX( const double from, const double to ) const
    {
      return m_fastInterp.meanX( sanitiseRange( from ), sanitiseRange( to ) );
    }

    /** Computes the definite integral of the function between limits
     *
     *  @param from       The lower parameter limit
     *  @param to         The upper parameter limit
     *
     *  @return the definite function integral
     */
    inline double integral( const double from, const double to ) const
    {
      return m_fastInterp.integral( sanitiseRange( from ), sanitiseRange( to ) );
    }

    /** Computes the first derivative of the function at the given parameter point
     *
     *  @param x The parameter value
     *
     *  @return the first derivative
     */
    inline double firstDerivative( const double x ) const noexcept
    {
      return m_fastInterp.firstDerivative( sanitiseRange( x ) );
    }

    /** Computes the R.M.S. value between the given parameter limits.
     *
     *  Optionally, an additional weight function can be given.
     *
     *  @param from       The lower parameter limit
     *  @param to         The upper parameter limit
     *  @param samples    Number of sample points to use in calculating the RMS
     *  @param weightFunc Optional weight function
     *
     *  @return the r.m.s. value
     */
    double rms( const double               from,
                const double               to,
                const unsigned int         samples    = 100,
                const TabulatedFunction1D *weightFunc = nullptr ) const;

    /** Computes the standard deviation between the given parameter limits
     *
     *  Optionally, an additional weight function can be given.
     *
     *  @param from    The lower parameter limit
     *  @param to      The upper parameter limit
     *  @param samples Number of sample points to use in calculating the S.D.
     *  @param weightFunc Optional weight function
     *
     *  @return the standard deviation
     */
    double standardDeviation( const double               from,
                              const double               to,
                              const unsigned int         samples    = 100,
                              const TabulatedFunction1D *weightFunc = nullptr ) const;

    /** The minimum parameter value for which the function is defined
     *
     *  @return The minimum valid paramter value
     */
    inline double minX() const noexcept { return m_fastInterp.minX(); }

    /** The function value for the minimum valid parameter
     *
     *  @return The function value at the minimum valid parameter
     */
    inline double minY() const noexcept { return m_fastInterp.minY(); }

    /** The maximum parameter value for which the function is defined
     *
     *  @return The minimum valid paramter value
     */
    inline double maxX() const noexcept { return m_fastInterp.maxX(); }

    /** The function value for the minimum valid parameter
     *
     *  @return The function value at the minimum valid parameter
     */
    inline double maxY() const noexcept { return m_fastInterp.maxY(); }

    /** The status of the interpolator.
     *
     *  @return boolean indicating whether the interpolator was correctly initialised
     */
    inline bool valid() const noexcept { return m_OK; }

    /** Check whether a given x parameter is within the valid input data range
     *
     *  @param x The parameter value
     *
     *  @return boolean indicating if the parameter is in range
     */
    inline bool withinInputRange( const double x ) const noexcept
    {
      return checkLowerBound( x ) && checkUpperBound( x );
    }

    /// Access the number of data points
    inline std::size_t nDataPoints() const noexcept { return m_fastInterp.nDataPoints(); }

  public:

    /** Initialisation from arrays containing x and y values
     *
     *  Arrays must be of the same size and ordered such that entry i in each
     *  correspond to each other.
     *
     *  @param x         Array of x values
     *  @param y         Array of y values
     *  @param size      Number of data points
     */
    bool initInterpolator( const double           x[],
                           const double           y[],
                           const unsigned int     size,
                           const gsl_interp_type *interType = gsl_interp_linear );

    /** Initialisation from std::vectors containing x and y values
     *
     *  Vectors must be of the same size and ordered such that entry i in each
     *  correspond to each other.
     *
     *  @param x         Vector of x values
     *  @param y         Vector of y values
     */
    bool initInterpolator( const std::vector< double > &x,
                           const std::vector< double > &y,
                           const gsl_interp_type *      interType = gsl_interp_linear );

    /** Initialisation from a map of x,y values
     *
     *  @param data      map containing x(key) and y(data) values
     */
    bool initInterpolator( const std::map< double, double > &data,
                           const gsl_interp_type *           interType = gsl_interp_linear );

    /** Initialisation from a vector of a pair of x,y values
     *
     *  @param data      std::vector containing and pair of x(first) and y(second) values
     */
    bool initInterpolator( const std::vector< std::pair< double, double > > &data,
                           const gsl_interp_type *interType = gsl_interp_linear );

  protected: // methods

    /// clear the interpolator
    inline void clearInterpolator() { m_fastInterp.clear(); }

    /// Default initialise the interpolator
    void initInterpolator();

  private: // data

    // Fast linear interpolator
    LinearInterp< double > m_fastInterp;

  protected:

    /// Status flag
    bool m_OK = false;
  };

} // namespace Rich
