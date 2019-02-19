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
// ============================================================================
// Include files
// ============================================================================
// STD& STL
// ============================================================================
#include <array>
#include <cassert>
#include <climits>
#include <numeric>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/GaudiException.h"
// ============================================================================
// LHCbMath
// ============================================================================
#include "LHCbMath/Bernstein.h"
#include "LHCbMath/Choose.h"
#include "LHCbMath/LHCbMath.h"
#include "LHCbMath/MoreFunctions.h"
#include "LHCbMath/Polynomials.h"
#include "LHCbMath/Power.h"
// ============================================================================
/** @file
 *  Implementation file for functions, related to Bernstein's polynomnials
 *
 *  @see http://en.wikipedia.org/wiki/Bernstein_polynomial
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2010-04-19
 */
// ============================================================================
namespace {
  // ==========================================================================
  /// equality criteria for doubles
  const LHCb::Math::Equal_To<double> s_equal{}; // equality criteria for doubles
  /// zero for doubles
  const LHCb::Math::Zero<double> s_zero{}; // zero for doubles
  /// zero fo vectors
  const LHCb::Math::Zero<std::vector<double>> s_vzero{}; // zero for vectors
  ///
  static_assert( std::numeric_limits<double>::is_specialized, "std::numeric_limits<double> is not specialized" );
  static_assert( std::numeric_limits<long double>::is_specialized,
                 "std::numeric_limits<long double> is not specialized" );
  /// small value
  const LHCb::Math::Small<long double> s_small( 2.0L * std::numeric_limits<double>::epsilon() );
  // ==========================================================================
  // De Casteljau's algorithm
  template <class ITERATOR>
  long double _casteljau_( ITERATOR first, ITERATOR last, const long double t0, const long double t1 ) {
    // the trivial cases
    if ( first == last ) { return 0; }
    //
    const std::size_t len = std::distance( first, last );
    //
    if ( 1 == len ) {
      return *first;
    } else if ( 2 == len ) {
      return t1 * ( *first ) + t0 * ( *( first + 1 ) );
    }
    //
    ITERATOR second = --last;
    //
    // prepare recursion
    for ( ITERATOR it = first; it != second; ++it ) { *it = t1 * ( *it ) + t0 * ( *( it + 1 ) ); }
    //
    // recursion
    return _casteljau_( first, second, t0, t1 );
  }
  // ==========================================================================
} // namespace
// ============================================================================
// constructor from the order
// ============================================================================
Gaudi::Math::Bernstein::Bernstein( const unsigned short N, const double xmin, const double xmax )
    : Gaudi::Math::PolySum( N ), m_xmin( std::min( xmin, xmax ) ), m_xmax( std::max( xmin, xmax ) ) {}
// ============================================================================
// constructor from the order
// ============================================================================
Gaudi::Math::Bernstein::Bernstein( const std::vector<double>& pars, const double xmin, const double xmax )
    : Gaudi::Math::PolySum( pars ), m_xmin( std::min( xmin, xmax ) ), m_xmax( std::max( xmin, xmax ) ) {}
// ============================================================================
namespace {
  // ==========================================================================
  inline double _mjk_( const unsigned short j, const unsigned short k, const unsigned short n,
                       Gaudi::Math::Bernstein& ba, Gaudi::Math::Bernstein& bb, const double abar, const double bbar ) {
    if ( j > n || k > n ) { return 0; }
    //
    const unsigned short imin = j + k <= n ? 0 : ( j + k - n );
    const unsigned short imax = std::min( j, k );
    //
    double m = 0;
    for ( unsigned short i = imin; i <= imax; ++i ) {
      ba.setPar( k - i, 1 );
      bb.setPar( i, 1 );
      m += ba.evaluate( abar ) * bb.evaluate( bbar );
      ba.setPar( k - i, 0 );
      bb.setPar( i, 0 );
    }
    return m;
  }
} // namespace
// ============================================================================
// constructor  from Bernstein polynomial from *different* domain
// ============================================================================
Gaudi::Math::Bernstein::Bernstein( const Gaudi::Math::Bernstein& poly, const double xmin, const double xmax )
    : Gaudi::Math::PolySum( poly ), m_xmin( std::min( xmin, xmax ) ), m_xmax( std::max( xmin, xmax ) ) {
  // recalculate domain ?
  if ( !s_equal( this->xmin(), poly.xmin() ) || !s_equal( this->xmax(), poly.xmax() ) ) {
    //
    std::vector<double> new_pars( npars(), 0 );
    //
    const double a    = poly.xmin();
    const double b    = poly.xmax();
    const double abar = this->xmin();
    const double bbar = this->xmax();
    //
    const unsigned short N = degree();
    for ( unsigned short j = 0; j <= N; ++j ) {
      //
      Gaudi::Math::Bernstein ba( N - j, a, b );
      Gaudi::Math::Bernstein bb( j, a, b );
      //
      for ( unsigned short k = 0; k <= N; ++k ) { new_pars[j] += _mjk_( j, k, N, ba, bb, abar, bbar ) * par( k ); }
    }
    //
    for ( unsigned short k = 0; k <= N; ++k ) { setPar( k, new_pars[k] ); }
  }
}
// ============================================================================
// construct the basic bernstein polinomial
// ============================================================================
Gaudi::Math::Bernstein::Bernstein( const Gaudi::Math::Bernstein::Basic& bb, const double xmin, const double xmax )
    : Gaudi::Math::PolySum( bb.N() )
    , m_xmin( std::min( xmin, xmax ) )
    , m_xmax( std::max( xmin, xmax ) )
//
{
  if ( bb.k() <= bb.N() ) { m_pars[bb.k()] = 1; }
}
// ============================================================================
/*  construct Bernstein interpolant
 *  @param x    vector of abscissas
 *  @param y    vector of function values
 *  @param xmin low  edge for Bernstein polynomial
 *  @param xmin high edge for Bernstein polynomial
 *  - if vector of y is longer  than vector x, extra values are ignored
 *  - if vector of y is shorter than vector x, missing entries are assumed to be zero
 *  It relies on Newton-Bernstein algorithm
 *  @see http://arxiv.org/abs/1510.09197
 *  @see Mark Ainsworth and Manuel A. Sanches,
 *       "Computing of Bezier control points of Largangian interpolant
 *       in arbitrary dimension", arXiv:1510.09197 [math.NA]
 *  @see http://adsabs.harvard.edu/abs/2015arXiv151009197A
 */
// ============================================================================
Gaudi::Math::Bernstein::Bernstein( const std::vector<double>& x, const std::vector<double>& y, const double xmin,
                                   const double xmax )
    : Bernstein( x.begin(), x.end(), y.begin(), y.end(), xmin, xmax ) {}
// ============================================================================
/* construct Bernstein polynomial from its roots
 *  Polinomial has a form
 *  \f$ B(x) = \prod_i (x-r_i) \prod_j (x-c_i)(x-c_i^*) \f$
 *  @param xmin low  edge for Bernstein polynomial
 *  @param xmax high edge for Bernstein polynomial
 *  @param r  the list of real  roots of the polinomial
 *  @param c  the list of complex roots (only one root from cc-pair is needed)
 */
// ============================================================================
Gaudi::Math::Bernstein::Bernstein( const double xmin, const double xmax, const std::vector<double>& r,
                                   const std::vector<std::complex<double>>& c )
    : Gaudi::Math::PolySum( r.size() + 2 * c.size() )
    , m_xmin( std::min( xmin, xmax ) )
    , m_xmax( std::max( xmin, xmax ) ) {
  //
  Bernstein result( std::vector<double>( 1, 1.0 ), xmin, xmax );
  Bernstein b1( std::vector<double>( 2, 1.0 ), xmin, xmax );
  Bernstein b2( std::vector<double>( 3, 1.0 ), xmin, xmax );
  for ( double rr : r ) {
    const double dmn = m_xmin - rr;
    const double dmx = m_xmax - rr;
    if ( s_zero( dmn ) ) {
      b1.setPar( 0, 0 );
      b1.setPar( 1, 1 );
    } else if ( s_zero( dmx ) ) {
      b1.setPar( 0, 1 );
      b1.setPar( 1, 0 );
    } else {
      b1.setPar( 0, dmn );
      b1.setPar( 1, dmx );
    }
    result = result * b1;
  }
  const double xmid = 0.5 * ( m_xmin + m_xmax );
  for ( std::complex<double> cr : c ) {
    //  a * x * x + bx + c
    const double a = 1;
    const double b = -2 * cr.real();
    const double c = std::norm( cr );
    //
    const double a0 = c + m_xmin * ( b + m_xmin * a );
    const double a1 = c + xmid * ( b + xmid * a );
    const double a2 = c + m_xmax * ( b + m_xmax * a );
    //
    b2.setPar( 0, a0 );
    b2.setPar( 1, 2 * a1 - 0.5 * ( a0 + a2 ) );
    b2.setPar( 2, a2 );
    //
    result = result * b2;
  }
  //
  this->m_pars = std::move( result.m_pars );
  //
  // scale it
  const short sf = LHCb::Math::frexp2( norm() ).second;
  LHCb::Math::scale_exp2( m_pars, -sf + 1 );
}
// ============================================================================
/*  construct Bernstein polynomial from its roots
 *
 *  Polinomial has a form
 *  \f$ B(x) = \prod_i (x-r_i) \prod_j (x-c_i)(x-c_i^*) \f$
 *
 *  @param xmin low  edge for Bernstein polynomial
 *  @param xmax high edge for Bernstein polynomial
 *  @param c  the list of complex roots (only one root from cc-pair is needed)
 *  @param r  the list of real  roots of the polinomial
 */
