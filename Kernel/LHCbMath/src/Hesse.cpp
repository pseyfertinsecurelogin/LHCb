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
#include <cmath>
#include <iterator>
#include <numeric>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/StatusCode.h"
// ============================================================================
// GSL
// ============================================================================
#include "gsl/gsl_math.h"
#include "gsl/gsl_multimin.h"
// ============================================================================
// LHCbMath
// ============================================================================
#include "LHCbMath/Hesse.h"
// ============================================================================
// Local
// ============================================================================
#include "GSL_sentry.h"
// ============================================================================
namespace {

  // ==========================================================================
  typedef Gaudi::Math::GSL::GSL_Error_Handler Sentry;
  // ==========================================================================
  typedef Gaudi::Math::GSL::Hesse::function function;
  // ==========================================================================
  /// 1/sqrt(2)
  const double s_SQRT2i = 1.0 / std::sqrt( 2.0 ); // 1/sqrt(2)
  // ==========================================================================
  // finite difference coefficients
  // ==========================================================================
  const std::array<double, 9> s_8 = {-1. / 560, +8. / 315, -1. / 5,   +8. / 5,  -205. / 72,
                                     +8. / 5,   -1. / 5,   +8. / 315, -1. / 560};
  // ==========================================================================
  // finite difference coefficients
  // ==========================================================================
  const std::array<double, 9> s_6 = {-0., +1. / 90, -3. / 20, +3. / 2, -49. / 18, +3. / 2, -3. / 20, +1. / 90, -0.};
  // ==========================================================================
  template <typename T>
  double dot( const T& v1, const T& v2 ) {
    return std::inner_product( std::begin( v1 ), std::end( v1 ), std::begin( v2 ), typename T::value_type{0} );
  }
  // ==========================================================================
  template <typename T>
  double absdot( const T& v1, const T& v2 ) {
    return std::inner_product(
        std::begin( v1 ), std::end( v1 ), std::begin( v2 ), typename T::value_type{0},
        std::plus<typename T::value_type>(),
        []( typename T::const_reference i, typename T::const_reference j ) { return std::abs( i * j ); } );
  }
  // ==========================================================================
  inline double deriv2_8( const std::array<double, 9>& values, const double x, const double h0, double* abserr_round,
                          double* abserr_trunc ) {
    //
    const double h02 = h0 * h0;
    //
    const double o8 = dot( values, s_8 );
    const double o6 = dot( values, s_6 );
    //
    const double o8a = absdot( values, s_8 );
    const double o6a = absdot( values, s_6 );
    //
    const double e6 = o6a * GSL_DBL_EPSILON;
    const double e8 = o8a * GSL_DBL_EPSILON;
    //
    const double r8 = o8 / h02;
    const double r6 = o6 / h02;
    //
    const double dy = 6 * GSL_MAX( std::abs( r8 ), std::abs( r6 ) ) * std::abs( x / h0 ) * GSL_DBL_EPSILON;
    //
    *abserr_trunc = std::abs( o6 - o8 ) / h02;
    *abserr_round = std::fabs( e8 + e6 ) / h02 + dy;
    //
    return r8;
  }
  // ===========================================================================
  /** calculate
   *  a = x+h*d
   *  @param x (INPUT)  vector x
   *  @param d (INPUT)  vector d
   *  @param h (INPUT)  scalar h
   *  @param a (UPDATE) output vector a
   */
  inline void _update_( const gsl_vector* x, const gsl_vector* d, const double h, gsl_vector* a ) {
    // fill the step vector
    for ( std::size_t i = 0; i < x->size; ++i ) {
      const double xi = gsl_vector_get( x, i );
      const double di = gsl_vector_get( d, i );
      gsl_vector_set( a, i, xi + h * di );
    }
  }
  // ==========================================================================
  // get the second derivative along the direction d
  // ==========================================================================
  double deriv2_8( const gsl_multimin_function* f, const gsl_vector* x, const gsl_vector* d, double h, double* round,
                   double* trunc, gsl_vector* a ) {
    //
    std::array<double, 9> values;
    const double          h0 = 0.25 * std::abs( h );
    //

    for ( std::size_t j = 0; j < 9; ++j ) {
      const double s = ( j - 4.0 ) * h0;
      _update_( x, d, s, a );
      values[j] = GSL_MULTIMIN_FN_EVAL_F( f, a );
    }
    //
    double xx = 0;
    for ( unsigned int j = 0; j < x->size; ++j ) {
      const double dj = gsl_vector_get( d, j );
      if ( 0 == dj ) { continue; }
      const double xj = gsl_vector_get( x, j );
      xx              = std::max( xx, std::abs( xj ) + std::abs( dj ) );
    }
    //
    return deriv2_8( values, xx, h0, round, trunc );
  }
  // ==========================================================================
  // get the second derivative along the direction d
  // ==========================================================================
  double deriv2( const gsl_multimin_function* f, const gsl_vector* x, const gsl_vector* d, double h, double* e,
                 gsl_vector* a ) {
    //
    double round;
    double trunc;
    //
    double result = deriv2_8( f, x, d, h, &round, &trunc, a );
    //
    *e = round + trunc;
    //
    double         h_new = h;
    double         res   = result;
    unsigned short iter  = 0;
    while ( 0 < trunc && 0 < round && ++iter < 16 ) {
      //
      const double rt   = round / trunc / 2;
      const double corr = 1 < rt && rt < 1000 ? pow( rt, 1.0 ) : pow( rt, 1. / 7 );
      //
      if ( std::abs( corr - 1 ) < 0.25 ) { break; } // BREAK
      //
      if ( 0 != corr ) {
        h_new *= corr;
      } else {
        h_new /= 2;
      }
      //
      res = deriv2_8( f, x, d, h_new, &round, &trunc, a );
      //
      if ( iter > 2 && ( *e ) < ( trunc + round ) ) { break; } // BREAK
      //
      if ( ( *e ) < ( trunc + round ) ) { continue; } // CONTINUE
      //
      result = res;
      *e     = trunc + round;
      //
    }
    //
    return result;
  }
  // ==========================================================================
  // get the second derivative along pseudo-axis
  // ==========================================================================
  double deriv2( const gsl_multimin_function* f, const gsl_vector* x, const unsigned short i, const unsigned short j,
                 double h, double* error,
                 gsl_vector* a,  // helper vector (workspace)
                 gsl_vector* b ) // helper vector (workspace)
  {
    //
    gsl_vector_set_zero( b );
    //
    // define the direction
    if ( i == j ) {
      gsl_vector_set( b, i, 1 );
    } else {
      gsl_vector_set( b, i, s_SQRT2i );
      gsl_vector_set( b, j, s_SQRT2i );
    }
    //
    return deriv2( f, x, b, h, error, a );
    // ========================================================================
  }
  // ==========================================================================
} // end of namespace
// ============================================================================
// HESSE
// ============================================================================
/*  constructor with all parameters
 *  @param f      the function to be used
 *  @param x      the point for hessian to be evaluated
 *  @param params the parameters for the function
 *  @param h the step-size (guess)
 */
