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
// ===========================================================================
// Include files
// ===========================================================================
// STD & STL
// ===========================================================================
#include <algorithm>
#include <type_traits>
// ===========================================================================
// local
// ===========================================================================
#include "LHCbMath/Interpolation.h"
#include "LHCbMath/LHCbMath.h"
#include "LHCbMath/Spline.h"
// ===========================================================================
// Boost
// ===========================================================================
#include "boost/multi_array.hpp"
// ===========================================================================
// ROOT
// ===========================================================================
#include "TAxis.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TH2.h"
// ===========================================================================
/** @file
 *  Implementation file for class Gaudi::Math::Spline
 *  @see Gaudi::Math::Spline
 *
 *  @date 2013-03-14
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *
 */
// ============================================================================
namespace {
  // ==========================================================================
  /// equality criteria for doubles
  LHCb::Math::Equal_To<double> s_equal; // equality criteria for doubles
  // ==========================================================================
  // check the match of basic types
  /// the type from GaudiGSL
  typedef Genfun::GaudiMathImplementation::SplineBase::Data2D _DATA_1_;
  /// locally defined type
  typedef Gaudi::Math::Splines::DATA _DATA_2_;
  static_assert( std::is_same<_DATA_1_, _DATA_2_>::value, "Data2D must be DATA" );
  // ==========================================================================
} // namespace
// ===========================================================================
// get the values from the histogram
// ===========================================================================
Gaudi::Math::Splines::DATA Gaudi::Math::Splines::getValues( const TH1& histo, const unsigned int begin,
                                                            const unsigned int end ) {
  //
  const TAxis*       axis  = histo.GetXaxis();
  const unsigned int nbins = std::min( (unsigned int)axis->GetNbins() + 1, end );
  //
  DATA result;
  result.reserve( nbins );
  //
  for ( unsigned int i = std::max( 1u, begin ); i < nbins; ++i ) {
    const double x = axis->GetBinCenter( i );
    const double y = histo.GetBinContent( i );
    result.push_back( std::make_pair( x, y ) );
  }
  //
  return result;
}
// ===========================================================================
// get the errors from the histogram
// ===========================================================================
Gaudi::Math::Splines::DATA Gaudi::Math::Splines::getErrors( const TH1& histo, const unsigned int begin,
                                                            const unsigned int end ) {
  //
  const TAxis*       axis  = histo.GetXaxis();
  const unsigned int nbins = std::min( (unsigned int)axis->GetNbins() + 1, end );
  //
  DATA result;
  result.reserve( nbins );
  //
  for ( unsigned int i = std::max( 1u, begin ); i <= nbins; ++i ) {
    const double x  = axis->GetBinCenter( i );
    const double ey = histo.GetBinError( i );
    result.push_back( std::make_pair( x, ey ) );
  }
  //
  return result;
}
// ===========================================================================
// get the data   from the histogram
// ===========================================================================
Gaudi::Math::Splines::DATAERR Gaudi::Math::Splines::getData( const TH1& histo, const unsigned int begin,
                                                             const unsigned int end ) {
  const TAxis*       axis  = histo.GetXaxis();
  const unsigned int nbins = std::min( (unsigned int)axis->GetNbins() + 1, end );
  //
  DATAERR result;
  result.reserve( nbins );
  //
  for ( unsigned int i = std::max( 1u, begin ); i <= nbins; ++i ) {
    const double x  = axis->GetBinCenter( i );
    const double y  = histo.GetBinContent( i );
    const double ey = histo.GetBinError( i );
    result.push_back( std::make_pair( x, Gaudi::Math::ValueWithError( y, ey * ey ) ) );
  }
  //
  return result;
}
// ===========================================================================
// get the values from data vector
// ===========================================================================
Gaudi::Math::Splines::DATA Gaudi::Math::Splines::getValues( const Gaudi::Math::Splines::DATAERR& data ) {
  DATA result;
  result.reserve( data.size() );
  //
  for ( DATAERR::const_iterator i = data.begin(); data.end() != i; ++i ) {
    const double x = i->first;
    const double y = i->second.value();
    result.push_back( std::make_pair( x, y ) );
  }
  //
  return result;
}
// ===========================================================================
// get the errors from data vector
// ===========================================================================
Gaudi::Math::Splines::DATA Gaudi::Math::Splines::getErrors( const Gaudi::Math::Splines::DATAERR& data ) {
  DATA result;
  result.reserve( data.size() );
  //
  for ( DATAERR::const_iterator i = data.begin(); data.end() != i; ++i ) {
    const double x  = i->first;
    const double ey = i->second.error();
    result.push_back( std::make_pair( x, ey ) );
  }
  //
  return result;
}
// ===========================================================================
// get the value from graph
// ===========================================================================
Gaudi::Math::Splines::DATA Gaudi::Math::Splines::getValues( const TGraph& graph, const unsigned int begin,
                                                            const unsigned int end ) {
  //
  const unsigned int npoints = std::min( (unsigned int)graph.GetN(), end );
  //
  DATA result;
  result.reserve( npoints );
  //
  for ( unsigned int i = begin; i < npoints; ++i ) {
    double x = 0;
    double y = 0;
    graph.GetPoint( i, x, y );
    //
    result.push_back( std::make_pair( x, y ) );
  }
  //
  return result;
}
// ===========================================================================
// get the errors from graph
// ===========================================================================
Gaudi::Math::Splines::DATA Gaudi::Math::Splines::getErrors( const TGraphErrors& graph, const unsigned int begin,
                                                            const unsigned int end ) {
  //
  const unsigned int npoints = std::min( (unsigned int)graph.GetN(), end );
  //
  DATA result;
  result.reserve( npoints );
  //
  for ( unsigned int i = begin; i < npoints; ++i ) {
    double x = 0;
    double y = 0;
    graph.GetPoint( i, x, y );
    const double ey = graph.GetErrorY( i );
    //
    result.push_back( std::make_pair( x, ey ) );
  }
  //
  return result;
}
// ===========================================================================
// get the data from graph
// ===========================================================================
Gaudi::Math::Splines::DATAERR Gaudi::Math::Splines::getData( const TGraphErrors& graph, const unsigned int begin,
                                                             const unsigned int end ) {
  const unsigned int npoints = std::min( (unsigned int)graph.GetN(), end );
  //
  DATAERR result;
  result.reserve( npoints );
  //
  for ( unsigned int i = begin; i < npoints; ++i ) {
    double x = 0;
    double y = 0;
    graph.GetPoint( i, x, y );
    const double ey = graph.GetErrorY( i );
    //
    result.push_back( std::make_pair( x, Gaudi::Math::ValueWithError( y, ey * ey ) ) );
  }
  //
  return result;
}
// ===========================================================================
/*  simple lagrange interpolation
 *  @attention it can be CPU-inefficient!
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2013-03-15
 */