// ============================================================================
Gaudi::Math::Bernstein::Bernstein( const double xmin, const double xmax, const std::vector<std::complex<double>>& c,
                                   const std::vector<double>& r )
    : Gaudi::Math::Bernstein::Bernstein( xmin, xmax, r, c ) {}
// ============================================================================
// copy assignement
// ============================================================================
Gaudi::Math::Bernstein& Gaudi::Math::Bernstein::operator=( const Gaudi::Math::Bernstein& right ) {
  if ( &right == this ) { return *this; }
  m_xmin                        = right.m_xmin;
  m_xmax                        = right.m_xmax;
  Gaudi::Math::PolySum::operator=( right );
  return *this;
}
// ============================================================================
// move assignement
// ============================================================================
Gaudi::Math::Bernstein& Gaudi::Math::Bernstein::operator=( Gaudi::Math::Bernstein&& right ) {
  if ( &right == this ) { return *this; }
  m_xmin                        = right.m_xmin;
  m_xmax                        = right.m_xmax;
  Gaudi::Math::PolySum::operator=( std::move( right ) );
  return *this;
}
// ============================================================================
// assignement from the constant
// ============================================================================
Gaudi::Math::Bernstein& Gaudi::Math::Bernstein::operator=( const double right ) {
  std::fill( m_pars.begin(), m_pars.end(), s_zero( right ) ? 0.0 : right );
  return *this;
}
// ============================================================================
// all coefficients are so small that  c+p == c ?
// ============================================================================
bool Gaudi::Math::Bernstein::small( const double c ) const {
  const static LHCb::Math::MuchSmaller<double> s_much_smaller{};
  return s_much_smaller( norm(), c );
}
// ============================================================================
// is it a increasing function?
// ============================================================================
bool Gaudi::Math::Bernstein::increasing() const {
  if ( m_pars.size() <= 1 ) { return true; }
  for ( std::vector<double>::const_iterator it = m_pars.begin() + 1; m_pars.end() != it; ++it ) {
    if ( ( *( it - 1 ) ) > ( *it ) && !s_equal( *( it - 1 ), *it ) ) { return false; }
  }
  return true;
}
// ============================================================================
// is it a decreasing function?
// ============================================================================
bool Gaudi::Math::Bernstein::decreasing() const {
  if ( m_pars.size() <= 1 ) { return true; }
  for ( std::vector<double>::const_iterator it = m_pars.begin() + 1; m_pars.end() != it; ++it ) {
    if ( ( *( it - 1 ) ) < ( *it ) && !s_equal( *( it - 1 ), *it ) ) { return false; }
  }
  return true;
}
// ============================================================================
// is it a constant function?
// ============================================================================
bool Gaudi::Math::Bernstein::constant() const {
  //
  if ( m_pars.size() <= 1 ) { return true; }
  for ( std::vector<double>::const_iterator it = m_pars.begin() + 1; m_pars.end() != it; ++it ) {
    if ( !s_equal( *( it - 1 ), *it ) ) { return false; }
  }
  //
  return true;
}
// ============================================================================
// get the integral between xmin and xmax
// ============================================================================
double Gaudi::Math::Bernstein::integral() const {
  return ( m_xmax - m_xmin ) * std::accumulate( m_pars.begin(), m_pars.end(), 0.0 ) / npars();
}
// ============================================================================
/** filter out very small terms
 *  the term is considered to be very small if
 *   - it is numerically zero
 *   - or if epsilon > 0,
 *          abs ( c(k) ) < epsilon
 *   - or if scale   > 0  ,
 *           scale + par ==  scale
 *   - or if scale   <= 0 ,
 *           norm  + pars == norm
 *  Since the maximum value for each term of
 *  \f$ c_k C^n_k \frac{ k^k (n-k)^{n-k}}{ n^n}\f$
 *  @param  epsilon  parameter to define "smalness" of terms
 *  @param  scale    parameter to define "smalness" of terms
 *  @return number of nullified terms
 */