// ============================================================================
Gaudi::Math::GSL::Hesse::Hesse( function f, const gsl_vector* x, void* params, const double h )
    //
    : m_func( f )
    , m_x( x )
    , m_params( params )
    //
    , m_h( std::abs( h ) )
    //
    , m_a{gsl_vector_calloc( x->size )}
    , m_b{gsl_vector_calloc( x->size )} {}
// ============================================================================
StatusCode Gaudi::Math::GSL::Hesse::calcHesse() {
  //
  // if ( 0 == m_x    ) { return InvalidPoint    ; }
  // if ( 0 == m_func ) { return InvalidFunction ; }
  //
  // allocate new matrix
  //
  m_hesse.reset( gsl_matrix_calloc( size(), size() ) );
  m_aux.reset( gsl_matrix_calloc( size(), size() ) );
  //
  double error = 0;
  //
  gsl_multimin_function F;
  F.f      = m_func;
  F.params = m_params;
  F.n      = m_x->size;
  //
  // fill auxillary Hesse matrix
  for ( std::size_t i = 0; i < m_aux->size1; ++i ) {
    //
    for ( std::size_t j = 0; j < m_aux->size2; ++j ) {
      //
      const double hij = deriv2( &F, m_x, i, j, m_h, &error, m_a.get(), m_b.get() );
      //
      gsl_matrix_set( m_aux.get(), i, j, hij );
      //
    }
    //
  }
  //
  // adjust hesse matrix
  for ( std::size_t i = 0; i < m_hesse->size1; ++i ) {
    const double hii = gsl_matrix_get( m_aux.get(), i, i );
    //
    gsl_matrix_set( m_hesse.get(), i, i, hii );
    //
    for ( std::size_t j = i + 1; j < m_hesse->size2; ++j ) {
      //
      const double hjj = gsl_matrix_get( m_aux.get(), j, j );
      //
      const double hij = gsl_matrix_get( m_aux.get(), i, j );
      const double hji = gsl_matrix_get( m_aux.get(), j, i );
      //
      const double h = 0.5 * ( hij + hji ) - 0.5 * ( hii + hjj );
      //
      gsl_matrix_set( m_hesse.get(), i, j, h );
      gsl_matrix_set( m_hesse.get(), j, i, h );
      //
    }
  }
  //   aux <--- hesse
  gsl_matrix_memcpy( m_aux.get(), m_hesse.get() );
  //
  return StatusCode::SUCCESS;
}
// ============================================================================
StatusCode Gaudi::Math::GSL::Hesse::calcCov2() {
  //
  Gaudi::Math::GSL::GSL_Error_Handler sentry;
  //
  m_cov2.reset();
  //
  if ( !m_hesse || !m_aux ) {
    StatusCode sc = calcHesse();
    if ( sc.isFailure() ) { return sc; }
  }
  //
  m_cov2.reset( gsl_matrix_calloc( size(), size() ) );
  //
  // copy hesse into aux
  gsl_matrix_memcpy( m_aux.get(), m_hesse.get() );
  ;
  //
  // use LU decomposition
  StatusCode sc = invert_LU_1( m_aux.get(), m_cov2.get() );
  if ( sc.isFailure() ) {
    m_cov2.reset();
    return sc;
  }
  //
  gsl_matrix_scale( m_cov2.get(), 2 );
  //
  return StatusCode::SUCCESS;
}
// ============================================================================
/*  invert the matrix using LU decomposition
 *  @param matrix (UPDATE) the matrix to be inverted
 *  @param result (UPDATE) the result
 *  @return status code
 *  @attention the input matrix will be screwed up!
 *  @author Vanya BELYAEV  Ivan.Belyaev@itep.ru
 *  @date 2012-05-28
 */