// ===========================================================================
double Gaudi::Math::Splines::interpolate( const Gaudi::Math::Splines::DATA& data, const double x ) {
  //
  return Gaudi::Math::Interpolation::lagrange( data.begin(), data.end(), data.begin(), data.end(), x, 0.0,
                                               []( auto x ) { return x.first; }, []( auto y ) { return y.second; } );
}
// ===========================================================================
/*  simple lagrange interpolation
 *  @attention it can be CPU-inefficient!
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2013-03-15
 */
// ===========================================================================
Gaudi::Math::ValueWithError Gaudi::Math::Splines::interpolate( const Gaudi::Math::Splines::DATAERR& data,
                                                               const double                         x ) {
  //
  return Gaudi::Math::Interpolation::lagrange( data.begin(), data.end(), data.begin(), data.end(), x,
                                               Gaudi::Math::ValueWithError(), []( auto x ) { return x.first; },
                                               []( auto y ) { return y.second; } );
}
// ===========================================================================

// ===========================================================================
// simple spline
// ===========================================================================

// ===========================================================================
// get the value
// ===========================================================================
double Gaudi::Math::Spline::operator()( const double x ) const {
  // scale and shift the variable
  const double y = x * m_scale + m_shift;
  // check the limits
  if ( m_null && ( y < m_xmin || y > m_xmax ) ) { return 0; }
  //
  return m_spline->eval( y );
}

