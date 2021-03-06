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
#ifndef LHCBMATH_EIGENSYSTEM_H
#define LHCBMATH_EIGENSYSTEM_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <vector>
// ============================================================================
// ROOT
// ============================================================================
#include "Math/SMatrix.h"
#include "Math/SVector.h"
// ============================================================================
// GSL
// ============================================================================
#include "gsl/gsl_eigen.h"
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/StatusCode.h"
// ============================================================================
namespace Gaudi {
  // ==========================================================================
  namespace Math {
    // ========================================================================
    namespace GSL {
      // ======================================================================
      /** @class EigenSystem EigenSystem.h
       *
       *  Helper class with allows to find eigenvalues and eigenvector
       *  for symmetrical MathLib matrices ("SMatrix") using GSL library
       *
       *  @author Vanya BELYAEV
       *  @date   2006-05-24
       */
      class EigenSystem {
        // ====================================================================
      public:
        // ====================================================================
        /// error codes
        enum {
          MatrixAllocationFailure    = 101,
          VectorAllocationFailure    = 102,
          WorkspaceAllocationFailure = 103,
          // the actual return value is ErrorFromGSL + error code )
          ErrorFromGSL = 199 ///< ErrorFromGSL + error code
        };
        // ====================================================================
      public:
        // ====================================================================
        /// Standard constructor
        EigenSystem();
        /// destructor
        ~EigenSystem();
        // ====================================================================
      public:
        // ====================================================================
        /** evaluate the eigenvalues of symmetrical matrix
         *
         *  @code
         *
         *  // create the evaluator
         *  EigenSystem eval ;
         *
         *  const Gaudi::SymMatrix3x3 matrix = ... ;
         *
         *  // get the sorted vector of eigenvalues:
         *  const Gaudi::Vector2 result = eval.eigenValues ( matrix ) ;
         *
         *  @endcode
         *
         *  @exception GaudiException is thrown in the case of errors
         *
         *  @param mtrx   (input) the matrix itself
         *  @param sorted (input) flag to be use for sorting
         *  @return vector of eigenvalues
         */
        template <class T, unsigned int D>
        inline ROOT::Math::SVector<T, D>
        eigenValues( const ROOT::Math::SMatrix<T, D, D, ROOT::Math::MatRepSym<T, D>>& mtrx,
                     const bool                                                       sorted = true ) const;
        // ====================================================================
        /** evaluate the eigenvalues of symmetrical matrix
         *
         *  @code
         *
         *  // create the evaluator
         *  EigenSystem eval ;
         *
         *  const Gaudi::SymMatrix3x3 matrix = ... ;
         *  Gaudi::Vector3 resutl
         *
         *  // find the eigenvalues:
         *  StatusCode sc = eval.eigenValues ( matrix , result ) ;
         *
         *  @endcode
         *
         *  @param mtrx   (input)  the matrix itself
         *  @param vals   (output) the vector fo eigenvalues
         *  @param sorted (input)  flag to be use for sorting
         *  @return status code
         */
        template <class T, unsigned int D>
        inline StatusCode eigenValues( const ROOT::Math::SMatrix<T, D, D, ROOT::Math::MatRepSym<T, D>>& mtrx,
                                       ROOT::Math::SVector<T, D>& vals, const bool sorted = true ) const;
        // ====================================================================
        /** evaluate the eigenvalues and eigenvectors of the symmetrical matrix
         *
         *  @code
         *
         *  // create the evaluator
         *  EigenSystem eval ;
         *
         *  const Gaudi::SymMatrix3x3 matrix = ... ;
         *  // matrix with eigenvectors:
         *  Gaudi::Matrix3x3          vectors ;
         *  // vector of eigenvalues:
         *  Gausi::Vector3            values  ;
         *
         *  // find the eigenvalues & eigenvectors:
         *  StatusCode sc = eval.eigenvectors ( matrix , values , vectors ) ;
         *
         *  @endcode
         *
         *  Eigenvectors are returned as columns for the matrix "vecs".
         *  This matrix could be easily used to for diagonalization of
         *  the initial matrix, e.g.
         *
         *  @code
         *
         *  // avoid long names:
         *  using namespace ROOT::Math ;
         *  // *NUMERICALLY* DIAGONAL MATRIX:
         *  Gaudi::SymMatrix3x3 res =
         *      Similarity ( Transpose ( vectors ) , matrix ) ;
         *
         *  @endcode
         *
         *
         *  @param mtrx   (input)  the matrix itself
         *  @param vals   (output) the vector fo eigenvalues
         *  @param vecs   (output) the matrix with eigenvectors
         *  @param sorted (input)  flag to be use for sorting
         *  @return status code
         */
        template <class T, unsigned int D, class R>
        inline StatusCode eigenVectors( const ROOT::Math::SMatrix<T, D, D, ROOT::Math::MatRepSym<T, D>>& mtrx,
                                        ROOT::Math::SVector<T, D>& vals, ROOT::Math::SMatrix<T, D, D, R>& vecs,
                                        const bool sorted = true ) const;
        // ====================================================================
        /** evaluate the eigenvalues and eigenvectors of the symmetrical matrix
         *
         *  @code
         *
         *  // create the evaluator
         *  EigenSystem eval ;
         *
         *  const Gaudi::SymMatrix3x3   matrix = ... ;
         *  // vector  with eigenvectors:
         *  std::vector<Gaudi::Vector3> vectors ;
         *  // vector of eigenvalues:
         *  Gaudi::Vector3              values  ;
         *
         *  // find the eigenvalues & eigenvectors:
         *  StatusCode sc = eval.eigenvectors ( matrix , values , vectors ) ;
         *
         *  @endcode
         *
         *  @param mtrx   (input)  the matrix itself
         *  @param vals   (output) the vector fo eigenvalues
         *  @param vecs   (output) the vector of eigenvectors
         *  @param sorted (input)  flag to be use for sorting
         *  @return status code
         */
        template <class T, unsigned int D>
        inline StatusCode eigenVectors( const ROOT::Math::SMatrix<T, D, D, ROOT::Math::MatRepSym<T, D>>& mtrx,
                                        ROOT::Math::SVector<T, D>& vals, std::vector<ROOT::Math::SVector<T, D>>& vecs,
                                        const bool sorted = true ) const;
        // ====================================================================
      protected:
        // ====================================================================
        /// find the eigenvalues   (& sort them if needed )
        StatusCode _fun1( const bool sorted ) const;
        /// find the eigenvalues&eigenvectors (& sort them if needed )
        StatusCode _fun2( const bool sorted ) const;
        // ====================================================================
      private:
        // ====================================================================
        /// fill the internal structures with the input data
        template <class T, unsigned int D>
        inline StatusCode _fill( const ROOT::Math::SMatrix<T, D, D, ROOT::Math::MatRepSym<T, D>>& mtrx ) const;
        /// check/adjust the  internal structures
        StatusCode _check( const unsigned int D ) const;
        // thrown the exception
        StatusCode Exception( const StatusCode& sc ) const;
        // ====================================================================
      private:
        // the size of workspace
        mutable unsigned int m_dim1; ///< the size of workspace
        mutable unsigned int m_dim2; ///< the size of workspace
        // workspace itself
        mutable gsl_eigen_symm_workspace* m_work1; ///< workspace itself
        // workspace itself
        mutable gsl_eigen_symmv_workspace* m_work2; ///< workspace itself
        // the matrix with input data
        mutable gsl_matrix* m_matrix; ///< the matrix with input data
        // the matrix with eigenvectors
        mutable gsl_matrix* m_evec; ///< the matrix with eigenvectors
        // the vector with eigenvalues
        mutable gsl_vector* m_vector; ///< the vector with eigenvalues
      };
      // ======================================================================
      /** copy GSL vector into MathLib vector
       *  @attention Fast!no checks are performed!
       *  @param input   GSL vector to be copyed (source)
       *  @param output  MathLib vector (destination)
       *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
       *  @date 2006-05-24
       */
      template <class T, unsigned int D>
      inline void _copy( const gsl_vector* input, ROOT::Math::SVector<T, D>& output );
      // ======================================================================
      /** copy GSL matrix into MathLib matrix into GSL
       *  @attention Fast!no checks are performed!
       *  @param input  GSL matrix (source)
       *  @param output MathLib matrix (destination)
       *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
       *  @date 2006-05-24
       */
      template <class T, unsigned int D, class R>
      inline void _copy( const gsl_matrix* input, ROOT::Math::SMatrix<T, D, D, R>& output );
      // ======================================================================
      /** copy symmetric MathLib matrix into GSL matrix
       *  @attention Fast!no checks are performed!
       *  @param input  MathLib symmetric matrix (source)
       *  @param output GSL matrix  (destination)
       *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
       *  @date 2006-05-24
       */
      template <class T, unsigned int D>
      inline void _copy( const ROOT::Math::SMatrix<T, D, D, ROOT::Math::MatRepSym<T, D>>& input, gsl_matrix* output );
      // ======================================================================
    } //                                                   end of namespace GSL
    // ========================================================================
  } //                                                    end of namespace Math
  // ==========================================================================
} //                                                     end of namespace Gaudi
// ============================================================================
#include "LHCbMath/EigenSystem.icpp"
#endif // LHCBMATH_EIGENSYSTEM_H