// ============================================================================
unsigned short Gaudi::Math::Bernstein::remove_noise( const double epsilon, const double scale ) {
  unsigned short       num = 0;
  const unsigned short N   = degree();
  const bool           eps = 0 < epsilon;
  const double         n   = norm();
  for ( unsigned short k = 0; k <= N; ++k ) {
    if ( s_zero( m_pars[k] ) ) {
      m_pars[k] = 0;
      ++num;
    } else if ( eps && ( 0 == k ) && std::abs( m_pars[k] ) < epsilon ) {
      m_pars[k] = 0;
      ++num;
    } else if ( eps && ( N == k ) && std::abs( m_pars[k] ) < epsilon ) {
      m_pars[k] = 0;
      ++num;
    } else if ( 0 < scale && s_equal( scale + m_pars[k], scale ) ) {
      m_pars[k] = 0;
      ++num;
    } else if ( 0 >= scale && s_equal( n + m_pars[k], n ) ) {
      m_pars[k] = 0;
      ++num;
    }
  }
  return num;
}
// ============================================================================
/*  get indefinite integral  as function object
 *  \f$ I(x) = \int^{x}_{x_{min}} B(t) dt + C \f$
 *  @param C the integration constant
 */
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::Bernstein::indefinite_integral( const double C ) const {
  //
  std::vector<long double> ck( npars() + 1, 0.0 );
  std::partial_sum( m_pars.begin(), m_pars.end(), ck.begin() + 1 );
  Gaudi::Math::scale( ck, ( m_xmax - m_xmin ) / npars() );
  //
  // add the integration constant
  if ( !s_zero( C ) ) {
    for ( std::vector<long double>::iterator ic = ck.begin(); ck.end() != ic; ++ic ) { ( *ic ) += C; }
  }
  //
  return Gaudi::Math::Bernstein( ck.begin(), ck.end(), m_xmin, m_xmax );
}
// ============================================================================
double Gaudi::Math::Bernstein::integral( const double low, const double high ) const {
  //
  if ( s_equal( low, high ) ) {
    return 0;
  } else if ( low > high ) {
    return -1 * integral( high, low );
  } else if ( high <= xmin() || low >= xmax() ) {
    return 0;
  } else if ( s_vzero( m_pars ) ) {
    return 0;
  } else if ( s_equal( low, m_xmin ) && s_equal( high, m_xmax ) ) {
    return integral();
  }
  //
  const double xlow  = std::max( low, m_xmin );
  const double xhigh = std::min( high, m_xmax );
  if ( xlow > xhigh ) { return 0; }
  //
  if ( 1 == npars() ) { return ( xhigh - xlow ) * m_pars[0]; }
  //
  if ( s_equal( xlow, m_xmin ) && s_equal( xhigh, m_xmax ) ) { return integral(); }
  //
  // make integration:
  //
  std::vector<long double> ck( npars() + 1, 0.0 );
  std::partial_sum( m_pars.begin(), m_pars.end(), ck.begin() + 1 );
  Gaudi::Math::scale( ck, ( m_xmax - m_xmin ) / npars() );
  //
  const Gaudi::Math::Bernstein b_int( ck.begin(), ck.end(), m_xmin, m_xmax );
  //
  return b_int( xhigh ) - b_int( xlow );
}
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::Bernstein::derivative() const {
  //
  std::vector<long double> ck( npars(), 0 );
  std::adjacent_difference( m_pars.begin(), m_pars.end(), ck.begin() );
  Gaudi::Math::scale( ck, ( npars() - 1 ) / ( m_xmax - m_xmin ) );
  //
  return Gaudi::Math::Bernstein( ck.begin() + 1, ck.end(), m_xmin, m_xmax );
}
// ============================================================================
double Gaudi::Math::Bernstein::derivative( const double x ) const {
  if ( m_pars.size() <= 1 ) {
    return 0;
  } else if ( x < m_xmin || x > m_xmax ) {
    return 0;
  }
  //
  std::vector<long double> ck( npars(), 0 );
  std::adjacent_difference( m_pars.begin(), m_pars.end(), ck.begin() );
  //
  // get the t-values
  //
  const double t0 = t( x );
  const double t1 = 1 - t0;
  //
  return _casteljau_( ck.begin() + 1, ck.end(), t0, t1 ) * ( npars() - 1 ) / ( m_xmax - m_xmin );
}
// ============================================================================
// get the value
// ============================================================================
double Gaudi::Math::Bernstein::evaluate( const double x ) const {
  //
  // treat the trivial cases
  //
  if ( m_pars.empty() ) {
    return 0;
  }
  //  needed for the proper integration with an exponential
  else if ( s_equal( x, m_xmin ) ) {
    return m_pars[0];
  } else if ( s_equal( x, m_xmax ) ) {
    return m_pars.back();
  } else if ( 1 == npars() ) {
    return m_pars[0];
  } else if ( s_vzero( m_pars ) ) {
    return 0;
  }
  //
  // get the t-values
  //
  const long double t0 = t( x );
  const long double t1 = 1 - t0;
  //
  // start de casteljau algorithm:
  //
  // use fixed size:
  if ( npars() < 16 ) {
    std::array<long double, 16> _pars;
    std::copy( m_pars.begin(), m_pars.end(), _pars.begin() );
    return _casteljau_( _pars.begin(), _pars.begin() + npars(), t0, t1 );
  }
  // generic case:
  std::vector<long double> dcj( m_pars.begin(), m_pars.end() );
  return _casteljau_( dcj.begin(), dcj.end(), t0, t1 );
}
// ============================================================================
Gaudi::Math::Bernstein& Gaudi::Math::Bernstein::operator+=( const double a ) {
  if ( s_zero( a ) ) { return *this; }
  LHCb::Math::shift( m_pars, a );
  return *this;
}
// ============================================================================
Gaudi::Math::Bernstein& Gaudi::Math::Bernstein::operator*=( const double a ) {
  if ( s_equal( a, 1 ) ) {
    return *this;
  } else if ( s_zero( a ) ) {
    std::fill( m_pars.begin(), m_pars.end(), 0 );
  }
  LHCb::Math::scale( m_pars, a );
  return *this;
}
// ============================================================================
Gaudi::Math::Bernstein& Gaudi::Math::Bernstein::operator-=( const double a ) {
  if ( s_zero( a ) ) { return *this; }
  LHCb::Math::shift( m_pars, -a );
  return *this;
}
// ============================================================================
Gaudi::Math::Bernstein& Gaudi::Math::Bernstein::operator/=( const double a ) {
  if ( s_equal( a, 1 ) ) { return *this; }
  LHCb::Math::scale( m_pars, 1 / a );
  return *this;
}
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::Bernstein::operator-() const {
  Bernstein b( *this );
  LHCb::Math::negate( b.m_pars );
  return b;
}
// ============================================================================
// Sum of Bernstein polynomial and a constant
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::Bernstein::__add__( const double value ) const { return ( *this ) + value; }
// ============================================================================
// Sum of Bernstein polynomial and a constant
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::Bernstein::__radd__( const double value ) const { return value + ( *this ); }
// ============================================================================
// Product of Bernstein polynomial and a constant
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::Bernstein::__mul__( const double value ) const { return ( *this ) * value; }
// ============================================================================
// Product of Bernstein polynomial and a constant
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::Bernstein::__rmul__( const double value ) const { return value * ( *this ); }
// ============================================================================
// Subtract a constant from Benrstein polynomial
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::Bernstein::__sub__( const double value ) const { return ( *this ) - value; }
// ============================================================================
// Subtract Bernstein polynomial from a constant
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::Bernstein::__rsub__( const double value ) const { return value - ( *this ); }
// ============================================================================
// Divide Benrstein polynomial by a constant
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::Bernstein::__div__( const double value ) const { return ( *this ) / value; }
// ============================================================================
// Negate Bernstein polynomial
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::Bernstein::__neg__() const { return -( *this ); }
// ============================================================================
// the sum two Bernstein polynomials
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::Bernstein::sum( const Gaudi::Math::Bernstein& other ) const {
  if ( this == &other ) {
    Bernstein result( *this );
    result *= 2;
    return result;
  }
  //
  if ( !s_equal( xmin(), other.xmin() ) || !s_equal( xmax(), other.xmax() ) ) {
    const double x_min = std::min( xmin(), other.xmin() );
    const double x_max = std::max( xmax(), other.xmax() );
    Bernstein    b1( *this, x_min, x_max );
    Bernstein    b2( other, x_min, x_max );
    return b1.sum( b2 );
  }
  //
  if ( degree() < other.degree() ) { return other.sum( this->elevate( other.degree() - degree() ) ); }
  if ( degree() > other.degree() ) { return sum( other.elevate( degree() - other.degree() ) ); }
  //
  Bernstein result( *this );
  for ( unsigned short i = 0; i < npars(); ++i ) { result.m_pars[i] += other.par( i ); }
  return result;
}
// =============================================================================
// subtract Bernstein polynomials (with the same domain!)
// =============================================================================
Gaudi::Math::Bernstein Gaudi::Math::Bernstein::subtract( const Gaudi::Math::Bernstein& other ) const {
  if ( this == &other ) { return Bernstein( degree(), xmin(), xmax() ); }
  Bernstein b( other );
  LHCb::Math::negate( b.m_pars );
  return sum( b );
}
// ============================================================================
// Sum of Bernstein polynomials (the same domain)
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::Bernstein::__add__( const Gaudi::Math::Bernstein& other ) const {
  return sum( other );
}
// ============================================================================
// Subtraction of Bernstein polynomials (the same domain)
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::Bernstein::__sub__( const Gaudi::Math::Bernstein& other ) const {
  return subtract( other );
}
// ============================================================================
// Multipky twp Bernstein polynomials (the same domain)
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::Bernstein::__mul__( const Gaudi::Math::Bernstein& other ) const {
  return multiply( other );
}
// ============================================================================
// swap  two Bernstein polynomials
// ============================================================================
void Gaudi::Math::Bernstein::swap( Bernstein& right ) {
  std::swap( m_xmin, right.m_xmin );
  std::swap( m_xmax, right.m_xmax );
  Gaudi::Math::PolySum::swap( right );
}
// ============================================================================
namespace {
  // ==========================================================================
  inline long double c_nk( const unsigned short n, const unsigned short k ) {
    return n < 63 ? Gaudi::Math::choose( n, k ) : Gaudi::Math::choose_double( n, k );
  }
  // ==========================================================================
} // namespace
// ============================================================================
/*  elevate it:
 *  represent as Bernstein polynomial of order N+r
 *  @param r  INPUT increase of degree
 *  @return new polynomial of order N+r
 */
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::Bernstein::elevate( const unsigned short r ) const {
  // no need in elevation
  if ( 0 == r ) { return *this; }
  //
  std::vector<long double>   _nc( npars() + r ); // new coefficients
  const std::vector<double>& _oc = pars();       // old coefficients
  // copy it
  const unsigned short N = degree();
  //
  std::copy( _oc.begin(), _oc.end(), _nc.begin() );
  std::fill( _nc.begin() + _oc.size(), _nc.end(), _oc.back() );
  //
  // repeate the elevation cycles:
  for ( unsigned short n = N; n < N + r; ++n ) {
    // "current" degree
    for ( unsigned short k = n; 1 <= k; --k ) {
      _nc[k] = ( n + 1 - k ) * _nc[k] + k * _nc[k - 1];
      _nc[k] /= n + 1;
    }
  }
  //
  return Bernstein( _nc.begin(), _nc.end(), xmin(), xmax() );
}
// ============================================================================
/*  reduce it
 *  represent as Bernstein polynomial of order N-r
 *  @param r  INPUT increase of degree
 *  @return new polynomial of order N-r
 */
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::Bernstein::reduce( const unsigned short r ) const {
  // no need in reducing
  if ( 0 == r ) { return *this; }
  //
  const unsigned short n    = degree();
  const unsigned short newd = r <= n ? n - r : 0;
  //
  const std::vector<double>& oc = pars();                // old coefficients
  std::vector<long double>   nc( oc.begin(), oc.end() ); // new coefficients
  //
  for ( unsigned short n = degree(); newd < n; --n ) {
    for ( unsigned short k = 1; k < n; ++k ) { nc[k] = ( n * nc[k] - k * nc[k - 1] ) / ( n - k ); }
  }
  return Bernstein( nc.begin(), nc.begin() + newd + 1, xmin(), xmax() );
}
// ============================================================================
namespace {
  // ==========================================================================
  /// get p-norm  of  vector of leading coefficients for bernstein basis
  double _p_norm_phi_( const unsigned short N, const double pinv ) {
    /// check i/p
    const double ip = pinv < 0 ? 0 : pinv > 1 ? 1 : pinv;
    ///
    if ( s_zero( ip ) ) {
      return N < 63 ? Gaudi::Math::choose( N, ( N + 1 ) / 2 ) : Gaudi::Math::choose_double( N, ( N + 1 ) / 2 );
    }
    ///
    const double p = 1 / ip;
    //
    // relatively small integer?
    //
    const bool           p_half = s_equal( ip, 0.5 );
    const bool           p_int  = 0.06 < ip && LHCb::Math::isint( p );
    const unsigned short p_i    = p_int ? LHCb::Math::round( p ) : 0;
    ///
    long double r = 1;
    long double c = 1;
    for ( unsigned short k = 1; k <= N; ++k ) {
      c *= ( N - k + 1 );
      c /= k;
      r += 1 == ip ? c : p_half ? Gaudi::Math::pow( c, 2 ) : p_int ? Gaudi::Math::pow( c, p_i ) : std::pow( c, p );
    }
    //
    return 1 == ip ? r : p_half ? std::sqrt( r ) : std::pow( r, ip );
  }
  // ==========================================================================
  template <class ITERATOR>
  inline double _head_( ITERATOR first, ITERATOR last ) {
    if ( first == last ) { return 0; }
    const unsigned short N = std::distance( first, last ) - 1;
    //
    long double  c = 1;
    unsigned int i = 0;
    double       h = 0;
    --last;
    const unsigned int N2 = N / 2 + 1;
    for ( ; i < N2; ++first, --last, ++i ) {
      if ( 0 < i ) {
        c *= ( N + 1 - i );
        c /= i;
      }
      if ( first == last ) {
        h += i % 2 ? c * ( *first ) : -c * ( *first );
        break;
      }
      h += i % 2 ? c * ( *first ) : -c * ( *first );
      h += ( N - i ) % 2 ? c * ( *last ) : -c * ( *last );
    }
    return h * ( 0 == N % 2 ? -1 : 1 );
  }
  // ==========================================================================
  inline long double _head_( const std::vector<double>& pars ) { return _head_( pars.begin(), pars.end() ); }
  // ==========================================================================
  inline long double _head_( const Gaudi::Math::Bernstein& b ) { return _head_( b.pars() ); }
  // ==========================================================================
} // namespace
// ============================================================================
/*  calculate ``nearest'' polynomial (in the sense of q-norm) of lower degree,
 *  where q-norm is defined as:
 *  \f$ \left| f \right|_{q} = \left( \sum_i \left|c_i\right|^q\right)^{\frac{1}{q}} \f$
 *
 *  - q_inv = 0.0 ->  \f$ max_k    \left|c_k\right|  \f$
 *  - q_inv = 0.5 ->  \f$ \sqrt{ \sum_k  c_k^2 }     \f$
 *  - q_inv = 1.0 ->  \f$ \sum_k \left| c_k \right|  \f$
 *  @see  N.Rezvani and R.M. Corless,
 *       "The Nearest Polynomial With A Given Zero, Revisited"
 *        ACM SIGSAM Bulletin, Vol. 39, No. 3, September 2005
 *  @see http://dl.acm.org/citation.cfm?doid=1113439.1113442
 */
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::Bernstein::nearest( const double qinv ) const {
  /// get the norm
  const double iq = 0 > qinv ? 0 : 1 < qinv ? 1 : qinv;
  //
  const double ip = 1 - iq;
  //
  // leading coefficients of the basis
  //
  std::vector<long double> lc_phi( m_pars.size() );
  lc_phi[0]              = 1;
  const unsigned short N = m_pars.size();
  for ( unsigned short i = 1; i < N; ++i ) { lc_phi[i] = ( lc_phi[i - 1] * ( N - i ) ) / i; }
  for ( unsigned short i = 1; i < N; i += 2 ) { lc_phi[i] *= -1; }
  //
  const double i_norm = 1 / _p_norm_phi_( degree(), ip );
  //
  const long double lc_f = _head_( m_pars.begin(), m_pars.end() );
  //
  if ( s_zero( lc_f ) ) { return *this; }
  //
  std::vector<long double> v( lc_phi );
  //
  for ( unsigned short i = 0; i < N; ++i ) {
    v[i] *= i_norm;
    if ( 0 == ip && 1 == N % 2 && i == ( N - 1 ) / 2 ) {
    } else if ( 0 == ip && 0 == N % 2 && ( i == N / 2 || i == N / 2 - 1 ) ) {
      v[i] /= 2;
    } else if ( !s_zero( v[i] ) ) {
      v[i] = std::pow( std::abs( v[i] ), 1 / ip - 2 ) * v[i];
    } else {
      v[i] = 0;
    }
  }
  //
  std::vector<long double> nc( m_pars.begin(), m_pars.end() );
  for ( unsigned short i = 0; i < N; ++i ) { nc[i] -= lc_f * v[i] * i_norm; }
  //
  return Bernstein( nc.begin(), nc.end(), xmin(), xmax() );
}
// ============================================================================
/*  calculate q-norm of the polynomial
 *  where q-norm is defined as:
 *  \f$ \left| f \right|_{q} = \left( \sum_i \left|c_i\right|^q\right)^{\frac{1}{q}} \f$
 *
 *  - q_inv = 0.0 ->  \f$ max_k    \left|c_k\right|  \f$
 *  - q_inv = 0.5 ->  \f$ \sqrt{ \sum_k  c_k^2 }     \f$
 *  - q_inv = 1.0 ->  \f$ \sum_k \left| c_k \right|  \f$
 */