// ===========================================================================
// get the derivative
// ===========================================================================
double Gaudi::Math::Spline::derivative( const double x ) const {
  // scale and shift the variable
  const double y = x * m_scale + m_shift;
  // check the limits
  if ( m_null && ( y < m_xmin || y > m_xmax ) ) { return 0; }
  //
  return m_spline->deriv( y ) / m_scale;
}
// ===========================================================================
// get the second derivative
// ===========================================================================
double Gaudi::Math::Spline::derivative2( const double x ) const {
  // scale and shift the variable
  const double y = x * m_scale + m_shift;
  // check the limits
  if ( m_null && ( y < m_xmin || y > m_xmax ) ) { return 0; }
  //
  return m_spline->deriv2( y ) / m_scale;
}
// ===========================================================================
// get the integral
// ===========================================================================
double Gaudi::Math::Spline::integral( const double xmin, const double xmax ) const {
  //
  if ( xmin > xmax ) {
    return -integral( xmax, xmin );
  } else if ( s_equal( xmin, xmax ) ) {
    return 0;
  }
  //
  // scale and shift the variable
  double ymin = xmin * m_scale + m_shift;
  double ymax = xmax * m_scale + m_shift;
  //
  if ( s_equal( ymin, ymax ) ) { return 0; } // RETURN
  //
  if ( m_null ) {
    ymin = std::max( ymin, m_xmin );
    ymax = std::min( ymax, m_xmax );
  }
  //
  return m_spline->integ( ymin, ymax ) * m_scale;
}
// ===========================================================================
// get the integral
// ===========================================================================
double Gaudi::Math::Spline::integral() const { return m_spline->integ( m_xmin, m_xmax ) * m_scale; }
// ======================================================================
/*  Standard constructor
 *  @param data  data to be interpolated
 *  @param type  interpolation type
 *  @param null  set to null outside the range?
 *  @param scale the scale for the input variable
 *  @param shift the bias  for the input variable
 */
// ======================================================================
Gaudi::Math::Spline::Spline( const Gaudi::Math::Spline::DATA& data, const GaudiMath::Interpolation::Type& type,
                             const bool null, const double scale, const double shift )
    : m_null( null ), m_scale( scale ), m_shift( shift ), m_xmin( 0 ), m_xmax( 0 ), m_spline() {
  //
  Gaudi::Math::Splines::DATA _data( data );
  //
  std::sort( _data.begin(), _data.end() );
  //
  m_spline.reset( new SPLINE( _data, type ) );
  //
  m_xmin = data.front().first;
  m_xmax = data.back().first;
  //
}
// ======================================================================
/*  Standard constructor
 *  @param data  data to be interpolated
 *  @param type  interpolation type
 *  @param null  set to null outside the range?
 *  @param scale the scale for the input variable
 *  @param shift the bias  for the input variable
 */
// ======================================================================
Gaudi::Math::Spline::Spline( const Gaudi::Math::Spline::DATAERR& data, const GaudiMath::Interpolation::Type& type,
                             const bool null, const double scale, const double shift )
    : m_null( null ), m_scale( scale ), m_shift( shift ), m_xmin( 0 ), m_xmax( 0 ), m_spline() {
  //
  Gaudi::Math::Splines::DATA _data( Gaudi::Math::Splines::getValues( data ) );
  //
  std::sort( _data.begin(), _data.end() );
  //
  m_spline.reset( new SPLINE( _data, type ) );
  //
  m_xmin = _data.front().first;
  m_xmax = _data.back().first;
  //
}
// ===============================================================================
/*  Standard constructor
 *  @param histo the histogram to be interpolated
 *  @param type  interpolation type
 *  @param null  set to null outside the range?
 *  @param scale the scale for the input variable
 *  @param shift the bias  for the input variable
 */