// ============================================================================
StatusCode Gaudi::Math::GSL::invert_LU_1( gsl_matrix* matrix, gsl_matrix* result ) {
  //
  if ( 0 == matrix ) { return StatusCode{10}; }
  if ( 0 == result ) { return StatusCode{11}; }
  if ( matrix->size1 != matrix->size2 ) { return StatusCode{12}; }
  if ( result->size1 != result->size2 ) { return StatusCode{13}; }
  if ( result->size1 != matrix->size1 ) { return StatusCode{14}; }
  if ( result->size2 != matrix->size2 ) { return StatusCode{15}; }
  //
  // Make LU decomposition of input matrix
  //
  // permutations :
  std::unique_ptr<gsl_permutation, details::gsl_deleter> perm{gsl_permutation_alloc( matrix->size1 )};
  //
  int sigdet;
  int ierror;
  //
  ierror = gsl_linalg_LU_decomp( matrix, perm.get(), &sigdet );
  if ( ierror ) {
    gsl_error( "Error from LU_decomp ", __FILE__, __LINE__, ierror );
    return StatusCode( 100 + ierror );
  }
  //
  // Invert the matrix m_aux
  ierror = gsl_linalg_LU_invert( matrix, perm.get(), result );
  if ( ierror ) {
    gsl_error( "Error from LU_invert ", __FILE__, __LINE__, ierror );
    return StatusCode( 200 + ierror );
  }
  //
  return StatusCode::SUCCESS;
}
// ======================================================================
/*  invert the matrix using LU decomposition
 *  @param matrix (INPUT) the matrix to be inverted
 *  @param result (UPDATE) the result
 *  @return status code
 *  @attention the input matrix will be preserved
 *  @author Vanya BELYAEV  Ivan.Belyaev@itep.ru
 *  @date 2012-05-28
 */
// ======================================================================
StatusCode Gaudi::Math::GSL::invert_LU_2( const gsl_matrix* matrix, gsl_matrix* result ) {
  //
  if ( !matrix ) { return StatusCode{10}; }
  if ( !result ) { return StatusCode{11}; }
  if ( matrix->size1 != matrix->size2 ) { return StatusCode{12}; }
  if ( result->size1 != result->size2 ) { return StatusCode{13}; }
  if ( result->size1 != matrix->size1 ) { return StatusCode{14}; }
  if ( result->size2 != matrix->size2 ) { return StatusCode{15}; }
  //
  // make the intermediate matrix
  //
  std::unique_ptr<gsl_matrix, details::gsl_deleter> aux{gsl_matrix_alloc( matrix->size1, matrix->size2 )};
  //
  gsl_matrix_memcpy( aux.get(), matrix );
  //
  StatusCode sc = invert_LU_1( aux.get(), result );
  //
  return sc;
}
// ============================================================================
// The END
// ============================================================================