// ============================================================================
double Gaudi::Math::Bernstein::norm( const double q_inv ) const {
  return LHCb::Math::p_norm( m_pars.begin(), m_pars.end(), q_inv );
}
// ============================================================================
/*  how close are two polynomials in q-norm?
 *  where q-norm is defined as:
 *  \f$ \left| f \right|_{q} = \left( \sum_i \left|c_i\right|^q\right)^{\frac{1}{q}} \f$
 *
 *  - q_inv = 0.0 ->  \f$ max_k    \left|c_k\right|  \f$
 *  - q_inv = 0.5 ->  \f$ \sqrt{ \sum_k  c_k^2 }     \f$
 *  - q_inv = 1.0 ->  \f$ \sum_k \left| c_k \right|  \f$
 */
// ============================================================================
double Gaudi::Math::Bernstein::distance( const Gaudi::Math::Bernstein& other, const double q_inv ) const {
  if ( &other == this ) { return 0; }
  //
  // 1) adjust the ranges
  //
  if ( !s_equal( xmin(), other.xmin() ) || !s_equal( xmax(), other.xmax() ) ) {
    return distance( Bernstein( other, xmin(), xmax() ), q_inv );
  }
  //
  // 2) adjust the degrees
  //
  if ( degree() > other.degree() ) {
    return distance( other.elevate( degree() - other.degree() ), q_inv );
  } else if ( degree() < other.degree() ) {
    return other.distance( this->elevate( other.degree() - degree() ), q_inv );
  }
  //
  // 3) make a real comparsion
  //
  std::vector<long double> v( m_pars.begin(), m_pars.end() );
  const unsigned short     N = degree();
  for ( unsigned short k = 0; k <= N; ++k ) { v[k] -= other.m_pars[k]; }
  //
  return LHCb::Math::p_norm( v.begin(), v.end(), q_inv );
}
// ============================================================================
// scale all coefficients with 2**i
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::Bernstein::ldexp( const short i ) const {
  if ( 0 == i ) { return *this; }
  Gaudi::Math::Bernstein result( *this );
  LHCb::Math::scale_exp2( result.m_pars, i );
  return result;
}
// ============================================================================
// multiply two Bernstein polynomials
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::Bernstein::multiply( const Gaudi::Math::Bernstein& other ) const {
  //
  if ( !s_equal( xmin(), other.xmin() ) || !s_equal( xmax(), other.xmax() ) ) {
    const double x_min = std::min( xmin(), other.xmin() );
    const double x_max = std::max( xmax(), other.xmax() );
    Bernstein    b1( *this, x_min, x_max );
    Bernstein    b2( other, x_min, x_max );
    return b1.multiply( b2 );
  }
  //
  if ( zero() || other.zero() ) { return Bernstein( degree(), xmin(), xmax() ); }
  //
  const unsigned short m = degree();
  const unsigned short n = other.degree();
  //
  Bernstein result( m + n, xmin(), xmax() );
  //
  long double c = 1;
  for ( unsigned short k = 0; k <= m + n; ++k ) {
    if ( 0 != k ) {
      c *= ( m + n - k + 1 );
      c /= k;
    }
    //
    const unsigned jmax = std::min( m, k );
    const unsigned jmin = k > n ? k - n : 0;
    long double    cc   = 0 == jmin ? c_nk( n, k - jmin ) : c_nk( m, jmin );
    for ( unsigned short j = jmin; j <= jmax; ++j ) {
      if ( j != jmin ) {
        cc *= ( m - j + 1 ) * ( k - j + 1 );
        cc /= j * ( n - k + j );
      }
      result.m_pars[k] += cc * m_pars[j] * other.m_pars[k - j];
    }
    result.m_pars[k] /= c;
  }
  return result;
}
// ============================================================================
// multiply two Bernstein polynomial and the basic bernstein polynomial
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::Bernstein::multiply( const Gaudi::Math::Bernstein::Basic& b ) const {
  Bernstein result( multiply( b.k(), b.N() - b.k() ) );
  Gaudi::Math::scale( result.m_pars, Gaudi::Math::choose( b.N(), b.k() ) );
  return result;
}
// ============================================================================
/*  multiply Bernstein polynomial with
 *  \f$ (x-x_{min})^i(x_{max}-x)^j \f$
 */
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::Bernstein::multiply( const unsigned short i1, const unsigned short i2 ) const {
  //
  const unsigned short m = i1 + i2;
  const unsigned short n = degree();
  //
  Bernstein result( n + m, xmin(), xmax() );
  //
  const unsigned short nK = result.m_pars.size();
  for ( unsigned short k = i1; k < nK; ++k ) {
    const unsigned short imin = k > n ? k - n : 0;
    const unsigned short imax = std::min( m, k );
    if ( imin <= i1 && i1 <= imax ) {
      result.m_pars[k] = m_pars[k - i1] * Gaudi::Math::choose( n, k - i1 ) / Gaudi::Math::choose( m + n, k );
    }
  }
  return result;
}
// ============================================================================
namespace {
  // ==========================================================================
  // power function
  // ==========================================================================
  inline Gaudi::Math::Bernstein _pow_( const Gaudi::Math::Bernstein& x, const unsigned short y,
                                       const Gaudi::Math::Bernstein& r ) {
    return 0 == y ? r
                  : 1 == y ? ( x.degree() >= r.degree() ? x.multiply( r ) : r.multiply( x ) )
                           : _pow_( x.multiply( x ), y / 2, y % 2 ? r * x : r );
  }
} // namespace
// ============================================================================
// power function
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::Bernstein::pow( const unsigned short i ) const {
  if ( 1 == i ) {
    return *this;
  } else if ( 2 == i ) {
    return multiply( *this );
  }
  //
  Gaudi::Math::Bernstein one( 0, xmin(), xmax() );
  one.m_pars[0] = 1;
  return _pow_( *this, i, one );
}
// ============================================================================
namespace {
  // ==========================================================================
  // implmentation of polynomial division
  // - operator Head
  // ==========================================================================
  template <class ITERATOR>
  inline double _m_head_( const unsigned short m, ITERATOR first, ITERATOR last ) {
    if ( first == last ) { return 0; }
    //
    long double h = 0;
    long double c = 1;
    for ( unsigned short i = 0; first != last; ++first, ++i ) {
      if ( 0 != i ) {
        c *= ( m + 1 - i );
        c /= i;
      }
      h += ( 0 == i % 2 ) ? c * ( *first ) : -c * ( *first );
    }
    return h;
  }
  // ==========================================================================
  // - operator Tail
  // ==========================================================================
  template <class ITERATOR, class OUTPUT>
  inline OUTPUT _tail_( ITERATOR first, ITERATOR last, OUTPUT output ) {
    if ( first == last ) { return output; }
    const unsigned short m = std::distance( first, last ) - 1;
    long double          c = 1;
    for ( unsigned short j = 0; j < m; ++j, ++output ) {
      if ( 0 != j ) {
        c *= j;
        c /= ( m - j );
      }
      const long double t = c * _m_head_( m, first, first + ( j + 1 ) );
      *output             = 0 == j % 2 ? t : -t;
    }
    return output;
  }
  // ==========================================================================
  // - operator Match
  // ==========================================================================
  template <class ITERATOR, class OUTPUT>
  inline OUTPUT _match_m_( const unsigned short m, ITERATOR first, ITERATOR last, OUTPUT output ) {
    if ( first == last ) { return output; }
    const unsigned short n = std::distance( first, last ) - 1;
    //
    long double c = 1;
    for ( unsigned short j = 0; j <= n; ++j, ++first, ++output ) {
      //
      if ( 0 != j ) {
        c *= ( n - j + 1 );
        c /= ( m - j + 1 );
      }
      *output = ( *first ) * c;
    }
    //
    for ( unsigned j = n + 1; j <= m; ++j, ++output ) { *output = 0; }
    //
    return output;
  }
  // ==========================================================================
  // - operator Quot
  // ==========================================================================
  template <class ITERATOR, class OUTPUT>
  inline OUTPUT _quot_k_( const unsigned short k, const unsigned short m, ITERATOR first, ITERATOR last, OUTPUT output,
                          const long double scale = 1 ) {
    if ( first == last ) { return output; }
    const unsigned short n  = std::distance( first, last ) - 1;
    const unsigned short k1 = k - ( m - n );
    //
    long double c = scale;
    for ( unsigned short j = 0; j <= k1; ++j, ++first, ++output ) {
      //
      if ( 0 != j ) {
        c *= ( k1 - j + 1 );
        c /= ( k - j + 1 );
      }
      *output += c;
    }
    //
    for ( unsigned j = k1 + 1; j <= k; ++j, ++output ) { *output += 0; }
    //
    return output;
  }
  // ==========================================================================
  template <class ITERATORF, class ITERATORG>
  inline std::vector<long double> _divmod_( ITERATORF f_first, ITERATORF f_last, ITERATORG g_first, ITERATORG g_last ) {
    //
    const unsigned short m = std::distance( f_first, f_last ) - 1;
    const unsigned short n = std::distance( g_first, g_last ) - 1;
    //
    std::vector<long double> _tail( m + 1, 0.0 );
    std::vector<long double> _match( m + 1, 0.0 );
    std::vector<long double> _quot( m - n + 1, 0.0 );
    //
    typedef std::vector<long double>::iterator IT;
    //
    ITERATORF fend = f_last;
    for ( unsigned short i = m; n <= i; --i ) {
      const long double h1 = _head_( f_first, fend );
      //
      if ( !s_zero( h1 ) ) {
        //
        IT mend = _match_m_( i, g_first, g_last, _match.begin() );
        //
        const long double h2 = _head_( _match.begin(), mend );
        //
        _quot_k_( m - n, i, g_first, g_last, _quot.begin(), h1 / h2 );
        //
        for ( unsigned short j = 0; j < m + 1; ++j ) { *( f_first + j ) -= h1 * _match[j] / h2; }
        //
      }
      //
      _tail_( f_first, fend, _tail.begin() );
      --fend;
      std::copy( _tail.begin(), _tail.end(), f_first );
      //
    }
    return _quot;
    // ==========================================================================
  }
} // namespace
// ==============================================================================
// the leading power coefficient
// ==============================================================================
double Gaudi::Math::Bernstein::head() const { return _head_( m_pars.begin(), m_pars.end() ); }
// ==============================================================================
/* polynomial division
 * \f$  f(x) = q(z)*g(x) + r(x) \f$
 * @return the pair q,r
 */