// ============================================================================
Gaudi::Math::Spline::Spline( const TH1& histo, const GaudiMath::Interpolation::Type& type, const bool null,
                             const double scale, const double shift, const unsigned int begin, const unsigned int end )
    : m_null( null )
    , m_scale( scale )
    , m_shift( shift )
    , m_xmin( histo.GetXaxis()->GetXmin() )
    , m_xmax( histo.GetXaxis()->GetXmax() )
    , m_spline() {
  //
  SPLINE::Data2D data = Gaudi::Math::Splines::getValues( histo, begin, end );
  //
  m_spline.reset( new SPLINE( data, type ) );
  //
}
// ============================================================================
/*  Standard constructor
 *  @param graph the graph to be interpolated
 *  @param type  interpolation type
 *  @param scale the scale for the input variable
 *  @param shift the bias  for the input variable
 */
// ============================================================================
Gaudi::Math::Spline::Spline( const TGraph& graph, const GaudiMath::Interpolation::Type& type, const bool null,
                             const double scale, const double shift, const unsigned int begin, const unsigned int end )
    : m_null( null ), m_scale( scale ), m_shift( shift ), m_xmin( 0 ), m_xmax( 0 ), m_spline() {
  //
  SPLINE::Data2D data = Gaudi::Math::Splines::getValues( graph, begin, end );
  //
  m_spline.reset( new SPLINE( data, type ) );
  //
  const unsigned int np = graph.GetN();
  //
  double x = 0;
  double y = 0;
  //
  graph.GetPoint( 0, x, y );
  m_xmin = x;
  graph.GetPoint( np - 1, x, y );
  m_xmax = x;
  //
}
// ===========================================================================

// ===========================================================================
// Spline with errors
// ===========================================================================

// ===========================================================================
/*  Standard constructor
 *  @param data  data to be interpolated
 *  @param type  interpolation type
 *  @param null  set to null outside the range?
 *  @param scale the scale for the input variable
 *  @param shift the bias  for the input variable
 */
// ===========================================================================
Gaudi::Math::SplineErrors::SplineErrors( const Gaudi::Math::SplineErrors::DATAERR& data,
                                         const GaudiMath::Interpolation::Type& type, const bool null,
                                         const double scale, const double shift )
    : m_spline( Gaudi::Math::Splines::getValues( data ), type, null, scale, shift )
    , m_errors( Gaudi::Math::Splines::getErrors( data ), type, null, scale, shift ) {}
// ======================================================================
/*  Standard constructor
 *  @param histo the histogram to be interpolated
 *  @param type  interpolation type
 *  @param null  set to null outside the range?
 *  @param scale the scale for the input variable
 *  @param shift the bias  for the input variable
 */
// ======================================================================
Gaudi::Math::SplineErrors::SplineErrors( const TH1& histo, const GaudiMath::Interpolation::Type& type, const bool null,
                                         const double scale, const double shift, const unsigned int begin,
                                         const unsigned int end )
    : m_spline( histo, type, null, scale, shift, begin, end )
    , m_errors( Gaudi::Math::Splines::getErrors( histo, begin, end ), type, false, scale, shift ) {}
// ======================================================================
/*  Standard constructor
 *  @param histo the histogram to be interpolated
 *  @param type  interpolation type
 *  @param null  set to null outside the range?
 *  @param scale the scale for the input variable
 *  @param shift the bias  for the input variable
 */
// ======================================================================
Gaudi::Math::SplineErrors::SplineErrors( const TGraphErrors& graph, const GaudiMath::Interpolation::Type& type,
                                         const bool null, const double scale, const double shift,
                                         const unsigned int begin, const unsigned int end )
    : m_spline( graph, type, null, scale, shift, begin, end )
    , m_errors( Gaudi::Math::Splines::getErrors( graph, begin, end ), type, false, scale, shift ) {}
