/*****************************************************************************\
* (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// Include files

#include <stdio.h>
#include <stdlib.h>
// from Gaudi
#include "GaudiKernel/MsgStream.h"
// from GSL
#include "gsl/gsl_blas.h"
#include "gsl/gsl_errno.h"
#include "gsl/gsl_linalg.h"
#include "gsl/gsl_math.h"
#include "gsl/gsl_matrix.h"
#include "gsl/gsl_multimin.h"
#include "gsl/gsl_vector.h"

// local
#include "EqSolver.h"

/** @file
 *  Implementation file for class : EqSolver
 *  @see EqSolver.h
 *  @author Kirill Miklyaev kirillm@iris1.itep.ru
 *  @date 2002-09-14
 */

// Declaration of the Tool Factory
DECLARE_COMPONENT( EqSolver )

// ============================================================================
// ============================================================================
EqSolver::EqSolverMisc::EqSolverMisc( const EqSolver::Equations& funcs, EqSolver::Arg& arg )
    : m_argum( arg ), m_eqs( &funcs ) {
  const size_t N = funcs.size();
  for ( size_t i = 0; i < N; ++i ) {
    Equations last;
    for ( size_t j = 0; j < N; ++j ) {
      Genfun::GENFUNCTION fij = funcs[i]->partial( j );
      last.push_back( fij.clone() );
    }
    m_jac.push_back( last );
  }
}
// ============================================================================

// ============================================================================
EqSolver::EqSolverMisc::~EqSolverMisc() {
  while ( !m_jac.empty() ) {
    Equations& last = m_jac.back();
    while ( !last.empty() ) {
      delete last.back();
      last.pop_back();
    }
    m_jac.pop_back();
  }
}
// ============================================================================

namespace {
  using namespace Genfun;

  /** The function which we solver
   */
  int fun_gsl( const gsl_vector* v, void* params, gsl_vector* f ) {
    EqSolver::EqSolverMisc*    local = static_cast<EqSolver::EqSolverMisc*>( params );
    const EqSolver::Equations& eqs   = *( local->equations() );
    Argument&                  arg   = local->argument();

    for ( unsigned int i = 0; i < v->size; ++i ) { arg[i] = gsl_vector_get( v, i ); }

    for ( unsigned int i = 0; i < v->size; ++i ) { gsl_vector_set( f, i, ( *eqs[i] )( arg ) ); }
    return GSL_SUCCESS;
  }

  /** The gradient of the function "fun_gsl"
   */
  int dfun_gsl( const gsl_vector* v, void* params, gsl_matrix* df ) {
    EqSolver::EqSolverMisc* local = static_cast<EqSolver::EqSolverMisc*>( params );
    const EqSolver::Jacobi& jac   = local->jacobi();
    Argument&               arg   = local->argument();

    for ( unsigned int i = 0; i < v->size; ++i ) { arg[i] = gsl_vector_get( v, i ); }

    for ( unsigned int i = 0; i < v->size; ++i ) {
      for ( unsigned int j = 0; j < v->size; ++j ) {
        Genfun::GENFUNCTION f = *( jac[i][j] );
        gsl_matrix_set( df, i, j, f( arg ) );
      }
    }
    return GSL_SUCCESS;
  }

  /** Compute both the function "fun_gsl"
   * and gradient of this function "dfun_gsl" together
   */
  int fdfun_gsl( const gsl_vector* v, void* params, gsl_vector* f, gsl_matrix* df ) {
    EqSolver::EqSolverMisc*    local = static_cast<EqSolver::EqSolverMisc*>( params );
    const EqSolver::Equations& eqs   = *( local->equations() );
    const EqSolver::Jacobi&    jac   = local->jacobi();
    Argument&                  arg   = local->argument();

    for ( unsigned int i = 0; i < v->size; ++i ) { arg[i] = gsl_vector_get( v, i ); }

    for ( unsigned int i = 0; i < v->size; ++i ) {
      gsl_vector_set( f, i, ( *eqs[i] )( arg ) );
      for ( unsigned int j = 0; j < v->size; ++j ) {
        Genfun::GENFUNCTION f1 = *( jac[i][j] );
        gsl_matrix_set( df, i, j, f1( arg ) );
      }
    }
    return GSL_SUCCESS;
  }
} // namespace