// ==============================================================================
namespace {
  inline std::pair<Gaudi::Math::Bernstein, Gaudi::Math::Bernstein> _divmod_( Gaudi::Math::Bernstein f,
                                                                             Gaudi::Math::Bernstein g ) {
    using namespace Gaudi::Math;
    ///  trivial case
    /// 1) f==0  or |f| << |g|
    if ( f.zero() || f.small( g.norm() ) ) {
      return std::make_pair( Bernstein( 0, g.xmin(), g.xmax() ), Bernstein( 0, g.xmin(), g.xmax() ) );
    }
    /// 2) g==0  or |g| << |f|
    if ( g.zero() || g.small( f.norm() ) ) {
      return std::make_pair( Bernstein( 0, f.xmin(), f.xmax() ), Bernstein( 0, f.xmin(), f.xmax() ) );
    }
    //
    if ( !s_equal( f.xmin(), g.xmin() ) || !s_equal( f.xmin(), g.xmin() ) ) {
      const double xmin = std::min( f.xmin(), g.xmin() );
      const double xmax = std::max( f.xmax(), g.xmax() );
      return _divmod_( Bernstein( f, xmin, xmax ), Bernstein( g, xmin, xmax ) );
    }
    //
    // get the leading coefficient of "f"
    //
    double fn = f.norm();
    while ( 0 < f.degree() && s_equal( fn + _head_( f ), fn ) ) {
      f = f.reduce( 1 );
      f.remove_noise();
      fn = f.norm();
    }
    //
    // get the leading coefficient of "g"
    //
    double gn = g.norm();
    while ( 0 < g.degree() && s_equal( gn + _head_( g ), gn ) ) {
      g = g.reduce( 1 );
      g.remove_noise();
      gn = g.norm();
    }
    //
    const std::vector<double>& pf = f.pars();
    const std::vector<double>& pg = g.pars();
    //
    const unsigned short m = f.degree();
    const unsigned short n = g.degree();
    //
    if ( m < n ) { return std::make_pair( Bernstein( 0, f.xmin(), f.xmax() ), f ); }
    //
    std::vector<long double> _f( pf.begin(), pf.end() );
    if ( n == 0 ) {
      LHCb::Math::scale( _f, 1 / g.par( 0 ) );
      return std::make_pair( Bernstein( _f.begin(), _f.end(), f.xmin(), f.xmax() ),
                             Bernstein( 0, f.xmin(), f.xmax() ) );
    }
    std::vector<long double> _q = _divmod_( _f.begin(), _f.end(), pg.begin(), pg.end() );
    //
    Bernstein q( _q.begin(), _q.end(), f.xmin(), f.xmax() );
    Bernstein r( _f.begin(), _f.begin() + n, f.xmin(), f.xmax() );
    //
    const double qn = q.norm();
    const double rn = r.norm();
    //
    if ( r.small( fn + qn * gn ) ) {
      r = Bernstein( 0, r.xmin(), r.xmax() );
    } else {
      r.remove_noise( 0, fn + qn * gn );
    }
    //
    if ( s_equal( qn * gn + fn + rn, fn + rn ) ) {
      q = Bernstein( 0, q.xmin(), q.xmax() );
    } else {
      q.remove_noise( 0, ( fn + rn ) / gn );
    }
    //
    return std::make_pair( q, r );
  }
  // ==========================================================================
} // namespace
// ============================================================================
/*  polynomial division
 *  \f$  f(x) = q(z)*g(x) + r(x) \f$
 *  @return the pair q(x),r(x)
 */
// ============================================================================
std::pair<Gaudi::Math::Bernstein, Gaudi::Math::Bernstein>
Gaudi::Math::Bernstein::divmod( const Gaudi::Math::Bernstein& g ) const {
  return _divmod_( *this, g );
}
// ============================================================================
/*  polynomial division
 *  \f$  f(x) = q(z)*g(x) + r(x) \f$
 *  @return the quotient q(x)
 */
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::Bernstein::quotient( const Gaudi::Math::Bernstein& g ) const {
  return _divmod_( *this, g ).first;
}
// ============================================================================
/*  polynomial division
 *  \f$  f(x) = q(z)*g(x) + r(x) \f$
 *  @return the reminder r(x)
 */
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::Bernstein::reminder( const Gaudi::Math::Bernstein& g ) const {
  return _divmod_( *this, g ).second;
}
// ============================================================================
/* de Casteljau algorithm for summation of Bernstein polynomials
 *  \f$ f(x) = \sum_i p_i B_ik(x) \f$
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2015-02-10
 */