// ===========================================================================
// get the value
// ===========================================================================
Gaudi::Math::ValueWithError Gaudi::Math::SplineErrors::operator()( const double x ) const {
  const double v = m_spline( x );
  const double e = m_errors( x );
  return Gaudi::Math::ValueWithError( v, e * e );
}
// ==============================================================================
namespace {
  typedef Gaudi::Math::ValueWithError VE;
  // ============================================================================
  inline VE bin( const TH2& histo, const int ibinx, const int ibiny ) {
    const double v = histo.GetBinContent( ibinx, ibiny );
    const double e = histo.GetBinError( ibinx, ibiny );
    return VE( v, e * e );
  }
  // =========================================================================
  // interpolate 2D-histo along y-axis
  template <class AXIS>
  VE lagrange_y( const TH2&         histo,
                 const unsigned int ix, // fixed
                 const unsigned int y0, // first y-bin
                 const unsigned int ny, // number of bins in y
                 const AXIS& yaxis, const double y ) {
    //
    double value = 0;
    double error = 0;
    //
    for ( unsigned int iy = 0; iy < ny; ++iy ) {
      //
      const double yi = yaxis[iy];
      const VE     ve = bin( histo, ix, iy + y0 );
      //
      double L = 1;
      for ( unsigned int jy = 0; jy < ny; ++jy ) {
        //
        if ( iy == jy ) { continue; }
        //
        const double yj = yaxis[jy];
        //
        L *= ( y - yj ) / ( yi - yj );
      }
      //
      value += ve.value() * L;
      error += ve.error() * L;
    }
    //
    return VE( value, error * error );
  }
  // ==========================================================================
  // interpolate 2D-histo along x-axis
  template <class AXIS>
  VE lagrange_x( const TH2&         histo,
                 const unsigned int x0, // first  x-bin
                 const unsigned int nx, // number of x-bins
                 const unsigned int iy, // fixed
                 const AXIS& xaxis, const double x ) {
    //
    double value = 0;
    double error = 0;
    //
    for ( unsigned int ix = 0; ix < nx; ++ix ) {
      //
      const double xi = xaxis[ix];
      const VE     ve = bin( histo, ix + x0, iy );
      //
      double L = 1;
      for ( unsigned int jx = 0; jx < nx; ++jx ) {
        //
        if ( ix == jx ) { continue; }
        //
        const double xj = xaxis[jx];
        //
        L *= ( x - xj ) / ( xi - xj );
      }
      //
      value += ve.value() * L;
      error += ve.error() * L;
    }
    //
    return VE( value, error * error );
  }
} // namespace
// ============================================================================
/* Standard constructor
 *  @param data  data to be interpolated
 *  @param type  interpolation type
 *  @param null  set to null outside the range?
 *  @param scale the scale for the input variable
 *  @param shift the bias  for the input variable
 */