//=============================================================================
/** Solving nonlinear system of the function "GenFunc"
 *  @see IEqSolver.h
 *  @return StatusCode
 */
StatusCode EqSolver::solver( const Equations& funcs, Arg& arg ) const
//=============================================================================
{
  using namespace Genfun;

  gsl_vector_view vect = gsl_vector_view_array( &arg[0], arg.dimension() );
  EqSolverMisc    local( funcs, arg );

  const gsl_multiroot_fdfsolver_type* T      = m_type;
  gsl_multiroot_fdfsolver*            s      = nullptr;
  int                                 status = 0;
  size_t                              iter   = 0;

  gsl_multiroot_function_fdf function;

  function.f      = &fun_gsl;
  function.df     = &dfun_gsl;
  function.fdf    = &fdfun_gsl;
  function.n      = vect.vector.size;
  function.params = &local;

  s = gsl_multiroot_fdfsolver_alloc( T, vect.vector.size );
  gsl_multiroot_fdfsolver_set( s, &function, &vect.vector );

  for ( iter = 0; iter < m_max_iter; ++iter ) {
    status = gsl_multiroot_fdfsolver_iterate( s );
    if ( status ) {
      return Error( "Error from gsl_gsl_multiroot_fdfsolver_iterate '" + std::string( gsl_strerror( status ) ) + "'" );
    }

    status = gsl_multiroot_test_residual( s->f, m_norm_residual );

    if ( status != GSL_CONTINUE ) { break; }
  }

  for ( unsigned int i = 0; i < vect.vector.size; ++i ) {
    gsl_vector_set( &vect.vector, i, gsl_vector_get( s->x, i ) );
  }

  if ( status == GSL_SUCCESS ) {
    debug() << "We stopped in the method on the " << iter << " iteration (we have maximum " << m_max_iter
            << " iterations)" << endmsg;
  } else if ( status == GSL_CONTINUE && iter <= m_max_iter ) {
    return Error( "Method finished with '" + std::string( gsl_strerror( status ) ) + "' error" );
  } else {
    return Error( "Method finished with '" + std::string( gsl_strerror( status ) ) + "' error" );
  }

  gsl_multiroot_fdfsolver_free( s );

  if ( status ) { return Error( "Method finished with '" + std::string( gsl_strerror( status ) ) + "' error" ); }

  return StatusCode( GSL_SUCCESS );
}

//=============================================================================
StatusCode EqSolver::initialize() {
  StatusCode sc = GaudiTool::initialize();
  if ( sc.isFailure() ) { return Error( "Could not initiliaze base class GaudiTool", sc ); }

  /* The algorithm for multiional root-finding
     (solving nonlinear systems with n equations in n unknowns)*/

  if ( "fdfsolver_hybridsj" == m_algType ) {
    m_type = gsl_multiroot_fdfsolver_hybridsj;
    debug() << "Root finding algorithm to be used: "
            << "'gsl_multiroot_fdfsolver_hybridsj'" << endmsg;
  } else if ( "fdfsolver_hybridj" == m_algType ) {
    m_type = gsl_multiroot_fdfsolver_hybridj;
    debug() << "Root finding algorithm to be used: "
            << "'gsl_multiroot_fdfsolver_hybridj'" << endmsg;
  } else if ( "fdfsolver_newton" == m_algType ) {
    m_type = gsl_multiroot_fdfsolver_newton;
    debug() << "Root findind algorithm to be used: "
            << "'gsl_multiroot_fdfsolver_newton'" << endmsg;
  } else if ( "fdfsolver_gnewton" == m_algType ) {
    m_type = gsl_multiroot_fdfsolver_gnewton;
    debug() << "Root findind algorithm to be used: "
            << "'gsl_multiroot_fdfsolver_gnewton'" << endmsg;
  } else {
    return Error( " Unknown algorithm type '" + std::string( m_algType ) + "'" );
  }

  return StatusCode::SUCCESS;
}
//=============================================================================