// ============================================================================
double Gaudi::Math::casteljau( const std::vector<double>& pars, const double x ) {
  std::vector<long double> _tmp( pars.begin(), pars.end() );
  //
  const long double t0 = x;
  const long double t1 = 1 - t0;
  //
  return _casteljau_( _tmp.begin(), _tmp.end(), t0, t1 );
}
// ============================================================================
namespace {
  // ==========================================================================
  /** transformation matrix from legendre to bernstein basis
   *  @see http://www.sciencedirect.com/science/article/pii/S0377042700003769 eq.20
   */
  inline double l2b_mtrx( const unsigned short j, const unsigned short k, const unsigned short n ) {
    //
    const unsigned short imin = std::max( 0, j + k - n );
    const unsigned short imax = std::min( j, k );
    long long            r    = 0;
    for ( unsigned short i = imin; i <= imax; ++i ) {
      0 == ( k + i ) % 2
          ? r += Gaudi::Math::choose( j, i ) * Gaudi::Math::choose( k, i ) * Gaudi::Math::choose( n - j, k - i )
          : r -= Gaudi::Math::choose( j, i ) * Gaudi::Math::choose( k, i ) * Gaudi::Math::choose( n - j, k - i );
    }
    //
    return r / double( Gaudi::Math::choose( n, k ) );
  }
  // ==========================================================================
  /** transformation matrix from chebyshev to bernstein basis
   *  http://www.degruyter.com/view/j/cmam.2003.3.issue-4/cmam-2003-0038/cmam-2003-0038.xml  eq. 15
   */
  inline long double c2b_mtrx( const unsigned short j, const unsigned short k, const unsigned short n ) {
    const unsigned short imin = std::max( 0, j + k - n );
    const unsigned short imax = std::min( j, k );
    long long            r    = 0;
    for ( unsigned short i = imin; i <= imax; ++i ) {
      0 == ( k - i ) % 2 ? r += Gaudi::Math::choose( 2 * k, 2 * i ) * Gaudi::Math::choose( n - k, j - i )
                         : r -= Gaudi::Math::choose( 2 * k, 2 * i ) * Gaudi::Math::choose( n - k, j - i );
    }
    //
    return r / ( (long double)Gaudi::Math::choose( n, j ) );
  }
  // ==========================================================================
  /** transformation matrix from monomial to bernstein basis
   */
  inline long double m2b_mtrx( const unsigned short j, const unsigned short k, const unsigned short n ) {
    //
    return j < k ? 0.0 : (double)( Gaudi::Math::choose( j, k ) ) / (double)( Gaudi::Math::choose( n, k ) );
  }
  // ==========================================================================
  /// affine transformation of polynomial
  inline long double m2m_mtrx_2( const unsigned short j, const unsigned short k ) {
    if ( k < j ) { return 0; }
    const long double c = Gaudi::Math::choose( k, j ) * Gaudi::Math::pow( 2, j );
    //
    return 0 == ( k - j ) % 2 ? c : -c;
  }
  // ==========================================================================
} // namespace
// ============================================================================
// constructor from Legendre polynomial
// ============================================================================
Gaudi::Math::Bernstein::Bernstein( const Gaudi::Math::LegendreSum& poly )
    : Gaudi::Math::PolySum( poly.degree() ), m_xmin( poly.xmin() ), m_xmax( poly.xmax() ) {
  for ( unsigned short i = 0; i < npars(); ++i ) {
    for ( unsigned short k = 0; k < npars(); ++k ) {
      const double p = poly.par( k );
      if ( s_zero( p ) ) { continue; }
      m_pars[i] += l2b_mtrx( i, k, degree() ) * p;
    }
  }
}
// ============================================================================
// constructor from Chebyshev polynomial
// ============================================================================
Gaudi::Math::Bernstein::Bernstein( const Gaudi::Math::ChebyshevSum& poly )
    : Gaudi::Math::PolySum( poly.degree() ), m_xmin( poly.xmin() ), m_xmax( poly.xmax() ) {
  //
  for ( unsigned short i = 0; i < npars(); ++i ) {
    for ( unsigned short k = 0; k < npars(); ++k ) {
      const double p = poly.par( k );
      if ( s_zero( p ) ) { continue; }
      m_pars[i] += c2b_mtrx( i, k, degree() ) * p;
    }
  }
  //
}
// ============================================================================
// constructor from simple monomial form
// ============================================================================
Gaudi::Math::Bernstein::Bernstein( const Gaudi::Math::Polynomial& poly )
    : Gaudi::Math::PolySum( poly.degree() ), m_xmin( poly.xmin() ), m_xmax( poly.xmax() ) {
  //
  const unsigned short np = npars();
  // 2-step transformation
  //
  // 1: affine transform to [0,1]
  //
  std::vector<double> _pars( np );
  for ( unsigned short i = 0; i < np; ++i ) {
    for ( unsigned short k = i; k < np; ++k ) {
      const double p = poly.par( k );
      if ( s_zero( p ) ) { continue; }
      _pars[i] += m2m_mtrx_2( i, k ) * p;
    }
  }
  //
  // 2: tramsform from shifted poly basis:
  //
  for ( unsigned short i = 0; i < np; ++i ) {
    for ( unsigned short k = 0; k <= i; ++k ) // ATTENTION!!
    {
      const double p = _pars[k];
      if ( s_zero( p ) ) { continue; }
      m_pars[i] += m2b_mtrx( i, k, degree() ) * p;
    }
  }
  //
}
// ============================================================================

// ============================================================================
/* get the integral between 0 and 1 for a product of basic  Bernstein
 *  polynom and the exponential function with the exponent tau
 *  \f[ \int_{0}^{1} \mathcal{B}_{ik} e^{\tau x } \mathrm{d}x \f]
 *  @param poly  bernstein polynomial
 *  @param tau   slope parameter for an exponential function
 */
// ============================================================================
double Gaudi::Math::integrate( const Gaudi::Math::Bernstein::Basic& b, const double tau ) {
  //
  if ( b.k() > b.N() ) { return 0; }
  if ( s_zero( tau ) ) { return 1.0 / ( b.N() + 1 ); }
  //
  // make use Kummer function as default scenario
  return Gaudi::Math::kummer( b.k() + 1, b.N() + 2, tau ) / ( b.N() + 1 );
  //
}
// ============================================================================
/* get the integral between \f$x_{min}\f$ and \f$x_{max}\f$ for a product of Bernstein
 *  polynom and the exponential function with the exponent tau
 *  \f[  \int_{x_{min}}^{x_{max}} \mathcal{B} e^{\tau x } \mathrm{d}x \f]
 *  @param poly  bernstein polynomial
 *  @param tau   slope parameter for exponential
 */
// ============================================================================
double Gaudi::Math::integrate( const Gaudi::Math::Bernstein& poly, const double tau ) {
  if ( s_zero( tau ) ) { return poly.integral(); }
  //
  const long double xmin = poly.xmin();
  const long double xmax = poly.xmax();
  //
  const long double _tau = ( xmax - xmin ) * tau;
  const long double _fac = std::exp( tau * xmin );
  //
  long double                result = 0;
  const unsigned short       N      = poly.degree();
  const std::vector<double>& pars   = poly.pars();
  //
  for ( std::vector<double>::const_iterator ip = pars.begin(); pars.end() != ip; ++ip ) {
    if ( s_zero( *ip ) ) { continue; } // skip zeroes
    const unsigned short k = ip - pars.begin();
    const long double    p = *ip;
    result += p * integrate( Gaudi::Math::Bernstein::Basic( k, N ), _tau );
  }
  //
  return result * ( xmax - xmin ) * _fac;
}
// ============================================================================
namespace {
  // ==========================================================================
  inline long double r_kNm( const unsigned short k, const unsigned short N, const unsigned short m ) {
    long double r = 1.0L;
    for ( unsigned short i = 1; i <= m; ++i ) {
      r *= ( k + i );
      r /= ( N + i );
      r /= i;
    }
    return r;
  }
  // ==========================================================================
} // namespace
// ============================================================================
/* get the integral between 0 and 1 for a product of basic  Bernstein
 *  polynom and monomial or degree m
 *  \f[  \int_{0}^{1} \mathcal{B} \frac{x^m}{m!} \mathrm{d}x \f]
 *  @param b     basic bernstein polynomial
 *  @param m     degree of monomial
 */
// ============================================================================
double Gaudi::Math::integrate_poly( const Gaudi::Math::Bernstein::Basic& b, const unsigned short m ) {
  //
  const unsigned short N = b.N();
  const unsigned short k = b.k();
  //
  return r_kNm( k, N, m ) / ( N + m + 1 );
}
// ============================================================================
/*  get the integral between xmin and xmax Bernstein
 *  polynom and monomial or degree m
 *  \f[  \int_{x_min}^{x_max} \mathcal{B} \frac{x^m}{m!} \mathrm{d}x \f]
 *  @param b     basic bernstein polynomial
 *  @param m     degree of monomial
 */
// ============================================================================
double Gaudi::Math::integrate_poly( const Gaudi::Math::Bernstein& b, const unsigned short m ) {
  //
  if ( 0 == m ) { return b.integral(); }
  //
  const std::vector<double>& pars = b.pars();
  const unsigned short       N    = b.degree();
  std::vector<long double>   nc( pars.size(), 0.0L );
  for ( unsigned short k = 0; k < nc.size(); ++k ) {
    const long double ci = pars[k];
    if ( s_zero( ci ) ) { continue; }
    nc[k] = r_kNm( k, N, m ) * ci;
  }
  //
  return Gaudi::Math::pow( b.xmax() - b.xmin(), m + 1 ) * std::accumulate( nc.begin(), nc.end(), 0.0L ) / ( N + m + 1 );
}
// ============================================================================
namespace {
  // ==========================================================================
  long double _integrate_poly_( const Gaudi::Math::Bernstein& b, const unsigned short m, const double low,
                                const double high ) {
    const std::vector<double>& pars = b.pars();
    const unsigned short       N    = b.degree();
    std::vector<long double>   nc( pars.size() + m, 0.0L );
    for ( unsigned short k = 0; k < pars.size(); ++k ) {
      const long double ci = pars[k];
      if ( s_zero( ci ) ) { continue; }
      nc[k + m] = r_kNm( k, N, m ) * ci;
    }
    //
    Gaudi::Math::Bernstein a( nc.begin(), nc.end(), b.xmin(), b.xmax() );
    //
    return Gaudi::Math::pow( b.xmax() - b.xmin(), m ) * a.integral( low, high );
  }
  // ==========================================================================
} // namespace
// ============================================================================
/** get the integral between xmin and xmax Bernstein
 *  polynom and monomial or degree m
 *  \f[  \int_{low}^{high} \mathcal{B} \frac{(x-x_min)^m}{m!} \mathrm{d}x \f]
 *  @param b     basic bernstein polynomial
 *  @param m     degree of monomial
 *  @param low   low  integration limit
 *  @param high  high integtation limit
 */
