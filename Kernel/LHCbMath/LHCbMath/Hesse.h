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
#ifndef LHCBMATH_HESSE_H
#define LHCBMATH_HESSE_H 1
// ============================================================================
// Include files
// ============================================================================
#include <memory>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/StatusCode.h"
// ============================================================================
// GSL
// ============================================================================
#include "gsl/gsl_linalg.h"
#include "gsl/gsl_matrix.h"
#include "gsl/gsl_vector.h"
// ============================================================================
namespace Gaudi {
  // ==========================================================================
  namespace Math {
    // ========================================================================
    namespace GSL {
      namespace details {
        struct gsl_deleter {
          void operator()( gsl_vector* p ) const {
            if ( p ) gsl_vector_free( p );
          }
          void operator()( gsl_matrix* p ) const {
            if ( p ) gsl_matrix_free( p );
          }
          void operator()( gsl_permutation* p ) const {
            if ( p ) gsl_permutation_free( p );
          }
        };
      } // namespace details
      // ======================================================================
      /** @class Hesse
       *  evaluate the hessian for the function
       *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
       *  @date 2012-05-27
       */
      class GAUDI_API Hesse final {
      public:
        // ====================================================================
        /// the actual type of function to be used for hessian calculation
        typedef double ( *function )( const gsl_vector* x, void* params );
        // ====================================================================
      public:
        // ====================================================================
        /*  constructor with all parameters
         *  @param f      the function to be used
         *  @param x      the point for hessian to be evaluated
         *  @param params the parameters for the function
         *  @param h the step-size (guess)
         */
        Hesse( function f, const gsl_vector* x, void* params, const double h );
        // ====================================================================
      private:
        // ====================================================================
        /// the copy constructor is disabled
        Hesse( const Hesse& ) = delete; // the copy constructor is disabled
        /// the assignement operator is disabled
        Hesse& operator=( const Hesse& ) = delete; // no assignement
        // ====================================================================
      public:
        // ====================================================================
        StatusCode calcHesse();
        StatusCode calcCov2();
        // ====================================================================
      public:
        // ====================================================================
        /// size of the problem
        std::size_t size() const { return m_x->size; }
        /// get the hesse matrix
        const gsl_matrix* hesse() const { return m_hesse.get(); }
        /// get the inverse hesse ("covariance") matrix
        const gsl_matrix* cov2() const { return m_cov2.get(); }
        // ====================================================================
      private:
        // ====================================================================
        /// the function
        function m_func; // the function
        /// the point
        const gsl_vector* m_x; // the point
        /// parameters
        void* m_params; // the parameters
        /// step-size
        double m_h; // the step-size
        // ====================================================================
      private:
        // ====================================================================
        /// the actual hessian
        std::unique_ptr<gsl_matrix, details::gsl_deleter> m_hesse; // the actual hesse matrix
        /// the axuxillary matrix
        std::unique_ptr<gsl_matrix, details::gsl_deleter> m_aux; // the axuxillary matrix
        /// the inverse hesse matrix
        std::unique_ptr<gsl_matrix, details::gsl_deleter> m_cov2; // the inverse hesse matrix
        ///
        // ====================================================================
      private:
        // ====================================================================
        /// helper vector
        std::unique_ptr<gsl_vector, details::gsl_deleter> m_a; // helper vector
        /// helper vector
        std::unique_ptr<gsl_vector, details::gsl_deleter> m_b; // helper vector
        // ====================================================================
      };
      // ======================================================================
      /** invert the matrix using LU decomposition
       *  @param matrix (UPDATE) the matrix to be inverted
       *  @param result (UPDATE) the result
       *  @return status code
       *  @attention the input matrix will be screwed up!
       *  @author Vanya BELYAEV  Ivan.Belyaev@itep.ru
       *  @date 2012-05-28
       */
      GAUDI_API
      StatusCode invert_LU_1( gsl_matrix* matrix, gsl_matrix* result );
      // ======================================================================
      /** invert the matrix using LU decomposition
       *  @param matrix (INPUT) the matrix to be inverted
       *  @param result (UPDATE) the result
       *  @return status code
       *  @attention the input matrix will be preserved
       *  @author Vanya BELYAEV  Ivan.Belyaev@itep.ru
       *  @date 2012-05-28
       */
      GAUDI_API
      StatusCode invert_LU_2( const gsl_matrix* matrix, gsl_matrix* result );
      // ======================================================================
    } // namespace GSL
  }   // namespace Math
  // ==========================================================================
} //                                                     end of namespace Gaudi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LHCBMATH_HESSE_H