// ============================================================================
Gaudi::Math::Interp2D::Interp2D( const TH2& histo, const Type& typex, const Type& typey, const bool null,
                                 const double scalex, const double scaley, const double shiftx, const double shifty )
    : m_histo( &histo )
    , m_null( null )
    //
    , m_typex( typex )
    , m_typey( typey )
    , m_scalex( scalex )
    , m_scaley( scaley )
    , m_shiftx( shiftx )
    , m_shifty( shifty ) {
  const TAxis* xaxis = histo.GetXaxis();
  const TAxis* yaxis = histo.GetYaxis();
  //
  const int xbins = xaxis->GetNbins();
  const int ybins = yaxis->GetNbins();
  //
  if ( m_typex > Last ) { m_typex = Last; }
  if ( m_typey > Last ) { m_typey = Last; }
  if ( m_typex >= xbins ) { m_typex = Type( xbins - 1 ); }
  if ( m_typey >= ybins ) { m_typey = Type( ybins - 1 ); }
  if ( m_typex > Last ) { m_typex = Last; }
  if ( m_typey > Last ) { m_typey = Last; }
  if ( m_typex < Nearest ) { m_typex = Nearest; }
  if ( m_typey < Nearest ) { m_typey = Nearest; }
  //
}
// ============================================================================
// get the value
// ============================================================================
Gaudi::Math::ValueWithError Gaudi::Math::Interp2D::Interp2D::operator()( const double x, const double y ) const {
  //
  const double zx = x * m_scalex + m_shiftx;
  const double zy = y * m_scaley + m_shifty;
  //
  const TAxis* xaxis = m_histo->GetXaxis();
  const TAxis* yaxis = m_histo->GetYaxis();
  //
  const int xbins = xaxis->GetNbins();
  const int ybins = yaxis->GetNbins();
  //
  const int xbin = xaxis->FindFixBin( zx );
  const int ybin = yaxis->FindFixBin( zy );
  //
  if ( m_null ) {
    if ( 0 >= xbin || xbin > xbins ) { return 0; } // RETURN
    if ( 0 >= ybin || ybin > ybins ) { return 0; } // RETURN
  }
  //
  if ( Nearest == m_typex && Nearest == m_typey ) { return bin( *m_histo, xbin, ybin ); } // RETURN
  //
  // find the proper data range & fill the store
  //
  const double xc = xaxis->GetBinCenter( xbin );
  const double yc = yaxis->GetBinCenter( ybin );
  //
  int iStartX = xbin;
  int iStartY = ybin;
  if ( 1 == m_typex % 2 ) {
    const int leftx = zx < xc ? 1 : -1;
    iStartX -= ( m_typex + leftx ) / 2;
  } else {
    iStartX -= m_typex / 2;
  }
  //
  if ( 1 == m_typey % 2 ) {
    const int lefty = zy < yc ? 1 : -1;
    iStartY -= ( m_typey + lefty ) / 2;
  } else {
    iStartY -= m_typey / 2;
  }
  //
  iStartX = std::max( 1, iStartX );
  iStartY = std::max( 1, iStartY );
  //
  iStartX = std::min( xbins - m_typex, iStartX );
  iStartY = std::min( ybins - m_typey, iStartY );
  //
  typedef Gaudi::Math::Splines::DATAERR STORE;
  typedef std::vector<double>           AXIS;
  //
  STORE y_int( m_typex + 1 ); // intepolated Y-values for X-columns
  AXIS  y_axis( m_typey + 1 );
  //
  // fill Y-axis
  for ( int iy = 0; iy <= m_typey; ++iy ) { y_axis[iy] = yaxis->GetBinCenter( iy + iStartY ); }
  //
  // loop for column and perform 1D-interpolation for each column:
  for ( int ix = 0; ix <= m_typex; ++ix ) {
    //
    const double xi = xaxis->GetBinCenter( ix + iStartX );
    //
    // for each column find the interpolated value of "y"
    const VE ya = lagrange_y( *m_histo, iStartX + ix, iStartY, m_typey + 1, y_axis, zy );
    // fill the local store
    y_int[ix] = std::make_pair( xi, ya );
  }
  //
  // finally interpolate X
  return Gaudi::Math::Splines::interpolate( y_int, zx );
}
// ===========================================================================

// ===========================================================================
// few oversimplified cases
// ===========================================================================
/*  constructor from two points
 *  @param x1 the first  abscissa
 *  @param x2 the second abscissa
 *  @param y1 function value at x1
 *  @param y2 function value at x2
 */
// ===========================================================================
Gaudi::Math::Linear::Linear( const double x1, const double y1, const double x2, const double y2 )
    : m_a( ( y2 - y1 ) / ( x2 - x1 ) ), m_b( ( y1 * x2 - y2 * x1 ) / ( x2 - x1 ) ) {}
// ===========================================================================

// ===========================================================================
/*  constructor from two points
 *  @param x1 the first  abscissa
 *  @param y1 function value at x1
 *  @param x2 the second abscissa
 *  @param y2 function value at x2
 */
// ===========================================================================
Gaudi::Math::LinearErr::LinearErr( const double x1, const Gaudi::Math::ValueWithError& y1, const double x2,
                                   const Gaudi::Math::ValueWithError& y2 )
    : m_x1( x1 ), m_x2( x2 ), m_y1( y1 ), m_y2( y2 ) {}
// ============================================================================
//  y = a*x + b
// ============================================================================
double Gaudi::Math::LinearErr::a() const { return ( m_y2.value() - m_y1.value() ) / ( m_x2 - m_x1 ); }
// ============================================================================
// y = a*x + b
// ============================================================================
double Gaudi::Math::LinearErr::b() const { return ( m_y1.value() * m_x2 - m_y2.value() * m_x1 ) / ( m_x2 - m_x1 ); }
// ============================================================================
// get the value:
// ============================================================================
Gaudi::Math::ValueWithError Gaudi::Math::LinearErr::operator()( const double x ) const {
  return ( m_y2 * ( x - m_x1 ) - m_y1 * ( x - m_x2 ) ) / ( m_x2 - m_x1 );
}
// ============================================================================