// ============================================================================
double Gaudi::Math::integrate_poly( const Gaudi::Math::Bernstein& b, const unsigned short m, const double low,
                                    const double high ) {
  //
  if ( s_equal( low, high ) ) {
    return 0;
  } else if ( 0 == m ) {
    return b.integral( low, high );
  } else if ( low > high ) {
    return -integrate_poly( b, m, high, low );
  } else if ( high < b.xmin() ) {
    return 0;
  } else if ( low > b.xmax() ) {
    return 0;
  } else if ( low < b.xmin() ) {
    return integrate_poly( b, m, b.xmin(), high );
  } else if ( high > b.xmax() ) {
    return integrate_poly( b, m, low, b.xmax() );
  } else if ( s_equal( low, b.xmin() ) && s_equal( high, b.xmax() ) ) {
    return integrate_poly( b, m );
  }
  //
  // make the actual integration
  return _integrate_poly_( b, m, low, high );
}
// ============================================================================
/*  get the integral between low and high for a product of Bernstein
 *  polynom and the exponential function with the exponent tau
 *  \f[  \int_{a}^{b} \mathcal{B} e^{\tau x } \mathrm{d}x \f]
 *  @param poly  bernstein polynomial
 *  @param tau   slope parameter for exponential
 *  @param low   low  integration range
 *  @param high  high integration range
 */
// ============================================================================
double Gaudi::Math::integrate( const Gaudi::Math::Bernstein& poly, const double tau, const double low,
                               const double high ) {
  if ( s_small( tau ) ) {
    return poly.integral( low, high );
  } else if ( s_equal( low, high ) ) {
    return 0;
  } else if ( poly.zero() ) {
    return 0;
  } else if ( low > high ) {
    return -integrate( poly, tau, high, low );
  } else if ( high < poly.xmin() || low > poly.xmax() ) {
    return 0;
  } else if ( low < poly.xmin() ) {
    return integrate( poly, tau, poly.xmin(), high );
  } else if ( high > poly.xmax() ) {
    return integrate( poly, tau, low, poly.xmax() );
  }
  //
  if ( s_equal( low, poly.xmin() ) && s_equal( high, poly.xmax() ) ) { return integrate( poly, tau ); }
  //
  // start series expansion
  //
  long double result = poly.integral( low, high );
  long double dd1    = 1;
  long double dd2    = 1;
  long double taum   = 1;
  //
  const long double xmin = poly.xmin();
  // const long double xmax = poly.xmax () ;
  //
  // const long double _tau =            ( xmax - xmin ) * tau ;
  const long double _fac = std::exp( tau * xmin );
  //
  for ( unsigned int m = 1; m < 10000; ++m ) {
    taum *= tau;
    dd2 = _integrate_poly_( poly, m, low, high ) * taum;
    result += dd2;
    if ( s_small( dd1 / result ) && s_small( dd2 / result ) ) { break; }
    dd1 = dd2;
  }
  //
  return result * _fac;
}
// ============================================================================

// ============================================================================
//  Deflate polynomial
// ============================================================================
/*  deflate Bernstein polynomial at  <code>x=xmin</code>
 *  \f$ b(x)-b(x_{min})=(x-x_{min})*d(x)\f$
 *  @param  b  berntein polynomial to be deflated
 *  @return deflated polinomial "d"
 */
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::deflate_left( const Gaudi::Math::Bernstein& b ) {
  // trivial case
  if ( 1 > b.degree() ) { return Gaudi::Math::Bernstein( 0, b.xmin(), b.xmax() ); }
  // simple  case
  const std::vector<double>& bpars = b.pars();
  std::vector<long double>   dpars( bpars.size() - 1 );
  //
  const double pz         = bpars.front();
  dpars.front()           = 0;
  const unsigned short Nd = dpars.size();
  for ( unsigned short i = 0; i < Nd; ++i ) {
    const long double p_i = bpars[i + 1] - pz;
    dpars[i]              = Nd * p_i / ( i + 1 );
  }
  // result
  return Gaudi::Math::Bernstein( dpars.begin(), dpars.end(), b.xmin(), b.xmax() );
}
// ============================================================================
/*  deflate Bernstein polynomial at  <code>x=xmax</code>
 *  \f$ b(x)-b(x_{max})=(x-x_{max})*d(x)\f$
 *  @param  b  berntein polynomial to be deflated
 *  @return deflated polinomial "d"
 */
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::deflate_right( const Gaudi::Math::Bernstein& b ) {
  // trivial case
  if ( 1 > b.degree() ) { return Gaudi::Math::Bernstein( 0, b.xmin(), b.xmax() ); }
  //
  // simple  case
  const std::vector<double>& bpars = b.pars();
  std::vector<long double>   dpars( bpars.size() - 1 );
  //
  const double pz = bpars.back();
  //
  dpars.back()            = 0;
  const unsigned short Nd = dpars.size();
  for ( unsigned short i = 0; i < Nd; ++i ) {
    const long double p_i = bpars[i] - pz;
    dpars[i]              = Nd * p_i / ( Nd - i );
  }
  // result
  return Gaudi::Math::Bernstein( dpars.begin(), dpars.end(), b.xmin(), b.xmax() );
}
// ============================================================================
/*  deflate Bernstein polynomial at  <code>x=x0</code>
 *  \f$ b(x)-b(x_{0})=(x-x_{0})*d(x)\f$
 *  @param  b  berntein polynomial to be deflated
 *  @param  x0 the delfation point
 *  @return deflated polinomial "d"
 */
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::deflate( const Gaudi::Math::Bernstein& b, const double x ) {
  //
  // trivial case
  if ( 1 > b.degree() ) { return Gaudi::Math::Bernstein( 0, b.xmin(), b.xmax() ); }
  //
  if ( s_equal( x, b.xmin() ) ) {
    return deflate_left( b );
  } else if ( s_equal( x, b.xmax() ) ) {
    return deflate_right( b );
  }
  //
  const long double v  = b.evaluate( x );
  const long double tt = b.t( x );
  //
  const bool   reversed = tt <= 0.5;
  const double tau      = reversed ? 1 - tt : tt;
  //
  const long double pz = v;
  //
  const std::vector<double>& bpars = b.pars();
  const unsigned short       Nd    = bpars.size() - 1;
  std::vector<long double>   dpars = reversed ? std::vector<long double>( bpars.rbegin(), bpars.rbegin() + Nd )
                                            : std::vector<long double>( bpars.begin(), bpars.begin() + Nd );
  //
  LHCb::Math::shift( dpars.begin(), dpars.end(), -pz );
  const long double u = ( 1 - tau ) / tau;
  for ( unsigned short i = 1; i < Nd; ++i ) {
    const long double p_i = dpars[i];
    dpars[i]              = ( Nd * p_i + i * u * dpars[i - 1] ) / ( Nd - i );
  }
  //
  if ( reversed ) { std::reverse( dpars.begin(), dpars.end() ); }
  //
  return Gaudi::Math::Bernstein( dpars.begin(), dpars.end(), b.xmin(), b.xmax() );
}

// ============================================================================
/*  get abscissas of crosssing points of the control polygon
 *  for Bernstein polynomial
 *  @param  b bernstein polynomial
 *  @reutrn abscissas of crossing points of the control  polygon
 */
// ============================================================================
std::vector<double> Gaudi::Math::crossing_points( const Gaudi::Math::Bernstein& b ) {
  // trivial case
  if ( 1 > b.degree() ) {
    if ( !s_zero( b.pars().front() ) ) {
      return std::vector<double>();
    } else {
      return std::vector<double>( 1, b.xmin() );
    }
  }
  //
  const double               norm  = b.norm();
  const std::vector<double>& bpars = b.pars();
  const unsigned short       N     = b.npars();
  //
  std::vector<double> cps;
  cps.reserve( b.degree() + 1 );
  //
  const double p0 = bpars[0];
  if ( s_zero( p0 ) || s_equal( p0 + norm, norm ) ) { cps.push_back( b.xmin() ); }
  //
  for ( unsigned short j = 1; j < N; ++j ) {
    const double pj = bpars[j];
    const double pi = bpars[j - 1];
    //
    const double xj = b.x( float( j ) / ( N - 1 ) );
    if ( s_zero( pj ) || s_equal( pj + norm, norm ) ) {
      cps.push_back( xj );
      continue;
    }
    //
    if ( s_zero( pi ) || s_equal( pi + norm, norm ) ) { continue; }
    //
    const signed char sj = LHCb::Math::signum( pj );
    const signed char si = LHCb::Math::signum( pi );
    //
    if ( 0 > si * sj ) // there is root here!
    {
      const double xi = b.x( float( j - 1 ) / ( N - 1 ) );
      const double cp = ( xj * pi - xi * pj ) / ( pi - pj );
      cps.push_back( cp );
    }
    //
  }
  //
  return cps;
}

// ============================================================================
/*  get number of (strickt) sign changes in trhe sequnce of coefficients
 *  for Bernstein polynomial
 *  if  N is number of sign changes, then the number of real roots R is
 *  \f$ R = N - 2K\f$, where K is non-negative integer
 */
// ============================================================================
unsigned short Gaudi::Math::sign_changes( const Gaudi::Math::Bernstein& b ) {
  const std::vector<double>&     bpars = b.pars();
  const LHCb::Math::Tiny<double> s_tiny{b.norm()};
  return LHCb::Math::sign_changes( bpars.begin(), bpars.end(), s_tiny );
} // ============================================================================
/*  get the most left crossing  point of convex hull with  x-axis
 *  (it is a step  towards finding the most left root, if any
 *  if convex hull does not cross the x-axis, xmax is returned
 */
// ============================================================================
double Gaudi::Math::left_line_hull( const Gaudi::Math::Bernstein& b ) {
  const double bn = b.norm();
  //
  const std::vector<double>& bpars = b.pars();
  const double               p0    = bpars.front();
  //
  // left point is already zero
  if ( s_zero( p0 ) || s_equal( p0 + bn, bn ) ) { return b.xmin(); }
  //
  const signed char s0 = LHCb::Math::signum( p0 );
  const bool        up = 0 > p0;
  //
  const unsigned short N = b.npars();
  //
  // find the first element with the opposite sign
  unsigned short i = 1;
  for ( ; i < N; ++i ) {
    const double pi = bpars[i];
    if ( s_zero( pi ) || s_equal( pi + bn, bn ) || 0 >= s0 * LHCb::Math::signum( bpars[i] ) ) { break; }
  }
  //
  // no  good points are found,
  if ( i == N ) { return b.xmax() + 10 * ( b.xmax() - b.xmin() ); } // RETURN
  //
  double si = ( bpars[i] - p0 ) / i;
  for ( unsigned short j = i + 1; j < N; ++j ) {
    const double sj = ( bpars[j] - p0 ) / j;
    if ( ( up && sj >= si ) || ( !up && sj <= si ) ) {
      i  = j;
      si = sj;
    }
  }
  //
  const double xi = double( i ) / ( N - 1 );
  const double yi = bpars[i];
  //
  return b.x( -xi * p0 / ( yi - p0 ) );
}
// ============================================================================
/*  get the most right rossing  point of convex hull with  x-axis
 *  (it is a step  towards finding the most right root, if any
 *  if convex hull does not cross the x-axis, xmin is returned
 */
// ============================================================================
double Gaudi::Math::right_line_hull( const Gaudi::Math::Bernstein& b ) {
  const double bn = b.norm();
  //
  const std::vector<double>& bpars = b.pars();
  const double               p0    = bpars.back(); //  ATTENTION!
  //
  // right point is already zero
  if ( s_zero( p0 ) || s_equal( p0 + bn, bn ) ) { return b.xmax(); }
  //
  const signed char s0 = LHCb::Math::signum( p0 );
  const bool        up = 0 > p0;
  //
  const unsigned short N = b.npars();
  //
  // find the first element with the opposite sign
  unsigned short i = 0;
  for ( ; i < N - 1; ++i ) {
    const double pi = bpars[i];
    if ( s_zero( pi ) || s_equal( pi + bn, bn ) || 0 >= s0 * LHCb::Math::signum( bpars[i] ) ) { break; }
  }
  //
  // no  good points are found,
  if ( i == N - 1 ) { return b.xmin() - 10 * ( b.xmax() - b.xmin() ); } // RETURN
  //
  double si = ( bpars[i] - p0 ) / ( N - i );
  for ( unsigned short j = i + 1; j < N; ++j ) {
    const double sj = ( bpars[j] - p0 ) / ( N - j );
    if ( ( up && sj >= si ) || ( !up && sj <= si ) ) {
      i  = j;
      si = sj;
    }
  }
  //
  const double xi = double( i ) / ( N - 1 );
  const double yi = bpars[i];
  //
  return b.x( ( yi - xi * p0 ) / ( yi - p0 ) );
}

// ============================================================================
//  Some interpolation utilities
// ============================================================================
/*  construct interpolation polynomial (in Bernstein form)
 *  @param x       vector of abscissas
 *  @param y       vector of function values
 *  @param xmin low  edge for Bernstein polynomial
 *  @param xmax high edge for Bernstein polynomial
 *  - if vector of y is longer  than vector x, extra values are ignored
 *  - if vector of y is shorter than vector x, missing entries are assumed to be zero
 *  It relies on Newton-Bernstein algorithm
 *  @see http://arxiv.org/abs/1510.09197
 *  @see Mark Ainsworth and Manuel A. Sanches,
 *       "Computing of Bezier control points of Largangian interpolant
 *       in arbitrary dimension", arXiv:1510.09197 [math.NA]
 *  @see http://adsabs.harvard.edu/abs/2015arXiv151009197A
 *  @see Gaudi::Math::Bernstein
 *  @code
 *  std::vector<double> x = ... ; // abscissas
 *  std::vector<double> y = ... ; // functionvalues
 *  Gaudi::Math::Bernstein p = interpolate ( x , y , -1 , 1 );
 *  std::cout << " interpolant at x=0.1 is " << p(0.1) << std::endl ;
 *  std::cout << " interpolant at x=0.2 is " << p(0.2) << std::endl ;
 *  @endcode
 */
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::Interpolation::bernstein( const std::vector<double>& x,
                                                              const std::vector<double>& y, const double xmin,
                                                              const double xmax ) {
  return Gaudi::Math::Bernstein( x.begin(), x.end(), y.begin(), y.end(), xmin, xmax );
}
// ============================================================================
/*  construct interpolation polynomial (in Bernstein form)
 *  @param func    the function
 *  @param x       vector of abscissas
 *  @param xmin low  edge for Bernstein polynomial
 *  @param xmax high edge for Bernstein polynomial
 *  - if vector of y is longer  than vector x, extra values are ignored
 *  - if vector of y is shorter than vector x, missing entries are assumed to be zero
 *  It relies on Newton-Bernstein algorithm
 *  @see http://arxiv.org/abs/1510.09197
 *  @see Mark Ainsworth and Manuel A. Sanches,
 *       "Computing of Bezier control points of Largangian interpolant
 *       in arbitrary dimension", arXiv:1510.09197 [math.NA]
 *  @see http://adsabs.harvard.edu/abs/2015arXiv151009197A
 *  @see Gaudi::Math::Bernstein
 *  @code
 *  auto f = [] ( double t ) { return std::sin ( t ) ; }
 *  std::vector<double> x = ... ; // abscissas
 *  Gaudi::Math::Bernstein p = interpolate ( f , x , -1 , 1 );
 *  std::cout << " interpolant at x=0.1 is " << p(0.1) << std::endl ;
 *  std::cout << " interpolant at x=0.2 is " << p(0.2) << std::endl ;
 *  @endcode
 */
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::Interpolation::bernstein( std::function<double( double )> func,
                                                              const std::vector<double>& x, const double xmin,
                                                              const double xmax ) {
  return Gaudi::Math::Interpolation::bernstein( func, x.begin(), x.end(), xmin, xmax );
}
// ============================================================================
/*  construct interpolation polynomial (in Bernstein form) using Gauss-Lobatto grid,
 *  that minimises Runge's effect.
 *  @param func      the function
 *  @param N         the interpolation  degree
 *  @param xmin low  edge for Bernstein polynomial
 *  @param xmax high edge for Bernstein polynomial
 *  - if vector of y is longer  than vector x, extra values are ignored
 *  - if vector of y is shorter than vector x, missing entries are assumed to be zero
 *  It relies on Newton-Bernstein algorithm
 *  @see http://arxiv.org/abs/1510.09197
 *  @see Mark Ainsworth and Manuel A. Sanches,
 *       "Computing of Bezier control points of Largangian interpolant
 *       in arbitrary dimension", arXiv:1510.09197 [math.NA]
 *  @see http://adsabs.harvard.edu/abs/2015arXiv151009197A
 *  @see Gaudi::Math::Bernstein
 *  @code
 *  auto f = [] ( double t ) { return std::sin ( t ) ; }
 *  Gaudi::Math::Bernstein p = bernstein ( f , 5 , -1 , 1 );
 *  std::cout << " interpolant at x=0.1 is " << p(0.1) << std::endl ;
 *  std::cout << " interpolant at x=0.2 is " << p(0.2) << std::endl ;
 *  @endcode
 */
// ============================================================================
Gaudi::Math::Bernstein Gaudi::Math::Interpolation::bernstein( std::function<double( double )> func,
                                                              const unsigned short N, const double xmin,
                                                              const double xmax ) {
  return Gaudi::Math::Interpolation::lobatto( func, N, xmin, xmax );
}
// ============================================================================

// ============================================================================
//  DUAL BASIC
// ============================================================================
// constructor from the order
// ============================================================================
Gaudi::Math::BernsteinDualBasis::BernsteinDualBasis( const unsigned short N, const unsigned short j )
    : m_k( j ), m_bernstein( N ) {
  if ( j <= N ) {
    const unsigned short n = N;
    for ( unsigned short k = 0; k <= N; ++k ) {
      double               ck   = 0.0;
      const unsigned short imax = std::min( j, k );
      for ( unsigned short i = 0; i <= imax; ++i ) {
        long double a = 2 * i + 1;
        a *= c_nk( n + i + 1, n - j );
        a *= c_nk( n - i, n - j );
        a *= c_nk( n + i + 1, n - k );
        a *= c_nk( n - i, n - k );
        //
        ck += a;
      }
      ck /= ( c_nk( n, j ) * c_nk( n, k ) );
      if ( ( j + k ) % 2 ) { ck = -ck; }
      m_bernstein.setPar( k, ck );
    }
  }
}
// ============================================================================
// copy constructor
// ============================================================================
Gaudi::Math::BernsteinDualBasis::BernsteinDualBasis( const Gaudi::Math::BernsteinDualBasis& right )
    : m_k( right.m_k ), m_bernstein( right.m_bernstein ) {}
// ============================================================================
// move constructor
// ============================================================================
Gaudi::Math::BernsteinDualBasis::BernsteinDualBasis( Gaudi::Math::BernsteinDualBasis&& right )
    : m_k( right.m_k ), m_bernstein( std::move( right.m_bernstein ) ) {}

// ============================================================================
// The END
// ============================================================================