// ============================================================================
/*  constructor from three points
 *  @param x1 the first  abscissa
 *  @param y1 function value at x1
 *  @param x2 the second abscissa
 *  @param y2 function value at x2
 *  @param x3 the third abscissa
 *  @param y3 function value at x3
 */
// ============================================================================
Gaudi::Math::Parabola::Parabola( const double x1, const double y1, const double x2, const double y2, const double x3,
                                 const double y3 )
    : m_a( 0 ), m_b( 0 ), m_c( 0 ) {
  //
  const double d12 = x1 - x2;
  const double d23 = x2 - x3;
  const double d13 = x1 - x3;
  //
  const double d1 = 1.0 / ( d12 * d13 );
  const double d2 = 1.0 / ( d12 * d23 );
  const double d3 = 1.0 / ( d13 * d23 );

  m_a = y1 * d1;
  m_a -= y2 * d2;
  m_a += y3 * d3;
  //
  m_b = -y1 * ( x2 + x3 ) * d1;
  m_b -= -y2 * ( x1 + x3 ) * d2;
  m_b += -y3 * ( x1 + x2 ) * d3;
  //
  m_c = y1 * x2 * x3 * d1;
  m_c -= y2 * x1 * x3 * d2;
  m_c += y3 * x1 * x2 * d3;
  //
}
// ============================================================================

// ============================================================================
/*  constructor from three points
 *  @param x1 the first  abscissa
 *  @param y1 function value at x1
 *  @param x2 the second abscissa
 *  @param y2 function value at x2
 *  @param x3 the third abscissa
 *  @param y3 function value at x3
 */
// ============================================================================
Gaudi::Math::ParabolaErr::ParabolaErr( const double x1, const Gaudi::Math::ValueWithError& y1, const double x2,
                                       const Gaudi::Math::ValueWithError& y2, const double x3,
                                       const Gaudi::Math::ValueWithError& y3 )
    : m_x1( x1 ), m_x2( x2 ), m_x3( x3 ), m_y1( y1 ), m_y2( y2 ), m_y3( y3 ), m_d1(), m_d2(), m_d3() {
  const double d12 = x1 - x2;
  const double d23 = x2 - x3;
  const double d13 = x1 - x3;
  //
  m_d1 = 1.0 / ( d12 * d13 );
  m_d2 = 1.0 / ( d12 * d23 );
  m_d3 = 1.0 / ( d13 * d23 );
  //
}
// ============================================================================
// y = a*x^2 + b*x + c
// ============================================================================
double Gaudi::Math::ParabolaErr::a() const {
  double _a = m_y1.value() * m_d1;
  _a -= m_y2.value() * m_d2;
  _a += m_y3.value() * m_d3;
  return _a;
}
// ============================================================================
// y = a*x^2 + b*x + c
// ============================================================================
double Gaudi::Math::ParabolaErr::b() const {
  double _b = -m_y1.value() * ( ( m_x2 + m_x3 ) * m_d1 );
  _b -= -m_y2.value() * ( ( m_x1 + m_x3 ) * m_d2 );
  _b += -m_y3.value() * ( ( m_x1 + m_x2 ) * m_d3 );
  return _b;
}
// ============================================================================
// y = a*x^2 + b*x + c
// ============================================================================
double Gaudi::Math::ParabolaErr::c() const {
  double _c = m_y1.value() * ( m_x2 * m_x3 * m_d1 );
  _c -= m_y2.value() * ( m_x1 * m_x3 * m_d2 );
  _c += m_y3.value() * ( m_x1 * m_x2 * m_d3 );
  return _c;
}
// ============================================================================
// get the value:
// ============================================================================
Gaudi::Math::ValueWithError Gaudi::Math::ParabolaErr::operator()( const double x ) const {
  //
  const double d1 = x - m_x1;
  const double d2 = x - m_x2;
  const double d3 = x - m_x3;
  //
  Gaudi::Math::ValueWithError result;
  result = m_y1 * ( d2 * d3 * m_d1 );
  result -= m_y2 * ( d1 * d3 * m_d2 );
  result += m_y3 * ( d1 * d2 * m_d3 );
  //
  return result;
}
// ============================================================================

// ===========================================================================
// The END
// ===========================================================================
